#include "camera.h"

Camera::Camera()
{
}

Camera::Camera(const CameraAttributes &attributes)
    : Camera()
{
    position_ = attributes.position;
    top_ = attributes.top;
    yaw_ = attributes.yaw;
    pitch_ = attributes.pitch;
    frustrum_ = attributes.frustrum;
    updateCoords();
}

void Camera::updateCoords()
{
    double yaw = radians(yaw_);
    double pitch = radians(pitch_);
    direction_.setX(cos(yaw) * cos(pitch));
    direction_.setY(sin(pitch));
    direction_.setZ(sin(yaw) * cos(pitch));
    direction_.normalize();
    right_ = top_ ^ direction_;
    right_.normalize();
    up_ = direction_ ^ right_;
    up_.normalize();
}
