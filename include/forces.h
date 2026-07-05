#pragma once
#include "particle.h"
#include <vector>

namespace physim {

constexpr double G        = 6.674e-11;  //gravitational constant
constexpr double K_E      = 8.988e9;    //Coulomb constant
constexpr double SOFTENING = 1e-5;      //softening length to avoid singularities

class ForceCalculator {
public:
    virtual ~ForceCalculator() = default;
    /**
    This is a function to accumulate the force this calculator contributes between two particles, applying it to their force members.
    Input(s):
    - a: the first particle
    - b: the second particle
    Output(s):
    - None: mutates the force members of a and/or b
    **/
    virtual void accumulate(Particle& a, Particle& b) const = 0;
};

class GravitationalForce : public ForceCalculator {
public:
    /**
    This is a function to accumulate the Newtonian gravitational force between two particles.
    Input(s):
    - a: the first particle
    - b: the second particle
    Output(s):
    - None: mutates the force members of a and b according to their masses and separation
    **/
    void accumulate(Particle& a, Particle& b) const override;
};

class ElectrostaticForce : public ForceCalculator {
public:
    /**
    This is a function to accumulate the Coulomb electrostatic force between two charged particles.
    Input(s):
    - a: the first particle
    - b: the second particle
    Output(s):
    - None: mutates the force members of a and b according to their charges and separation
    **/
    void accumulate(Particle& a, Particle& b) const override;
};

class SpringForce : public ForceCalculator {
public:
    /**
    This is a function to construct a spring force calculator with a stiffness and rest length.
    Input(s):
    - k: the spring stiffness constant
    - rest_length: the spring's natural (unstretched) length
    Output(s):
    - None: initializes the calculator's stiffness and rest length
    **/
    explicit SpringForce(double k, double rest_length)
        : k_(k), rest_(rest_length) {}
    /**
    This is a function to accumulate the spring force pulling/pushing two particles toward the rest length.
    Input(s):
    - a: the first particle
    - b: the second particle
    Output(s):
    - None: mutates the force members of a and b based on displacement from the rest length
    **/
    void accumulate(Particle& a, Particle& b) const override;
private:
    double k_, rest_;
};

} //namespace physim
