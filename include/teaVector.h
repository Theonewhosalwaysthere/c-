#pragma once
#ifndef TEA_VECTOR_H
#define TEA_VECTOR_H
#endif

// floats

typedef struct {
    float x, y;
} teaVector2f;

typedef struct {
    float x, y, z;
} teaVector3f;

typedef struct {
    float x, y, z, w;
} teaVector4f;

// ints

typedef struct {
    int x, y;
} teaVector2i;

typedef struct {
    int x, y, z;
} teaVector3i;

typedef struct {
    int x, y, z, w;
} teaVector4i;

// 8bit / chars

struct teaVector3Color
{
    union
    {
        struct
        {
            unsigned char x, y, z;
        };
        struct
        {
            unsigned char r, g, b;
        };
        unsigned char rgb[3]; // access as array
    };

    teaVector3Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0)
        : x(r), y(g), z(b) {}

    bool operator==(const teaVector3Color& other) const {
    return r == other.r && g == other.g && b == other.b;

    }

    teaVector3Color operator+(const teaVector3Color& other) const {
        return teaVector3Color(r + other.r, g + other.g, b + other.b);
    }
};