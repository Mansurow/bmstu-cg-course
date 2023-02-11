#include "controller.h"

#include <QtDebug>

Controller::Controller()
{
    shadowDir = { {1, 0, 0},
                  {-1, 0, 0},
                  {0, 1, 0},
                  {0, -1, 0},
                  {0, 0, 1},
                  {0, 0, -1}
                };
    shadowUp = { {0, 1, 0},
                 {0, 1, 0},
                 {0, 0, -1},
                 {0, 0, 1},
                 {0, 1, 0},
                 {0, 1, 0}
               };
    shadowRight = { {0, 0, -1},
                    {0, 0, 1},
                    {1, 0, 0},
                    {1, 0, 0},
                    {1, 0, 0},
                    {-1, 0, 0}
                  };
}

Controller::Controller(QImage *frame, const int &screen_width, const int &screen_height,
                       const CameraAttributes &cameraAttributes, const LightAttributes &lightAttributes)
    : Controller()
{
    renderManager = RenderManager(frame, screen_width, screen_height);
    cameraManager = CameraManager(2, 0.25);
    Camera camera(cameraAttributes);
    sceneContainer.setCamera(camera);
    Light light(lightAttributes);
    sceneContainer.setLight(light);
    renderScene();
}

void Controller::changeLight(const LightTransformation &transformation)
{
    Light &light = sceneContainer.getLight();
    if (std::holds_alternative<Vector3D<double>>(transformation))
    {
        Vector3D<double> position = std::get<Vector3D<double>>(transformation);
        light.setPosition(position);
        renderShadow();
    }
    else
    {
        QColor color = std::get<QColor>(transformation);
        light.setColor(Color(color.redF(), color.greenF(), color.blueF()));
    }
    renderScene();
}

int Controller::addModel(const ModelAttributes &attributes, const Material &material)
{
    Model model(attributes, material);
    sceneContainer.addModel(model);
    renderShadow();
    renderScene();
    return sceneContainer.countModels();
}

ModelAttributes Controller::getModelAttributes(const int &index)
{
    const Model &model = sceneContainer.getModel(index);
    return model.getAttributes();
}

Material Controller::getModelMaterial(const int &index)
{
    const Model &model = sceneContainer.getModel(index);
    return model.getMaterial();
}

void Controller::deleteModel(const int &index)
{
    sceneContainer.deleteModel(index);
    renderShadow();
    renderScene();
}

void Controller::changeModel(const ModelTransformation &transformation, const int &index)
{
    Model &model = sceneContainer.getModel(index);
    if (std::holds_alternative<ModelMovement>(transformation))
    {
        ModelMovement movement = std::get<ModelMovement>(transformation);
        if (fabs(movement.angleX) > EPS)
        {
            modelManager.rotateByX(model, movement.angleX);
        }
        if (fabs(movement.angleY) > EPS)
        {
            modelManager.rotateByY(model, movement.angleY);
        }
        if (fabs(movement.angleZ) > EPS)
        {
            modelManager.rotateByZ(model, movement.angleZ);
        }
        if (fabs(movement.dx) > EPS)
        {
            modelManager.translateByX(model, movement.dx);
        }
        if (fabs(movement.dy) > EPS)
        {
            modelManager.translateByY(model, movement.dy);
        }
        if (fabs(movement.dz) > EPS)
        {
            modelManager.translateByZ(model, movement.dz);
        }
        renderShadow();
    }
    else if (std::holds_alternative<ModelChange>(transformation))
    {
        ModelChange change = std::get<ModelChange>(transformation);
        switch (change.type)
        {
        case ModelParam::TOP:
            model.changeTopLength(change.value);
            break;
        case ModelParam::BOT:
            model.changeBotLength(change.value);
            break;
        case ModelParam::HEIGHT:
            model.changeHeight(change.value);
            break;
        case ModelParam::VERTS:
            model.changeVerticesCount(change.value);
            break;
        default:
            break;
        }
        renderShadow();
    }
    else
    {
        Material material = std::get<Material>(transformation);
        model.setMaterial(material);
    }
    renderScene();
}

double Controller::getYaw()
{
    Camera &camera = sceneContainer.getCamera();
    return camera.getYaw();
}

double Controller::getPitch()
{
    Camera &camera = sceneContainer.getCamera();
    return camera.getPitch();
}

Vector3D<double> Controller::getCameraPos()
{
    Camera &camera = sceneContainer.getCamera();
    return camera.getPosition();
}

void Controller::deleteAllModels()
{
    sceneContainer.deleteAllModels();
    renderShadow();
    renderScene();
}

