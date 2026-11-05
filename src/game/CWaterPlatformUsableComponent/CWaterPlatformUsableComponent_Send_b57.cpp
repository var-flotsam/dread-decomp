// CWaterPlatformUsableComponent unit (tools/tu_map.py): with the flag at +0x5c6 set, reads the level of
// the owner's (+0x30, its +0x220) entry registered under g_unk_71727537e0 (fn_71002d57ac; null unless it
// is of the class fn_71002e2544 describes) through fn_71002e2958, and when the level at +0x54c minus a
// hundredth of the result's +0x1c reaches the one at +0x554, sends the sub-object a fixed hash with mode
// 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h, written out so the result keeps the top stack slot); then
// clears the flag. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkEntry;
UnkEntry* fn_71002d57ac(void* registry, const void* key);

struct UnkWaterEntry_b57 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkWaterEntry_b57*)>(vtable[0])(this);
    }
};

struct UnkWaterLevel_b57 {
    u8 pad_0x0[0x1c];
    f32 field_0x1c;
    u8 pad_0x20[0x8];
    u64 field_0x28;
    u8 pad_0x30[0x10];
};
UnkWaterLevel_b57 fn_71002e2958(UnkWaterEntry_b57* entry);

extern __attribute__((visibility("hidden"))) char g_unk_71727537e0[];

struct UnkWaterSub_b57 {
    u8 pad_0x0[0x40];
    void* field_0x40;
};

struct UnkWaterOwner_b57 {
    u8 pad_0x0[0x220];
    UnkWaterSub_b57* field_0x220;
};

struct UnkWaterPlatformSend_b57 {
    u8 pad_0x0[0x30];
    UnkWaterOwner_b57* owner;
    u8 pad_0x38[0x54c - 0x38];
    f32 field_0x54c;
    u8 pad_0x550[4];
    f32 field_0x554;
    u8 pad_0x558[0x5c6 - 0x558];
    bool field_0x5c6;
};

static inline UnkWaterEntry_b57* FindEntry_b57(UnkWaterPlatformSend_b57* self) {
    UnkWaterEntry_b57* e =
        reinterpret_cast<UnkWaterEntry_b57*>(fn_71002d57ac(self->owner->field_0x220->field_0x40, g_unk_71727537e0));
    return e && fn_710009d50c(e->vf_0x0(), fn_71002e2544()) ? e : nullptr;
}

void fn_7100aa6898(UnkWaterPlatformSend_b57* self) {
    if (!self->field_0x5c6)
        return;
    UnkWaterLevel_b57 level = fn_71002e2958(FindEntry_b57(self));
    f32 drop = level.field_0x1c * -0.01f;
    if (self->field_0x54c + drop >= self->field_0x554) {
        void* target = self->owner->field_0x220;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x6a17390fc912b900, 0x94b6e812cbf3fb75, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
    self->field_0x5c6 = false;
}
