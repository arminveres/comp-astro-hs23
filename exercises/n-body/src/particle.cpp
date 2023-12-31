#include "particle.hpp"

#include <algorithm>
#include <iostream>

auto Particle3D::calc_orign_distance() -> void { this->distance = this->position.norm(); }

auto Particle3D::print_summary() const -> void {
    std::cout << "Position = " << this->position << std::endl;
    std::cout << "Potential = " << this->potential << std::endl;
    std::cout << "Velocity = " << this->velocity << std::endl;
    std::cout << "Distance = " << this->distance << std::endl;
}

auto Particles::get_max_distance(const std::vector<Particle3D> &particles) -> Particle3D {
    auto result =
        std::max_element(particles.begin(), particles.end(), [](Particle3D a, Particle3D b) {
            return a.distance < b.distance;
        });

    result->print_summary();

    return *result;
}
