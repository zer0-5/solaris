#pragma once

#include "vec2.hpp"

#include <array>
// #include <boost/container_hash/hash.hpp>
#include <iostream>
#include <unordered_map>

class Vec3 {
  private:
    float _x, _y, _z;
    Vec3(float x, float y, float z) : _x(x), _y(y), _z(z) {}

  public:
    Vec3() = default;
    Vec3(Vec2 v2) : _x(v2.x()), _y(0), _z(v2.y()) {}
    static Vec3 cartesian(float, float, float);
    static Vec3 spherical(float, float, float);
    float x() const;
    float y() const;
    float z() const;
    void set_x(float);
    void set_y(float);
    void set_z(float);
    void sum_x(float);
    void sum_y(float);
    void sum_z(float);
    float radius() const noexcept;
    float alpha() const noexcept;
    float beta() const noexcept;
    void sum_alpha(float);
    void sum_beta(float);
    Vec3& normalize() noexcept;
    Vec3& dot(Vec3) noexcept;
    Vec3& cross(Vec3) noexcept;
    Vec3 operator+(Vec3) const noexcept;
    Vec3& operator+=(const Vec3&) noexcept;
    Vec3 operator-(Vec3) const noexcept;
    Vec3& operator-=(const Vec3&) noexcept;
    Vec3 operator*(float) const noexcept;
    Vec3& operator*=(const float&) noexcept;
    bool operator==(const Vec3&) const noexcept;
    bool operator!=(const Vec3&) const noexcept;
    auto friend operator<<(std::ostream&, Vec3 const&) -> std::ostream&;
};

// namespace std {
// template <>
// struct hash<Vec3> {
//     size_t operator()(const Vec3& p) const {
//         size_t seed = 0;
//         boost::hash_combine(seed, p.x());
//         boost::hash_combine(seed, p.y());
//         boost::hash_combine(seed, p.z());
//
//         return seed;
//     }
// };
// } // namespace std

Vec3 operator*(float, Vec3) noexcept;
