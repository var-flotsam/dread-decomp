// UnkTU253: reads the current colour of a slot of the slot table (clamped to non-negative), then
// records a fade for it in the entry list: the entry with the same id, kind and mode, or a new one
// while fewer than nine are in use. Returns false when the slot is missing or the list is full.
// Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/UnkSlots.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;

// An entry of the list as this function fills it.
struct UnkSlotFade_l1c2ec0 {
    CColor4F from;  // 0x0
    CColor4F to;    // 0x10
    f32 duration;   // 0x20
    f32 remaining;  // 0x24
    u32 id;         // 0x28
    u32 kind;       // 0x2c
    u32 mode;       // 0x30
};

static inline f32 ClampPositive(f32 x) { return x <= 0.0f ? 0.0f : x; }

// The colour of the slot, each component clamped to be non-negative (as fn_71001c287c).
static inline bool GetSlotColour(UnkSlots* self, u32 id, u32 kind, CColor4F* out, u32 mode) {
    CVector4D box = {};
    bool found = fn_71001997c8(self->info, id, kind, &box, mode);
    CColor4F clamped;
    clamped.r = ClampPositive(box.x);
    clamped.g = ClampPositive(box.y);
    clamped.b = ClampPositive(box.z);
    clamped.a = ClampPositive(box.w);
    *out = clamped;
    return found;
}

bool fn_71001c2ec0(void* target, u32 id, u32 kind, CVector4D* colour, u32 mode, f32 time) {
    UnkSlots* self = static_cast<UnkSlots*>(target);
    CColor4F current(1.0f, 1.0f, 1.0f, 1.0f);
    bool found = GetSlotColour(self, id, kind, &current, mode);
    if (!found)
        return false;

    u32 count = self->count;
    bool exists = false;
    s32 index = count;
    for (u32 i = 0; i < count; ++i) {
        UnkSlotFade_l1c2ec0* e = reinterpret_cast<UnkSlotFade_l1c2ec0*>(self->entries[i]);
        if (e->id == id && e->kind == kind && e->mode == mode) {
            exists = true;
            index = i;
        }
    }
    if (count >= 9 && !exists)
        return false;
    if (!exists)
        self->count = count + 1;

    UnkSlotFade_l1c2ec0* e = reinterpret_cast<UnkSlotFade_l1c2ec0*>(self->entries[index]);
    e->from = current;
    e->to = reinterpret_cast<const CColor4F&>(*colour);
    e->duration = time;
    e->remaining = time;
    e->id = id;
    e->kind = kind;
    e->mode = mode;
    return true;
}
