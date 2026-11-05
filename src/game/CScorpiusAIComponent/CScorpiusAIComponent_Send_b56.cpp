// The CScorpiusAIComponent unit: sends 0xea4b14555468f96a under kind 0x722b01c64fe0ffe0 to the
// owner's sub-object at +0x220 with mode 1, its first descriptor a local record (-1, then `value`
// and 1) in place of the global one (otherwise as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkScorpiusOwner_b56a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkScorpiusAI_b56a3 {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner_b56a3* owner;
};

// A typed value: a 4-byte payload and its kind (1: float).
struct UnkScorpiusValue_b56a3 {
    f32 value;
    u32 kind;
};

struct UnkScorpiusArg_b56a3 {
    s64 field_0x0;
    UnkScorpiusValue_b56a3 value;

    UnkScorpiusArg_b56a3(UnkScorpiusValue_b56a3 v) : field_0x0(-1), value(v) {}
};

static inline UnkScorpiusValue_b56a3 MakeFloat_b56a3(f32 value) {
    UnkScorpiusValue_b56a3 v;
    v.value = value;
    v.kind = 1;
    return v;
}

void fn_7100697f44(UnkScorpiusAI_b56a3* self, f32 value) {
    void* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    UnkScorpiusArg_b56a3 arg(MakeFloat_b56a3(value));
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0xea4b14555468f96a, 0x722b01c64fe0ffe0, &mode, &arg, &g_unk_7172769740, &mask);
}
