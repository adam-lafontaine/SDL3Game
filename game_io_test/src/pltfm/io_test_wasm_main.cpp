#include "../../../libs/io/window.hpp"
#include "../../../libs/io/input/input.hpp"
#include "../../../libs/datetime/datetime.hpp"
#include "../../../libs/math/math.hpp"
#include "../../../libs/stb_libs/qsprintf.hpp"

#include "../app/app.hpp"

#include <emscripten.h>
#include <cstdio>
#include <cstdlib>


namespace img = image;
namespace dt = datetime;
namespace game = game_io_test;

using Stopwatch = datetime::Stopwatch;


constexpr auto WINDOW_TITLE = game::APP_TITLE;


#ifndef APP_FULLSCREEN

constexpr u32 WINDOW_WIDTH = 800;
constexpr u32 WINDOW_HEIGHT = 600;

#endif


constexpr f64 NANO = 1'000'000'000;

constexpr f64 TARGET_FPS = 60.0;
constexpr f64 TARGET_NS_PER_FRAME = NANO / TARGET_FPS;


/* static main variables */

enum class RunState : int
{
    Begin,
    Run,
    Error,
    End
};


namespace mn
{
    constexpr int MAIN_ERROR = 1;
    constexpr int MAIN_OK = 0;

    RunState run_state = RunState::Begin;

    window::Window window;
    input::InputArray inputs;

    game::AppState app_state;
    Stopwatch frame_sw;

    constexpr u64 target_frame_ns = TARGET_NS_PER_FRAME;
    
}


void end_program()
{
    mn::run_state = RunState::End;
}


static inline bool is_running()
{
    return mn::run_state != RunState::End;
}


static void cap_framerate()
{
    constexpr f64 fudge = 0.9;

    u64 ns = mn::frame_sw.get_time_nano();
    
    if (ns < mn::target_frame_ns)
    {
        auto sleep_ns = mn::target_frame_ns - ns;
        dt::delay_nano((u64)(sleep_ns * fudge));
    }
    
    mn::frame_sw.start();
}


bool create_window(Vec2Du32 game_dims)
{
#ifndef APP_FULLSCREEN

    Vec2Du32 window_dims = {
        math::max(game_dims.x, WINDOW_WIDTH),
        math::max(game_dims.y, WINDOW_HEIGHT)
    };

    if (!window::create(mn::window, game::APP_TITLE, window_dims, game_dims))
    {
        return false;
    }

#else

    if (!window::create_fullscreen(mn::window, game::APP_TITLE, game_dims))
    {
        return false;
    }

#endif

    return true;
}


img::ImageView make_window_view()
{
    static_assert(window::PIXEL_SIZE == sizeof(img::Pixel));

    img::ImageView view{};
    view.matrix_data_ = (img::Pixel*)mn::window.pixel_buffer;
    view.width = mn::window.width_px;
    view.height = mn::window.height_px;

    return view;
}


static bool main_init()
{
    if (!window::init())
    {
        return false;
    }

    auto result = game::init(mn::app_state);
    if (!result.success)
    {
        // result.error_code
        return false;
    }

    if (!create_window(result.screen_dimensions))
    {
        return false;
    }

    if (!game::set_screen_memory(mn::app_state, make_window_view()))
    {
        return false;
    }

    return true;
}


void main_close()
{
    mn::run_state = RunState::End;

    game::close(mn::app_state);
    input::close();
    window::close();
}


static void main_loop()
{
    input::record_input(mn::inputs);
    auto& input = mn::inputs.curr();

    if (input.cmd_end_program)
    {
        end_program();
    }

    game::update(mn::app_state, input);

    window::render(mn::window, input.window_size_changed);

    if (!is_running())
    {
        emscripten_cancel_main_loop();
    }

    mn::inputs.swap();
    cap_framerate();
}


int main()
{
    if (!main_init())
    {
        main_close();
        return mn::MAIN_ERROR;
    }    

    mn::run_state = RunState::Run;
    mn::frame_sw.start();

    emscripten_set_main_loop(main_loop, 0, 1);

    main_close();

    return mn::MAIN_OK;
}