// UnkTU2704 (CSpawnPointComponent's unit): whether the spawn point may spawn now. A pending request waits
// out the field_0x60 object's cooldown; otherwise the component and its owner must be enabled. Then the
// spawn must be allowed, the activation time reached and the generated count under the limit, and, with
// bCheckCollisions, the character class's collision box must be free at the spawn position. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBombGun.h"
#include "game/CDrop.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2734.h"
#include "game/UnkTU344.h"
#include "game/UnkTU54.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkEmmyCheckActor_b0025;
struct UnkObjectSet;
struct UnkSetObject;
struct UnkActorDef_A1b71;
struct UnkActorDefEntry_A1b71;
struct UnkActor;

bool fn_710112a3bc(void* holder);
bool fn_7100daa560(void* self, UnkEmmyCheckActor_b0025* actor);
bool fn_710119daa0(UnkObjectSet* self, UnkSetObject* const* object);
UnkReflObject* fn_7101193ee0(UnkActorDefFactory* self, const CStrId* id);
UnkActorDefEntry_A1b71* fn_71002552a4(const UnkActorDef_A1b71* self, const char* name);
void fn_710112ab08(CSpawnPointComponent* self, UnkVec3f* pos, UnkVec3f* angle);

// Debug switches: bit 4 checks the sub-area cache before spawning.
extern u8 g_unk_7172a0ba4c;
// The Autector's tunable.
extern UnkTunable g_unk_71726bdb98;

struct UnkAutectorTunable_l12a558 : UnkTunable {
    u8 pad_0x8[0x28];
    bool field_0x30;
};

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};
struct UnkCollisionShape {
    u8 data[0xc0];
};
struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};

// What the field_0x60 object gives (fn_71007c04f0): its cooldown and what a spawn resets.
struct UnkSpawnTarget_l12a558 {
    u8 pad_0x0[0x13a];
    bool field_0x13a;
    u8 pad_0x13b[0x17c - 0x13b];
    u32 field_0x17c;
    f32 field_0x180;  // the cooldown
    f32 field_0x184;  // when it started
};

// The base component's enabled flag (+0x20).
struct UnkComponentView_l12a558 {
    u8 pad_0x0[0x20];
    bool field_0x20;
};

struct UnkSpawnOwner_l12a558 {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
};

// The character class's collision component: the box's centre offset.
struct UnkCollisionDef_l12a558 : UnkReflObject {
    u8 pad_0x8[0x84];
    f32 field_0x8c;
    f32 field_0x90;
};

struct UnkCharClassDef_l12a558 {
    u8 pad_0x0[0x148];
    f32 field_0x148;  // half height
    f32 field_0x14c;
    f32 field_0x150;
};

static inline UnkSpawnTarget_l12a558* SpawnTarget(CSpawnPointComponent* self) {
    CActor* actor = self->field_0x60.Target();
    return actor ? static_cast<UnkSpawnTarget_l12a558*>(fn_71007c04f0(actor)) : nullptr;
}

static inline f32 Now() {
    return g_unk_7101d47310->field_0x2444;
}

