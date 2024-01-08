#include "system.hpp"
#include "logging.hpp"

#include <algorithm>
#include <format>
#include <future>
#include <numbers>
#include <ranges>
#include <tuple>

System::System() {}

System::~System() {}

auto System::transform_vectors()
    -> std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> {
    // TODO: (aver) convert to openmp
    //
    auto future_x = std::async(std::launch::async, [&]() {
        auto transformed_x = m_particles | std::views::transform([&](const Particle3D &particle) {
                                 return particle.position.x();
                             });
        std::vector<double> transformed_vector_x(transformed_x.begin(), transformed_x.end());
        return transformed_vector_x;
    });
    auto future_y = std::async(std::launch::async, [&]() {
        auto transformed_y = m_particles | std::views::transform([&](const Particle3D &particle) {
                                 return particle.position.y();
                             });
        std::vector<double> transformed_vector_y(transformed_y.begin(), transformed_y.end());
        return transformed_vector_y;
    });
    auto future_z = std::async(std::launch::async, [&]() {
        auto transformed_z = m_particles | std::views::transform([&](const Particle3D &particle) {
                                 return particle.position.z();
                             });
        std::vector<double> transformed_vector_z(transformed_z.begin(), transformed_z.end());
        return transformed_vector_z;
    });

    auto x = future_x.get();
    auto y = future_y.get();
    auto z = future_z.get();

    return {x, y, z};
}
// TODO: (aver) add min function
auto System::get_max_distance() -> Particle3D {
    auto result =
        std::max_element(m_particles.begin(), m_particles.end(), [](Particle3D a, Particle3D b) {
            return a.distance < b.distance;
        });

    result->print_summary();

    return *result;
}
auto System::calc_total_mass() -> void {
    m_total_mass = 0.;

#pragma omp parallel for
    for (const auto &part : m_particles) {
#pragma omp atomic
        m_total_mass += part.mass;
    }

    // NOTE: (aver) consider simpler omp loop
    // std::mutex mtx;
    // std::for_each(
    //     std::execution::par, particles.begin(), particles.end(), [&mtx](const Particle3D &part) {
    //         std::lock_guard<std::mutex> guard(mtx); // Protect m_shells
    //         Particles::g_total_mass += part.mass;
    //     });

    Logging::info(std::format("Total mass of system: {}", m_total_mass));
}

auto System::calc_half_mass(const ShellVec &shells) -> double {

    auto temp_mass = 0.;
    for (const auto &shell : shells) {
        if (temp_mass >= m_total_mass * 0.5) {
            return temp_mass;
        }
        temp_mass += shell.m_mass;
    }
    Logging::err("No half mass found... Exiting");
    std::exit(-1);
}
auto System::update_min_rad(const double rad) -> void { m_min_rad = std::min(m_min_rad, rad); }
auto System::update_max_rad(const double rad) -> void { m_max_rad = std::max(m_max_rad, rad); }
