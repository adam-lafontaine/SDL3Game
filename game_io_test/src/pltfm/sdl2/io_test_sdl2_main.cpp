#include "../../../../libs/output/window.hpp"
#include "../../../../libs/input/input.hpp"
#include "../../../../libs/util/stopwatch.hpp"

#include <thread>


constexpr f64 NANO = 1'000'000'000;

constexpr f64 TARGET_FPS = 60.0;
constexpr f64 TARGET_NS_PER_FRAME = NANO / TARGET_FPS;


static void cap_framerate(Stopwatch& sw, f64 target_ns)
{
    constexpr f64 fudge = 0.9;

    auto sleep_ns = target_ns - sw.get_time_nano();
    if (sleep_ns > 0.0)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds((i64)(sleep_ns * fudge)));
    }

    sw.start();
}


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
    input::InputArray input;
}


void end_program()
{
    mn::run_state = RunState::End;
}


static bool is_running()
{
    return mn::run_state == RunState::Run;
}


static bool main_init()
{
    auto title = "IO Test";
    u32 width = 500;
    u32 height = 500;
    // icon?


    if (!window::init())
    {
        return false;
    }

    if (!window::create(mn::window, title, width, height))
    {
        return false;
    }

    if (!input::init(mn::input))
    {
        return false;
    }

    return true;
}


void main_close()
{
    mn::run_state = RunState::End;

    input::close();
    window::close();
}


static void main_loop()
{
    Stopwatch sw;
    sw.start();

    while(is_running())
    {
        input::record_input(mn::input);


        mn::input.swap();
        cap_framerate(sw, TARGET_NS_PER_FRAME);
    }
}


int main()
{
    if (!main_init())
    {
        return mn::MAIN_ERROR;
    }

    mn::run_state = RunState::Run;

    main_loop();

    main_close();

    return mn::MAIN_OK;
}


#include "main_o.cpp"