void Controller::changeCamera(const CameraTransformation &transformation)
{
    Camera &camera = sceneContainer.getCamera();
    if (std::holds_alternative<CameraMovement>(transformation))
    {
        CameraMovement movement = std::get<CameraMovement>(transformation);
        cameraManager.move(camera, movement);
    }
    else if (std::holds_alternative<CameraRotation>(transformation))
    {
        CameraRotation rotation = std::get<CameraRotation>(transformation);
        cameraManager.rotate(camera, rotation);
    }
    else
    {
        CameraChange change = std::get<CameraChange>(transformation);
        switch (change.type)
        {
        case CameraParam::YAW:
            camera.setYaw(change.value);
            camera.updateCoords();
            break;
        case CameraParam::PITCH:
            camera.setPitch(change.value);
            camera.updateCoords();
            break;
        case CameraParam::FOV:
        {
            ViewFrustrum frustrum = camera.getViewFrustrum();
            frustrum.field_of_view = change.value;
            camera.setViewFrustrum(frustrum);
            break;
        }
        case CameraParam::NEAR_PLANE:
        {
            ViewFrustrum frustrum = camera.getViewFrustrum();
            frustrum.near_plane = change.value;
            camera.setViewFrustrum(frustrum);
            break;
        }
        case CameraParam::FAR_PLANE:
        {
            ViewFrustrum frustrum = camera.getViewFrustrum();
            frustrum.far_plane = change.value;
            camera.setViewFrustrum(frustrum);
            break;
        }
        case CameraParam::POS_X:
        {
            Vector3D<double> position(camera.getPosition());
            position.setX(change.value);
            camera.setPosition(position);
            break;
        }
        case CameraParam::POS_Y:
        {
            Vector3D<double> position(camera.getPosition());
            position.setY(change.value);
            camera.setPosition(position);
            break;
        }
        case CameraParam::POS_Z:
        {
            Vector3D<double> position(camera.getPosition());
            position.setZ(change.value);
            camera.setPosition(position);
            break;
        }
        case CameraParam::SENSITIVITY:
            cameraManager.setSensitivity(change.value);
            break;
        default:
            break;
        }
    }
    renderScene();
}

void Controller::renderScene()
{
    Camera camera = sceneContainer.getCamera();
    Light light = sceneContainer.getLight();
    renderManager.clearFrame();
    Matrix<double> rotation = cameraManager.getRotation(camera);
    Matrix<double> vpMatrix = cameraManager.getLookAt(camera) * cameraManager.getProjection(camera);
    Matrix<double> modelMatrix(4, 4);
    SceneShader sceneShader;
    Model model;
    sceneShader.setCameraPosition(camera.getPosition());
    sceneShader.setLightPosition(light.getPosition());
    sceneShader.setVpMatrix(vpMatrix);
    for (int i = 0; i < sceneContainer.countModels(); i++)
    {
        model = sceneContainer.getModel(i);
        modelMatrix = modelManager.getModelView(model);
        sceneShader.setModelMatrix(modelMatrix);
        sceneShader.setMaterial(model.getMaterial());
        sceneShader.setLightColor(light.getColor());
        renderManager.renderModel(sceneShader, model, i);
    }
    LightShader lightShader;
    lightShader.setCameraPosition(camera.getPosition());
    lightShader.setVpMatrix(vpMatrix);
    model = light.getModel();
    modelMatrix = modelManager.getModelView(model);
    lightShader.setModelMatrix(modelMatrix);
    lightShader.setLightColor(light.getColor());
    renderManager.renderModel(lightShader, model, -1);
    renderManager.renderCoordinateSystem(rotation);
}

void Controller::renderShadow()
{
    Camera camera;
    Light light = sceneContainer.getLight();
    camera.setViewFrustrum({90, 0.1, 1000, 1});
    renderManager.clearShadow();
    Matrix<double> projection = cameraManager.getProjection(camera);
    Matrix<double> modelMatrix(4, 4);
    Matrix<double> vpMatrix(4, 4);
    SceneShader shader;
    shader.setCameraPosition(light.getPosition());
    shader.setLightPosition(light.getPosition());
    camera.setPosition(light.getPosition());
    for (int j = 0; j < 6; j++)
    {
        camera.setDirection(shadowDir[j]);
        camera.setUp(shadowUp[j]);
        camera.setRight(shadowRight[j]);
        vpMatrix = cameraManager.getLookAt(camera) * projection;
        shader.setVpMatrix(vpMatrix);
        for (int i = 0; i < sceneContainer.countModels(); i++)
        {
            Model model = sceneContainer.getModel(i);
            modelMatrix = modelManager.getModelView(model);
            shader.setModelMatrix(modelMatrix);
            renderManager.renderShadowModel(shader, model, j);
        }
    }

}
