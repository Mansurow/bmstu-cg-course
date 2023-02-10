#ifndef SHADOWCUBE_H
#define SHADOWCUBE_H

#include <vector>
#include "geometry/geometry.hpp"

using namespace GeometrySpace;

class ShadowCube
{
public:
    ShadowCube();
    ShadowCube(const int &width, const int &height);
    inline void setPixel(const int &bufferIndex, const int &x, const int &y, const double &z) { shadowBuffers[bufferIndex][y][x] = z; }
    inline double getDepthByIndex(const int &bufferIndex, const int &x, const int &y) const { return shadowBuffers[bufferIndex][y][x]; }
    double getDepthByVector(const Vector3D<double> &vec) const;
    inline int getWidth() { return bufferWidth; }
    inline int getHeight() { return bufferHeight; }
    void clear();
private:
    std::vector<std::vector<std::vector<double>>> shadowBuffers;
    int bufferWidth;
    int bufferHeight;
};

#endif // SHADOWCUBE_H
