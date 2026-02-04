#include "bn_core.h"
#include "bn_color.h"
#include "bn_bg_palettes.h"

int main()
{
    bn::core::init();

    // Set a visible backdrop color (blue) so we know it's running
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 31));

    while(true)
    {
        bn::core::update();
    }
}
