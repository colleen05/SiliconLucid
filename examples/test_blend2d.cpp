// Project: Slew
// File: examples/test_blend2d.cpp
// Description: TEST example for Blend2D.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>
#include <blend2d.h>

int main() {
    slew::print_lib_info();

    // CREDIT: The following code was released by the Blend2D developers at
    // https://blend2d.com/doc/getting-started.html into the public domain.
    
    BLImage img(480, 480, BL_FORMAT_PRGB32);
    BLContext ctx(img);

    ctx.setCompOp(BL_COMP_OP_SRC_COPY);
    ctx.fillAll();

    // First shape filld by a radial gradient.
    BLGradient radial(BLRadialGradientValues(180, 180, 180, 180, 180));
    radial.addStop(0.0, BLRgba32(0xFFFFFFFF));
    radial.addStop(1.0, BLRgba32(0xFFFF6F3F));

    ctx.setCompOp(BL_COMP_OP_SRC_OVER);
    ctx.setFillStyle(radial);
    ctx.fillCircle(180, 180, 160);

    // Second shape filled by a linear gradient.
    BLGradient linear(BLLinearGradientValues(195, 195, 470, 470));
    linear.addStop(0.0, BLRgba32(0xFFFFFFFF));
    linear.addStop(1.0, BLRgba32(0xFF3F9FFF));

    ctx.setCompOp(BL_COMP_OP_DIFFERENCE);
    ctx.setFillStyle(linear);
    ctx.fillRoundRect(195, 195, 270, 270, 25);

    ctx.end();
    img.writeToFile("blend2d_output.png");

    return 0;
}