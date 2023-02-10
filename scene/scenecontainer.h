#ifndef SCENECONTAINER_H
#define SCENECONTAINER_H

#include <vector>
#include "scene/model/model.h"
#include "scene/camera/camera.h"
#include "scene/light/light.h"

class SceneContainer
{
public:
    SceneContainer() {}
    inline void setCamera(const Camera &camera) { camera_ = camera; }
    inline Camera &getCamera() { return camera_; }
    inline void setLight(const Light &light) { light_ = light; }
    inline Light &getLight() { return light_; }
    inline int countModels() const { return models_.size(); }
    inline Cube &getModel(const int &index) { return models_[index]; }
    inline void setModel(const Cube &model, const int &index) { models_[index] = model; }
    inline void addModel(const Cube &model) { models_.push_back(model); }
    inline void deleteModel(const int &index) { models_.erase(models_.begin() + index); }
    inline void deleteAllModels() { models_.clear(); }

private:
    std::vector<Cube> models_;
    Camera camera_;
    Light light_;
};

#endif // SCENECONTAINER_H
