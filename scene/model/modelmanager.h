#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <variant>
#include "scene/model/model.h"

struct ModelMovement
{
    double angleX;
    double angleY;
    double angleZ;
    double dx;
    double dy;
    double dz;
};

class ModelManager
{
public:
    ModelManager();
    void rotateByX(BaseModel &model, const double &angle) const;
    void rotateByY(BaseModel &model, const double &angle) const;
    void rotateByZ(BaseModel &model, const double &angle) const;
    inline void translateByX(BaseModel &model, const double &translation) const {
        model.setTranslation(model.getTranslation() + Vector3D<double>(translation, 0, 0));
    }
    inline void translateByY(BaseModel &model, const double &translation) const {
        model.setTranslation(model.getTranslation() + Vector3D<double>(0, translation, 0));
    }
    inline void translateByZ(BaseModel &model, const double &translation) const {
        model.setTranslation(model.getTranslation() + Vector3D<double>(0, 0, translation));
    }
    Matrix<double> getModelView(const BaseModel &model) const;
private:
    std::vector<Model> models;
};

#endif // MODELMANAGER_H
