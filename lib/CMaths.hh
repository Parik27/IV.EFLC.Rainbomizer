#pragma once

struct SinCos
{
    float sinA;
    float cosA;

    SinCos () = default;
    
    SinCos (float angle);
    SinCos &operator= (const float &other);
};

struct Vector2
{
    float x;
    float y;

    void rotate(const float angle);
    void rotate (const SinCos &sinCos);
};

struct Vector3
{
    float x;
    float y;
    float z;

    Vector3 &operator+= (const Vector3 &rhs);

    friend Vector3
    operator+ (Vector3 lhs, const Vector3 &rhs)
    {
        lhs += rhs;
        return lhs;
    }
};

struct Vector3_SinCos
{
    SinCos x;
    SinCos y;
    SinCos z;

    Vector3_SinCos &operator= (const Vector3 &other);
};

struct Vector3_pad : Vector3
{
    float __pad;
};

struct Matrix34
{
    Vector3_pad right;
    Vector3_pad up;
    Vector3_pad at;
    Vector3_pad pos;

    void scale (const Vector3 &coef, bool adjustTransform);

    template<typename T>
    void rotateX (const T angle);
    template<typename T>
    void rotateY (const T angle);
    template<typename T>
    void rotateZ (const T angle);

    template<typename T>
    void rotate (const T &angles);
};

struct grcColour
{
    float r;
    float g;
    float b;
    float _pad;
};
