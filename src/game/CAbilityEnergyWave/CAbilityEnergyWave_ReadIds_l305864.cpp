// CAbilityEnergyWave unit: when the message's stream has data, reads three names from it and stores them
// as the string ids at +0x390, +0x398 and +0x3a0; returns whether it did. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU56.h"
#include "types.h"

struct UnkIdStream_l305864 {
    u8 pad_0x0[0x8];
    u64 pos;  // 0x8
    u8 pad_0x10[0x10];
    u64 size;  // 0x20
};

struct UnkIdMessage_l305864 {
    u8 pad_0x0[0x38];
    UnkIdStream_l305864 stream;  // 0x38
};

struct UnkWaveIds_l305864 {
    u8 pad_0x0[0x390];
    base::global::UnkStrId field_0x390;
    base::global::UnkStrId field_0x398;
    base::global::UnkStrId field_0x3a0;
};

bool fn_7100305864(UnkWaveIds_l305864* self, UnkIdMessage_l305864* message) {
    UnkIdStream_l305864* stream = &message->stream;
    if (stream->pos >= stream->size)
        return false;
    const char* first;
    const char* second;
    const char* third;
    fn_710007cee4(stream, &first);
    fn_710007cee4(stream, &second);
    fn_710007cee4(stream, &third);
    {
        base::global::UnkStrId id(first, true);
        self->field_0x390 = static_cast<base::global::UnkStrId&&>(id);
    }
    {
        base::global::UnkStrId id(second, true);
        self->field_0x398 = static_cast<base::global::UnkStrId&&>(id);
    }
    {
        base::global::UnkStrId id(third, true);
        self->field_0x3a0 = static_cast<base::global::UnkStrId&&>(id);
    }
    return true;
}
