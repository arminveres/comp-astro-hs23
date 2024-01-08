#ifndef PARTICLES_H_
#define PARTICLES_H_

#include <Eigen/Dense>
#include <vector>

class Particle3D {
  public:
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    double softening;
    double potential;
    double distance;
    double mass;

    /// Calculate the Norm of the position vector and set the value
    auto calc_orign_distance() -> void;

    /// Print a summary of the particle properties
    auto print_summary() const -> void;
};

/// Utility functions to do work with a `std::vector<Particle3D>`
namespace Particles {

/// Keep track of total mass of all particles in system
extern double g_total_mass;
extern double g_min_rad;
extern double g_max_rad;

/// @brief Return the particle that is the furthest away
/// @note In a running system, the distances need to be calculated at each step
auto get_max_distance(const std::vector<Particle3D> &particles) -> Particle3D;

/// Calculate the total mass and set the global variable `g_total_mass`
auto calc_total_mass(const std::vector<Particle3D> &particles) -> void;
} // namespace Particles

#endif // ! PARTICLES_H_
