#include "shadowcube.h"
#include <QtDebug>

ShadowCube::ShadowCube()
{
}

ShadowCube::ShadowCube(const int &width, const int &height)
     :bufferWidth(width),
      bufferHeight(height)
{
    for (int i = 0; i < 6; i++)
    {
        shadowBuffers.push_back(std::vector<std::vector<double>> (bufferHeight, std::vector<double>(bufferWidth, std::numeric_limits<double>::max())));
    }
}

double ShadowCube::getDepthByVector(const Vector3D<double> &vec) const
{
    double absX = fabs(vec.x());
    double absY = fabs(vec.y());
    double absZ = fabs(vec.z());

    int isXPositive = vec.x() > -EPS ? 1 : 0;
    int isYPositive = vec.y() > -EPS ? 1 : 0;
    int isZPositive = vec.z() > -EPS ? 1 : 0;
    int index;
    double maxAxis, u, v;

    // POSITIVE X
    if (isXPositive && absX >= absY && absX >= absZ)
    {
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        u = -vec.z();
        v = vec.y();
        index = 0;
    }
    // NEGATIVE X
    else if (!isXPositive && absX >= absY && absX >= absZ)
    {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        u = vec.z();
        v = vec.y();
        index = 1;
    }
    // POSITIVE Y
    else if (isYPositive && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        u = vec.x();
        v = -vec.z();
        index = 2;
    }
    // NEGATIVE Y
    else if (!isYPositive && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        u = vec.x();
        v = vec.z();
        index = 3;
    }
    // POSITIVE Z
    else if (isZPositive && absZ >= absX && absZ >= absY)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        u = vec.x();
        v = vec.y();
        index = 4;
    }
    // NEGATIVE Z
    else
    {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        u = -vec.x();
        v = vec.y();
        index = 5;
    }

    // Convert range from -1 to 1 to 0 to 1
    u = 0.5 * (u / maxAxis + 1.0) * bufferWidth;
    v = bufferHeight - 0.5 * (v / maxAxis + 1.0) * bufferHeight;
    int ui = std::min(static_cast<int>(u), bufferWidth - 1);
    int vi = std::min(static_cast<int>(v), bufferHeight - 1);
    return shadowBuffers[index][vi][ui];
}

void ShadowCube::clear()
{
    for (int i = 0; i < 6; i++)
    {
        shadowBuffers[i] = std::vector<std::vector<double>> (bufferHeight, std::vector<double>(bufferWidth, std::numeric_limits<double>::max()));
    }
}
