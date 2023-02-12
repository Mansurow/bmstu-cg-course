#include "cameramanager.h"

CameraManager::CameraManager()
{
}

CameraManager::CameraManager(const double &speed, const double &sensitivity)
    : speed_(speed),
      sensitivity_(sensitivity)
{
}

void CameraManager::rotate(Camera &camera, const CameraRotation &rotation)
{
    double yaw = camera.getYaw();
    double pitch = camera.getPitch();
    yaw += rotation.dYaw * sensitivity_;
    pitch += rotation.dPitch * sensitivity_;
    if (pitch >= 90)
    {
        pitch = 90;
    }
    else if (pitch <= -90)
    {
        pitch = -90;
    }
    if (yaw >= 180)
    {
        yaw = -180;
    }
    else if (yaw <= -180)
    {
        yaw = 180;
    }
    camera.setYaw(yaw);
    camera.setPitch(pitch);
    camera.updateCoords();
}

void CameraManager::move(Camera &camera, const CameraMovement &movement)
{
    Vector3D<double> position(camera.getPosition());
    Vector3D<double> direction(camera.getDirection());
    Vector3D<double> right(camera.getRight());
    if (movement.forward)
        position += direction * speed_;
    if (movement.backward)
        position -= direction * speed_;
    if (movement.left)
        position -= right * speed_;
    if (movement.right)
        position += right * speed_;
    camera.setPosition(position);
}

Matrix<double> CameraManager::getLookAt(const Camera &camera) const
{
    Matrix<double> rotate(4,4);
    Matrix<double> translate(4,4);
    Vector3D<double> direction(camera.getDirection());
    Vector3D<double> up(camera.getUp());
    Vector3D<double> right(camera.getRight());
    Vector3D<double> position(camera.getPosition());
    translate.makeIdentity();
    rotate[3][3] = 1;
    for (int i = 0; i < 3; i++)
    {
        rotate[i][0] = right[i];
        rotate[i][1] = up[i];
        rotate[i][2] = direction[i];
        translate[3][i] = -position[i];
    }
    return translate * rotate;
}

Matrix<double> CameraManager::getRotation(const Camera &camera) const
{
    Matrix<double> rotate(4,4);
    Vector3D<double> direction(camera.getDirection());
    Vector3D<double> up(camera.getUp());
    Vector3D<double> right(camera.getRight());
    rotate[3][3] = 1;
    for (int i = 0; i < 3; i++)
    {
        rotate[i][0] = right[i];
        rotate[i][1] = up[i];
        rotate[i][2] = direction[i];
    }
    return rotate;
}

Matrix<double> CameraManager::getProjection(const Camera &camera) const {
    ViewFrustrum frustrum = camera.getViewFrustrum();
    Matrix<double> projection(4,4);
    projection[1][1] = 1 / tan(radians(frustrum.field_of_view / 2));
    projection[0][0] = projection[1][1] /frustrum.aspect_ratio;
    projection[2][2] = (frustrum.far_plane + frustrum.near_plane) / (frustrum.far_plane - frustrum.near_plane);
    projection[3][2] = -2 * frustrum.near_plane * frustrum.far_plane / (frustrum.far_plane - frustrum.near_plane);
    projection[2][3] = 1;
    return projection;
}

