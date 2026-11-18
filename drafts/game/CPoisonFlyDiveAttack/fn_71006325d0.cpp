// The CPoisonFlyDiveAttack unit: the attack's update. After the base update, by the state (+0x54): 1 turns
// the owner toward its target's side and moves to 2; 2 and 3 steer toward the vector at +0x48; 4 ends the
// dive (state 6) once the owner is within 100 of the point at +0x3c or fn_71006328c0 says so, and
// otherwise looks for a new target; 6 faces the side the owner's matrix points to. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkSpitAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

// A direction as the kind-5 block carries it: three words (the floats' bits), as the unit's other
// sources see it.
struct UnkDiveDir_l6325d0 {
    u32 x;
    u32 y;
    u32 z;
};

// The two facings: toward +x and toward -x.
extern const UnkDiveDir_l6325d0 g_unk_710164a35c __attribute__((visibility("hidden")));
extern const UnkDiveDir_l6325d0 g_unk_710164a368 __attribute__((visibility("hidden")));

// The kind-5 block (game_s2/UnkParams.h) carrying a direction.
struct UnkDiveParams_l6325d0 {
    void** vtable;
    UnkDiveDir_l6325d0 direction;  // 0x8
    bool has_direction;   // 0x14
    u8 pad_0x15[3];
    u32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;

    UnkDiveParams_l6325d0() {
        field_0x30 = false;
        field_0x20 = 0;
        field_0x28 = 0;
        vtable = g_unk_7101a25b10 + 2;
        field_0x39 = false;
        field_0x3b = false;
        field_0x3d = false;
        kind = 5;
        field_0x18 = 0;
        field_0x1c = true;
    }
};
static_assert(__builtin_offsetof(UnkDiveParams_l6325d0, kind) == 0x34, "UnkDiveParams_l6325d0 layout");

// What fn_7100632aac fills when it finds a new target: two counted references and an index.
struct UnkDiveTarget_l6325d0 {
    UnkRefPtr first;
    UnkRefPtr second;  // 0x8
    s32 index;         // 0x10
    u32 field_0x14[12];

    UnkDiveTarget_l6325d0() : first(), second(), index(-1), field_0x14() {}
};

struct UnkPoisonFlyDive_l6325d0 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x2c];
    CVector3D destination;         // 0x3c
    UnkDiveDir_l6325d0 direction;  // 0x48
    s32 state;              // 0x54
};

struct UnkPoisonFlyDive_b61;
struct UnkPoisonFlyDive_ovb0133;
void fn_7100632204(UnkPoisonFlyDive_b61* self, u32 state);
void fn_71006314b4(UnkPoisonFlyDive_ovb0133* self);
void fn_7100631650(UnkPoisonFlyDive_l6325d0* self);
bool fn_71006328c0(UnkPoisonFlyDive_l6325d0* self);
bool fn_7100632aac(UnkPoisonFlyDive_l6325d0* self, UnkDiveTarget_l6325d0* target);
void fn_7100632c8c(UnkPoisonFlyDive_l6325d0* self, UnkDiveTarget_l6325d0* target);

static inline void SetState(UnkPoisonFlyDive_l6325d0* self, u32 state) {
    fn_7100632204(reinterpret_cast<UnkPoisonFlyDive_b61*>(self), state);
}

// Hands the owner's +0x298 the direction to face.
static inline void Face(UnkPoisonFlyDive_l6325d0* self, UnkDiveDir_l6325d0 direction) {
    void* sink = self->owner->field_0x298;
    UnkDiveParams_l6325d0 params;
    params.direction = direction;
    params.has_direction = true;
    fn_71006f4b0c(sink, &params, 1);
}

void fn_71006325d0(UnkPoisonFlyDive_l6325d0* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    switch (self->state) {
    case 1: {
        fn_7100631650(self);
        UnkActor* target = fn_7100d7c990(self->owner);
        Face(self, target->field_0x44 - self->owner->field_0x44 > 0.0f ? g_unk_710164a35c : g_unk_710164a368);
        SetState(self, 2);
        break;
    }
    case 2:
    case 3:
    {
        fn_71006314b4(reinterpret_cast<UnkPoisonFlyDive_ovb0133*>(self));
        void* sink = self->owner->field_0x298;
        UnkDiveParams_l6325d0 params;
        params.direction.x = self->direction.x;
        params.direction.y = self->direction.y;
        params.direction.z = self->direction.z;
        params.has_direction = true;
        fn_71006f4b0c(sink, &params, 1);
        break;
    }
    case 4: {
        UnkActor* owner = self->owner;
        f32 dx = self->destination.x - owner->field_0x44;
        f32 dy = self->destination.y - owner->field_0x54;
        f32 dz = self->destination.z - owner->field_0x64;
        if (dx * dx + dy * dy + dz * dz <= 10000.0f || fn_71006328c0(self)) {
            SetState(self, 6);
            break;
        }
        UnkDiveTarget_l6325d0 target;
        if (fn_7100632aac(self, &target))
            fn_7100632c8c(self, &target);
        break;
    }
    case 6: {
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        Face(self, m.m[2] > 0.0f ? g_unk_710164a35c : g_unk_710164a368);
        break;
    }
    }
}
