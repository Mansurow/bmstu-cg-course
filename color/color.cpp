#include "color.h"

Color::Color()
{
}

Color::Color(const float &red, const float &green, const float &blue)
    : r_(red),
      g_(green),
      b_(blue)
{
}


Color Color::operator + (const Color &other) const
{
    Color result;
    result.r_ = std::min(r_ + other.r_, 1.0f);
    result.g_ = std::min(g_ + other.g_, 1.0f);
    result.b_ = std::min(b_ + other.b_, 1.0f);
    return result;
}

Color Color::operator - (const Color &other) const
{
    Color result;
    result.r_ = std::max(r_ - other.r_, 0.0f);
    result.g_ = std::max(g_ - other.g_, 0.0f);
    result.b_ = std::max(b_ - other.b_, 0.0f);
    return result;
}

Color Color::operator *(const Color &other) const
{
    Color result;
    result.r_ = std::min(r_ * other.r_, 1.0f);
    result.g_ = std::min(g_ * other.g_, 1.0f);
    result.b_ = std::min(b_ * other.b_, 1.0f);
    return result;
}

Color Color::operator * (const double &f) const
{
    Color result;
    result.r_ = std::min(r_ * f, 1.0);
    result.g_ = std::min(g_ * f, 1.0);
    result.b_ = std::min(b_ * f, 1.0);
    return result;
}
