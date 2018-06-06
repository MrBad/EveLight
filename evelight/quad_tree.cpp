#include "quad_tree.h"
#include "aabb.h"

namespace evl {

// QTNode methods

QTNode::QTNode(const AABB& limits, QTNode* parent)
    : mLimits(limits)
    , mParent(parent)
{
    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++)
        mChildren[i] = 0;

    mDepth = parent ? parent->mDepth + 1 : 0;
}

bool QTNode::IsLeaf() const
{
    if (mChildren[NE] == nullptr) {
        for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++)
            assert(mChildren[i] == nullptr);
    }

    return mChildren[NE] == nullptr;
}

bool QTNode::HasRoom() const
{
    return mEntities.size() < QT_TREE_MAX_OBJECTS;
}

bool QTNode::IsEmpty() const
{
    return mEntities.size() == 0;
}

bool QTNode::BrothersAreEmpty() const
{
    uint numChlildrendObjects = 0;

    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++) {
        numChlildrendObjects += mParent->mChildren[i]->mEntities.size();
        if (!mParent->mChildren[i]->IsLeaf() || numChlildrendObjects > 0)
            return false;
    }

    return true;
}

/**
 * Checks if this object(limits) fits in any of this node children.
 * @param obj - The object
 * @return child index on success, or -1 if it cannot fit
 */
int QTNode::GetChildIndex(QTEntity* entity) const
{
    assert(!IsLeaf());

    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++)
        if (entity->GetAABB().FitsIn(mChildren[i]->mLimits))
            return i;

    return -1;
}

/**
 * Allocates the 4 children nodes and their boundaries.
 */
void QTNode::Split()
{
    glm::vec2 center = mLimits.GetCenter();

    mChildren[NE] = new QTNode(AABB(center.x, center.y, mLimits.maxX, mLimits.maxY), this);
    mChildren[NW] = new QTNode(AABB(mLimits.minX, center.y, center.x, mLimits.maxY), this);
    mChildren[SW] = new QTNode(AABB(mLimits.minX, mLimits.minY, center.x, center.y), this);
    mChildren[SE] = new QTNode(AABB(center.x, mLimits.minY, mLimits.maxX, center.y), this);
}

bool QTNode::Add(QTEntity* entity)
{
    bool split, anyMoved;
    split = anyMoved = false;
    if (!entity->GetAABB().FitsIn(mLimits))
        return false;

    if (IsLeaf()) {
        if (HasRoom()) {
            mEntities.push_back(entity);
            entity->SetQTNode(this);
            return true;
        } else {
            Split();
            split = true;
            // Move old objects down to children, if possible //
            for (auto it = mEntities.begin(); it != mEntities.end();) {
                QTEntity* oldEntity = *it;
                int idx = GetChildIndex(oldEntity);
                if (idx > -1) {
                    if (mChildren[idx]->Add(oldEntity)) {
                        it = mEntities.erase(it);
                        anyMoved = true;
                        continue;
                    }
                }
                it++;
            }
        }
    }
    // Add the new object //
    int idx = GetChildIndex(entity);
    if (idx > -1) {
        mChildren[idx]->Add(entity);
    } else {
        // If we splitted the node, but none of the current QTObjects
        //  moved down to children, and nor this object can fit in one of the
        //  children, makes not sense to have children, because nobody will
        //  update and delete them up! TODO: check before split!!!
        if (split && !anyMoved) {
            for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++) {
                assert(mChildren[i]->IsEmpty() && mChildren[i]->IsLeaf());
                delete mChildren[i];
                mChildren[i] = nullptr;
            }
        }
        mEntities.push_back(entity);
        entity->SetQTNode(this);
    }

    return true;
}

bool QTNode::DeleteUp()
{
    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++)
        assert(mChildren[i]->IsLeaf() && mChildren[i]->IsEmpty());

    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++) {
        delete mChildren[i];
        mChildren[i] = nullptr;
    }
    if (IsLeaf() && IsEmpty() && BrothersAreEmpty())
        return mParent->DeleteUp();

    return true;
}

bool QTNode::Update(QTEntity* entity, QTNode** root, bool expand)
{
    // If we still fit in this node, and this node is split
    //   check if we can fit into a child.
    if (entity->GetAABB().FitsIn(mLimits)) {
        if (!IsLeaf()) {
            int idx = GetChildIndex(entity);
            if (idx > -1) {
                mEntities.remove(entity);
                return mChildren[idx]->Add(entity);
            }
        }
        return true;
    }

    // Climb up the tree until we found a parent that can adopt our new limits.
    bool found = false;
    QTNode* newNode;
    for (newNode = entity->GetQTNode(); newNode; newNode = newNode->mParent) {
        if (entity->GetAABB().FitsIn(newNode->mLimits)) {
            found = true;
            break;
        }
    }

    if (!found) {
        if (!expand) {
            std::cerr << "Request for a limit out of root range. Update fail!"
                      << std::endl;
            return false;
        }
        assert(root);
        std::cout << "info: Expand root on update" << std::endl;
        newNode = *root = (*root)->Expand(entity->GetAABB());
    }

    mEntities.remove(entity);
    bool ret = newNode->Add(entity);

    if (IsLeaf() && IsEmpty() && BrothersAreEmpty())
        mParent->DeleteUp();

    return ret;
}

