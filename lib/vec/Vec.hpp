#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdint>

#define PI 3.14159265

template<typename T, size_t N> class Vec;

template<typename T> class Vec<T, 3> {
    public:
        union {T x; T u; T r;};
        union {T y; T v; T g;};
        union {T z; T w; T b;};

        // Constructors
        constexpr Vec(T _x, T _y, T _z);
        constexpr Vec();
        constexpr Vec(Vec<T, 3> const& vec_copy);

        static constexpr unsigned int size() {return 3;}
        typedef unsigned int size_type;

        // Accessing components with []
        /*
        Accessing by reference with the ability to change the value
        */ 
        constexpr T& operator[](size_type i);
        /*
        Accessing by reference without the ability to change the value
            T& const - a constant reference to a T (REDUNDANT)
                Because a reference refers to the same object since its creation
                the const is redundant
            
            T const& - a reference to a constant T (READ ONLY)
                T cannot be modified through this, compiler will give error
            
            const T& - a reference to a T& that happens to be constant
                ***equivelent to T const &***

            const after function definition means the method of the class
            cannot modify any member variables
        */ 
        constexpr T const& operator[](size_type i) const;

        constexpr Vec& operator=(Vec<T, 3> const& copy_vector);

        constexpr T length() const;
        
        constexpr Vec<T, 3> normalize() const;
};

// Basic constructor 
template<typename T>
inline constexpr Vec<T, 3>::Vec(T _x, T _y, T _z)
    :   x { _x }, y { _y }, z { _z } 
{}

// Zero constructor
template <typename T>
inline constexpr Vec<T, 3>::Vec()
    :   x {(T) 0}, y {(T) 0}, z {(T) 0} 
{}

// Copy constructor
template <typename T>
inline constexpr Vec<T, 3>::Vec(Vec<T, 3> const &vec_copy)
    :   x { vec_copy.x }, y { vec_copy.y }, z { vec_copy.z } 
{}

template <typename T>
inline constexpr T &Vec<T, 3>::operator[](size_type i)
{
    assert(i < this->size());
    switch(i)
    {
    default:
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    }
}

template<typename T>
inline constexpr T const& Vec<T, 3>::operator[](size_type i) const
{
    assert(i < this->size());
    switch(i)
    {
    default:
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    }
}


template<typename T>
inline constexpr Vec<T, 3>& Vec<T, 3>::operator=(Vec<T, 3> const& copy_vector)
{
    this->x = static_cast<T>(copy_vector.x);
    this->y = static_cast<T>(copy_vector.y);
    this->z = static_cast<T>(copy_vector.z);
    return *this;
}

template <typename T>
inline constexpr T Vec<T, 3>::length() const
{
    return std::sqrt(x*x + y*y + z*z);
}

template <typename T>
inline constexpr Vec<T, 3> Vec<T, 3>::normalize() const
{
    T l = length();
    if (l == (T)0){
        // std::cout << "length of a vector trying to be normalized was zero!\n";
        return Vec<T, 3>(0.0f, 0.0f, 0.0f);
    }
    return Vec<T, 3>(x / l, y / l, z / l);
}

template<typename T> 
inline constexpr std::ostream& operator<<(std::ostream& os, const Vec<T, 3>& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Vec<uint8_t, 3>& vec) {
    os << "(" << (int)vec.x << ", " << (int)vec.y << ", " << (int)vec.z << ")";
    return os;
}

template<typename T> 
inline constexpr Vec<T, 3> operator-(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2) {
    return Vec<T, 3>(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

template<typename T> 
inline constexpr Vec<T, 3> operator+(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2) {
    return Vec<T, 3>(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

template<typename T> 
inline constexpr Vec<T, 3> operator-(const Vec<T, 3>& vec) {
    return Vec<T, 3>(-vec.x, -vec.y, -vec.z);
}

template<typename T>
inline constexpr Vec<T, 3> operator*(const T& scalar, const Vec<T, 3>& vec) {
    return Vec<T, 3>(scalar * vec.x, scalar * vec.y, scalar * vec.z);
}

inline constexpr Vec<uint8_t, 3> operator*(const float& scalar, const Vec<uint8_t, 3>& vec) {
    return Vec<uint8_t, 3>(std::round(scalar * (float)vec.x), std::round(scalar * (float)vec.y), std::round(scalar * (float)vec.z));
}

template<typename T> 
inline constexpr Vec<T, 3> operator*(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2) {
    return Vec<T, 3>(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
}

template<typename T> 
inline constexpr Vec<T, 3> operator/( const Vec<T, 3>& vec, const T& scalar) {
    return Vec<T, 3>(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

// bitwise and with 2 vectors is dot product
template<typename T> 
inline constexpr T operator&(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

// bitwise or with 2 vectors is cross product
template<typename T> 
inline constexpr Vec<T, 3> operator|(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2) {
    return Vec<T, 3> (
        (vec1.y*vec2.z - vec1.z*vec2.y),
        (vec1.z*vec2.x - vec1.x*vec2.z),
        (vec1.x*vec2.y - vec1.y*vec2.x)
    );
}

template<typename T> 
inline constexpr bool operator==(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2)
{
   if (vec1.x == vec2.x
       && vec1.y == vec2.y
       && vec1.z == vec2.z)
      return true;
  return false;
}


// Short hand type definitions
typedef Vec<float, 3> vec3f;

typedef Vec<uint8_t, 3> vec3u8;

static constexpr uint8_t span_u8(const float& f) {
    if (f > 1.0f){
        return 255;
    }
    else if (f < 0.0f){
        return 0;
    }
    else {
        return static_cast<uint8_t>(f * 255);
    }
}

inline vec3u8 to_vec3u8(const vec3f& color){
    return vec3u8{span_u8(color.x), span_u8(color.y), span_u8(color.z)};
}

inline vec3f to_vec3f(vec3u8 col){
    return vec3f(((float)col.x / 255.0f), ((float)col.y / 255.0f), ((float)col.z / 255.0f));
}
