// CAbilityGhostAura unit: the position on the recorded trail at a distance along it (value times the scale),
// moving the cursor forward and interpolating between the two points around it; reports a landing point
// passed on the way. Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

// As src/game/CAbilityGhostAura/CAbilityGhostAura_Update_ovb0040.cpp defines it.
struct UnkGhostAuraEntry_b0040 {
    u32 words[3];
    f32 field_0xc;
    u16 field_0x10;

    UnkGhostAuraEntry_b0040() {
        const UnkGhostAuraEntry_b0040 init = {{0, 0, 0}, -1.0f, 0};
        __builtin_memcpy(this, &init, 0x12);
    }
    constexpr UnkGhostAuraEntry_b0040(const u32 (&w)[3], f32 c, u16 h) : words{w[0], w[1], w[2]}, field_0xc(c), field_0x10(h) {}
    UnkGhostAuraEntry_b0040(UnkGhostAuraEntry_b0040&& o) {
        words[0] = o.words[0];
        words[1] = o.words[1];
        words[2] = o.words[2];
        __builtin_memcpy(&field_0xc, &o.field_0xc, sizeof(field_0xc) + sizeof(field_0x10));
    }
};

using UnkGhostAuraTrail_l3139c0 = base::global::CRntVector<UnkGhostAuraEntry_b0040>;

void fn_7100314c88(base::global::CRntVector<UnkGhostAuraEntry_b0040>* self, u32 n);

struct UnkGhostAuraPoint_l3139c0 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkGhostAuraActor_l3139c0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkGhostAuraLink_l3139c0 {
    u8 pad_0x0[0x30];
    UnkGhostAuraActor_l3139c0* actor;
};

struct UnkGhostAura_l3139c0 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraLink_l3139c0* link;
    u8 pad_0xc8[0x3c8 - 0xc8];
    UnkGhostAuraTrail_l3139c0 trail;  // 0x3c8
    u8 pad_0x3f0[0x458 - 0x3f0];
    u32 cursor;  // 0x458
    u8 pad_0x45c[0x470 - 0x45c];
    f32 scale;  // 0x470
    u8 pad_0x474[0x496 - 0x474];
    u8 landing;  // 0x496
};
static_assert(__builtin_offsetof(UnkGhostAura_l3139c0, landing) == 0x496, "UnkGhostAura_l3139c0 layout");

// An entry's position (its first three words) and whether it is a landing (the low byte at 0x10).
static inline const UnkGhostAuraPoint_l3139c0& Pos(const UnkGhostAuraEntry_b0040& e) {
    return *reinterpret_cast<const UnkGhostAuraPoint_l3139c0*>(e.words);
}
static inline u8 Landing(const UnkGhostAuraEntry_b0040& e) {
    return static_cast<u8>(e.field_0x10);
}

// The entry at `index`, growing the trail to cover it first when it may grow (CRntVector::GrowAt with the
// unit's out-of-line resize).
static inline UnkGhostAuraEntry_b0040& At(UnkGhostAuraTrail_l3139c0* trail, u32 index) {
    if (trail->field_0x14 && trail->count <= index) {
        if (trail->capacity <= index && trail->step != 0)
            trail->Reserve(trail->GrownCapacity(index));
        fn_7100314c88(trail, index + 1);
    }
    return trail->data[index];
}

// The point at `distance` along the trail between `prev` and `next`; false when they coincide.
static inline bool Interpolate(const UnkGhostAuraEntry_b0040& prev, const UnkGhostAuraEntry_b0040& next,
                               f32 distance, UnkGhostAuraPoint_l3139c0* out) {
    f32 dx = Pos(next).x - Pos(prev).x;
    f32 dy = Pos(next).y - Pos(prev).y;
    f32 dz = Pos(next).z - Pos(prev).z;
    f32 len_sq = dx * dx + dy * dy + dz * dz;
    if (len_sq <= 0.0f)
        return false;
    f32 t = (distance - prev.field_0xc) / sqrtf(len_sq);
    out->x = Pos(prev).x + dx * t;
    out->y = Pos(prev).y + dy * t;
    out->z = dz * t + Pos(prev).z;
    return true;
}

bool fn_71003139c0(UnkGhostAura_l3139c0* self, UnkGhostAuraPoint_l3139c0* out, bool* landed,
                   UnkGhostAuraPoint_l3139c0* landing_pos, f32 value) {
    UnkGhostAuraTrail_l3139c0* trail = &self->trail;
    u32 i = self->cursor;
    u32 count = trail->count;
    if (i >= count) {
        if (count == 0) {
            UnkGhostAuraActor_l3139c0* actor = self->link->actor;
            out->x = actor->x;
            out->y = actor->y;
            out->z = actor->z;
            return true;
        }
        *out = Pos(trail->data[count - 1]);
        return true;
    }

    f32 distance = self->scale * value;
    UnkGhostAuraEntry_b0040* data = trail->data;
    if (!(distance <= data[i].field_0xc)) {
        while (data[self->cursor].field_0xc < distance) {
            if (++self->cursor >= count)
                break;
        }
        i = self->cursor;
        if (i == 0 || i >= count) {
            *out = Pos(At(trail, count - 1));
            return true;
        }
        UnkGhostAuraEntry_b0040& prev = At(trail, i - 1);
        UnkGhostAuraEntry_b0040& next = At(trail, self->cursor);
        self->landing = Landing(next);
        if (landing_pos != nullptr && landed != nullptr && Landing(prev)) {
            *landed = true;
            *landing_pos = Pos(prev);
        }
        return Interpolate(prev, next, distance, out);
    }

    self->landing = Landing(data[i]);
    if (i == 0 || (distance - 0.1f <= data[i].field_0xc && data[i].field_0xc <= distance + 0.1f)) {
        *out = Pos(data[i]);
        return true;
    }
    UnkGhostAuraEntry_b0040& prev = At(trail, i - 1);
    UnkGhostAuraEntry_b0040& next = At(trail, self->cursor);
    return Interpolate(prev, next, distance, out);
}
