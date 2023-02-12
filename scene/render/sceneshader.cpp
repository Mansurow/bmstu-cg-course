#include "sceneshader.h"

SceneShader::SceneShader()
    : mvpMatrix_(4, 4),
      invVpMatrix_(4, 4),
      modelMatrix_(4, 4),
      vpMatrix_(4, 4),
      triangle_(3),
      w_(3)
{
    planes_ = { {-1, 0, 0, 1},
                {1, 0, 0, 1},
                {0, -1, 0, 1},
                {0, 1, 0, 1},
                {0, 0, 1, 1},
                {0, 0, -1, 1}};
}

SceneShader::SceneShader(const Matrix<double> &vpMatrix, const Matrix<double> &modelMatrix,
               const Vector3D<double> &cameraPosition, const Vector3D<double> &lightPosition)
    : SceneShader()
{
    mvpMatrix_ = modelMatrix * vpMatrix;
    modelMatrix_ = modelMatrix;
    vpMatrix_ = vpMatrix;
    invVpMatrix_ = vpMatrix;
    invVpMatrix_.inverse();
    cameraPosition_ = cameraPosition;
    lightPosition_ = lightPosition;
}

void SceneShader::setVpMatrix(const Matrix<double> &vpMatrix)
{
    vpMatrix_ = vpMatrix;
    invVpMatrix_ = vpMatrix;
    invVpMatrix_.inverse();
}

void SceneShader::setModelMatrix(const Matrix<double> &modelMatrix)
{
    modelMatrix_ = modelMatrix;
    mvpMatrix_ = modelMatrix_ * vpMatrix_;
}

void SceneShader::setMaterial(const Material &material)
{
    material_.diffuse_ = material.diffuse_;
    material_.specular_ = material.specular_;
    material_.shininess_ = material.shininess_;
}

int SceneShader::vertex(std::vector<Vector4D<double>> &result, const std::vector<Vector3D<double>> &triangle, const Vector3D<double> &normal)
{
    int count = 0;
    normal_ = normal * modelMatrix_;
    if (normal_ * ((triangle[0] ^ modelMatrix_) - cameraPosition_) < EPS)
    {
        std::vector<Vector4D<double>> clippedPolygon(9);
        for (int i = 0; i < 3; i++)
        {
            clippedPolygon[i] = triangle[i];
            clippedPolygon[i].setW(1);
            clippedPolygon[i] = clippedPolygon[i] * mvpMatrix_;
        }
        count = 3;
        for (int i = 0; i < (int) planes_.size(); i++)
        {
            count = clipPolygon(result, clippedPolygon, planes_[i], count);
            if (count == 0) break;
            clippedPolygon = result;
        }
    }
    return count;
}

void SceneShader::geometry(const std::vector<Vector4D<double>> &triangle)
{
    for (int i = 0; i < 3; i++)
    {
        triangle_[i] = triangle[i] * invVpMatrix_;
        w_[i] = 1 / triangle[i].w();
    }
}

Color SceneShader::fragment(const Vector3D<double> &barycentric, const ShadowModel &shadowModel) const
{
    Vector3D<double> bar(barycentric);
    bar.setX(bar.x() * w_[0]);
    bar.setY(bar.y() * w_[1]);
    bar.setZ(bar.z() * w_[2]);
    double w = 1 / (bar.x() + bar.y() + bar.z());
    Vector3D<double> position((triangle_[0] * bar.x()  + triangle_[1] * bar.y() + triangle_[2] * bar.z()) * w);
    Vector3D<double> lightDir(position - lightPosition_);
    double ambient = 0.1;
//    double depth = lightDir.module();
//    qDebug() << depth << shadowCube.getDepthByVector(lightDir);
    if (lightDir.module() - 5 < shadowModel.getDepthByVector(lightDir))
    {
        lightDir = lightDir * (-1);
        Vector3D<double> eyeDir(cameraPosition_ - position);
        lightDir.normalize();
        eyeDir.normalize();
        Vector3D<double> halfWay(lightDir + eyeDir);
        halfWay.normalize();
        double specComponent = std::pow(std::max(normal_ * halfWay, 0.0), material_.shininess_);
        double diffComponent = std::max(normal_ * lightDir, 0.0);
        return lightColor_ * (material_.specular_ * specComponent + material_.diffuse_ * diffComponent) +  material_.diffuse_ * ambient;
    }
    return material_.diffuse_ * ambient;
}

double SceneShader::countShadowDepth(Vector3D<double> &barycentric) const
{
    barycentric.setX(barycentric.x() * w_[0]);
    barycentric.setY(barycentric.y() * w_[1]);
    barycentric.setZ(barycentric.z() * w_[2]);
    double w = 1 / (barycentric.x() + barycentric.y() + barycentric.z());
    return (lightPosition_ - ((triangle_[0] * barycentric.x()  + triangle_[1] * barycentric.y() + triangle_[2] * barycentric.z()) * w)).module();
}

void SceneShader::findIntersection(Vector4D<double> &C, const Vector4D<double> &plane, const Vector4D<double> &A, const Vector4D<double> &B) const
{
    double dA = plane * A;
    double dB = plane * B;
    double t = dA / (dA - dB);
    C = A + (B - A) * t;
}

int SceneShader::clipPolygon(std::vector<Vector4D<double>> &result, const std::vector<Vector4D<double>> &polygon, const Vector4D<double> &plane, const int &length) const
{
    int first = length - 1;
    int second;
    int count = 0;
    Vector4D<double> intersection;
    for (int i = 0; i < length; i++)
    {
        second = i;
        if (pointIsVisible(plane, polygon[second]))
        {
            if (pointIsVisible(plane, polygon[first]))
            {
                result[count] = polygon[second];
                count++;
            }
            else
            {
                findIntersection(intersection, plane, polygon[first], polygon[second]);
                result[count] = intersection;
                count++;
                result[count] = polygon[second];
                count++;
            }
        }
        else
        {
            if (pointIsVisible(plane, polygon[first]))
            {
                findIntersection(intersection, plane, polygon[first], polygon[second]);
                result[count] = intersection;
                count++;
            }
        }
        first = second;
    }
    return count;
}
