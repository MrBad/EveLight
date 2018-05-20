#include "quad_tree.h"
#include "aabb.h"

// QTObject methods

QTObject::QTObject(const AABB& bounds, void* data)
    : limits(bounds)
    , data(data)
{
}

QTObject::~QTObject()
{
}

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
    return mObjects.size() < QT_TREE_MAX_OBJECTS;
}

bool QTNode::IsEmpty() const
{
    return mObjects.size() == 0;
}

bool QTNode::BrothersAreEmpty() const
{
    uint numChlildrendObjects = 0;

    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++) {
        numChlildrendObjects += mParent->mChildren[i]->mObjects.size();
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
int QTNode::GetChildIndex(QTObject* obj) const
{
    assert(!IsLeaf());

    for (uint i = 0; i < QTNODE_NUM_CHILDREN; i++)
        if (obj->limits.FitsIn(mChildren[i]->mLimits))
            return i;

    return -1;
}

/**
 * Allocates the children nodes and their boundaries for this.
 */
void QTNode::Split()
{
    glm::vec2 center = mLimits.GetCenter();

    mChildren[NE] = new QTNode(AABB(center.x, center.y, mLimits.maxX, mLimits.maxY), this);
    mChildren[NW] = new QTNode(AABB(mLimits.minX, center.y, center.x, mLimits.maxY), this);
    mChildren[SW] = new QTNode(AABB(mLimits.minX, mLimits.minY, center.x, center.y), this);
    mChildren[SE] = new QTNode(AABB(center.x, mLimits.minY, mLimits.maxX, center.y), this);
}

bool QTNode::Add(QTObject* obj)
{
    bool split, anyMoved;
    split = anyMoved = false;
    if (!obj->limits.FitsIn(mLimits))
        return false;

    if (IsLeaf()) {
        if (HasRoom()) {
            mObjects.push_back(obj);
            obj->node = this;
            return true;
        } else {
            Split();
            split = true;
            // Move old objects down to children, if possible //
            for (auto it = mObjects.begin(); it != mObjects.end();) {
                QTObject* oldObj = *it;
                int idx = GetChildIndex(oldObj);
                if (idx > -1) {
                    if (mChildren[idx]->Add(oldObj)) {
                        it = mObjects.erase(it);
                        anyMoved = true;
                        continue;
                    }
                }
                it++;
            }
        }
    }
    // Add the new object //
    int idx = GetChildIndex(obj);
    if (idx > -1) {
        mChildren[idx]->Add(obj);
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
        mObjects.push_back(obj);
        obj->node = this;
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

bool QTNode::Update(QTObject* obj, const AABB& newLimits, QTNode** root, bool expand)
{
    // If we still fit in this node, and this node is split
    //   check if we can fit into a child.
    if (newLimits.FitsIn(mLimits)) {
        obj->limits = newLimits;
        if (!IsLeaf()) {
            int idx = GetChildIndex(obj);
            if (idx > -1) {
                mObjects.remove(obj);
                return mChildren[idx]->Add(obj);
            }
        }
        return true;
    }

    // Climb up the tree until we found a parent that can adopt our new limits.
    bool found = false;
    QTNode* newNode;
    for (newNode = obj->node; newNode; newNode = newNode->mParent) {
        if (newLimits.FitsIn(newNode->mLimits)) {
            found = true;
            break;
        }
    }

    if (!found) {
        if (!expand) {
            std::cerr << "Request for a limit out of root range. Update fail!" << std::endl;
            return false;
        }
        assert(root);
        std::cout << "info: Expand root on update" << std::endl;
        newNode = *root = (*root)->Expand(newLimits);
    }

    mObjects.remove(obj);
    obj->limits = newLimits;
    bool ret = newNode->Add(obj);

    if (IsLeaf() && IsEmpty() && BrothersAreEmpty())
        mParent->DeleteUp();

    return ret;
}

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

int QTNode::GetIntersections(const AABB& queryBox, std::vector<QTObject*>& outResults)
{
    int numRes = 0;
    if (!mLimits.Intersects(queryBox))
        return numRes;

    for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
        QTObject* obj = *it;
        if (obj->limits.Intersects(queryBox)) {
            outResults.push_back(obj);
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

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        delete *it;
}

void QTNode::Traverse(std::function<bool(QTNode*)> cbFunc)
{
    for (int i = 0; i < QTNODE_NUM_CHILDREN; i++) {
        if (mChildren[i])
            mChildren[i]->Traverse(cbFunc);
    }

    cbFunc(this);
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

QTObject* QuadTree::Add(const AABB& limits, void* data)
{
    if (!limits.FitsIn(mRoot->mLimits)) {
        if (!mExpandable) {
            std::cerr
                << "Requested limits out of tree max range. Will not expand root!"
                << std::endl;
            return nullptr;
        }
        mRoot = mRoot->Expand(limits);
        printf("Expanded Root\n");
    }
    QTObject* obj = new QTObject(limits, data);
    if (!mRoot->Add(obj)) {
        delete obj;
        return nullptr;
    }
    mItems++;

    return obj;
}

bool QuadTree::Update(QTObject* obj, const AABB& newLimits)
{
    return obj->node->Update(obj, newLimits, &mRoot, mExpandable);
}

int QuadTree::GetIntersections(const AABB& queryBox, std::vector<QTObject*>& outResults)
{
    return mRoot->GetIntersections(queryBox, outResults);
}

QuadTree::~QuadTree()
{
    delete mRoot;
}

void QuadTree::Traverse(std::function<bool(QTNode*)> cbFunc)
{
    return mRoot->Traverse(cbFunc);
}

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
    assert(tree.Add(box, (void*)"First Object generic data"));
    assert(tree.mRoot->mObjects.size() == 1);
    assert(tree.mRoot->IsLeaf());
    assert(tree.mRoot->mObjects.back()->limits.minX == 3);
    assert(tree.mRoot->mObjects.back()->limits.maxX == 5);

    box = AABB(-5, 2, -3, 4);
    std::string str2("Second Object");
    assert(tree.Add(box, (void*)str2.c_str()));
    assert(tree.mRoot->mObjects.size() == 2);
    assert(tree.mRoot->IsLeaf()); // still leaf for 2 nodes
    assert(tree.mRoot->mObjects.back()->limits.minX == -5);
    assert(tree.mRoot->mObjects.back()->limits.maxX == -3);
    assert(str2.compare((char*)tree.mRoot->mObjects.back()->data) == 0);

    // Here the node should split //
    box = AABB(-5, -7, -3, -5);
    assert(tree.Add(box, (void*)"Third Object"));
    assert(tree.mRoot->mObjects.size() == 0);
    assert(tree.mRoot->IsEmpty());
    assert(!tree.mRoot->IsLeaf());
    assert(tree.mRoot->mChildren[NE]->mObjects.size() == 1);
    assert(tree.mRoot->mChildren[NW]->mObjects.size() == 1);
    assert(tree.mRoot->mChildren[SW]->mObjects.size() == 1);
    assert(tree.mRoot->mChildren[SE]->mObjects.size() == 0);

    box = AABB(2, 4, 4, 6);
    assert(tree.Add(box, (void*)"Forth Object"));

    box = AABB(1, 1, 3, 3);
    assert(tree.Add(box, (void*)"Fifth Object"));

    box = AABB(6, 6, 8, 8);
    assert(tree.Add(box, (void*)"Sixth Object"));

    std::vector<QTObject*> results;
    AABB queryBox = AABB(0, 0, 10, 10);
    int numRes = tree.GetIntersections(queryBox, results);
    assert(numRes == 4);
    assert(results.size() == 4);
    for (uint i = 0; i < results.size(); i++)
        assert(results[i]->limits.Intersects(queryBox));

    QuadTree t2;
    t2.Init(AABB(0, 0, 10, 10), false);
    box = AABB(2, 2, 3, 3);
    t2.Add(box, (void*)"Rect 1");
    box = AABB(3, 3, 4, 4);
    QTObject* obj1 = t2.Add(box, (void*)"Rect 2");
    box = AABB(4, 4, 5, 5);
    QTObject* obj2 = t2.Add(box, (void*)"Rect 3");

    box = AABB(2, 2, 4, 4);
    t2.Update(obj1, box);
    assert(t2.mRoot->mChildren[2]->mObjects.size() == 2);
    t2.Update(obj2, box);
    assert(t2.mRoot->mChildren[2]->mObjects.size() == 3);

    return true;
}
