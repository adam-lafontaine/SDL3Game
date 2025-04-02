#pragma once

#include "../../../libs/input/input.hpp"
#include "../../../libs/image/image.hpp"


namespace game_io_test
{
    constexpr auto APP_TITLE = "IO Test";
    constexpr auto VERSION = "1.0.0";


    class StateData;


    class AppState
    {
    public:
        image::ImageView screen;

        StateData* data = nullptr;        
    };


    class AppResult
    {
    public:
        bool success = false;

        Vec2Du32 screen_dimensions;
    };


    AppResult init(AppState& state);

    bool set_screen_memory(AppState& state, image::ImageView screen);

    void update(AppState& state, input::Input const& input);

    void reset(AppState& state);

    void close(AppState& state);
}