#include "../../../libs/tools/assets_to_bin.hpp"


constexpr auto SRC_ROOT = "/home/adam/Repos/SDL3Game/game_io_test/assets";
constexpr auto DST_ROOT = "/home/adam/Repos/SDL3Game/game_io_test/src/res";


int main()
{
    a2b::files_to_binary(SRC_ROOT, DST_ROOT, "io_test");
}


#include "../../../libs/alloc_type/alloc_type.cpp"
#include "../../../libs/span/span.cpp"
#include "../../../libs/image/image.cpp"
#include "../../../libs/stb_libs/stb_libs.cpp"