static inline UnkManager_71726af468* Manager() {
    if (!g_unk_71726af468) {
        auto* manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return g_unk_71726af468;
}

static inline UnkSubAreaGuiCache* Cache() {
    if (!g_unk_71727254f8) {
        auto* cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    return g_unk_71727254f8;
}

inline const CStrId& Kind_COLLISION() {
    static const CStrId s_id("COLLISION", true);
    return s_id;
}

// Whether the spawned class is the one named.
static inline bool SpawnsClass(CSpawnPointComponent* self, const char* name) {
    ++self->field_0x48.data->reads;
    return self->field_0x48.data != &g_unk_7101d47108->empty &&
           CRntString_Contains(&self->field_0x48.data->String(), name);
}

bool fn_710112a558(CSpawnPointComponent* self, f32 time) {
    self->ResolveField0x60();
    UnkSpawnTarget_l12a558* target = SpawnTarget(self);

    bool requested = false;
    if (self->field_0x330) {
        if (target->field_0x180 <= 1.1920929e-07f || target->field_0x184 < 0.0f ||
            Now() - target->field_0x184 >= target->field_0x180) {
            self->field_0x330 = false;
            self->field_0x332 = false;
        } else {
            if (!self->field_0x331)
                return false;
            requested = true;
        }
    }
    if (!requested) {
        if (!reinterpret_cast<UnkComponentView_l12a558*>(self)->field_0x20 ||
            !self->Owner<UnkSpawnOwner_l12a558>()->field_0x1b4)
            return false;
    }

    if (!fn_710112a3bc(self))
        return false;
    if (!fn_7100daa560(Manager(), reinterpret_cast<UnkEmmyCheckActor_b0025*>(self)))
        return false;

    bool ready = time < 0.0f || time + 1.1920929e-07f >= self->fTimeToActivate;
    bool busy = target && target->field_0x13a;
    if (ready & busy) {
        ready = self->iMaxNumToGenerate < 0 || *reinterpret_cast<u32*>(self->pad_0x388) < (u32)self->iMaxNumToGenerate;
    } else if (!busy && ready && self->field_0x2b8 && target->field_0x180 > 1.1920929e-07f &&
               target->field_0x184 >= 0.0f) {
        ready = Now() - target->field_0x184 >= target->field_0x180;
    }

    if (g_unk_7172a0ba4c & 0x10) {
        bool listed = fn_710119daa0(reinterpret_cast<UnkObjectSet*>(Cache()),
                                    reinterpret_cast<UnkSetObject* const*>(&self->field_0x48));
        if (ready & listed)
            ready = fn_710119db04(Cache(), &self->field_0x48) > 0;
    }
    if (!ready)
        return false;

    bool autector = SpawnsClass(self, "autector");
    if (!static_cast<UnkAutectorTunable_l12a558&>(g_unk_71726bdb98).field_0x30 && autector)
        return false;

    if (self->bCheckCollisions) {
        UnkVec3f pos(0.0f, 0.0f, 0.0f);
        UnkVec3f angle(0.0f, 0.0f, 0.0f);
        fn_710112ab08(self, &pos, &angle);
        UnkVec3f spawn_pos = pos;
        UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
        void* tracker = sub ? sub->field_0x820 : nullptr;
        if (tracker) {
            auto* char_class = reinterpret_cast<UnkCharClassDef_l12a558*>(
                fn_7101193ee0(g_unk_71726a0220, &self->field_0x48));
            if (char_class) {
                UnkReflObject* component = reinterpret_cast<UnkReflObject*>(
                    fn_71002552a4(reinterpret_cast<UnkActorDef_A1b71*>(char_class), Kind_COLLISION().Text()));
                f32 x = 0.0f;
                f32 y = 0.0f;
                if (component && fn_710009d50c(component->vf_0x0(), fn_710079a218())) {
                    x = static_cast<UnkCollisionDef_l12a558*>(component)->field_0x8c;
                    y = static_cast<UnkCollisionDef_l12a558*>(component)->field_0x90;
                }
                base::math::CVector2D half;
                {
                    f32 cx = (x + char_class->field_0x14c) * 0.5f;
                    f32 cy = (y + char_class->field_0x150) * 0.5f;
                    base::math::CVector3D centre = {cx, cy, 0.0f};
                    fn_710007b430(&half, &centre);
                }
                UnkCollisionShape shape;
                {
                    f32 h = char_class->field_0x148;
                    UnkBox2D box(-half.x, h - half.y, half.x, h + half.y);
                    fn_7100d5e1f8(&shape, &box);
                }
                bool hit;
                {
                    UnkMatrix44_7100078c9c xf = fn_7100078cd8(spawn_pos);
                    hit = fn_7100d5b484(static_cast<UnkTree*>(tracker), 0x10a0, reinterpret_cast<UnkShape*>(&shape),
                                        reinterpret_cast<const UnkMtx44*>(&xf), nullptr, 0);
                }
                if (hit) {
                    self->ResolveField0x60();
                    if (UnkSpawnTarget_l12a558* t = SpawnTarget(self))
                        t->field_0x17c = 0;
                    fn_7100d5e2c8(&shape);
                    return false;
                }
                fn_7100d5e2c8(&shape);
            }
        }
    }

    if (self->bAllowSpawnInFrustum)
        return true;
    return !fn_7100d7bea0(self->Owner<UnkActor>(), 0);
}
