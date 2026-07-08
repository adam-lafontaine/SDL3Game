#pragma once

#include "../io/audio.hpp"
#include "../io/filesystem.hpp"
#include "../math/math.hpp"
#include "../alloc_type/alloc_type.hpp"

#include "sdl_include.hpp"

#include <SDL3_mixer/SDL_mixer.h>


#define ASSERT_AUDIO
#define LOG_AUDIO


#ifndef NDEBUG

#ifdef LOG_AUDIO
#define audio_log(...) SDL_Log(__VA_ARGS__)
#else
#define audio_log(...)
#endif

#ifdef ASSERT_AUDIO
#define audio_assert(condition) SDL_assert(condition)
#else
#define audio_assert(...)
#endif

#else

#define audio_log(...)
#define audio_assert(...)

#endif


/* definitions */

namespace audio
{
    using Mixer_p = MIX_Mixer*;
    using Track_p = MIX_Track*;
    using Audio_p = MIX_Audio*;

    using SoundId = Sound::ID;
    using MusicId = Music::ID;


    constexpr u32 MAX_SOUND_TRACKS = 8;


    class MusicTrack
    {
    public:
        Audio_p data = 0;
        Track_p track = 0;
        Music* music = 0;

        MusicId music_id = { -1 };

        f32 volume = 0.5f;
    };


    class SoundTrack
    {
    public:
        Audio_p data = 0;
        Track_p track = 0;
        Sound* sound = 0;

        f32 volume = 0.5f;
    };


    static void music_track_stopped_cb(void* user_data, Track_p track);

    static void sound_track_stopped_cb(void* user_data, Track_p track);
}


/* music */

namespace audio
{
namespace internal
{
    


    static bool create_music_track(MusicTrack& mt, Mixer_p mixer)
    {
        mt.data = 0;
        mt.music = 0;
        mt.track = MIX_CreateTrack(mixer);
        if (!mt.track)
        {
            sdl::print_error("MIX_CreateTrack");
            return false;
        }

        MIX_SetTrackStoppedCallback(mt.track, music_track_stopped_cb, 0);

        return true;
    }


    static void remove_music(MusicTrack& mt, Music const& music)
    {
        if ((Audio_p)music.handle == mt.data)
        {
            MIX_SetTrackAudio(mt.track, NULL);
            MIX_SetTrackStoppedCallback(mt.track, NULL, NULL);
        }

        mt.data = 0;
        mt.music = 0;
    }


    static bool set_music(MusicTrack& mt, Music& music)
    {        
        auto data = (Audio_p)music.handle;
        if (!data)
        {
            return false;
        }

        if (!mt.track)
        {
            return false;
        }

        if (mt.music_id.value >= 0 && music.id.value == mt.music_id.value)
        {
            return true;
        }

        bool ok = true;

        ok &= MIX_SetTrackAudio(mt.track, data);
        ok &= MIX_SetTrackStoppedCallback(mt.track, music_track_stopped_cb, (void*)(&music));

        if (ok)
        {
            mt.data = data;
            mt.music = &music;
            mt.music_id.value++;
            music.id = mt.music_id;
            music.is_on = false;
            music.is_paused = false;
        }
        
        return ok;
    }


    static f32 set_music_track_volume(MusicTrack& music, f32 volume)
    {
        volume = math::cxpr::clamp(volume, 0.0f, 1.0f);

        auto gain = volume / music.volume;        

        MIX_SetTrackGain(music.track, gain);
        
        music.volume = volume;

        return volume;
    }


    static bool play_music_track(MusicTrack const& music)
    {
        constexpr int FOREVER = -1;

        static SDL_PropertiesID props = 0;
        if (!props)
        {
            props = SDL_CreateProperties();
            SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, FOREVER);
        }

        bool ok = MIX_PlayTrack(music.track, props);
        if (!ok)
        {
            sdl::print_error("MIX_PlayTrack(music.track, props)");
        }

        return ok;
    }


