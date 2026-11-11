// CMagnetSlidingBlockRailComponent's unit (tools/tu_map.py unit 1414): the rail's glow material (key
// 0x700c49f3f0afc866 in the model's slot table, found through the material id g_unk_710164b250): switched
// on or off with a 0.1 fade (fn_7100922e30, only on a change of +0x3d) and set to a given value (fn_7100922f28),
// the other components the material's own, clamped to be non-negative. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "game/UnkSlots.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::color::CColor4F;

bool fn_7100199d88(void* map, const u64* key, u32* index);
extern const u64 g_unk_710164b250;

struct UnkRailMaterial_ovb0097 {
    u8 pad_0x0[0x14c];
    f32 r, g, b, a;  // 0x14c
};

struct UnkRailMaterials_ovb0097 {
    u8 pad_0x0[0x48];
    UnkRailMaterial_ovb0097** entries;  // 0x48
};

struct UnkRailModel_ovb0097 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // 0x138
};

struct UnkRailModelComponent_ovb0097 {
    u8 pad_0x0[0x50];
    UnkRailModel_ovb0097* model;  // 0x50
};

struct UnkRailOwner_ovb0097 {
    u8 pad_0x0[0x228];
    UnkRailModelComponent_ovb0097* model;  // 0x228
};

struct UnkMagnetSlidingBlockRail_ovb0097 {
    u8 pad_0x0[0x30];
    UnkRailOwner_ovb0097* owner;  // 0x30
    u8 pad_0x38[5];
    bool on;  // 0x3d
};

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

static inline CColor4F MaterialColor(const UnkRailMaterial_ovb0097* mat) {
    f32 r = mat->r, g = mat->g, b = mat->b, a = mat->a;
    return CColor4F(NonNegative(r), NonNegative(g), NonNegative(b), NonNegative(a));
}

static inline UnkRailMaterial_ovb0097* FindMaterial(UnkRailModel_ovb0097* model, u32& index) {
    index = 0;
    fn_7100199d88(model->slots->info, &g_unk_710164b250, &index);
    u32 i = index;
    return reinterpret_cast<UnkRailMaterials_ovb0097*>(model->slots->info)->entries[i];
}

void fn_7100922e30(UnkMagnetSlidingBlockRail_ovb0097* self, bool on) {
    if (self->on == on)
        return;
    self->on = on;
    u32 index;
    UnkRailMaterial_ovb0097* mat = FindMaterial(self->owner->model->model, index);
    CColor4F color = MaterialColor(mat);
    color.r = on ? 1.0f : 0.0f;
    fn_71001c3040(self->owner->model->model->slots, 0x700c49f3f0afc866, 0, &color.r, 1, 0.1f);
}

void fn_7100922f28(UnkMagnetSlidingBlockRail_ovb0097* self, f32 alpha) {
    u32 index;
    UnkRailMaterial_ovb0097* mat = FindMaterial(self->owner->model->model, index);
    CColor4F color = MaterialColor(mat);
    color.g = NonNegative(alpha);
    fn_71001c28f8(self->owner->model->model->slots, 0x700c49f3f0afc866, 0, &color, 1);
}
