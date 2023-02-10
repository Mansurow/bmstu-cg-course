#include "basemodel.h"

BaseModel::BaseModel()
    :rotation_(4, 4)
{
    rotation_.makeIdentity();
}

BaseModel::~BaseModel()
{
}

void BaseModel::getTriangle(std::vector<Vector3D<double> > &triangle, const int &num) const
{
    int index = num * 3;
    triangle[0] = vertices_[triangles_[index]];
    triangle[1] = vertices_[triangles_[index + 1]];
    triangle[2] = vertices_[triangles_[index + 2]];
}