    static bool fade_in_music_track(MusicTrack const& music, u32 fade_ms)
    {
        constexpr int FOREVER = -1;

        static SDL_PropertiesID props = 0;
        if (!props)
        {
            props = SDL_CreateProperties();
            SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, FOREVER);            
        }

        SDL_SetNumberProperty(props, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fade_ms);        
        
        return MIX_PlayTrack(music.track, props);
    }


    static void fade_out_music_track(MusicTrack const& music, u32 fade_ms)
    {
        MIX_StopTrack(music.track, fade_ms);
    }


    static void stop_music_track(MusicTrack const& music)
    {
        MIX_StopTrack(music.track, 0);
    }
}
}


/* sound list */

namespace audio
{
namespace internal
{
    class SoundList
    {
    public:
        static constexpr u32 capacity = MAX_SOUND_TRACKS;

        u32 count = 0;

        SoundTrack tracks[capacity] = { 0 };

        
        SoundTrack& get(Sound const& sound) { return tracks[sound.id.value]; }
        
    };


    static bool create_sound_list(SoundList& list, Mixer_p mixer)
    {
        bool ok = true;

        for (u64 i = 0; i < list.capacity; i++)
        {
            auto& item = list.tracks[i];
            item.data = 0;
            item.sound = 0;
            item.track = MIX_CreateTrack(mixer);
            if (!item.track)
            {
                ok = false;
                sdl::print_error("MIX_CreateTrack");
                break;
            }
            
            MIX_SetTrackStoppedCallback(item.track, sound_track_stopped_cb, (void*)(i));
        }

        return ok;
    }


    static void remove_sound(SoundList& list, Sound const& sound)
    {
        auto& item = list.get(sound);

        MIX_SetTrackAudio(item.track, NULL);        
        MIX_SetTrackStoppedCallback(item.track, NULL, NULL);        
        item.data = 0;
        list.count--;
    }


    static bool add_sound(SoundList& list, Sound& sound, Audio_p data)
    {
        if (!data)
        {
            return false;
        }

        u32 i = 0;
        for (; i < list.capacity; i++)
        {
            auto& item = list.tracks[i];
            if (!item.data)
            {
                item.data = data;
                sound.id.value = (i32)i;
                sound.handle = (u64)data;
                MIX_SetTrackAudio(item.track, item.data);
                list.count++;                
                return true;
            }
        }
        
        return false;
    }


    static f32 set_sound_track_volume(SoundTrack& sound, f32 volume)
    {
        volume = math::cxpr::clamp(volume, 0.0f, 1.0f);

        auto gain = volume / sound.volume;        

        MIX_SetTrackGain(sound.track, gain);
        
        sound.volume = volume;

        return volume;
    }


    static bool play_sound_track_once(SoundTrack const& sound)
    {
        constexpr int N_REPEATS = 0;

        static SDL_PropertiesID props = 0;
        if (!props)
        {
            props = SDL_CreateProperties();
            SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, N_REPEATS);
        }

        return MIX_PlayTrack(sound.track, props);
    }


    static bool play_sound_track_loop(SoundTrack const& sound)
    {
        constexpr int FOREVER = -1;

        static SDL_PropertiesID props = 0;
        if (!props)
        {
            props = SDL_CreateProperties();
            SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, FOREVER);
        }

        return MIX_PlayTrack(sound.track, props);
    }


    static void stop_sound_track(SoundTrack const& sound)
    {
        MIX_StopTrack(sound.track, 0);
    }
}
}


/* static data */

namespace audio
{
    static internal::SoundList sound_list;

    static MusicTrack music_track = {};
    static int n_music_tracks = 0;

    static Mixer_p mixer = 0;

    static bool audio_initialized = false;

}


/* callbacks */

namespace audio
{
    static void music_track_stopped_cb(void* user_data, Track_p track)
    {
        auto& item = music_track;
        if (!item.music)
        {
            return;
        }

        item.music->is_on = false;
    }


    static void sound_track_stopped_cb(void* user_data, Track_p track)
    {
        auto id = (u64)user_data;
        auto& item = sound_list.tracks[id];
        if (!item.sound)
        {
            return;
        }

        item.sound->is_on = false;
    }
}


