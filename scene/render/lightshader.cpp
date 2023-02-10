#include "lightshader.h"

LightShader::LightShader()
    : mvpMatrix_(4, 4),
      invVpMatrix_(4, 4),
      modelMatrix_(4, 4),
      vpMatrix_(4, 4)
{
    planes_ = { {-1, 0, 0, 1},
                {1, 0, 0, 1},
                {0, -1, 0, 1},
                {0, 1, 0, 1},
                {0, 0, 1, 1},
                {0, 0, -1, 1}};
}

void LightShader::setVpMatrix(const Matrix<double> &vpMatrix)
{
    vpMatrix_ = vpMatrix;
    invVpMatrix_ = vpMatrix;
    invVpMatrix_.inverse();
}

void LightShader::setModelMatrix(const Matrix<double> &modelMatrix)
{
    modelMatrix_ = modelMatrix;
    mvpMatrix_ = modelMatrix_ * vpMatrix_;
}

int LightShader::vertex(std::vector<Vector4D<double> > &result, const std::vector<Vector3D<double> > &triangle, const Vector3D<double> &normal)
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
        for (int i = 0; i < planes_.size(); i++)
        {
            count = clipPolygon(result, clippedPolygon, planes_[i], count);
            if (count == 0) break;
            clippedPolygon = result;
        }
    }
    return count;

}

void LightShader::findIntersection(Vector4D<double> &C, const Vector4D<double> &plane, const Vector4D<double> &A, const Vector4D<double> &B) const
{
    double dA = plane * A;
    double dB = plane * B;
    double t = dA / (dA - dB);
    C = A + (B - A) * t;
}

int LightShader::clipPolygon(std::vector<Vector4D<double> > &result, const std::vector<Vector4D<double> > &polygon, const Vector4D<double> &plane, const int &length) const
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


