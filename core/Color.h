#ifndef COLOR_H
#define COLOR_H

#include <Quantity_NameOfColor.hxx>

class Color
{
public:
    static Quantity_NameOfColor color(int index);
    static int num();

private:
    static Quantity_NameOfColor s_colors[];
    static int s_num;
};

#endif // COLOR_H
