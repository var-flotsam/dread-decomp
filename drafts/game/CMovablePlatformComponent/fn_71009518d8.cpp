// CMovablePlatformComponent unit: before the platform moves by `delta`, sweeps its collider one step
// against the player's shape; returns false (and in `out` the move cut short at the hit) when the player is
// in the way, true otherwise. `out` starts as the full move. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/CCharacterMovement.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

extern void* g_unk_7101a22c60[];  // A vtable.

// The sweep's hit record (vtable g_unk_7101a22c60): the fraction of the move done before the hit at +0x30.
struct UnkSweepHit_l9518d8 {
    void** vtable;
    s32 index;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 fraction;
    u32 field_0x34;

    UnkSweepHit_l9518d8()
        : vtable(&g_unk_7101a22c60[2]), index(-1), field_0xc(0), field_0x10(0), field_0x14(0), field_0x18(0),
          field_0x1c(0), field_0x20(0), field_0x24(0), field_0x28(0), field_0x2c(0), fraction(0), field_0x34(0) {}
};
static_assert(sizeof(UnkSweepHit_l9518d8) == 0x38, "");

// A collider or shape as the tester takes it: its shape at +0x158.
struct UnkSweptShape_l9518d8 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

struct UnkPlatformPlayer_l9518d8 {
    u8 pad_0x0[0x1f8];
    void* field_0x1f8;
    UnkFlagged* movement;  // +0x200
};

struct UnkPlatformOwner_l9518d8 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkPlatformDesc_l9518d8 {
    u8 pad_0x0[0x68];
    CStrId collider_name;
};

struct UnkMovablePlatform_l9518d8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260();
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290();
    virtual bool vf_0x298();
    u8 pad_0x8[0x28];
    UnkPlatformOwner_l9518d8* owner;  // +0x30
    u8 pad_0x38[0x1f8];
    UnkPlatformDesc_l9518d8* desc;  // +0x230
};
static_assert(__builtin_offsetof(UnkMovablePlatform_l9518d8, desc) == 0x230, "");

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

// The sweep offset as the pair test takes it: a copy of the caller's, passed by value (in memory: its
// copy is user-written).
struct UnkOffset_d5925c {
    f32 x, y;
    explicit UnkOffset_d5925c(const CVector2D& v) : x(v.x), y(v.y) {}
    UnkOffset_d5925c(const UnkOffset_d5925c& o) : x(o.x), y(o.y) {}
};

// The pair test of the two shapes at their transforms, the first one moved by `offset`.
static inline UnkColliderResult Sweep(UnkShapeTester_A5_b68* tester, UnkShape* a, UnkMatrix44* ta, UnkShape* b,
                                      const UnkMatrix44& tb, UnkOffset_d5925c offset, UnkSweepHit_l9518d8* hit) {
    return fn_7100d541c0(tester, a, ta, b, const_cast<UnkMatrix44*>(&tb), &offset, hit, false);
}

static inline bool NearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

static inline UnkSweptShape_l9518d8* Swept(void* p) {
    return reinterpret_cast<UnkSweptShape_l9518d8*>(p);
}

bool fn_71009518d8(UnkMovablePlatform_l9518d8* self, const CVector3D* delta, CVector3D* out) {
    if (out) {
        out->x = delta->x;
        out->y = delta->y;
        out->z = delta->z;
    }
    if (NearZero(delta->x) && NearZero(delta->y) && NearZero(delta->z))
        return true;
    if (!self->vf_0x298())
        return true;
    UnkPlatformPlayer_l9518d8* player =
        reinterpret_cast<UnkPlatformPlayer_l9518d8*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->field_0x1f8)
        return true;

    UnkCollisionContainer* collision = self->owner->collision;
    UnkCollisionObject* collider = collision ? fn_71008640a4(collision, &self->desc->collider_name) : nullptr;
    UnkShape* shape = fn_7100976588(player->movement);
    if (collider && shape) {
        CVector3D dir = Normalized(*delta);
        CVector2D move;
        fn_710007b430(&move, delta);
        UnkSweepHit_l9518d8 hit;
        UnkMatrix44 from = fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider));
        from.m[3] -= dir.x;
        from.m[7] -= dir.y;
        from.m[11] -= dir.z;
        {
            CVector2D step;
            fn_710007b430(&step, &dir);
            move = step + move;
        }
        UnkColliderResult result = Sweep(Tester(), Swept(collider)->shape, &from, Swept(shape)->shape,
                                         fn_7100d36a2c(shape), UnkOffset_d5925c(move), &hit);
        if (result.bits & 1) {
            if (out)
                *out = CVector3D{hit.fraction * move.x, hit.fraction * move.y, 0.0f} - dir;
            return false;
        }
    }
    return true;
}