/**
 * Grows the tree up (aka inverse of split) to fit newLimits
 */
QTNode* QTNode::Expand(const AABB& newLimits)
{
    assert(!mParent); // should be root

    // find the direction where we should grow;
    glm::vec2 direction = glm::normalize(mLimits.GetDistance(newLimits));

    float width = mLimits.maxX - mLimits.minX,
          height = mLimits.maxY - mLimits.minY;

    AABB doubledLimits = mLimits;
    int appendTo;

    if (direction.x > 0) { // East
        if (direction.y > 0) { // NE
            doubledLimits = AABB(
                mLimits.minX, mLimits.minY,
                mLimits.maxX + width, mLimits.maxY + height);
            appendTo = SW;
        } else { // SE
            doubledLimits = AABB(
                mLimits.minX, mLimits.minY - height,
                mLimits.maxX + width, mLimits.maxY);
            appendTo = NW;
        }
    } else { // West
        if (direction.y > 0) { // NW
            doubledLimits = AABB(
                mLimits.minX - width, mLimits.minY,
                mLimits.maxX, mLimits.maxY + height);
            appendTo = SE;
        } else { // SW
            doubledLimits = AABB(
                mLimits.minX - width, mLimits.minY - height,
                mLimits.maxX, mLimits.maxY);
            appendTo = NE;
        }
    }
    printf("Doubled the limits to: {%.2f, %.2f, %.2f, %.2f}\n",
        doubledLimits.minX, doubledLimits.minY,
        doubledLimits.maxX, doubledLimits.maxY);

    // New parent node
    QTNode* parent = new QTNode(doubledLimits, nullptr);
    parent->Split();

    delete parent->mChildren[appendTo];
    parent->mChildren[appendTo] = this;
    this->mParent = parent;

    // Increment depth
    auto f = [&](QTNode* qNode) {
        qNode->SetDepth(qNode->GetDepth() + 1);
        return true;
    };
    Traverse(f);

    if (!newLimits.FitsIn(doubledLimits))
        parent = parent->Expand(newLimits);

    return parent;
}

int QTNode::GetIntersections(const AABB& queryBox, std::vector<QTEntity*>& outResults)
{
    int numRes = 0;
    if (!mLimits.Intersects(queryBox))
        return numRes;

    for (auto it = mEntities.begin(); it != mEntities.end(); it++) {
        QTEntity* entity = *it;
        if (entity->GetAABB().Intersects(queryBox)) {
            outResults.push_back(entity);
            numRes++;
        }
    }
    if (IsLeaf())
        return numRes;

    for (int i = 0; i < QTNODE_NUM_CHILDREN; i++)
        numRes += mChildren[i]->GetIntersections(queryBox, outResults);

    return numRes;
}

QTNode::~QTNode()
{
    if (!IsLeaf())
        for (int i = 0; i < QTNODE_NUM_CHILDREN; i++)
            delete mChildren[i];

#if 0
    for (auto it = mEntities.begin(); it != mEntities.end(); it++)
        delete *it;
#endif
}

void QTNode::Traverse(std::function<bool(QTNode*)> func)
{
    if (!func(this))
        return;

    for (int i = 0; i < QTNODE_NUM_CHILDREN; i++) {
        if (mChildren[i])
            mChildren[i]->Traverse(func);
    }
}

void QTNode::TraverseEntities(std::function<bool(QTEntity*)> func)
{
    for (auto it = mEntities.begin(); it != mEntities.end(); it++)
        if (!func(*it))
            return;

    for (int i = 0; i < QTNODE_NUM_CHILDREN; i++) {
        if (mChildren[i])
            mChildren[i]->TraverseEntities(func);
    }
}

// QuadTree methods

QuadTree::QuadTree()
    : mRoot(nullptr)
    , mItems(0)
    , mExpandable(false)
{
}

void QuadTree::Init(const AABB& limits, bool expandable)
{
    mItems = 0;
    mRoot = new QTNode(limits, nullptr);
    mExpandable = expandable;
}

bool QuadTree::Add(QTEntity* entity)
{
    if (!entity->GetAABB().FitsIn(mRoot->mLimits)) {
        if (!mExpandable) {
            std::cerr
                << "Requested limits out of tree max range. Will not expand root!"
                << std::endl;
            return false;
        }
        mRoot = mRoot->Expand(entity->GetAABB());
        printf("Expanded Root\n");
    }

    if (!mRoot->Add(entity)) {
        return false;
    }
    mItems++;

    return true;
}

bool QuadTree::Update(QTEntity* entity)
{
    return entity->GetQTNode()->Update(entity, &mRoot, mExpandable);
}

