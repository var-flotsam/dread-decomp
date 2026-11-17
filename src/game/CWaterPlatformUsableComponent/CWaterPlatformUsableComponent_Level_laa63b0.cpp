// CWaterPlatformUsableComponent unit (tools/tu_map.py): sets the platform's height (+0x5b8) from the water
// level of the owner's bone-control entry (as fn_7100aa6898 reads it), eased ("qq") and scaled by +0x560,
// sends the owner's +0x220 sub-object a fixed hash with mode 0 and runs fn_7100aa5980. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU399.h"
#include "game/UnkTU53.h"
#include "types.h"

struct UnkEntry;

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

struct UnkWaterPlatformLevel_laa63b0 {
    u8 pad_0x0[0x30];
    UnkWaterOwner_b57* owner;
    u8 pad_0x38[0x54c - 0x38];
    f32 full_level;  // 0x54c
    u8 pad_0x550[0x10];
    f32 travel;  // 0x560
    u8 pad_0x564[0x54];
    f32 height;  // 0x5b8
};
static_assert(__builtin_offsetof(UnkWaterPlatformLevel_laa63b0, height) == 0x5b8, "layout");

struct UnkWaterPlatform_A5_b73;
bool fn_7100aa5980(UnkWaterPlatform_A5_b73* self);

static inline UnkWaterEntry_b57* FindEntry_laa63b0(UnkWaterPlatformLevel_laa63b0* self) {
    UnkWaterEntry_b57* e =
        reinterpret_cast<UnkWaterEntry_b57*>(fn_71002d57ac(self->owner->field_0x220->field_0x40, g_unk_71727537e0));
    return e && fn_710009d50c(e->vf_0x0(), fn_71002e2544()) ? e : nullptr;
}

void fn_7100aa63b0(UnkWaterPlatformLevel_laa63b0* self) {
    base::reflection::UnkReflTypeIdArg mode;
    UnkWaterLevel_b57 level = fn_71002e2958(FindEntry_laa63b0(self));
    f32 water = level.field_0x1c;
    f32 full = self->full_level;
    self->height = self->travel * fn_7100076020((full + water * -0.01f) / full, "qq");
    void* target = self->owner->field_0x220;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0xa0e33be014f44bd7, 0x94b6e812cbf3fb75, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);
    fn_7100aa5980(reinterpret_cast<UnkWaterPlatform_A5_b73*>(self));
}
