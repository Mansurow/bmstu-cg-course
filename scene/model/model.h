#ifndef MODEL_H
#define MODEL_H

#include "scene/model/basemodel.h"

class Cube: public BaseModel
{
public:
    Cube();
    Cube(const ModelAttributes &attributes, const Material &material);
    void changeVerticesCount(const int &nVerts) override;
    void changeTopLength(const double &length) override;
    void changeBotLength(const double &length) override;
    void changeHeight(const double &height) override;
    ModelAttributes getAttributes() const override;
private:
    void computeVertices(const ModelAttributes &attributes) override;
    void computeTriangles(const int &nVerts) override;
    void computeNormals(const int &nVerts) override;
};

#endif // MODEL_H
