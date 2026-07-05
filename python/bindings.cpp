/**
 * Python bindings for physim using pybind11.
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "simulation.h"
#include "forces.h"
#include "integrators.h"
#include "collision.h"

namespace py = pybind11;
using namespace physim;

PYBIND11_MODULE(physim, m) {
    m.doc() = "physim: N-body and particle physics simulation engine";

    py::class_<Vector3>(m, "Vector3")
        .def(py::init<double, double, double>(), py::arg("x")=0, py::arg("y")=0, py::arg("z")=0)
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z)
        .def("norm",       &Vector3::norm)
        .def("norm2",      &Vector3::norm2)
        .def("dot",        &Vector3::dot)
        .def("cross",      &Vector3::cross)
        .def("normalized", &Vector3::normalized)
        .def("__repr__",   [](const Vector3& v) {
            return "Vector3(" + std::to_string(v.x) + ", " +
                   std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
        });

    py::class_<Particle>(m, "Particle")
        .def(py::init<uint64_t, double, Vector3, Vector3, double, double>(),
             py::arg("id"), py::arg("mass"),
             py::arg("position") = Vector3::zero(),
             py::arg("velocity") = Vector3::zero(),
             py::arg("charge") = 0.0,
             py::arg("radius") = 1.0)
        .def_readwrite("id",       &Particle::id)
        .def_readwrite("mass",     &Particle::mass)
        .def_readwrite("charge",   &Particle::charge)
        .def_readwrite("radius",   &Particle::radius)
        .def_readwrite("position", &Particle::position)
        .def_readwrite("velocity", &Particle::velocity)
        .def_readwrite("fixed",    &Particle::fixed)
        .def_readwrite("label",    &Particle::label)
        .def("kinetic_energy",     &Particle::kinetic_energy)
        .def("momentum",           &Particle::momentum);

    //Fix: return copies of particle list to avoid dangling references
    //when Python holds a reference past the next step() call.
    py::class_<Simulation>(m, "Simulation")
        .def(py::init([](const std::string& integrator) {
            if (integrator != "euler")
                throw std::invalid_argument("Only 'euler' integrator is available via Python bindings");
            return std::make_unique<Simulation>(std::make_unique<EulerIntegrator>());
        }), py::arg("integrator") = "euler")
        .def("add_particle",            &Simulation::add_particle)
        .def("add_gravitational_force", [](Simulation& s) { s.add_force(std::make_shared<GravitationalForce>()); })
        .def("add_electrostatic_force", [](Simulation& s) { s.add_force(std::make_shared<ElectrostaticForce>()); })
        .def("add_sphere_collisions",   [](Simulation& s, double r) {
            s.set_collision_detector(std::make_shared<SphereCollisionDetector>());
            s.set_collision_resolver(std::make_shared<ElasticCollisionResolver>(r));
        }, py::arg("restitution") = 1.0)
        .def("step",  &Simulation::step)
        .def("run",   [](Simulation& s, double t, double dt) { s.run(t, dt); })
        .def("particles", [](const Simulation& s) { return s.particles(); })  //returns copy
        .def("time",  &Simulation::time)
        .def("total_kinetic_energy", &Simulation::total_kinetic_energy);
}
