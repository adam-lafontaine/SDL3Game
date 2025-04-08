// asset_sizes.cpp

const struct {

    struct {
        struct { unsigned size; unsigned offset; } keyboard;
        struct { unsigned size; unsigned offset; } controller;
        struct { unsigned size; unsigned offset; } mouse;
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
    },
    {
        { 924117, 5489 },
        { 2214212, 929606 },
        { 2923535, 3143818 },
        { 609497, 6067353 },
    },
    {
        { 14169, 6676850 },
        { 12588, 6691019 },
        { 9834, 6703607 },
        { 54824, 6713441 },
    },
};
