#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <variant>

#include "scene/camera/camera.h"


struct CameraRotation
{
    double dYaw;
    double dPitch;
};

struct CameraMovement
{
    bool forward;
    bool backward;
    bool left;
    bool right;
};

class CameraManager
{
public:
    CameraManager();
    CameraManager(const double &speed, const double &sensitivity);

    inline void setSpeed(const double &speed) { speed_ = speed; }
    inline void setSensitivity(const double &sensitivity) { sensitivity_ = sensitivity; }
    inline double getSpeed() const { return speed_; }
    inline double getSensitivity() const{ return sensitivity_; }
    void rotate(Camera &camera, const CameraRotation &rotation);
    void move(Camera &camera, const CameraMovement &movement);
    Matrix<double> getLookAt(const Camera &camera) const;
    Matrix<double> getRotation(const Camera &camera) const;
    Matrix<double> getProjection(const Camera &camera) const;
private:
    double speed_;
    double sensitivity_;
};

#endif // CAMERAMANAGER_H
