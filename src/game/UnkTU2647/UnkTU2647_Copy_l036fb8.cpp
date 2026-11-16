// Unit 2647 (sound::CAudioPreset): builds a preset as a copy of another: the asset base is copied, the
// members get their defaults, then the other's values are assigned over them. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2647.h"
#include "types.h"

using base::global::CStrId;

// The asset base's copy constructor (unit 24).
void fn_7100016c44(void* self, const void* other);

struct UnkPathIdTable_710103c4e8;
void fn_7101036a9c(UnkPathIdTable_710103c4e8* self);

inline const UnkGameManagerIdTable& PresetIds_l036fb8() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

// A pair of values (a range, as far as the defaults tell).
struct UnkPresetRange_l036fb8 {
    f32 min;
    f32 max;
};

// The preset's own values, with their defaults.
struct UnkPresetSettings_l036fb8 {
    s32 field_0x568 = 0x7fffffff;
    s32 field_0x56c = 0x7fffffff;
    u32 field_0x570 = 1;
    u32 field_0x574 = 0;
    s32 field_0x578 = -1;
    UnkPresetRange_l036fb8 field_0x57c{0.0f, 0.0f};
    UnkPresetRange_l036fb8 field_0x584{0.0f, 1.0f};
    UnkPresetRange_l036fb8 field_0x58c{0.0f, 1.0f};
    f32 field_0x594 = 1.0f;
    f32 field_0x598 = 0.0f;
    f32 field_0x59c = 0.0f;
    f32 field_0x5a0 = 0.0f;
    f32 field_0x5a4 = 0.0f;
    f32 field_0x5a8 = 0.0f;
    f32 field_0x5ac = -1.0f;
    f32 field_0x5b0 = -1.0f;
    f32 field_0x5b4 = 0.6f;
    f32 field_0x5b8 = 10.0f;
    f32 field_0x5bc = 20.0f;
    f32 field_0x5c0 = 10.0f;  // not copied from the other preset
    bool field_0x5c4 = false;
    bool field_0x5c5 = false;
    bool field_0x5c6 = false;
    bool field_0x5c7 = true;
    bool field_0x5c8 = true;
    bool field_0x5c9 = false;
};

struct UnkAudioPreset_l036fb8 {
    void* vtable;
    u8 unk_0x8[0x50];
    CStrId id_a;      // 0x58
    CStrId id_b;      // 0x60
    u8 items[0x500];  // 0x68: 16 pairs of path-id lists
    UnkPresetSettings_l036fb8 settings;  // 0x568
};
static_assert(__builtin_offsetof(UnkAudioPreset_l036fb8, settings) == 0x568, "layout");

void fn_7101036fb8(void* value, const void* arg) {
    auto* self = static_cast<UnkAudioPreset_l036fb8*>(value);
    const auto* other = static_cast<const UnkAudioPreset_l036fb8*>(arg);
    fn_7100016c44(self, other);
    self->vtable = g_unk_7101cac748 + 0x10;
    new (&self->id_a) CStrId(PresetIds_l036fb8().field_0x14e0);
    new (&self->id_b) CStrId(g_unk_7101d470e0);
    fn_7101036a9c(reinterpret_cast<UnkPathIdTable_710103c4e8*>(self->items));
    new (&self->settings) UnkPresetSettings_l036fb8;

    self->id_a = other->id_a;
    self->id_b = other->id_b;
    fn_71010372dc(self->items, other->items);
    UnkPresetSettings_l036fb8& to = self->settings;
    const UnkPresetSettings_l036fb8& from = other->settings;
    to.field_0x568 = from.field_0x568;
    to.field_0x56c = from.field_0x56c;
    to.field_0x570 = from.field_0x570;
    to.field_0x574 = from.field_0x574;
    to.field_0x578 = from.field_0x578;
    to.field_0x57c.min = from.field_0x57c.min;
    to.field_0x57c.max = from.field_0x57c.max;
    to.field_0x584.min = from.field_0x584.min;
    to.field_0x584.max = from.field_0x584.max;
    to.field_0x58c.min = from.field_0x58c.min;
    to.field_0x58c.max = from.field_0x58c.max;
    to.field_0x594 = from.field_0x594;
    to.field_0x598 = from.field_0x598;
    to.field_0x59c = from.field_0x59c;
    to.field_0x5a0 = from.field_0x5a0;
    to.field_0x5a4 = from.field_0x5a4;
    to.field_0x5a8 = from.field_0x5a8;
    to.field_0x5ac = from.field_0x5ac;
    to.field_0x5b0 = from.field_0x5b0;
    to.field_0x5b4 = from.field_0x5b4;
    to.field_0x5b8 = from.field_0x5b8;
    to.field_0x5bc = from.field_0x5bc;
    to.field_0x5c4 = from.field_0x5c4;
    to.field_0x5c5 = from.field_0x5c5;
    to.field_0x5c6 = from.field_0x5c6;
    to.field_0x5c7 = from.field_0x5c7;
    to.field_0x5c8 = from.field_0x5c8;
    to.field_0x5c9 = from.field_0x5c9;
}