/* internal */

namespace audio
{      
namespace internal
{
    static bool has_extension(cstr filename, const char* ext)
    {
        auto file_length = span::strlen(filename);
        auto ext_length = span::strlen(ext);

        return !span::strcmp(&filename[file_length - ext_length], ext);
    }


    static bool is_valid_audio_file(cstr filename)
    {
        return 
            //has_extension(filename, ".mp3") || 
            //has_extension(filename, ".MP3") ||
            has_extension(filename, ".ogg") || 
            has_extension(filename, ".OGG") ||
            has_extension(filename, ".wav") ||
            has_extension(filename, ".WAV");
    }


    static void reset_music(Music& music)
    {
        music.id.value = -1;
        music.handle = 0;
        music.is_on = 0;
        music.is_paused = 0;
    }


    static void reset_sound(Sound& sound)
    {
        sound.id.value = -1;
        sound.handle = 0;
        sound.is_on = 0;
    }


    static bool is_initialized()
    {
        return audio_initialized;
    }
}
}


/* api */

namespace audio
{
    void destroy_music(Music& music)
    {
        if (music.id.value == music_track.music->id.value)
        {
            internal::remove_music(music_track, music);
        }
        
        if (music.handle)
        {
            MIX_DestroyAudio((Audio_p)music.handle);
            mem::untag((u8*)music.handle);
        }

        internal::reset_music(music);
    }


    void destroy_sound(Sound& sound)
    {
        if (sound.id.value >= 0)
        {            
            internal::remove_sound(sound_list, sound);
        }

        if (sound.handle)
        {
            MIX_DestroyAudio((Audio_p)sound.handle);
            mem::untag((u8*)sound.handle);
        }

        internal::reset_sound(sound);
    }


    bool init_audio()
    {
        if (!MIX_Init())
        {
            sdl::print_error("MIX_Init()");
            return false;
        }

        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (!mixer)
        {
            sdl::print_error("MIX_CreateMixerDevice");
            return false;
        }

        bool ok = true;

        ok &= internal::create_music_track(music_track, mixer);
        if (!ok)
        {
            return false;
        }

        ok &= internal::create_sound_list(sound_list, mixer);
        if (!ok)
        {
            return false;
        }

        audio_initialized = true;

        return true;
    }


    void stop_audio()
    {
        MIX_StopAllTracks(mixer, 0);
    }


    void close_audio()
    {
        stop_audio();
        MIX_Quit();

        audio_initialized = false;
    }


    bool load_music_from_file(cstr music_file_path, Music& music)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        auto is_valid_file = internal::is_valid_audio_file(music_file_path);
        audio_assert(is_valid_file && " *** invalid music file *** ");

        internal::reset_music(music);

        if (!is_valid_file)
        {
            audio_log("Invalid music file: %s\n", music_file_path);
            return false;
        }

        auto size = fs::file_size(music_file_path);
        if (!size)
        {
            audio_log("Music file size zero: %s\n", music_file_path);
            return false;
        }

        auto data = MIX_LoadAudio(mixer, music_file_path, true);
        if (!data)
        {
            sdl::print_error("Load Music");
            return false;
        }

        mem::tag((u8*)data, size, fs::get_file_name(music_file_path));

        music.handle = (u64)data;

