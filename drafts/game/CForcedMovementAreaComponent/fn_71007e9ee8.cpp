// CForcedMovementAreaComponent's unit (tools/tu_map.py unit 1256): fn_71007e9ee8 handles a collider touching
// the area: it registers the area with the sub-area manager (placing its push direction from the toucher when
// it has none), runs the emmy door logic, and either flags the central unit's emmy or pushes the toucher.
// Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CAttackComponent.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CForcedMovementAreaComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkCentralUnitDoor_b46a2;
UnkCentralUnitDoor_b46a2* fn_71004056fc(void* object);
void* fn_71007ea32c(void* actor);
bool fn_7100eea7b0(void* manager);

// An actor's position (its transform's translation).
struct UnkTouchPos_l7e9ee8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkTouchCollision_l7e9ee8 {
    u8 pad_0x0[0x30];
    UnkTouchPos_l7e9ee8* owner;  // 0x30
};

struct UnkTouchHit_l7e9ee8 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkTouchHitRef_l7e9ee8 {
    u8 pad_0x0[0x10];
    UnkTouchHit_l7e9ee8* hit;  // 0x10
};

struct UnkTouchArg_l7e9ee8 {
    u8 pad_0x0[0x10];
    UnkTouchHitRef_l7e9ee8* ref;  // 0x10
};

struct UnkTouchReg_l7e9ee8 {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkForcedAreaTouch_l7e9ee8 {
    u8 pad_0x0[0x18];
    UnkTouchPos_l7e9ee8* owner;  // 0x18
    u8 pad_0x20[0x79];
    bool pushed;  // 0x99
    u8 pad_0x9a[0xe];
    CVector3D push;                 // 0xa8
    UnkTouchReg_l7e9ee8 sub_area;   // 0xb8
    UnkTouchReg_l7e9ee8 emmy_door;  // 0xd0
};
static_assert(__builtin_offsetof(UnkForcedAreaTouch_l7e9ee8, emmy_door) == 0xd0, "layout");

struct UnkTouchUnitLink_l7e9ee8 {
    u8 pad_0x0[0x10];
    void* actor;  // 0x10
};

struct UnkTouchUnit_l7e9ee8 {
    u8 pad_0x0[0x6d8];
    UnkTouchUnitLink_l7e9ee8* link;  // 0x6d8
};

struct UnkTouchUnitActor_l7e9ee8 {
    u8 pad_0x0[0x258];
    void* attack;  // 0x258
};

struct UnkTouchEmmy_l7e9ee8 {
    u8 pad_0x0[0x2a6c0];
    bool stunned;  // 0x2a6c0
};

void fn_71007ea564(UnkForcedAreaTouch_l7e9ee8* self, UnkTouchHit_l7e9ee8* hit, UnkTouchPos_l7e9ee8* toucher);

static inline void* Manager_l7e9ee8() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return manager;
}

static inline UnkTouchUnit_l7e9ee8* CentralUnit_l7e9ee8() {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    return reinterpret_cast<UnkTouchUnit_l7e9ee8*>(fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(reg)));
}

// Moves a registration into the manager's set at `offset`.
static inline void Register_l7e9ee8(UnkTouchReg_l7e9ee8* r, u64 offset) {
    void* manager = Manager_l7e9ee8();
    if (r->set) {
        fn_71004f3ea4(r->set, r);
        r->set = nullptr;
    }
    r->set = reinterpret_cast<base::UnkDeferredSet<void>*>(static_cast<char*>(manager) + offset);
    fn_71004f3cf0(r->set, r);
}

// Whether `a` is within `eps` of `b`.
static inline bool Near_l7e9ee8(f32 a, f32 b, f32 eps) {
    return a + eps >= b && a - eps <= b;
}

static inline bool IsZero_l7e9ee8(const CVector3D& v) {
    return Near_l7e9ee8(v.x, g_unk_710164779c.x, 0.001f) && Near_l7e9ee8(v.y, g_unk_710164779c.y, 0.001f) &&
           Near_l7e9ee8(v.z, g_unk_710164779c.z, 0.001f);
}

bool fn_71007e9ee8(UnkForcedAreaTouch_l7e9ee8* self, UnkTouchArg_l7e9ee8* arg) {
    UnkReflObject* component = arg->ref->hit->component;
    auto* collision = reinterpret_cast<UnkTouchCollision_l7e9ee8*>(
        fn_710009d50c(component->vf_0x0(), fn_710085fc44()) ? component : nullptr);
    UnkTouchPos_l7e9ee8* toucher = collision->owner;

    if (fn_71004056fc(self->owner)) {
        Register_l7e9ee8(&self->emmy_door, 0x88);
        if (IsZero_l7e9ee8(self->push)) {
            UnkTouchPos_l7e9ee8* owner = self->owner;
            f32 dx = owner->x - toucher->x;
            f32 dy = owner->y - toucher->y;
            f32 dz = owner->z - toucher->z;
            self->push.x = dx;
            self->push.y = dy;
            self->push.z = dz;
        }
    }

    if (fn_71007ea250(reinterpret_cast<UnkWeakTarget*>(self->owner))) {
        UnkTouchUnit_l7e9ee8* unit = CentralUnit_l7e9ee8();
        if (unit && unit->link && unit->link->actor) {
            void* emmy = fn_71007ea32c(unit->link->actor);
            if (emmy && fn_710024fbc8(emmy) && fn_71005436e8(static_cast<UnkEmmyAIComponent_b46a1*>(emmy)))
                return false;
        }
        Register_l7e9ee8(&self->sub_area, 8);
        if (!fn_7100eea7b0(Manager_l7e9ee8()))
            fn_71007ea408(reinterpret_cast<UnkForcedMovementArea_A5_b68e*>(self));
    }

    UnkTouchUnit_l7e9ee8* unit = CentralUnit_l7e9ee8();
    if (unit && unit->link) {
        if (auto* actor = static_cast<UnkTouchUnitActor_l7e9ee8*>(unit->link->actor)) {
            auto* emmy = static_cast<UnkTouchEmmy_l7e9ee8*>(fn_71007ea32c(actor));
            if (emmy && actor->attack && fn_7101136308(actor->attack)) {
                emmy->stunned = true;
                return false;
            }
        }
    }
    if (collision) {
        fn_71007ea564(self, arg->ref ? arg->ref->hit : nullptr, toucher);
        self->pushed = true;
    }
    return false;
}
