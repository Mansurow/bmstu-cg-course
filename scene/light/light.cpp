#include "light.h"

Light::Light()
{

}

Light::Light(const LightAttributes &attributes)
    : position_(attributes.position),
      color_(attributes.color)
{
    model_ = Model({0, 10, 15, 3}, { Color(0,0,0), color_, 1} );
    model_.setTranslation(position_);
}

void Light::setPosition(const Vector3D<double> &position)
{
    position_ = position;
    model_.setTranslation(position_);
}
