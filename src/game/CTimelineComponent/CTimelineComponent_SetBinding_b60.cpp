// CTimelineComponent (unit from tools/tu_map.py): a bound call taking three values (id, delay,
// unique): sets the id on the owner's timeline component (+0x8) to the all-flags mask. Each value
// is read out of a holder (in place, or behind a pointer when the holder is in mode 1 or its type is
// wider than 8 bytes; the id is null when its holder has no type), as in
// src/base/spatial/CAABox2D_Setters.cpp. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkValueType_b60 {
    u8 pad_0x0[0x18];
    u64 size;
};

struct UnkValue_b60 {
    union {
        u8 inline_data[8];
        u8* data;
    };
    UnkValueType_b60* type;
    u8 pad_0x10[0x4];
    u32 mode;
};

struct UnkTimelineBindingOwner_b60 {
    u8 pad_0x0[0x8];
    UnkTimelineComponent* timeline;
};

static inline const u8* ValueData_b60(const UnkValue_b60* v) {
    return (v->mode == 1 || v->type->size >= 9) ? v->data : v->inline_data;
}

void fn_7100a2da0c(UnkTimelineBindingOwner_b60* owner, UnkValue_b60* const* values) {
    UnkValue_b60* args = *values;
    UnkTimelineComponent* timeline = owner->timeline;
    const u64* id = args[0].type ? reinterpret_cast<const u64*>(ValueData_b60(&args[0])) : nullptr;
    f32 delay = *reinterpret_cast<const f32*>(ValueData_b60(&args[1]));
    bool unique = *reinterpret_cast<const bool*>(ValueData_b60(&args[2]));
    alignas(8) u32 mask;
    mask = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask, unique, delay);
}
