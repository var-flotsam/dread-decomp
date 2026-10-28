// From unit 1730 of tools/tu_map.py (0x7100b8c7c0-0x7100b8eb60, no reflected class): times on an
// animation's clock given in one of three units (frames, seconds, or a fraction of the whole), and
// their conversion. An animation's timing has its frame count at +0x24 and its length in seconds
// at +0x28; fn_7100b8aff0 turns seconds into frames and fn_7100b8af0c frames into seconds, through
// its table of (frame, second, rate) points. Names and layouts are placeholders.
// Holds fn_7100b8deec and fn_7100b8dff0.
#include "types.h"

// The units a time can be in.
enum UnkAnimTimeUnit : s32 {
    kAnimTimeFrames = 0,
    kAnimTimeSeconds = 1,
    kAnimTimeFraction = 2,
};

struct UnkAnimTiming {
    u8 pad_0x0[0x24];
    s32 frame_count;
    f32 length;
};

f32 fn_7100b8aff0(f32 seconds, const UnkAnimTiming* timing);  // seconds -> frames
f32 fn_7100b8af0c(f32 frames, const UnkAnimTiming* timing);   // frames -> seconds

struct UnkAnimTime {
    f32 value;
    s32 unit;
};

// `time` in `unit` on `timing`'s clock; 0 for an unknown unit.
// NON_MATCHING(minor): the original tests the target unit as 0, 2, then 1 and lays the branches
// out in that order; Clang 9 turns ours into a switch tested 2, 1, 0. The frame count is loaded
// before the value in the original's products.
f32 fn_7100b8deec(const UnkAnimTime* time, s32 unit, const UnkAnimTiming* timing) {
    switch (unit) {
    case kAnimTimeFrames:
        if (time->unit == kAnimTimeSeconds)
            return fn_7100b8aff0(time->value, timing);
        if (time->unit != kAnimTimeFrames)
            return time->value * (f32)timing->frame_count;
        return time->value;
    case kAnimTimeSeconds:
        if (time->unit == kAnimTimeSeconds)
            return time->value;
        if (time->unit != kAnimTimeFrames)
            return fn_7100b8af0c(time->value * (f32)timing->frame_count, timing);
        return fn_7100b8af0c(time->value, timing);
    case kAnimTimeFraction: {
        f32 frames;
        if (time->unit == kAnimTimeFrames)
            frames = time->value;
        else if (time->unit == kAnimTimeSeconds)
            frames = fn_7100b8aff0(time->value, timing);
        else
            return time->value;
        return frames / (f32)timing->frame_count;
    }
    default:
        return 0.0f;
    }
}

// A player of an animation: its timing and the current frame.
struct UnkAnimPlayer {
    u8 pad_0x0[0x10];
    const UnkAnimTiming* timing;
    u8 pad_0x18[0x60 - 0x18];
    f32 frame;
};

// What a time is measured from: an absolute time, the player's current position, or the
// position counted back from the end.
enum UnkAnimTimeRefKind : s32 {
    kAnimTimeAbsolute = 0,
    kAnimTimeFromPlayer = 1,
    kAnimTimeFromPlayerToEnd = 2,
};

struct UnkAnimTimeRef {
    s32 kind;
    s32 unit;
    f32 value;
};

// `ref` in seconds on `timing`'s clock: the time itself, the player's position (converted to
// ref's unit on the player's clock, read back on `timing`'s) or what is left of `timing` after it.
// 0 without a player.
// NON_MATCHING(minor): the original keeps the result in d8 (saved in the frame) and stores the
// player's position as one zero-extended word; ours returns from each branch and stores two words.
f32 fn_7100b8dff0(const UnkAnimTimeRef* ref, const UnkAnimPlayer* player, const UnkAnimTiming* timing) {
    if (ref->kind == kAnimTimeAbsolute) {
        if (ref->unit == kAnimTimeSeconds)
            return ref->value;
        if (ref->unit == kAnimTimeFrames)
            return fn_7100b8af0c(ref->value, timing);
        return fn_7100b8af0c(ref->value * (f32)timing->frame_count, timing);
    }
    f32 result = 0.0f;
    if (player) {
        UnkAnimTime pos = {player->frame, kAnimTimeFrames};
        f32 t = fn_7100b8deec(&pos, ref->unit, player->timing);
        if (ref->unit == kAnimTimeFrames)
            t = fn_7100b8af0c(t, timing);
        else if (ref->unit != kAnimTimeSeconds)
            t = fn_7100b8af0c(t * (f32)timing->frame_count, timing);
        if (ref->kind == kAnimTimeFromPlayer)
            result = t;
        else if (ref->kind == kAnimTimeFromPlayerToEnd)
            result = timing->length - t;
    }
    return result;
}
