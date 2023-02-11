#include "rendermanager.h"

#include <QDebug>

RenderManager::RenderManager()
{
}

RenderManager::RenderManager(QImage *frame, const int &sc_width, const int &sc_height)
    : screenWidth(sc_width),
      screenHeight(sc_height),
      frameBuffer(frame),
      shadowModel(1024, 1024)
{
    depthBuffer = std::vector<std::vector<double>> (screenHeight, std::vector<double>(screenWidth, 2));
    objectsBuffer = std::vector<std::vector<char>> (screenHeight, std::vector<char>(screenWidth, -1));
    frameBuffer->fill(Qt::black);
}

// отрисовка модели
void RenderManager::renderModel(BaseShader &shader, const BaseModel &model, const int &index)
{
    char objectIndex = index;
    std::vector<Vector3D<double>> triangle(3);
    std::vector<Vector4D<double>> result(9);
    int count;
    for (int j = 0; j < model.countTriangles(); j++)
    {
        model.getTriangle(triangle, j);
        count = shader.vertex(result, triangle, model.getNormal(j));
        for (int i = 0; i < count - 2; i++)
        {
            shader.geometry({result[i+2], result[i+1], result[0]});
            triangle[2] = result[0];
            triangle[1] = result[i + 1];
            triangle[0] = result[i + 2];
            renderTriangle(triangle, objectIndex, shader);
        }
    }
}

// отрисовка теней модели
void RenderManager::renderShadowModel(SceneShader &shader, const BaseModel &model, const int &bufferIndex)
{
    std::vector<Vector3D<double>> triangle(3);
    std::vector<Vector4D<double>> result(9);
    int count;
    for (int j = 0; j < model.countTriangles(); j++)
    {
        model.getTriangle(triangle, j);
        count = shader.vertex(result, triangle, model.getNormal(j));
        for (int i = 0; i < count - 2; i++)
        {
            shader.geometry({result[i+2], result[i+1], result[0]});
            triangle[2] = result[0];
            triangle[1] = result[i + 1];
            triangle[0] = result[i + 2];
            renderShadowTriangle(triangle, bufferIndex, shader);
        }
    }
}

