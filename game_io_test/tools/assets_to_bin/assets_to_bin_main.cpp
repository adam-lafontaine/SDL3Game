#include "../../../libs/tools/assets_to_bin.hpp"

using p32 = image::Pixel;

constexpr auto SRC_ROOT = "/home/adam/Repos/SDL3Game/game_io_test/assets";
constexpr auto DST_ROOT = "/home/adam/Repos/SDL3Game/game_io_test/src/res";


int main()
{
    auto root = fs::path(SRC_ROOT);

    a2b::AssetFiles af;

    a2b::create(af, DST_ROOT, "io_test_data");
    
    a2b::append_file_dir(af, root / "masks");
    a2b::append_file_dir(af, root / "music");
    a2b::append_file_dir(af, root / "sfx");

    a2b::save_and_close(af, "asset_sizes");
}


#include "../../../libs/alloc_type/alloc_type.cpp"
#include "../../../libs/span/span.cpp"
#include "../../../libs/image/image.cpp"
#include "../../../libs/stb_libs/stb_libs.cpp"