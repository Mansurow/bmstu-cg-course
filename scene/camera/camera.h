#ifndef CAMERA_H
#define CAMERA_H

#include "geometry/geometry.hpp"

using namespace GeometrySpace;

inline double radians(const double &angle) { return angle * M_PI / 180; }
inline double degrees(const double &angle) { return angle * 180 / M_PI; }

struct ViewFrustrum
{
    double field_of_view;
    double near_plane;
    double far_plane;
    double aspect_ratio;
};

struct CameraAttributes
{
    Vector3D<double> position;
    double yaw;
    double pitch;
    Vector3D<double> top;
    ViewFrustrum frustrum;
};

class Camera
{
public:
    Camera();
    Camera(const CameraAttributes &attributes);
    inline void setPosition(const Vector3D<double> &position) { position_ = position; }
    inline void setDirection(const Vector3D<double> &direction) { direction_ = direction; }
    inline void setUp(const Vector3D<double> &up) { up_ = up; }
    inline void setRight(const Vector3D<double> &right) { right_ = right; }
    inline void setTop(const Vector3D<double> &top) { top_ = top; }
    inline void setYaw(const double &yaw) { yaw_ = yaw; }
    inline void setPitch(const double &pitch) { pitch_ = pitch; }
    inline void setViewFrustrum(const ViewFrustrum &frustrum) { frustrum_ = frustrum; }
    inline Vector3D<double> getPosition() const { return position_; }
    inline Vector3D<double> getDirection() const { return direction_; }
    inline Vector3D<double> getUp() const { return up_; }
    inline Vector3D<double> getRight() const { return right_; }
    inline ViewFrustrum getViewFrustrum() const { return frustrum_; }
    inline Vector3D<double> getTop() const { return top_; }
    inline double getYaw() const { return yaw_; }
    inline double getPitch() const { return pitch_; }
    void updateCoords();
private:
    Vector3D<double> position_;
    Vector3D<double> direction_;
    Vector3D<double> up_;
    Vector3D<double> right_;
    Vector3D<double> top_;
    double yaw_;
    double pitch_;
    ViewFrustrum frustrum_;
};


#endif // CAMERA_H
