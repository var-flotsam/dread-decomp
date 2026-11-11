// CScorpiusMovingPoisonousGasAttack unit (tools/tu_map.py, the name from reflection): fn_7100688420 goes on to
// fn_7100689c08 only when the owner's AI component is a CScorpiusAIComponent with its flag at +0x14cf9 set (and
// +0x14cfa set or +0x14c29 clear), whose current attack (fn_7101105970) is neither this one nor the one named
// by g_unk_7172703318 (fn_7101109df4). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkScorpiusObject_b69a4 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkScorpiusObject_b69a4*)>(vtable[0])(this);
    }
};

struct UnkScorpiusAI_b69a4 : UnkScorpiusObject_b69a4 {
    u8 pad_0x8[0x14c21];
    bool field_0x14c29;
    u8 pad_0x14c2a[0xcf];
    bool field_0x14cf9;
    bool field_0x14cfa;
};

struct UnkGasAttack_b69a4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

extern char g_unk_7172703318[] __attribute__((visibility("hidden")));  // reached without the GOT
void* fn_7101105970(UnkScorpiusAI_b69a4* ai);
bool fn_7100689c08(UnkGasAttack_b69a4* self, void* arg, u32 flag);

bool fn_7100688420(UnkGasAttack_b69a4* self, void* arg, u32 flag) {
    UnkScorpiusAI_b69a4* ai = static_cast<UnkScorpiusAI_b69a4*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_7100690c74()))
        return false;
    if (!ai->field_0x14cf9)
        return false;
    if (!ai->field_0x14cfa && ai->field_0x14c29)
        return false;
    if (fn_7101105970(ai) == self)
        return false;
    if (fn_7101105970(ai) == fn_7101109df4(ai, g_unk_7172703318))
        return false;
    return fn_7100689c08(self, arg, flag);
}
