#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <thread>
#include "geometry/geometry.hpp"
#include "controller/controller.h"

#define MAX_MODELS 3

using namespace GeometrySpace;

class RenderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *parent = nullptr);
    inline void setModel(const int &model) { currentModel = model; }
    void setCameraSpeed(const int &speed);
    int addModel(const ModelAttributes &attributes, const Material &material);
    void moveModel(const ModelMovement &movement);
    void deleteModel();
    void deleteAllModels();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QPointF curPosition;
    QTimer movementTimer;
    Controller controller;
    int widgetHeight;
    int widgetWidth;
    QImage *image;
    int cameraSpeed;
    int currentModel;
    bool cameraRotating;
    CameraMovement cameraMovement;

private slots:
    void moveCamera();
    void changeCamera(CameraChange change);
    void changeModel(ModelChange change);
    void changeMaterial(Material material);
    void moveLight(double x, double y, double z);
    void changeLight(QColor);
signals:
    void pitchChanged(double);
    void yawChanged(double);
    void cameraXChanged(double);
    void cameraYChanged(double);
    void cameraZChanged(double);
    void catchedModel(ModelAttributes, Material);
    void catchedNothing();
};

#endif // RENDERWIDGET_H
