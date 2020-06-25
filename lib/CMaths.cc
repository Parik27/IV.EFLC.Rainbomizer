#include "CMaths.hh"
#include <cmath>

void
Vector2::rotate(const float angle)
{
    float sinA = sinf(angle);
    float cosA = cosf(angle);

    float aux_x = x;
    float aux_y = y;

    x = cosA * aux_x - sinA * aux_y;
    y = sinA * aux_x + cosA * aux_y;
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

void
Matrix34::rotateX(const float angle)
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

void
Matrix34::rotateZ(const float angle)
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

void
Matrix34::rotateY (const float angle)
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

void
Matrix34::rotate (const Vector3 &angles)
{
    if (angles.x)
        rotateX(angles.x);
    
    if (angles.y)
        rotateY(angles.y);

    if (angles.z)
        rotateZ(angles.z);
}