        return true;
    }


    bool load_sound_from_file(cstr sound_file_path, Sound& sound)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");
        
        auto is_valid_file = internal::is_valid_audio_file(sound_file_path);
        audio_assert(is_valid_file && " *** invalid music file *** ");

        internal::reset_sound(sound);

        if (!is_valid_file)
        {
            audio_log("Invalid sound file: %s\n", sound_file_path);
            return false;
        }

        auto size = fs::file_size(sound_file_path);
        if (!size)
        {
            // file size zero
            return false;
        }

        auto data = MIX_LoadAudio(mixer, sound_file_path, true);
        if (!data)
        {
            sdl::print_error("Load Sound");
            return false;
        }

        mem::tag((u8*)data, size, fs::get_file_name(sound_file_path));

        return internal::add_sound(sound_list, sound, data);
    }


    bool load_music_from_bytes(ByteView const& bytes, Music& music, cstr tag)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        audio_assert(bytes.data && " *** no bytes data *** ");
        audio_assert(bytes.length && " *** no bytes length *** ");

        /* Does not copy data */

        internal::reset_music(music);

        if (!bytes.data || !bytes.length)
        {
            return false;
        }

        auto stream = SDL_IOFromConstMem((void*)bytes.data, bytes.length);

        auto data = MIX_LoadAudio_IO(mixer, stream, true, true);
        if (!data)
        {
            sdl::print_error("Load Music");
            return false;
        }

        mem::tag((u8*)data, bytes.length, tag);

        music.handle = (u64)data;

        return true;
    }


    bool load_sound_from_bytes(ByteView const& bytes, Sound& sound, cstr tag)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        audio_assert(bytes.data && " *** no bytes data *** ");
        audio_assert(bytes.length && " *** no bytes length *** ");

        /* Does not copy data */

        internal::reset_sound(sound);

        if (!bytes.data || !bytes.length)
        {
            return false;
        }

        auto stream = SDL_IOFromConstMem((void*)bytes.data, bytes.length);

        auto data = MIX_LoadAudio_IO(mixer, stream, true, true);
        if (!data)
        {
            sdl::print_error("Load Music");
            return false;
        }

        mem::tag((u8*)data, bytes.length, tag);

        return internal::add_sound(sound_list, sound, data);
    }


    f32 set_music_volume(f32 volume)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        return internal::set_music_track_volume(music_track, volume);
    }


    f32 set_sound_volume(f32 volume)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        volume = math::cxpr::clamp(volume, 0.0f, 1.0f);

        for (u32 i = 0; i < sound_list.capacity; i++)
        {
            internal::set_sound_track_volume(sound_list.tracks[i], volume);
        }

        return volume;
    }


    f32 set_sound_volume(Sound& sound, f32 volume)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        return internal::set_sound_track_volume(sound_list.get(sound), volume);
    }
    

    void play_music(Music& music)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        if (!internal::set_music(music_track, music))
        {
            return;
        }

        if (music.is_on)
        {
            return;
        }

        auto res = internal::play_music_track(music_track);
        if (res)
        {
            music.is_on = true;
            music.is_paused = false;
        }
    }


    void toggle_pause_music()
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");
        audio_assert(music_track.data && " *** music_track not set *** ");

        auto& music = *music_track.music;
        auto track = music_track.track;

        if (MIX_TrackPaused(track))
        {
            MIX_ResumeTrack(track);
            music.is_paused = false;
        }
        else
        {
            MIX_PauseTrack(track);
            music.is_paused = true;
        }
    }


    void stop_music()
    {
        internal::stop_music_track(music_track);
    }


    void fade_in_music(Music& music, u32 fade_ms)
    {
        if (internal::set_music(music_track, music) && internal::fade_in_music_track(music_track, fade_ms))
        {
            music.is_on = true;
            music.is_paused = false;
        }
    }


    void fade_out_music(u32 fade_ms)
    {
        internal::fade_out_music_track(music_track, fade_ms);
    }


    void play_sound(Sound& sound)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        sound.is_on = internal::play_sound_track_once(sound_list.get(sound));
    }   


    void play_sound_loop(Sound& sound)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");
        
        sound.is_on = internal::play_sound_track_loop(sound_list.get(sound));
    }


    void stop_sound(Sound& sound)
    {
        audio_assert(internal::is_initialized() && " *** audio not initialized *** ");

        if (!sound.is_on)
        {
            return;
        }

        internal::stop_sound_track(sound_list.get(sound));
    }


    void stop_sound()
    {
        if (!internal::is_initialized())
        {
            return;
        }

        for (u32 i = 0; i < sound_list.capacity; i++)
        {
            internal::stop_sound_track(sound_list.tracks[i]);
        }
    }
   
}