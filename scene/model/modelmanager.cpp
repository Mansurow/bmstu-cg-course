#include "modelmanager.h"

ModelManager::ModelManager()
{
}


void ModelManager::rotateByX(BaseModel &model, const double &angle) const
{
    double rad = angle * M_PI / 180;
    Matrix<double> rotateX(4, 4);
    rotateX.makeIdentity();
    rotateX[1][1] = cos(rad);
    rotateX[1][2] = sin(rad);
    rotateX[2][1] = -rotateX[1][2];
    rotateX[2][2] = rotateX[1][1];
    model.setRotation(model.getRotation() * rotateX);
}

void ModelManager::rotateByY(BaseModel &model, const double &angle) const
{
    double rad = angle * M_PI / 180;
    Matrix<double> rotateY(4, 4);
    rotateY.makeIdentity();
    rotateY[0][0] = cos(rad);
    rotateY[0][2] = -sin(rad);
    rotateY[2][0] = - rotateY[0][2];
    rotateY[2][2] = rotateY[0][0];
    model.setRotation(model.getRotation() * rotateY);
}

void ModelManager::rotateByZ(BaseModel &model, const double &angle) const
{
    double rad = angle * M_PI / 180;
    Matrix<double> rotateZ(4, 4);
    rotateZ.makeIdentity();
    rotateZ[0][0] = cos(rad);
    rotateZ[0][1] = sin(rad);
    rotateZ[1][0] = -rotateZ[0][1];
    rotateZ[1][1] = rotateZ[0][0];
    model.setRotation(model.getRotation() * rotateZ);
}

Matrix<double> ModelManager::getModelView(const BaseModel &model) const
{
    Matrix<double> translation(4, 4);
    Vector3D<double> translate(model.getTranslation());
    translation.makeIdentity();
    translation[3][0] = translate.x();
    translation[3][1] = translate.y();
    translation[3][2] = translate.z();
    return model.getRotation() * translation;
}
