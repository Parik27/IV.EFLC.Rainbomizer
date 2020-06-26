#include "CMaths.hh"
#include <cmath>

void
Vector2::rotate(const SinCos &sinCos)
{
    float aux_x = x;
    float aux_y = y;

    x = sinCos.cosA * aux_x - sinCos.sinA * aux_y;
    y = sinCos.sinA * aux_x + sinCos.cosA * aux_y;
}

void
Vector2::rotate(const float angle)
{
    rotate (SinCos (angle));
}

Vector3 &Vector3::operator+= (const Vector3 &rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    
    return *this;
}

void
Matrix34::scale (const Vector3 &scale, bool adjustTransform)
{
    if (adjustTransform)
        {
            this->right.x = this->right.x * scale.x;
            this->right.y = this->right.y * scale.x;
            this->right.z = this->right.z * scale.x;
            this->up.x    = scale.y * this->up.x;
            this->up.y    = scale.y * this->up.y;
            this->up.z    = scale.y * this->up.z;
            this->at.x    = scale.z * this->at.x;
            this->at.y    = scale.z * this->at.y;
            this->at.z    = scale.z * this->at.z;
        }
    else
        {
            this->right.x = this->right.x * scale.x;
            this->right.y = this->right.y * scale.y;
            this->right.z = this->right.z * scale.z;
            this->up.x    = this->up.x * scale.x;
            this->up.y    = scale.y * this->up.y;
            this->up.z    = this->up.z * scale.z;
            this->at.x    = this->at.x * scale.x;
            this->at.y    = this->at.y * scale.y;
            this->at.z    = this->at.z * scale.z;
            this->pos.x   = this->pos.x * scale.x;
            this->pos.y   = scale.y * this->pos.y;
            this->pos.z   = this->pos.z * scale.z;
        }
}

template<typename T>
void
Matrix34::rotateX(const T angle)
{
    Vector2 v;

    // X axis
    v.x = up.x;
    v.y = at.x;
    v.rotate(angle);
    up.x = v.x;
    at.x = v.y;

    // Y axis
    v.x = up.y;
    v.y = at.y;
    v.rotate(angle);
    up.y = v.x;
    at.y = v.y;

    // Z axis
    v.x = up.z;
    v.y = at.z;
    v.rotate(angle);
    up.z = v.x;
    at.z = v.y;
    
}

template<typename T>
void
Matrix34::rotateZ(const T angle)
{
    Vector2 v;
    
    // X axis
    v.x = right.x;
    v.y = up.x;
    v.rotate (angle);
    right.x = v.x;
    up.x = v.y;

    // Y axis
    v.x = right.y;
    v.y = up.y;
    v.rotate (angle);
    right.y = v.x;
    up.y = v.y;

    // Z axis
    v.x = right.z;
    v.y = up.z;
    v.rotate (angle);
    right.z = v.x;
    up.z    = v.y;
}

template<typename T>
void
Matrix34::rotateY (const T angle)
{
    Vector2 v;
    
    v.x = at.x;
    v.y = right.x;
    v.rotate(angle);
    at.x = v.x;
    right.x = v.y;

    // eje Y
    v.x = at.y;
    v.y = right.y;
    v.rotate(angle);
    at.y = v.x;
    right.y = v.y;

    // eje Z
    v.x = at.z;
    v.y = right.z;
    v.rotate(angle);
    at.z = v.x;
    right.z = v.y;
}

template<typename T>
void
Matrix34::rotate (const T &angles)
{
    rotateX(angles.x);
    rotateY(angles.y);
    rotateZ(angles.z);
}

SinCos::SinCos(float angle)
{
    *this = angle;
}

SinCos &SinCos::operator= (const float &other)
{
    sinA = sinf (other);
    cosA = cosf (other);

    return *this;
}

Vector3_SinCos &Vector3_SinCos::operator= (const Vector3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    return *this;
}

template void Matrix34::rotate<Vector3>(const Vector3&);
template void Matrix34::rotate<Vector3_SinCos>(const Vector3_SinCos&);

template void Matrix34::rotateX<float>(const float);
template void Matrix34::rotateX<SinCos>(const SinCos);

template void Matrix34::rotateY<float>(const float);
template void Matrix34::rotateY<SinCos>(const SinCos);

template void Matrix34::rotateZ<float>(const float);
template void Matrix34::rotateZ<SinCos>(const SinCos);
