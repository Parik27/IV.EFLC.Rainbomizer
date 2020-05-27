#pragma once

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Vector3_pad
{
    float x;
    float y;
    float z;
    float __pad;
};

struct Matrix34
{
    Vector3_pad right;
    Vector3_pad up;
    Vector3_pad at;
    Vector3_pad pos;

    void scale (const Vector3 &coef, bool adjustTransform);
};
