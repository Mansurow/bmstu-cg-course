#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <QImage>
#include <QPainter>
#include <chrono>
#include <thread>
#include "scene/light/light.h"
#include "scene/render/sceneshader.h"
#include "scene/render/lightshader.h"
#include "scene/render/shadowmodel.h"

struct ThreadParams
{
    int startRow;
    int endRow;
    int startCol;
    int endCol;
};

using namespace GeometrySpace;

class RenderManager
{
public:
    RenderManager();
    RenderManager(QImage *frame, const int &sc_width, const int &sc_height);
    void renderModel(BaseShader &shader, const BaseModel &model, const int &index);
    void renderShadowModel(SceneShader &shader, const BaseModel &model, const int &bufferIndex);
    void renderCoordinateSystem(const Matrix<double> &rotation);
    inline int getIndexByPosition(const int &x, const int &y) const { return static_cast<int>(objectsBuffer[y][x]); }
    void clearFrame();
    inline void clearShadow() { shadowModel.clear(); }
private:
    void renderTriangle(std::vector<Vector3D<double>> &triangle, const char &objectIndex, const BaseShader &shader);
    void renderShadowTriangle(std::vector<Vector3D<double>> &triangle, const int &bufferIndex, const SceneShader &shader);
    void renderFrameBuffer(ThreadParams params, const std::vector<Vector3D<double>> &triangle, const char &objectIndex, const BaseShader &shader, const double &square);
    void barycentric(Vector3D<double> &barCoords, const std::vector<Vector3D<double>> &triangle, const QPoint &P, const double &square);
    void viewPort(Vector3D<double> &point, const int &wdith, const int &height);
    int screenWidth;
    int screenHeight;
    std::vector<std::vector<double>> depthBuffer; // z-буфер
    std::vector<std::vector<char>> objectsBuffer;
    QImage *frameBuffer; // буфер-кадра
    ShadowModel shadowModel;
};

#endif // RENDERMANAGER_H
