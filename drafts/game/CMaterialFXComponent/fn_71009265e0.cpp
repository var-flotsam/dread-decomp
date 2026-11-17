// CMaterialFXComponent unit (tools/tu_map.py): advances one recorded material colour change by a time step.
// While its delay runs nothing else happens; then the colour is blended from the start colour to the target
// one along the record's curve and applied to the key's slot (kind 0: +0x240 and its target +0x2e0; kind 1:
// +0x14c and +0x1f0). When the change is over (or its key is gone) the target colour is applied and the
// record is swapped out of the table. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU1273.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkMaterialFXRecord_l9265e0 {
    u32 curve;
    u32 slot;  // 0x4
    u32 kind;  // 0x8
    CVector4D color;   // 0xc: the start colour
    CVector4D target;  // 0x1c
    u64 key[3];        // 0x30
    f32 elapsed;       // 0x48
    f32 duration;      // 0x4c
    f32 delay;         // 0x50
};
static_assert(sizeof(UnkMaterialFXRecord_l9265e0) == 0x58, "UnkMaterialFXRecord_l9265e0 size");

struct UnkMaterialFXTarget_l9265e0 {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkMaterialFXSlots_l9265e0 {
    u8 pad_0x0[0x14c];
    CVector4D field_0x14c[10];
    UnkMaterialFXTarget_l9265e0* field_0x1f0[10];
    CVector4D field_0x240[10];
    UnkMaterialFXTarget_l9265e0* field_0x2e0[10];
};

struct UnkMaterialFXComponent_l9265e0 {
    u8 pad_0x0[0x48];
    u32 count;  // 0x48
    u8 pad_0x4c[0x4];
    UnkMaterialFXRecord_l9265e0* records[50];  // 0x50
};

static inline UnkKey* Key(UnkMaterialFXRecord_l9265e0* record) {
    return reinterpret_cast<UnkKey*>(record->key);
}

static inline UnkMaterialFXSlots_l9265e0* Slots(UnkKey* key) {
    return reinterpret_cast<UnkMaterialFXSlots_l9265e0*>(fn_710080fa28(key));
}

// Applies `color` to the record's slot of its key, by the record's kind.
static inline void ApplyColor(UnkMaterialFXRecord_l9265e0* r, const CVector4D& color) {
    switch (r->kind) {
    case 0: {
        UnkMaterialFXSlots_l9265e0* s = Slots(Key(r));
        u32 n = r->slot;
        base::math::Assign(s->field_0x240[n], color);
        if (UnkMaterialFXTarget_l9265e0* t = s->field_0x2e0[n])
            *t->value = color;
        break;
    }
    case 1: {
        UnkMaterialFXSlots_l9265e0* s = Slots(Key(r));
        u32 n = r->slot;
        base::math::Assign(s->field_0x14c[n], color);
        if (UnkMaterialFXTarget_l9265e0* t = s->field_0x1f0[n])
            *t->value = color;
        break;
    }
    }
}

// How far the change has got, from 0 to 1.
static inline f32 Progress(const UnkMaterialFXRecord_l9265e0* r) {
    f32 t = r->elapsed / r->duration;
    return t < 1.0f ? t : 1.0f;
}

// The colour at `k` between the start colour (0) and the target (1).
static inline UnkVector4D Blend(UnkMaterialFXRecord_l9265e0* r, f32 k) {
    if (k >= 1.0f)
        return UnkVector4D(r->target);
    if (k < 0.0f)
        return UnkVector4D(r->color);
    UnkVector4D from = fn_710007b5c0(&r->color, 1.0f - k);
    UnkVector4D to = fn_710007b5c0(&r->target, k);
    return fn_710007b5ec(&from, &to);
}

void fn_71009265e0(UnkMaterialFXComponent_l9265e0* self, u32 index, f32 dt) {
    UnkMaterialFXRecord_l9265e0* r = self->records[index];
    if (r->delay > 0.0f) {
        f32 delay = r->delay - dt;
        r->delay = delay > 0.0f ? delay : 0.0f;
        return;
    }
    r->elapsed += dt;
    if (fn_710080fb08(Key(r)) && !(r->elapsed >= r->duration)) {
        // Curves: 0 linear, 2 ease in, 3 ease out; 1 and unknown ones are not blended.
        u32 curve = r->curve;
        if (curve > 3 || curve == 1)
            return;
        f32 t = Progress(r);
        f32 k = curve == 3 ? 1.0f - (1.0f - t) * (1.0f - t) : curve == 2 ? t * t : t;
        UnkVector4D color = Blend(r, k);
        ApplyColor(r, color);
        return;
    }
    if (fn_710080fb08(Key(r)))
        ApplyColor(r, r->target);
    u32 last = self->count - 1;
    UnkMaterialFXRecord_l9265e0* tmp = self->records[last];
    self->records[last] = self->records[index];
    self->records[index] = tmp;
    self->count = last;
}
