#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "scene/render/baseshader.h"

class LightShader : public BaseShader
{
public:
    LightShader();
    void setVpMatrix(const Matrix<double> &vpMatrix);
    void setModelMatrix(const Matrix<double> &modelMatrix);
    inline void setCameraPosition(const Vector3D<double> &cameraPosition) { cameraPosition_ = cameraPosition; }
    inline void setLightColor(const Color &color) { lightColor_ = color; }
    inline Vector3D<double> getCameraPosition() const { return cameraPosition_; }
    inline Matrix<double> getMVPMatrix() const { return mvpMatrix_; }
    inline Matrix<double> getModelMatrix() const { return modelMatrix_; }
    inline Matrix<double> getVPMatrix() const { return vpMatrix_; }
    int vertex(std::vector<Vector4D<double>> &result, const std::vector<Vector3D<double>> &triangle, const Vector3D<double> &normal) override;
    inline void geometry(const std::vector<Vector4D<double>> &triangle) override {}
    inline Color fragment(const Vector3D<double> &barycentric, const ShadowModel &shadowCube) const override { return lightColor_; }
    ~LightShader() override {}
private:
    inline bool pointIsVisible(const Vector4D<double> &plane, const Vector4D<double> &point) const { return (plane * point) > -EPS; }
    void findIntersection(Vector4D<double> &C, const Vector4D<double> &plane, const Vector4D<double> &A, const Vector4D<double> &B) const;
    int clipPolygon(std::vector<Vector4D<double>> &result, const std::vector<Vector4D<double>> &polygon, const Vector4D<double> &plane, const int &length) const;
    Matrix<double> mvpMatrix_;
    Matrix<double> invVpMatrix_;
    Matrix<double> modelMatrix_;
    Matrix<double> vpMatrix_;
    Vector3D <double> cameraPosition_;
    Vector3D<double> normal_;
    std::vector<Vector4D<double>> planes_;
    Color lightColor_;

};

#endif // LIGHTSHADER_H
