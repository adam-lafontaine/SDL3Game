#include "../../../libs/tools/image_to_code.hpp"


using p32 = image::Pixel;


constexpr auto SRC_ROOT = "/home/adam/Repos/SDL3Game/game_io_test/assets";
constexpr auto DST_ROOT = "/home/adam/Repos/SDL3Game/game_io_test/src/res";


u8 to_mask(p32 p)
{
    if (!p.alpha) // transparent
    {
        return 0;
    }

    auto sum = p.red + p.green + p.blue;
    if (!sum) // black
    {
        return 1;
    }

    return 2;
}


int main()
{    
    auto src_dir = fs::path(SRC_ROOT) / "masks";
    auto dst_dir = fs::path(DST_ROOT) / "masks";

    i2c::images_to_code(src_dir, dst_dir, to_mask);
    i2c::image_dimensions_to_code(src_dir, dst_dir, "mask_sizes");
}


#include "../../../libs/alloc_type/alloc_type.cpp"
#include "../../../libs/span/span.cpp"
#include "../../../libs/image/image.cpp"
#include "../../../libs/stb_libs/stb_libs.cpp"