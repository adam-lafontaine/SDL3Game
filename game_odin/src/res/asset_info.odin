package res
// This file should be generated

import "core:fmt"


AssetType :: enum
{
    Bytes,
    PNG,
    OGG
}

ImageInfo :: struct
{
    type: AssetType,
    offset: u32,
    size: u32,
    width: u32,
    height: u32,
}


MusicInfo :: struct
{
    type: AssetType,
    offset: u32,
    size: u32,
    
}


SoundInfo :: struct
{
    type: AssetType,
    offset: u32,
    size: u32,
}


ImageID :: enum
{
    keyboard,
    gamepad,
    mouse,
    arrow
}


@(rodata)
masks := [ImageID]ImageInfo {
    .keyboard = { .PNG, 0,    2588, 272, 92 },
    .gamepad  = { .PNG, 2588, 2094, 192, 92 },
    .mouse    = { .PNG, 4682, 807,  80,  92 },
    .arrow    = { .PNG, 5489, 169,  25,  25 }
}


MusicID :: enum 
{
    game_02,
    game_03,
    game_00,
    game_01
}


@(rodata)
music := [MusicID]MusicInfo {
    .game_02 = { .OGG, 5658,    924117 },
    .game_03 = { .OGG, 929775,  2214212 },
    .game_00 = { .OGG, 3143987, 2923535 },
    .game_01 = { .OGG, 6067522, 609497 },
}


SoundId :: enum
{
    confirmation_002,
    laserRetro_000,
    open_001,
    explosionCrunch_003
}


@(rodata)
sound := [SoundId]SoundInfo {
    .confirmation_002    = {.OGG, 6677019, 14169 },
    .laserRetro_000      = {.OGG, 6691188, 12588 },
    .open_001            = {.OGG, 6703776, 9834 },
    .explosionCrunch_003 = {.OGG, 6713610, 54824 }
}