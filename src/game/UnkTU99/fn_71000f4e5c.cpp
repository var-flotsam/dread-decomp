// Unit 99 (CTunableSoundSystemATK): progress of the position through the current section's range.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSection {
    s32 start;
    s32 end;
    u8 pad_0x08[0x18];
};

struct UnkTunable_f4e5c {
    u8 pad_0x00[0x38];
    s32 count;
    u8 pad_0x3c[0x10];
    UnkSection sections[1];
    u8 pad_0x6c[0x13c - 0x6c];
    s32 current;
    u8 pad_0x140[0x08];
    s64 position;
};

static_assert(__builtin_offsetof(UnkTunable_f4e5c, sections) == 0x4c, "");
static_assert(__builtin_offsetof(UnkTunable_f4e5c, position) == 0x148, "");

f32 fn_71000f4e5c(UnkTunable_f4e5c* self) {
    u32 index = self->current;
    if ((s32)index < 0 || (s32)index >= self->count)
        return 0.0f;
    UnkSection& section = self->sections[index];
    s32 end = section.end;
    s32 start = section.start;
    if (end <= start)
        return 0.0f;
    s64 pos = self->position;
    s64 elapsed = pos - start;
    if (pos < start)
        return 0.0f;
    if (pos >= end)
        return 1.0f;
    s32 length = end - start;
    if (length < 1)
        return 0.0f;
    return (f32)elapsed / (f32)length;
}
