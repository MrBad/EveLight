#pragma once

#include "aabb.h"
#include <functional>
#include <iostream>
#include <list>
#include <vector>

enum {
    NE,
    NW,
    SW,
    SE,
    QTNODE_NUM_CHILDREN
};

/**
 * Number of objects a node should keep, before split. This need to be tuned.
 *  NOTE: Tests are made only for value of 2, but this needs to be tuned.
 */
#define QT_TREE_MAX_OBJECTS 2;

struct QTNode;
class QuadTree;

/**
 * This will need to be extended by the objects using the tree.
 *  TODO: Write FitsIn / Intersects virtual functions instead GetAABB.
 */
class QTEntity {
public:
    /* This will need to be implemented by Entity object */
    virtual AABB GetAABB() = 0;
    virtual ~QTEntity() {}

    void SetQTNode(QTNode* node) { mQTNode = node; }
    QTNode* GetQTNode() { return mQTNode; }

private:
    QTNode* mQTNode;
};

class QTNode {
public:
    const AABB& GetLimits() const { return mLimits; }
    uint GetNumObjects() const { return mEntities.size(); }
    uint GetDepth() const { return mDepth; }
    void SetDepth(uint depth) { mDepth = depth; }

private:
    AABB mLimits; // This node boundaries.
    QTNode* mParent; // Parent node.
    QTNode* mChildren[QTNODE_NUM_CHILDREN]; // Children nodes.
    std::list<QTEntity*> mEntities; // Objects belonging to this node.
    uint mDepth;

    QTNode(const AABB& limits, QTNode* parent);
    ~QTNode();
    bool Add(QTEntity* entity);
    bool Update(QTEntity* entity, QTNode** root, bool expand);
    int GetIntersections(const AABB& queryBox, std::vector<QTEntity*>& outResults);

    void Split();
    QTNode* Expand(const AABB& newLimits);
    bool IsLeaf() const;
    bool IsEmpty() const;
    bool HasRoom() const;
    bool BrothersAreEmpty() const;
    int GetChildIndex(QTEntity* entity) const;
    bool DeleteUp();
    void Traverse(std::function<bool(QTNode*)> func);
    void TraverseEntities(std::function<bool(QTEntity*)> func);

    friend class QuadTree;
};

class QuadTree {
public:
    QuadTree();
    ~QuadTree();
    void Init(const AABB& limits, bool expandable);
    uint GetItems() { return mItems; }
    bool Add(QTEntity* entity);
    bool Update(QTEntity* entity);
    int GetIntersections(const AABB& queryBox, std::vector<QTEntity*>& outResults);
    void Traverse(std::function<bool(QTNode*)> func);
    void TraverseEntities(std::function<bool(QTEntity*)> func);

    static bool Test();

private:
    QTNode* mRoot;
    uint mItems;
    bool mExpandable;
};
