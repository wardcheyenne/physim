#pragma once
#include "vector3.h"
#include "particle.h"
#include <vector>
#include <memory>
#include <array>

namespace physim {

struct AABB {
    Vector3 center, half;

    /**
    This is a function to check whether a point lies within this axis-aligned bounding box.
    Input(s):
    - p: the point to test
    Output(s):
    - bool: true if p is within half-extents of center on all axes (with a small tolerance), false otherwise
    **/
    bool contains(const Vector3& p) const {
        return std::abs(p.x - center.x) < half.x + 1e-12 &&
               std::abs(p.y - center.y) < half.y + 1e-12 &&
               std::abs(p.z - center.z) < half.z + 1e-12;
    }

    /**
    This is a function to check whether this bounding box overlaps another.
    Input(s):
    - o: the other AABB to test against
    Output(s):
    - bool: true if the boxes overlap on all axes, false otherwise
    **/
    bool intersects(const AABB& o) const {
        return std::abs(center.x - o.center.x) < (half.x + o.half.x) &&
               std::abs(center.y - o.center.y) < (half.y + o.half.y) &&
               std::abs(center.z - o.center.z) < (half.z + o.half.z);
    }
};

class Octree {
public:
    static constexpr int MAX_DEPTH    = 8;
    static constexpr int MAX_PER_NODE = 8;

    /**
    This is a function to construct an Octree node covering a given bounding region at a given depth.
    Input(s):
    - bounds: the AABB region this node covers
    - depth: this node's depth in the tree (default 0, the root)
    Output(s):
    - None: initializes the node's bounds and depth with no children and no stored particles
    **/
    explicit Octree(AABB bounds, int depth = 0)
        : bounds_(bounds), depth_(depth) {}

    /**
    This is a function to insert a particle pointer into the octree, subdividing this node if it exceeds capacity.
    Input(s):
    - p: pointer to the particle to insert
    Output(s):
    - None: stores p in this node or recursively in a child node
    **/
    void insert(const Particle* p);
    /**
    This is a function to collect all particles whose node(s) overlap a query region.
    Input(s):
    - region: the AABB region to query
    - out: output vector to append matching particle pointers into
    Output(s):
    - None: appends matching particle pointers to out
    **/
    void query(const AABB& region, std::vector<const Particle*>& out) const;
    /**
    This is a function to remove all particles and child nodes from this octree node.
    Input(s):
    - None
    Output(s):
    - None: clears stored particles and destroys child nodes, marking the node as not subdivided
    **/
    void clear();

private:
    AABB bounds_;
    std::vector<const Particle*> data_;
    std::array<std::unique_ptr<Octree>, 8> children_;
    bool subdivided_ = false;
    int  depth_      = 0;

    /**
    This is a function to split this node into 8 child octants and redistribute its stored particles among them.
    Input(s):
    - None
    Output(s):
    - None: creates the 8 child Octree nodes and marks this node as subdivided
    **/
    void subdivide();
    /**
    This is a function to compute the bounding box of one of this node's 8 child octants.
    Input(s):
    - idx: the child octant index (0-7)
    Output(s):
    - AABB: the bounding box of the specified child octant
    **/
    AABB child_bounds(int idx) const;
};

} //namespace physim
