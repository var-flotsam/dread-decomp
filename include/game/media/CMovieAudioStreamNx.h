#pragma once

// CMovieAudioStreamNx: functions and globals of this translation unit that other files use (tools/tu_map.py unit 326,
// 0x7100220adc-0x7100222970). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
extern void* g_unk_7101a204f0[];

struct UnkMovieAudioOut;
struct UnkPlayer_7100221938;
// Opens the audio output at `sampleRate` for `channels`; `outResampling` says whether it resamples.
s32 fn_7100220d2c(UnkMovieAudioOut* self, s32 sampleRate, s32 channels, bool* outResampling);
// Sets the stream up for a format and channel count (`flag`: the description's kind is 4) and opens
// its audio output at 48000 Hz; returns the open's result.
s32 fn_7100221698(UnkPlayer_7100221938* self, u32 format, u32 channels, bool flag);