// отрисовка координатной оси и ее поворот
void RenderManager::renderCoordinateSystem(const Matrix<double> &rotation)
{
    std::vector<Vector3D<double>> system {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    for (auto &element: system)
    {
        element = element * rotation;
    }
    std::vector<QColor> colors{qRgb(26, 143, 17), qRgb(186, 19, 7), qRgb(11, 57, 143)};
    std::vector<char> letters{'X', 'Y', 'Z'};
    if (system[0].z() > system[1].z())
    {
        qSwap(system[0], system[1]);
        qSwap(colors[0], colors[1]);
        qSwap(letters[0], letters[1]);
    }
    if (system[1].z() > system[2].z())
    {
        qSwap(system[1], system[2]);
        qSwap(colors[1], colors[2]);
        qSwap(letters[1], letters[2]);
    }
    if (system[0].z() > system[1].z())
    {
        qSwap(system[0], system[1]);
        qSwap(colors[0], colors[1]);
        qSwap(letters[0], letters[1]);
    }
    QPainter painter(frameBuffer);
    QFont font = painter.font();
    font.setPixelSize(18);
    painter.setFont(font);
    QPoint center(110, screenHeight - 50);
    for (int i = 2; i >= 0; i--)
    {
        painter.setPen(QPen(colors[i], 5));
        painter.drawLine(center.x(), center.y(), center.x() + system[i].x() * 80, center.y() - system[i].y() * 80);
        painter.drawText(center.x() + system[i].x() * 100, center.y() - system[i].y() * 100, QString(letters[i]));
    }
}

void RenderManager::clearFrame()
{
    frameBuffer->fill(Qt::black);
    depthBuffer = std::vector<std::vector<double>> (screenHeight, std::vector<double>(screenWidth, 2));
    objectsBuffer = std::vector<std::vector<char>> (screenHeight, std::vector<char>(screenWidth, -1));
}

void RenderManager::renderTriangle(std::vector<Vector3D<double>> &triangle, const char &objectIndex, const BaseShader &shader)
{
    for (auto &point: triangle)
    {
        viewPort(point, screenWidth, screenHeight);
        point.setZ(1 / point.z());
    }
    QPoint leftCorner(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    QPoint rightCorner(0, 0);
    for (const auto &point: triangle)
    {
        rightCorner.setX(std::max(rightCorner.x(), static_cast<int>(point.x())));
        rightCorner.setY(std::max(rightCorner.y(),static_cast<int>(point.y())));
        leftCorner.setX(std::min(leftCorner.x(), static_cast<int>(point.x())));
        leftCorner.setY(std::min(leftCorner.y(), static_cast<int>(point.y())));
    }
    rightCorner.setX(std::min(rightCorner.x(), screenWidth - 1));
    rightCorner.setY(std::min(rightCorner.y(),screenHeight - 1));
    double square = (triangle[0].y() - triangle[2].y()) * (triangle[1].x() - triangle[2].x()) +
            (triangle[1].y() - triangle[2].y()) * (triangle[2].x() - triangle[0].x());
    std::vector<std::thread> threads(8);
    ThreadParams params;
    params.startRow = leftCorner.y();
    params.endRow = rightCorner.y();
    double start = leftCorner.x();
    double step = (rightCorner.x() - leftCorner.x()) / 8.0;
    for (auto &thread: threads)
    {
        params.startCol = round(start);
        params.endCol = round(start + step);
        thread = std::thread(&RenderManager::renderFrameBuffer, this, params, std::cref(triangle), std::cref(objectIndex), std::cref(shader), std::cref(square));
        start += step;
    }
    for (auto &thread:threads)
    {
        thread.join();
    }
}

void RenderManager::renderShadowTriangle(std::vector<Vector3D<double> > &triangle, const int &bufferIndex, const SceneShader &shader)
{
    for (auto &point: triangle)
    {
        viewPort(point, shadowModel.getWidth(), shadowModel.getHeight());
        point.setZ(1 / point.z());
    }
    QPoint leftCorner(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    QPoint rightCorner(0, 0);
    for (const auto &point: triangle)
    {
        rightCorner.setX(std::max(rightCorner.x(), static_cast<int>(point.x())));
        rightCorner.setY(std::max(rightCorner.y(),static_cast<int>(point.y())));
        leftCorner.setX(std::min(leftCorner.x(), static_cast<int>(point.x())));
        leftCorner.setY(std::min(leftCorner.y(), static_cast<int>(point.y())));
    }
    rightCorner.setX(std::min(rightCorner.x(), shadowModel.getWidth() - 1));
    rightCorner.setY(std::min(rightCorner.y(), shadowModel.getHeight() - 1));
    double square = (triangle[0].y() - triangle[2].y()) * (triangle[1].x() - triangle[2].x()) +
            (triangle[1].y() - triangle[2].y()) * (triangle[2].x() - triangle[0].x());
    Vector3D<double> barCoords;
    double z;
    for (int i = leftCorner.x(); i <= rightCorner.x(); i++)
    {
        for (int j = leftCorner.y(); j <= rightCorner.y(); j++)
        {
            barycentric(barCoords, triangle, QPoint(i, j), square);
            if (barCoords.x() >= -EPS && barCoords.y() >= -EPS && barCoords.z() >= -EPS)
            {
                z = shader.countShadowDepth(barCoords);
                if (z <= shadowModel.getDepthByIndex(bufferIndex, i, j))
                {
                    shadowModel.setPixel(bufferIndex, i, j, z);
                }
            }
        }
    }
}

// отрисовка буфер кадра
void RenderManager::renderFrameBuffer(ThreadParams params, const std::vector<Vector3D<double> > &triangle, const char &objectIndex, const BaseShader &shader, const double &square)
{
    Vector3D<double> barCoords;
    double z;
    Color color;
    for (int i = params.startCol; i <= params.endCol; i++)
    {
        for (int j = params.startRow; j <= params.endRow; j++)
        {
            barycentric(barCoords, triangle, QPoint(i, j), square);
            if (barCoords.x() >= -EPS && barCoords.y() >= -EPS && barCoords.z() >= -EPS)
            {
                z = 1 / (barCoords.x() * triangle[0].z() + barCoords.y() * triangle[1].z() + barCoords.z() * triangle[2].z());
                if (z <= depthBuffer[j][i])
                {
                    depthBuffer[j][i] = z;
                    objectsBuffer[j][i] = objectIndex;
                    color = shader.fragment(barCoords, shadowModel);
                    frameBuffer->setPixel(i, j, color.rgb());
                }
            }
        }
    }
}

void RenderManager::viewPort(Vector3D<double> &point, const int &width, const int &height)
{
    point.setX((point.x() + 1) * 0.5 * width);
    point.setY(height - (point.y() + 1) * 0.5 * height);
    point.setZ((point.z() + 2) * 0.5);
}

//
void RenderManager::barycentric(Vector3D<double> &barCoords, const std::vector<Vector3D<double>> &triangle, const QPoint &P, const double &square)
{
    barCoords.setX(((P.y() - triangle[2].y()) * (triangle[1].x() - triangle[2].x()) + (triangle[1].y() - triangle[2].y()) * (triangle[2].x() - P.x())) / square);
    barCoords.setY(((P.y() - triangle[0].y()) * (triangle[2].x() - triangle[0].x()) + (triangle[2].y() - triangle[0].y()) * (triangle[0].x() - P.x())) / square);
    barCoords.setZ(1 - barCoords.x() - barCoords.y());
}
