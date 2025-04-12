// asset_sizes.cpp

const struct {

    struct {
        struct { unsigned size; unsigned offset; } keyboard;
        struct { unsigned size; unsigned offset; } controller;
        struct { unsigned size; unsigned offset; } mouse;
        struct { unsigned size; unsigned offset; } arrow;
    } masks;

    struct {
        struct { unsigned size; unsigned offset; } game_02;
        struct { unsigned size; unsigned offset; } game_03;
        struct { unsigned size; unsigned offset; } game_00;
        struct { unsigned size; unsigned offset; } game_01;
    } music;

    struct {
        struct { unsigned size; unsigned offset; } confirmation_002;
        struct { unsigned size; unsigned offset; } laserRetro_000;
        struct { unsigned size; unsigned offset; } open_001;
        struct { unsigned size; unsigned offset; } explosionCrunch_003;
    } sfx;

}
asset_sizes = 
{
    {
        { 2588, 0 },
        { 2094, 2588 },
        { 807, 4682 },
        { 154, 5489 },
    },
    {
        { 924117, 5643 },
        { 2214212, 929760 },
        { 2923535, 3143972 },
        { 609497, 6067507 },
    },
    {
        { 14169, 6677004 },
        { 12588, 6691173 },
        { 9834, 6703761 },
        { 54824, 6713595 },
    },
};
