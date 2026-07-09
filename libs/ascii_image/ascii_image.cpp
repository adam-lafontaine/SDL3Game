#pragma once

#include "ascii_image.hpp"


//#define LOG_ALLOC_TYPE
#define ASSERT_ALLOC_TYPE



namespace ascii
{

    using MaskView = img::GrayView;
    using MaskSubView = img::GraySubView;


    static inline u32 min(u32 a, u32 b) { return a < b ? a : b; }

    static inline u32 max(u32 a, u32 b) { return a > b ? a : b; }


namespace t_ascii
{
    template <class ASCII>
    static MaskView make_char_mask(ASCII const& ascii, char c)
    {        
        auto id = c - ' ';

        MaskView view{};
        view.width = (u32)ascii.widths[id];
        view.height = (u32)ascii.height;
        view.matrix_data_ = (u8*)ascii.u8_pixel_data[id];

        return view;
    }


    template <class ASCII>
    static u32 count_text_width(ASCII const& ascii, StringView const& text)
    {      
        u32 width = 0;

        for (u32 i = 0; i < text.length; i++)
        {
            auto id = text.data[i] - ' ';
            width += (u32)ascii.widths[id];
        }

        return width;
    }


    template <class ASCII>
    static Vec2Du32 text_dimensions(ASCII const& ascii, StringView const& text)
    {
        Vec2Du32 dims;

        dims.x = count_text_width(ascii, text);
        dims.y = ascii.height;

        return dims;
    }

}


namespace font_ascii_5
{
#include "./font/ascii_5.cpp"


    static MaskView make_mask(char c)
    {
        return t_ascii::make_char_mask(ascii_chars, c);
    }


    static u32 count_text_width(StringView const& text)
    {
        return t_ascii::count_text_width(ascii_chars, text);
    }


    static Vec2Du32 text_dimensions(StringView const& text)
    {
        return t_ascii::text_dimensions(ascii_chars, text);
    }
}


namespace font_joystick_8
{
#include "./font/ascii_joystick_8.cpp"


    static MaskView make_mask(char c)
    {
        return t_ascii::make_char_mask(ascii_chars, c);
    }


    static u32 count_text_width(StringView const& text)
    {
        return t_ascii::count_text_width(ascii_chars, text);
    }


    static Vec2Du32 text_dimensions(StringView const& text)
    {
        return t_ascii::text_dimensions(ascii_chars, text);
    }
}


namespace internal
{
    using AF = ascii::Font;


    static inline u32 font_height(ascii::Font font)
    {
        switch (font)
        {
        case AF::Ascii5:    return font_ascii_5::ascii_chars.height;
        case AF::Joystick8: return font_joystick_8::ascii_chars.height;

        default: return 0;
        }
    }


    static inline u32 text_width(StringView const& text, ascii::Font font)
    {
        switch (font)
        {
        case AF::Ascii5:    return font_ascii_5::count_text_width(text);
        case AF::Joystick8: return font_joystick_8::count_text_width(text);

        default: return 0;
        }
    }


    static inline Vec2Du32 text_dimensions(StringView const& text, ascii::Font font)
    {
        switch (font)
        {
        case AF::Ascii5:   return font_ascii_5::text_dimensions(text);
        case AF::Joystick8: return font_joystick_8::text_dimensions(text);

        default: return {0};
        }
    }


    static inline auto mask_fn(ascii::Font font)
    {
        switch (font)
        {
        case AF::Ascii5: return font_ascii_5::make_mask;
        case AF::Joystick8: return font_joystick_8::make_mask;

        default: return font_ascii_5::make_mask;
        }
    }


    static void transform_mask(MaskSubView const& src, img::SubView const& dst, p32 color)
    {
        auto const w = src.width;
        auto const h = src.height;

        for (u32 y = 0; y < h; y++)
        {
            auto s = img::row_span(src, y);
            auto d = img::row_span(dst, y);

            for (u32 i = 0; i < s.length; i++)
            {
                if (s.data[i])
                {
                    d.data[i] = color;
                }
            }
        }
    }


