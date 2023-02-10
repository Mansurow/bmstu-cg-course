#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "scene/camera/cameramanager.h"
#include "scene/model/modelmanager.h"
#include "scene/render/rendermanager.h"
#include "scene/scenecontainer.h"

enum class CameraParam{ YAW, PITCH, POS_X, POS_Y, POS_Z, NEAR_PLANE, FAR_PLANE, FOV, SENSITIVITY };

struct CameraChange
{
    CameraParam type;
    double value;
};

enum class ModelParam{ TOP, BOT, HEIGHT, VERTS };

struct ModelChange
{
    ModelParam type;
    double value;
};

using CameraTransformation = std::variant<CameraRotation, CameraMovement, CameraChange>;

using ModelTransformation = std::variant<ModelChange, ModelMovement, Material>;

using LightTransformation = std::variant<Vector3D<double>, QColor>;

class Controller
{
public:
    Controller();
    Controller(QImage *frame, const int &screen_width, const int &screen_height,
               const CameraAttributes &cameraAttributes, const LightAttributes &lightAttributes);
    void changeLight(const LightTransformation &transformation);
    void changeCamera(const CameraTransformation &transformation);
    inline int getObjectIndex(const int &x, const int &y) const { return renderManager.getIndexByPosition(x, y); }
    int addModel(const ModelAttributes &attributes, const Material &material);
    ModelAttributes getModelAttributes(const int &index);
    Material getModelMaterial(const int &index);
    void deleteModel(const int &index);
    void changeModel(const ModelTransformation &transformation, const int &index);
    double getYaw();
    double getPitch();
    Vector3D<double> getCameraPos();
    void deleteAllModels();
private:
    CameraManager cameraManager;
    ModelManager modelManager;
    RenderManager renderManager;
    SceneContainer sceneContainer;
    std::vector<Vector3D<double>> shadowDir;
    std::vector<Vector3D<double>> shadowUp;
    std::vector<Vector3D<double>> shadowRight;
    void renderScene();
    void renderShadow();
};


#endif // CONTROLLER_H
