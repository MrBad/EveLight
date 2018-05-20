#pragma once

#include "entity.h"
#include <functional>
#include <list>

enum {
    NE,
    NW,
    SW,
    SE,
    QTNODE_NUM_CHILDREN
};
// Number of objects a node should keep, before split. This need to be tuned.
// NOTE: Tests are made only for value of 2
#define QT_TREE_MAX_OBJECTS 2;

struct QTNode;
class QuadTree;

/* TODO: this should be an interface, implemented by Entity. 
 *   It should take care of FitsIn, to be more generic.
 */
struct QTObject {
    AABB limits;
    QTNode* node;
    void* data;

    QTObject(const AABB& bounds, void* data);
    ~QTObject();
};

class QTNode {
public:
    const AABB& GetLimits() const { return mLimits; }
    uint GetNumObjects() const { return mObjects.size(); }
    uint GetDepth() const { return mDepth; }
    void SetDepth(uint depth) { mDepth = depth; }

private:
    AABB mLimits; // This node boundaries.
    QTNode* mParent; // Parent node.
    QTNode* mChildren[QTNODE_NUM_CHILDREN]; // Children nodes.
    std::list<QTObject*> mObjects; // Objects belonging to this node.
    uint mDepth;

    QTNode(const AABB& limits, QTNode* parent);
    ~QTNode();
    bool Add(QTObject* object);
    bool Update(QTObject* obj, const AABB& newLimits, QTNode** root, bool expand);
    int GetIntersections(const AABB& queryBox, std::vector<QTObject*>& outResults);

    void Split();
    QTNode* Expand(const AABB& newLimits);
    bool IsLeaf() const;
    bool IsEmpty() const;
    bool HasRoom() const;
    bool BrothersAreEmpty() const;
    int GetChildIndex(QTObject* obj) const;
    bool DeleteUp();
    void Traverse(std::function<bool(QTNode*)> cbFunc);

    friend class QuadTree;
};

class QuadTree {
public:
    QuadTree();
    ~QuadTree();
    void Init(const AABB& limits, bool expandable);
    uint GetItems() { return mItems; }
    QTObject* Add(const AABB& limits, void* data);
    bool Update(QTObject* obj, const AABB& limits);
    int GetIntersections(const AABB& queryBox, std::vector<QTObject*>& outResults);
    void Traverse(std::function<bool(QTNode*)> cbFunc);

    static bool Test();

private:
    QTNode* mRoot;
    uint mItems;
    bool mExpandable;
};
