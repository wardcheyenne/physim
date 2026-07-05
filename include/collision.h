#pragma once
#include "particle.h"
#include <vector>

namespace physim {

struct CollisionEvent {
    size_t  i, j;       //indices of colliding particles
    double  time;       //simulation time of collision
    Vector3 normal;     //collision normal pointing from i to j
};

class CollisionDetector {
public:
    virtual ~CollisionDetector() = default;
    /**
    This is a function to detect all collisions among a set of particles at a given simulation time.
    Input(s):
    - particles: the particles to check for collisions
    - time: the current simulation time, recorded on any detected events
    Output(s):
    - std::vector<CollisionEvent>: the collision events found, each with the colliding particle indices, time, and collision normal
    **/
    virtual std::vector<CollisionEvent> detect(
        const std::vector<Particle>& particles, double time) const = 0;
};

class SphereCollisionDetector : public CollisionDetector {
public:
    /**
    This is a function to detect collisions between particles treated as spheres, based on their positions and radii.
    Input(s):
    - particles: the particles to check for collisions
    - time: the current simulation time, recorded on any detected events
    Output(s):
    - std::vector<CollisionEvent>: the collision events found where particle spheres overlap
    **/
    std::vector<CollisionEvent> detect(
        const std::vector<Particle>& particles, double time) const override;
};

class CollisionResolver {
public:
    virtual ~CollisionResolver() = default;
    /**
    This is a function to resolve a collision between two particles, updating their velocities and/or positions.
    Input(s):
    - a: the first colliding particle
    - b: the second colliding particle
    - ev: the collision event describing the contact normal and time
    Output(s):
    - None: mutates a and b to reflect the outcome of the collision
    **/
    virtual void resolve(Particle& a, Particle& b, const CollisionEvent& ev) = 0;
};

class ElasticCollisionResolver : public CollisionResolver {
public:
    /**
    This is a function to construct an elastic collision resolver with a given coefficient of restitution.
    Input(s):
    - restitution: the coefficient of restitution controlling bounce energy retention (default 1.0, fully elastic)
    Output(s):
    - None: initializes the resolver's restitution coefficient
    **/
    explicit ElasticCollisionResolver(double restitution = 1.0)
        : restitution_(restitution) {}
    /**
    This is a function to resolve a collision between two particles using elastic collision physics.
    Input(s):
    - a: the first colliding particle
    - b: the second colliding particle
    - ev: the collision event describing the contact normal and time
    Output(s):
    - None: mutates a and b's velocities based on the elastic collision response and configured restitution
    **/
    void resolve(Particle& a, Particle& b, const CollisionEvent& ev) override;
private:
    double restitution_;
};

} //namespace physim
