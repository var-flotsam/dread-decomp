// CAbilityShinespark's unit (tools/tu_map.py, unit 433): a contact while sparking (state 4). Hands the
// contact to one of three handlers by the spark's direction (vertical, horizontal or diagonal), then records
// whether the body moved under 50 units, unless a breakable tile among the contact points took the hit.
// Names and layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityLayout.h"
#include "game/CBreakableTileGroup.h"
#include "types.h"

struct UnkQueryObj;

// As src/game/CAbilityShinespark/CAbilityShinespark_Flags_l32b328.cpp declares it.
struct UnkEnergyWaveFlags_ovb22 {
    u32 bits;
    UnkEnergyWaveFlags_ovb22() : bits(0) {}
    UnkEnergyWaveFlags_ovb22(const UnkEnergyWaveFlags_ovb22& o) : bits(o.bits) {}
};

UnkEnergyWaveFlags_ovb22 fn_710032b328(CAbility* entry, void* a, void* b);

struct UnkVec3_l32a5ac {
    f32 x, y, z;
    UnkVec3_l32a5ac() {}
    UnkVec3_l32a5ac(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3_l32a5ac(const UnkVec3_l32a5ac& o) : x(o.x), y(o.y), z(o.z) {}
};

static inline UnkVec3_l32a5ac operator+(const UnkVec3_l32a5ac& a, const UnkVec3_l32a5ac& b) {
    return UnkVec3_l32a5ac(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline UnkVec3_l32a5ac operator-(const UnkVec3_l32a5ac& a, const UnkVec3_l32a5ac& b) {
    return UnkVec3_l32a5ac(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline UnkVec3_l32a5ac operator*(const UnkVec3_l32a5ac& a, f32 s) {
    return UnkVec3_l32a5ac(a.x * s, a.y * s, a.z * s);
}

static inline f32 Dot(const UnkVec3_l32a5ac& a, const UnkVec3_l32a5ac& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 Length(const UnkVec3_l32a5ac& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

static inline bool Near(f32 a, f32 b) {
    f32 lo = a - 0.001f;
    f32 hi = a + 0.001f;
    return hi >= b && lo <= b;
}

static inline bool Near(const UnkVec3_l32a5ac& a, const UnkVec3_l32a5ac& b) {
    return Near(a.x, b.x) && Near(a.y, b.y) && Near(a.z, b.z);
}

struct UnkBox_l32a5ac {
    UnkVec3_l32a5ac min;
    UnkVec3_l32a5ac max;
};

// The body's shape: its bounds (virtual slot 0x28, given its transform at +0x8) and its centre (slot 0x68).
struct UnkShape_l32a5ac {
    void** vtable;
    u8 transform[1];  // 0x8

    UnkBox_l32a5ac Bounds() {
        return reinterpret_cast<UnkBox_l32a5ac (*)(UnkShape_l32a5ac*, void*)>(vtable[0x28 / 8])(this, transform);
    }
    UnkVec3_l32a5ac Center() {
        return reinterpret_cast<UnkVec3_l32a5ac (*)(UnkShape_l32a5ac*)>(vtable[0x68 / 8])(this);
    }
};

struct UnkBody_l32a5ac {
    u8 pad_0x0[0x158];
    UnkShape_l32a5ac* shape;  // 0x158
};

struct UnkColliderLink_l32a5ac {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;  // 0x10
};

struct UnkCollider_l32a5ac {
    u8 pad_0x0[0x10];
    UnkColliderLink_l32a5ac* link;  // 0x10
};

// One point of a contact (0x48 bytes, 4-aligned: an array of them starts at +0x5c of the contact).
struct __attribute__((packed, aligned(4))) UnkContactPoint_l32a5ac {
    UnkCollider_l32a5ac* collider;
    u8 pad_0x8[0xc];
    UnkVec3_l32a5ac pos;  // 0x14
    u8 pad_0x20[0xc];
    UnkVec3_l32a5ac normal;  // 0x2c
    u8 pad_0x38[0x10];
};
static_assert(sizeof(UnkContactPoint_l32a5ac) == 0x48, "UnkContactPoint_l32a5ac layout");

struct UnkContact_l32a5ac {
    u8 pad_0x0[0x8];
    UnkBody_l32a5ac* body;  // 0x8
    u8 pad_0x10[0x4];
    UnkVec3_l32a5ac velocity;  // 0x14
    u8 pad_0x20[0x18];
    UnkVec3_l32a5ac pos;  // 0x38
    u8 pad_0x44[0xc];
    f32 field_0x50;
    u8 pad_0x54[0x4];
    u32 count;                           // 0x58
    UnkContactPoint_l32a5ac points[1];   // 0x5c
};
static_assert(__builtin_offsetof(UnkContact_l32a5ac, points) == 0x5c, "UnkContact_l32a5ac layout");

struct UnkShinesparkOwner_l32a5ac {
    u8 pad_0x0[0x1f8];
    void* field_0x1f8;
};

struct UnkShinesparkState_l32a5ac {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkAbilityShinespark_l32a5ac : CAbility {
    u8 pad_0x390[0x3ac - 0x390];
    UnkVec3_l32a5ac direction;  // 0x3ac
    u8 pad_0x3b8[0x440 - 0x3b8];
    bool slow;  // 0x440
    u8 pad_0x441[0x6b0 - 0x441];
    UnkShinesparkState_l32a5ac* state;  // 0x6b0
};

void fn_710032aad8(UnkAbilityShinespark_l32a5ac* self, UnkContact_l32a5ac* contact, const UnkVec3_l32a5ac* half,
                   const UnkVec3_l32a5ac* delta, bool along_y, bool along_x, const UnkVec3_l32a5ac* point,
                   UnkContactPoint_l32a5ac* hit);
void fn_710032ac08(UnkAbilityShinespark_l32a5ac* self, UnkContact_l32a5ac* contact, const UnkVec3_l32a5ac* half,
                   const UnkVec3_l32a5ac* delta, bool along_y, bool along_x, const UnkVec3_l32a5ac* point,
                   UnkContactPoint_l32a5ac* hit);
void fn_710032ad30(UnkAbilityShinespark_l32a5ac* self, UnkContact_l32a5ac* contact, const UnkVec3_l32a5ac* half,
                   bool along_y, bool along_x, const UnkVec3_l32a5ac* point, const UnkBox_l32a5ac* box,
                   UnkContactPoint_l32a5ac* hit);

static inline bool IsTileGroup(base::reflection::UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), fn_7100ca0484());
}

void fn_710032a5ac(UnkAbilityShinespark_l32a5ac* self, UnkContactPoint_l32a5ac* hit, void* unused_a,
                   void* unused_b, void* unused_c, UnkContact_l32a5ac* contact) {
    if (!self->state || self->state->kind != 4)
        return;
    if (contact->field_0x50 <= 0.0f)
        return;

    UnkShape_l32a5ac* shape = contact->body->shape;
    UnkBox_l32a5ac box = shape->Bounds();
    UnkVec3_l32a5ac half = (box.max - box.min) * 0.5f;
    UnkVec3_l32a5ac point = contact->pos + shape->Center();
    UnkVec3_l32a5ac delta = point - hit->pos;

    const UnkVec3_l32a5ac& dir = self->direction;
    bool vertical = Near(dir, UnkVec3_l32a5ac(0.0f, 1.0f, 0.0f)) || Near(dir, UnkVec3_l32a5ac(0.0f, -1.0f, 0.0f));
    bool horizontal =
        Near(dir, UnkVec3_l32a5ac(1.0f, 0.0f, 0.0f)) || Near(dir, UnkVec3_l32a5ac(-1.0f, 0.0f, 0.0f));
    bool along_y = Near(fabsf(Dot(hit->normal, UnkVec3_l32a5ac(0.0f, 1.0f, 0.0f))), 1.0f);
    bool along_x = Near(fabsf(Dot(hit->normal, UnkVec3_l32a5ac(1.0f, 0.0f, 0.0f))), 1.0f);

    if (vertical)
        fn_710032aad8(self, contact, &half, &delta, along_y, along_x, &point, hit);
    else if (horizontal)
        fn_710032ac08(self, contact, &half, &delta, along_y, along_x, &point, hit);
    else
        fn_710032ad30(self, contact, &half, along_y, along_x, &point, &box, hit);

    bool slow = Length(contact->velocity) < 50.0f;
    if (!slow && static_cast<UnkShinesparkOwner_l32a5ac*>(self->component->field_0x30)->field_0x1f8) {
        base::reflection::UnkReflObject* object = hit->collider->link->object;
        if ((!object || !IsTileGroup(object)) && contact->count != 0) {
            UnkContactPoint_l32a5ac* end = contact->points + contact->count;
            for (UnkContactPoint_l32a5ac* p = contact->points; p != end; ++p) {
                base::reflection::UnkReflObject* other = p->collider->link->object;
                if (!other || !IsTileGroup(other))
                    continue;
                UnkQueryObj* tile = fn_7100ca2048(other, &p->pos, 1);
                if (tile && (fn_710032b328(self, tile, nullptr).bits & 1))
                    return;
            }
        }
    }
    self->slow = slow;
}
