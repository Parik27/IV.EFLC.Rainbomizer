#include "CMaths.hh"

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