    static void transform_mask_v(MaskSubView const& src, img::SubView const& dst, p32 color)
    {
        assert(src.width == dst.height);
        assert(src.height == dst.width);

        auto const sw_dh = src.width;
        auto const sh_dw = src.height;

        u32 dp = dst.matrix_width;

        u32 sx = 0;
        u32 sy = 0;

        u32 dx = 0;
        u32 dy = 0;

        u8* s = 0;
        p32* d = 0;

        for (sy = 0; sy < sh_dw; sy++)
        {             
            dx = sh_dw - sy - 1;
            dy = 0;
            s = img::row_span(src, sy).data;
            d = img::row_span(dst, dy).data;
            for (sx = 0; sx < sw_dh; sx++)
            {
                if (s[sx])
                {                    
                    d[dx] = color;
                }

                d += dp;                
            }
        }
    }
}

}


namespace ascii
{

    void render_text(StringView const& text, img::SubView const& dst, Font font, p32 color)
    {
        auto mask_fn = internal::mask_fn(font);

        u32 sw = 0;
        u32 const sh = min(internal::font_height(font), dst.height);
        i32 w_remaining = dst.width;

        Rect2Du32 d_range = {};
        d_range.y_begin = 0;
        d_range.y_end = sh;
        d_range.x_begin = 0;
        d_range.x_end = 0;

        for (u32 i = 0; i < text.length && w_remaining > 0; i++)
        {
            auto mask = mask_fn(text.data[i]);

            sw = min(mask.width, (u32)w_remaining);
            d_range.x_end += sw;

            auto s_view = img::sub_view(mask, img::make_rect(sw, sh));
            auto d_view = img::sub_view(dst, d_range);

            internal::transform_mask(s_view, d_view, color);

            d_range.x_begin = d_range.x_end;
            w_remaining -= sw;
        }
    }


    void render_text(StringView const& text, img::SubView const& dst, Font font, u32 desired_height, p32 color)
    {
        u32 const base_height = internal::font_height(font);
        auto mask_fn = internal::mask_fn(font);

        auto const scale = max(desired_height / base_height, 1U);

        u32 s_width = 0;
        u32 d_width = 0;
        u32 const s_height = base_height;
        u32 const d_height = min(s_height * scale, dst.height);
        i32 w_remaining = (i32)dst.width;

        Rect2Du32 d_range{};
        d_range.y_begin = 0;
        d_range.y_end = d_height;

        auto const mask_char = [color](u8 mask, p32 p)
        {
            return (mask) ? color : p;
        };

        for (u32 i = 0; i < text.length && w_remaining > 0; i++)
        {
            auto mask = mask_fn(text.data[i]);

            s_width = min(mask.width, (u32)w_remaining / scale);
            d_width = s_width * scale;

            d_range.x_end += d_width;

            auto s_view = img::sub_view(mask, img::make_rect(s_width, s_height));
            auto d_view = img::sub_view(dst, d_range);

            img::transform_scale_up(s_view, d_view, scale, mask_char);

            d_range.x_begin = d_range.x_end;
            w_remaining -= d_width;
        }


    }


    u32 font_height(Font font)
    {
        return internal::font_height(font);
    }


    u32 text_width(StringView const& text, Font font)
    {
        return internal::text_width(text, font);
    }


    u32 text_width(StringView const& text, Font font, u32 scale)
    {
        return internal::text_width(text, font) * scale;
    }


    Vec2Du32 text_dimensions(StringView const& text, Font font)
    {
        return internal::text_dimensions(text, font);
    }
}


namespace ascii
{
    void render_text_v(StringView const& text, img::SubView const& dst, Font font, p32 color)
    {
        auto mask_fn = internal::mask_fn(font);

        u32 sw_dh = 0;
        u32 const sh_dw = min(internal::font_height(font), dst.width);
        i32 dh_remaining = dst.height;

        Rect2Du32 d_range = {};
        d_range.x_begin = 0;
        d_range.x_end = sh_dw;

        d_range.y_begin = 0;
        d_range.y_end = 0;

        for (u32 i = 0; i < text.length && dh_remaining > 0; i++)
        {
            auto mask = mask_fn(text.data[i]);

            sw_dh = min(mask.width, (u32)dh_remaining);
            d_range.y_end += sw_dh;

            auto s_view = img::sub_view(mask, img::make_rect(sw_dh, sh_dw));
            auto d_view = img::sub_view(dst, d_range);            

            internal::transform_mask_v(s_view, d_view, color);

            d_range.y_begin = d_range.y_end;
            dh_remaining -= sw_dh;
        }
    }
}