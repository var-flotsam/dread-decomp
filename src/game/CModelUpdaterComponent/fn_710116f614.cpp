// CModelUpdaterComponent's unit (tools/tu_map.py unit 2719): copies a description's three named ranges into
// the updater: each range's id, and its four values clamped at zero when the id is set; then two words.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkModelRangeDesc_l16f614 {
    f32 values[4];
    CStrId id;
    u64 field_0x18;
};

struct UnkModelUpdaterDesc_l16f614 {
    u8 pad_0x0[0x18];
    UnkModelRangeDesc_l16f614 ranges[3];  // 0x18
    u32 field_0x78;
    u32 field_0x7c;
};

struct UnkModelUpdater_l16f614 {
    u32 field_0x0;
    f32 values[3][4];  // 0x4
    u8 pad_0x34[0x34];
    CStrId ids[3];  // 0x68
    u32 field_0x80;
    u32 field_0x84;
};

static inline f32 ClampPositive(f32 value) { return value <= 0.0f ? 0.0f : value; }

// Takes a range's id, and its values when the id is set.
static inline void SetRange(CStrId& id, f32* values, const UnkModelRangeDesc_l16f614& range) {
    id = range.id;
    if (id.data != &g_unk_7101d47108->empty) {
        values[0] = ClampPositive(range.values[0]);
        values[1] = ClampPositive(range.values[1]);
        values[2] = ClampPositive(range.values[2]);
        values[3] = ClampPositive(range.values[3]);
    }
}

void fn_710116f614(UnkModelUpdater_l16f614* self, const UnkModelUpdaterDesc_l16f614* desc) {
    if (!desc)
        return;
    self->field_0x80 = desc->field_0x78;
    self->field_0x84 = desc->field_0x7c;
    SetRange(self->ids[0], self->values[0], desc->ranges[0]);
    SetRange(self->ids[1], self->values[1], desc->ranges[1]);
    SetRange(self->ids[2], self->values[2], desc->ranges[2]);
}
