#include "forces.h"
#include <cmath>

namespace physim {

/**
This is a function to accumulate the Newtonian gravitational force between two particles.
Input(s):
- a: the first particle
- b: the second particle
Output(s):
- None: mutates the force members of a and b according to their masses and separation, skipping fixed particles
**/
void GravitationalForce::accumulate(Particle& a, Particle& b) const {
    Vector3 r   = b.position - a.position;
    double  d2  = r.norm2() + SOFTENING * SOFTENING;
    double  d   = std::sqrt(d2);
    double  mag = G * a.mass * b.mass / d2;
    Vector3 f   = r * (mag / d);
    if (!a.fixed) a.force += f;
    if (!b.fixed) b.force -= f;
}

/**
This is a function to accumulate the Coulomb electrostatic force between two charged particles.
Input(s):
- a: the first particle
- b: the second particle
Output(s):
- None: mutates the force members of a and b according to their charges and separation, skipping fixed particles
**/
void ElectrostaticForce::accumulate(Particle& a, Particle& b) const {
    Vector3 r   = b.position - a.position;
    double  d2  = r.norm2() + SOFTENING * SOFTENING;
    double  d   = std::sqrt(d2);
    double  mag = K_E * a.charge * b.charge / d2;
    Vector3 f   = r * (mag / d);
    if (!a.fixed) a.force -= f;
    if (!b.fixed) b.force += f;
}

/**
This is a function to accumulate the spring force pulling/pushing two particles toward the rest length.
Input(s):
- a: the first particle
- b: the second particle
Output(s):
- None: mutates the force members of a and b based on displacement from the rest length, skipping fixed particles; does nothing if the particles are coincident
**/
void SpringForce::accumulate(Particle& a, Particle& b) const {
    Vector3 r      = b.position - a.position;
    double  d      = r.norm();
    if (d < 1e-15) return;
    double  stretch = d - rest_;
    Vector3 f       = r * (k_ * stretch / d);
    if (!a.fixed) a.force += f;
    if (!b.fixed) b.force -= f;
}

} //namespace physim