int QuadTree::GetIntersections(const AABB& queryBox, std::vector<QTEntity*>& outResults)
{
    return mRoot->GetIntersections(queryBox, outResults);
}

QuadTree::~QuadTree()
{
    delete mRoot;
}

void QuadTree::Traverse(std::function<bool(QTNode*)> func)
{
    return mRoot->Traverse(func);
}

void QuadTree::TraverseEntities(std::function<bool(QTEntity*)> func)
{
    return mRoot->TraverseEntities(func);
}

/**
 * Some tests, not part of the tree functionality
 */
class StrEnt : public QTEntity {
public:
    StrEnt(const AABB& aabb, const std::string& str)
        : mStr(str)
        , mAABB(aabb)
    {
    }
    ~StrEnt() {}
    AABB GetAABB() { return mAABB; }

    void SetAABB(const AABB& aabb) { mAABB = aabb; }
    std::string mStr;

private:
    AABB mAABB;
    QTNode* mQTNode;
};

bool QuadTree::Test()
{
    QuadTree tree;

    std::cout << "Testing Quad Tree" << std::endl;

    tree.Init(AABB(-10, -10, 10, 10), false);
    assert(tree.mItems == 0);
    assert(tree.mRoot != nullptr);
    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++)
        assert(tree.mRoot->mChildren[i] == nullptr);

    AABB box(3, 5, 5, 7);
    StrEnt* strEnt;
    strEnt = new StrEnt(box, "First Object generic data");
    assert(tree.Add(strEnt));
    assert(tree.mRoot->mEntities.size() == 1);
    assert(tree.mRoot->IsLeaf());
    assert(tree.mRoot->mEntities.back()->GetAABB().minX == 3);
    assert(tree.mRoot->mEntities.back()->GetAABB().maxX == 5);

    box = AABB(-5, 2, -3, 4);
    std::string str2("Second Object");
    strEnt = new StrEnt(box, str2);
    assert(tree.Add(strEnt));
    assert(tree.mRoot->mEntities.size() == 2);
    assert(tree.mRoot->IsLeaf()); // still leaf for 2 nodes
    assert(tree.mRoot->mEntities.back()->GetAABB().minX == -5);
    assert(tree.mRoot->mEntities.back()->GetAABB().maxX == -3);
    assert(str2.compare(((StrEnt*)tree.mRoot->mEntities.back())->mStr) == 0);

    // Here the node should split //
    box = AABB(-5, -7, -3, -5);
    strEnt = new StrEnt(box, "Third Object");
    assert(tree.Add(strEnt));
    assert(tree.mRoot->mEntities.size() == 0);
    assert(tree.mRoot->IsEmpty());
    assert(!tree.mRoot->IsLeaf());
    assert(tree.mRoot->mChildren[NE]->mEntities.size() == 1);
    assert(tree.mRoot->mChildren[NW]->mEntities.size() == 1);
    assert(tree.mRoot->mChildren[SW]->mEntities.size() == 1);
    assert(tree.mRoot->mChildren[SE]->mEntities.size() == 0);

    box = AABB(2, 4, 4, 6);
    strEnt = new StrEnt(box, "Forth Object");
    assert(tree.Add(strEnt));

    box = AABB(1, 1, 3, 3);
    strEnt = new StrEnt(box, "Fifth Object");
    assert(tree.Add(strEnt));

    box = AABB(6, 6, 8, 8);
    strEnt = new StrEnt(box, "Sixth Object");
    assert(tree.Add(strEnt));

    std::vector<QTEntity*> results;
    AABB queryBox = AABB(0, 0, 10, 10);
    int numRes = tree.GetIntersections(queryBox, results);
    assert(numRes == 4);
    assert(results.size() == 4);
    for (uint i = 0; i < results.size(); i++)
        assert(results[i]->GetAABB().Intersects(queryBox));

    auto delEntity = [](QTEntity* entity) {
        delete entity;
        return true;
    };
    tree.TraverseEntities(delEntity);

    QuadTree t2;
    t2.Init(AABB(0, 0, 10, 10), false);
    box = AABB(2, 2, 3, 3);
    strEnt = new StrEnt(box, "Rect 1");
    t2.Add(strEnt);
    box = AABB(3, 3, 4, 4);
    StrEnt* obj1 = new StrEnt(box, "Rect 2");
    t2.Add(obj1);
    box = AABB(4, 4, 5, 5);
    StrEnt* obj2 = new StrEnt(box, "Rect 3");
    t2.Add(obj2);

    box = AABB(2, 2, 4, 4);
    obj1->SetAABB(box);
    t2.Update(obj1);

    assert(t2.mRoot->mChildren[2]->mEntities.size() == 2);
    obj2->SetAABB(box);
    t2.Update(obj2);

    assert(t2.mRoot->mChildren[2]->mEntities.size() == 3);

    t2.TraverseEntities(delEntity);

    return true;
}
}
