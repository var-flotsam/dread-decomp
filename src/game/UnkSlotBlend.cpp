// Timed blends of the slot values of a model instance (translation unit 253 of data/tu_map.csv,
// the object of game/UnkSlots.h; its bone queries are src/game_s1/fn_71001c1b70.cpp). Up to nine
// blends (the entries at +0x308) each move one slot (id, kind, mode) of the slot table from the
// value it had to a target over a duration: fn_71001c3040 starts or restarts one, fn_71001c113c
// advances them all and applies the blended values. Names and layouts are placeholders inferred
// from the code.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkSlots.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;

// A blend of a slot's colour: the entries of UnkSlots seen whole.
struct UnkSlotBlend {
    CColor4F from;
    CColor4F to;
    f32 time;  // left, counting down to 0
    f32 duration;
    u32 id;
    u32 kind;
    u32 mode;
};
static_assert(__builtin_offsetof(UnkSlotBlend, id) == 0x28, "UnkSlotBlend placeholder layout");

static inline UnkSlotBlend* UnkBlendAt(UnkSlots* self, u64 i) {
    return reinterpret_cast<UnkSlotBlend*>(self->entries[i]);
}

// The global's time scale (+0x38).
static inline f32 UnkTimeScale(const UnkGlobal_710b372a98* g) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(g) + 0x38);
}

static inline f32 UnkNonNegative(f32 x) { return x <= 0.0f ? 0.0f : x; }

bool fn_7100199d88(void* table, const u64* key, u32* id);

// Sets a slot of the table, unless the global flag blocks requests (as fn_71001c2764 does).
static inline bool UnkSetSlot(UnkSlots* self, u32 id, u32 kind, const CColor4F* color, u32 mode) {
    if (g_unk_710b372a98->field_0x318367c)
        return false;
    return fn_71001996e0(self->info, id, kind, fn_7100030878(color), mode);
}

// Advances every blend by the scaled time step and, unless the global flag blocks requests, sets
// its slot to the blended value. A blend that runs out is moved past the end of the list (swapped
// with the last one) but still applied once, with its final value.
// NON_MATCHING(minor): the three slot fields are loaded in another order before the conversion call
// (the id alone and the kind and mode as a pair, where the original pairs the id and kind).
void fn_71001c113c(UnkSlots* self, f32 dt) {
    f32 step = UnkTimeScale(g_unk_710b372a98) * dt;
    if (!(step > 0.0f) || g_unk_710b372a98->field_0x318367c || !self->count)
        return;
    for (u32 i = 0; i < self->count;) {
        UnkSlotBlend* blend = UnkBlendAt(self, i);
        blend->time -= step;
        if (blend->time < 0.0f) {
            blend->time = 0.0f;
            if (--self->count != 0) {
                self->entries[i] = self->entries[self->count];
                self->entries[self->count] = reinterpret_cast<UnkSlotEntry*>(blend);
            }
        } else {
            ++i;
        }
        f32 t = blend->duration > 0.0f ? blend->time / blend->duration : 1.0f;
        // `to` when the time has run out (t = 0), `from` at the start (t = 1).
        CColor4F value = fn_710003088c(t, blend->to, blend->from);
        u32 mode = blend->mode;
        UnkSetSlot(self, blend->id, blend->kind, &value, mode);
    }
}

// The colour of the slot `id` (kind, mode), each component made non-negative, into `out`: the
// slot read of fn_71001c287c (src/game_lo/fn_71001c287c.cpp), inlined here.
static inline bool UnkGetSlot(UnkSlots* self, u32 id, u32 kind, CColor4F* out, u32 mode) {
    CVector4D value{};
    bool ok = fn_71001997c8(self->info, id, kind, &value, mode);
    CColor4F clamped;
    clamped.r = UnkNonNegative(value.x);
    clamped.g = UnkNonNegative(value.y);
    clamped.b = UnkNonNegative(value.z);
    clamped.a = UnkNonNegative(value.w);
    fn_7100030728(out, &clamped);
    return ok;
}

// Starts a blend of the slot registered under `key` (kind, mode) from its current value (each
// component made non-negative) to `target` over `duration`. A running blend of the same slot is
// restarted; otherwise a free one is taken, if fewer than nine run. False when the slot is unknown,
// its value can't be read or no blend is free.
// NON_MATCHING(minor): with no blend running the original still tests the count and the flag (the
// new blend is taken directly here), and it branches on the two conditions where this combines
// them; the index and flag registers are swapped.
bool fn_71001c3040(UnkSlots* self, u64 key, u32 kind, const CColor4F* target, u32 mode, f32 duration) {
    u32 id = 0;
    CColor4F from(1.0f, 1.0f, 1.0f, 1.0f);
    if (!fn_7100199d88(self->info, &key, &id))
        return false;
    if (!UnkGetSlot(self, id, kind, &from, mode))
        return false;
    u32 found = id;
    u32 count = self->count;
    s32 index = count;
    bool running = false;
    for (u64 i = 0; i < count; ++i) {
        UnkSlotBlend* blend = UnkBlendAt(self, i);
        if (blend->id == found && blend->kind == kind) {
            bool same = blend->mode == mode;
            if (same)
                index = i;
            running |= same;
        }
    }
    if (count < 9 || running) {
        if (!running)
            self->count = count + 1;
        UnkSlotBlend* blend = UnkBlendAt(self, index);
        fn_7100030728(&blend->from, &from);
        fn_7100030728(&blend->to, target);
        blend->time = duration;
        blend->duration = duration;
        blend->id = found;
        blend->kind = kind;
        blend->mode = mode;
        return true;
    }
    return false;
}
