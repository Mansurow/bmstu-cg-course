#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "scene/model/model.h"
#include "scene/camera/camera.h"
#include "scene/light/light.h"

class Scene
{
public:
    Scene() {}
    inline void setCamera(const Camera &camera) { camera_ = camera; }
    inline Camera &getCamera() { return camera_; }
    inline void setLight(const Light &light) { light_ = light; }
    inline Light &getLight() { return light_; }
    inline int countModels() const { return models_.size(); }
    inline Model &getModel(const int &index) { return models_[index]; }
    inline void setModel(const Model &model, const int &index) { models_[index] = model; }
    inline void addModel(const Model &model) { models_.push_back(model); }
    inline void deleteModel(const int &index) { models_.erase(models_.begin() + index); }
    inline void deleteAllModels() { models_.clear(); }

private:
    std::vector<Model> models_;
    Camera camera_;
    Light light_;
};

#endif // SCENE_H
