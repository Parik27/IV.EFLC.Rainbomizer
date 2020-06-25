#pragma once

struct Vector2
{
    float x;
    float y;

    void rotate(const float angle);
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

    void rotateX (const float angle);
    void rotateY (const float angle);
    void rotateZ (const float angle);
    
    void rotate (const Vector3 &angles);
};

struct grcColour
{
    float r;
    float g;
    float b;
    float _pad;
};
