#ifndef LIGHT_H
#define LIGHT_H

#include "scene/model/model.h"

using namespace GeometrySpace;

struct LightAttributes
{
    Vector3D<double> position;
    Color color;
};

class Light
{
public:
    Light();
    Light(const LightAttributes &attributes);
    void setPosition(const Vector3D<double> &position);
    inline void setColor(const Color &color) { color_ = color; }
    inline Color getColor() const { return color_; }
    inline Cube getModel() const { return  model_; }
    inline Vector3D<double> getPosition() const { return position_; }
private:
    Vector3D<double> position_;
    Color color_;
    Cube model_;
};

#endif // LIGHT_H
