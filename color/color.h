#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <QColor>

class Color
{
public:
    Color();
    Color(const float &red, const float &green, const float &blue);
    inline void setRed(const float &red) { r_ = red; }
    inline void setGreen(const float &green) { g_ = green; }
    inline void setBlue(const float &blue) { b_ = blue; }
    inline unsigned char r() const { return r_; }
    inline unsigned char g() const  { return g_; }
    inline unsigned char b() const { return b_; }
    inline QRgb rgb() { return qRgb(r_ * 255, g_ * 255, b_ * 255); }
    Color operator + (const Color &other) const;
    Color operator - (const Color &other) const;
    Color operator * (const Color &other) const;
    Color operator * (const double &f) const;
private:
    float r_;
    float g_;
    float b_;

};

#endif // COLOR_H
