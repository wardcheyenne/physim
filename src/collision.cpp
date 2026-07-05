#include "collision.h"
#include <cmath>

namespace physim {

/**
This is a function to detect collisions between particles treated as spheres, based on their positions and radii.
Input(s):
- particles: the particles to check for collisions
- time: the current simulation time, recorded on any detected events
Output(s):
- std::vector<CollisionEvent>: the collision events found where particle spheres overlap
**/
std::vector<CollisionEvent> SphereCollisionDetector::detect(
    const std::vector<Particle>& particles, double time) const
{
    std::vector<CollisionEvent> events;
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            const auto& a = particles[i];
            const auto& b = particles[j];
            Vector3 r  = b.position - a.position;
            double  d  = r.norm();
            double  sr = a.radius + b.radius;
            if (d < sr && d > 1e-15) {
                events.push_back({i, j, time, r * (1.0 / d)});
            }
        }
    }
    return events;
}

/**
This is a function to resolve a collision between two particles using elastic collision physics, applying an impulse along the collision normal and correcting overlap.
Input(s):
- a: the first colliding particle
- b: the second colliding particle
- ev: the collision event describing the contact normal and time
Output(s):
- None: mutates a and b's velocities and positions
**/
void ElasticCollisionResolver::resolve(
    Particle& a, Particle& b, const CollisionEvent& ev)
{
    //Fix: use the collision normal from the event, not recompute
    const Vector3& n = ev.normal;
    Vector3 dv = b.velocity - a.velocity;
    double  vn = dv.dot(n);
    if (vn >= 0) return;  //already separating

    double j_num = -(1.0 + restitution_) * vn;
    double j_den = 1.0/a.mass + 1.0/b.mass;
    double impulse = j_num / j_den;

    a.velocity -= n * (impulse / a.mass);
    b.velocity += n * (impulse / b.mass);

    //Positional correction to prevent overlap
    double overlap = (a.radius + b.radius) - (b.position - a.position).norm();
    if (overlap > 0) {
        double corr = overlap * 0.5;
        a.position -= n * corr;
        b.position += n * corr;
    }
}

} //namespace physim
