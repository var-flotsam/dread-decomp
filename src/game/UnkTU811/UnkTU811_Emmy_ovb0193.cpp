// CEmmyAIComponent's unit (tools/tu_map.py unit 811, no reliable name): helpers of the Emmy AI component.
// fn_7100545570 hands the owner's AI object (+0x218) a shape (its slot 0x2a8, or the collision object
// g_unk_71726e1f68) and runs fn_71005455e8; fn_71005540d8 tests the owner's heading against a path point;
// fn_710055749c vets two navigation-mesh cells before forwarding to slot 0x568; fn_71005593f0 steps the
// sound-handle state; fn_71005579b0, fn_71005592ac, fn_7100541e64, fn_710054782c and fn_7100545138 are queries
// and resets of the AI's state. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAudioComponent.h"
#include "game/CBlackboard.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;


struct UnkEmmyAI_b0193;
struct UnkEmmyActor_b0193;
struct UnkEmmyAIObj_b0193;

struct UnkEmmyAIObjVtbl_b0193 {
    void* unk_0x0[85];
    void* (*vf_0x2a8)(UnkEmmyAIObj_b0193* self);
    void (*vf_0x2b0)(UnkEmmyAIObj_b0193* self, void* arg);
};

struct UnkEmmyAIObj_b0193 {
    UnkEmmyAIObjVtbl_b0193* vtbl;
};

struct UnkEmmyOwner_b0193 {
    u8 unk_0x0[0x218];
    UnkEmmyAIObj_b0193* field_0x218;
    u8 unk_0x220[0x2a0 - 0x220];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkEmmyOther_b0193 {
    u8 unk_0x0[0x34];
    u8 field_0x34;
};

struct UnkEmmyAI_b0193 {
    u8 unk_0x0[0x30];
    UnkEmmyOwner_b0193* field_0x30;
};

void fn_710110fbc4(UnkEmmyAI_b0193* self, UnkEmmyOther_b0193* other);
void fn_71005455e8(UnkEmmyAI_b0193* self);
extern CStrId g_unk_71726e1f68 __attribute__((visibility("hidden")));

void fn_7100545570(UnkEmmyAI_b0193* self, UnkEmmyOther_b0193* other) {
    fn_710110fbc4(self, other);
    UnkEmmyOwner_b0193* owner = self->field_0x30;
    if (__builtin_expect(other->field_0x34 & 1, 0)) {
        void* obj = fn_71008640a4(owner->field_0x2a0, &g_unk_71726e1f68);
        if (obj)
            self->field_0x30->field_0x218->vtbl->vf_0x2b0(self->field_0x30->field_0x218, obj);
    } else {
        void* obj = owner->field_0x218->vtbl->vf_0x2a8(owner->field_0x218);
        if (obj)
            self->field_0x30->field_0x218->vtbl->vf_0x2b0(self->field_0x30->field_0x218, obj);
    }
    fn_71005455e8(self);
}






struct UnkAII_b0144;
struct UnkPathPoint_b67a4;
bool fn_7101112070(UnkAII_b0144* self, UnkVec3f* out);
bool fn_7100707df4(char* self, UnkPathPoint_b67a4* out);
UnkMatrix44 fn_7100078fc8(const UnkMatrix44& m);

struct UnkKindInner_b0193 {
    u8 unk_0x0[0x10];
    s32 field_0x10;
};
struct UnkKindMid_b0193 {
    u8 unk_0x0[0x10];
    UnkKindInner_b0193* field_0x10;
};
struct UnkPathSlot_b0193 {
    UnkKindMid_b0193* field_0x0;
    char* field_0x8;
};
struct UnkPathHolder_b0193 {
    u8 unk_0x0[0xb0];
    UnkPathSlot_b0193* field_0xb0;
};
struct UnkPathOwner_b0193 {
    u8 unk_0x0[0x38];
    UnkXform field_0x38;
    u8 unk_0x38_pad[0x298 - 0x38 - sizeof(UnkXform)];
    UnkPathHolder_b0193* field_0x298;
};
struct UnkPathAI_b0193 {
    u8 unk_0x0[0x30];
    UnkPathOwner_b0193* field_0x30;
};

bool fn_71005540d8(UnkPathAI_b0193* self) {
    UnkVec3f dir(0.0f, 0.0f, 0.0f);
    if (!fn_7101112070(reinterpret_cast<UnkAII_b0144*>(self), &dir))
        return false;
    UnkVec3f point(0.0f, 0.0f, 0.0f);
    if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z < 100.0f) {
        UnkPathSlot_b0193* slot = self->field_0x30->field_0x298->field_0xb0;
        if (slot->field_0x0 && slot->field_0x0->field_0x10 && slot->field_0x0->field_0x10->field_0x10 == 1 &&
            slot->field_0x8) {
            if (fn_7100707df4(slot->field_0x8 + 0x140, reinterpret_cast<UnkPathPoint_b67a4*>(&point))) {
                UnkMatrix44 m = fn_7100078fc8(*reinterpret_cast<const UnkMatrix44*>(&point));
                dir = UnkVec3f(m.m[2], m.m[6], m.m[10]);
            }
        }
    }
    const UnkMatrix44& axes = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&self->field_0x30->field_0x38));
    f32 ax = axes.m[2], ay = axes.m[6], az = axes.m[10];
    return ax * dir.x + ay * dir.y + az * dir.z < 0.0f;
}



// ---- 710055749c

struct UnkNavVtbl_b0193;
struct UnkNavAI_b0193 {
    bool (**vtable)(UnkNavAI_b0193* self, u16 id, u32 a3, void* a4, void* a5, void* a6, u32 a7, u32 a8);
};

bool fn_710055749c(UnkNavAI_b0193* self, const base::math::CVector2D* point, u16 id, u32 a3, void* a4, void* a5, void* a6,
                   u32 a7, u32 a8) {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(sizeof(UnkNavMeshManager), 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    UnkNavMesh* mesh = manager->navmesh;
    if (mesh && fn_71002b005c(fn_710027a33c(mesh, id)) &&
        !fn_71002b005c(fn_710027a33c(mesh, fn_710027a41c(mesh, point, 1))))
        return false;
    return self->vtable[0x568 / 8](self, id, a3, a4, a5, a6, a7, a8);
}

// ---- 71005593f0
struct UnkSound;
struct UnkShipRechargeEntity_b49a4;
struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};

static inline UnkSound* GetSound_b0193(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

struct UnkEmmyStateAI_b0193 {
    u8 unk_0x0[0x30];
    UnkShipRechargeEntity_b49a4* field_0x30;
    u8 unk_0x38[0x14c54 - 0x38];
    u32 field_0x14c54;
    u8 field_0x14c58;
    u8 unk_0x14c59[0x294e8 - 0x14c59];
    UnkSoundHandle field_0x294e8[2];
};

void fn_7100559518(UnkEmmyStateAI_b0193* self);
void fn_71005596c4(UnkEmmyStateAI_b0193* self);
void fn_7100559c58(UnkEmmyStateAI_b0193* self);
void fn_7100541e64(UnkEmmyStateAI_b0193* self);

bool fn_71005593f0(UnkEmmyStateAI_b0193* self, bool force, u32 id) {
    fn_7100559518(self);
    if (!force) {
        if (self->field_0x14c54 != id) {
            fn_7100541e64(self);
            return false;
        }
        if (self->field_0x14c58 && !GetSound_b0193(&self->field_0x294e8[0])) {
            if (GetSound_b0193(&self->field_0x294e8[1])) {
                fn_7100d7c510(self->field_0x30, &self->field_0x294e8[1], 0.0f, 0.0f);
                self->field_0x294e8[1] = g_unk_7108b6c460;
            }
            self->field_0x14c58 = 0;
            return false;
        }
    }
    fn_71005596c4(self);
    fn_7100559c58(self);
    return self->field_0x14c58 != 0;
}

// ---- 71005579b0

extern base::global::CStrId g_unk_71726b7cb8;
extern char g_unk_71726debe0[];
bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);
bool fn_7101110268(void* self);

struct UnkTrackedVtbl_b0193 {
    void* unk_0x0[0x2d0 / 8];
    UnkNavMeshCell* (*vf_0x2d0)(void* self);
};
struct UnkTracked_b0193 {
    UnkTrackedVtbl_b0193* vtbl;
};
struct UnkChaseOwner_b0193 {
    u8 unk_0x0[0x1f8];
    UnkTracked_b0193* field_0x1f8;
    u8 unk_0x200[0x220 - 0x200];
    UnkAnimationComponent* field_0x220;
};
struct UnkChaseAI_b0193 {
    u8 unk_0x0[0x30];
    UnkChaseOwner_b0193* field_0x30;
    u8 unk_0x38[0x1260 - 0x38];
    CBlackboard field_0x1260;
};
struct UnkChaseArgs_b0193 {
    u8 unk_0x0[0x4c];
    bool field_0x4c;
};

bool fn_71005579b0(UnkChaseAI_b0193* self, UnkChaseArgs_b0193* args) {
    u8* flags = (u8*)self + 0x29f26;
    bool ready = false;
    if (fn_7101110268(self) && !flags[0x7ca])
        ready = flags[0x332] == 0;
    UnkAnimationComponent* track = self->field_0x30->field_0x220;
    if (track) {
        if (!(ready & !fn_710117b348(track, fn_7100c18138())))
            return false;
    } else if (!ready) {
        return false;
    }
    UnkTracked_b0193* tracked = self->field_0x30->field_0x1f8;
    UnkNavMeshCell* cell;
    if (!(tracked && (cell = tracked->vtbl->vf_0x2d0(tracked)) && fn_71002b005c(cell))) {
        if (fn_710002fb70((u8*)self + 0x29418))
            return false;
        if (flags[0])
            return false;
    }
    if (args->field_0x4c)
        return true;
    s32 value = 0;
    if (CBlackboard::CSection* section = fn_7101047270(&self->field_0x1260, g_unk_71726b7cb8))
        fn_7100395a30(section, g_unk_71726debe0, &value);
    return value == 0;
}

// ---- 7100541e64

struct UnkHandleOwner_b0193 {
    u8 unk_0x0[0x2b8];
    void* field_0x2b8;
};
struct UnkHandleAI_b0193 {
    u8 unk_0x0[0x30];
    UnkHandleOwner_b0193* field_0x30;
    u8 unk_0x38[0x294e8 - 0x38];
    UnkSoundHandle field_0x294e8[4];
};

#define RELEASE_HANDLE_b0193(self, effects, i)                                \
    if (GetSound_b0193(&(self)->field_0x294e8[i])) {                           \
        fn_7100719894(effects, (self)->field_0x294e8[i], -1.0f, 0.0f);         \
        (self)->field_0x294e8[i] = g_unk_7108b6c460;                           \
    }

void fn_7100541e64(UnkHandleAI_b0193* self) {
    void* effects = self->field_0x30 ? self->field_0x30->field_0x2b8 : nullptr;
    RELEASE_HANDLE_b0193(self, effects, 0)
    RELEASE_HANDLE_b0193(self, effects, 1)
    RELEASE_HANDLE_b0193(self, effects, 2)
    RELEASE_HANDLE_b0193(self, effects, 3)
}



using base::global::CStrId;

struct UnkEmmyRelaxAI_b59;
struct UnkFlagged;
struct UnkShape;

struct UnkDoorList_b0193 {
    base::reflection::UnkReflObject** items;
    u32 count;
};
UnkDoorList_b0193* fn_71007b5d90(UnkShape* shape);
u32 fn_7100d57f44(UnkShape* shape, UnkCollisionObject* object, void* a, u32 b);

struct UnkDoorOwner_b0193 {
    u8 unk_0x0[0x1b4];
    bool field_0x1b4;
    u8 unk_0x1b5[0x2a0 - 0x1b5];
    UnkCollisionContainer* field_0x2a0;
};
struct UnkDoorFx_b0193 {
    u8 unk_0x0[0x30];
    UnkDoorOwner_b0193* field_0x30;
};
struct UnkDoorListed_b0193 {
    u8 unk_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};
struct UnkDoorSeeker_b0193 {
    u8 unk_0x0[0x30];
    UnkActor* field_0x30;
};

bool fn_710054782c(UnkEmmyRelaxAI_b59* self_) {
    UnkDoorSeeker_b0193* self = reinterpret_cast<UnkDoorSeeker_b0193*>(self_);
    UnkActor* actor = fn_7100d7c990(self->field_0x30);
    if (!actor)
        return false;
    u32 found = 0;
    if (actor->field_0x1f8) {
        UnkShape* shape = fn_7100976588(reinterpret_cast<UnkFlagged*>(actor->field_0x1f8));
        if (shape) {
            UnkDoorList_b0193* list = fn_71007b5d90(shape);
            for (u32 i = 0; i < list->count; ++i) {
                base::reflection::UnkReflObject* object =
                    reinterpret_cast<UnkDoorListed_b0193*>(list->items[i])->field_0x10;
                UnkDoorFx_b0193* door = reinterpret_cast<UnkDoorFx_b0193*>(
                    fn_710009d50c(object->vf_0x0(), fn_71007b55e4()) ? object : nullptr);
                if (door->field_0x30 && door->field_0x30->field_0x1b4 && door->field_0x30->field_0x2a0) {
                    static CStrId s_door("door", true);
                    if (UnkCollisionObject* collision = fn_71008640a4(door->field_0x30->field_0x2a0, &s_door)) {
                        if (fn_7100d57f44(shape, collision, nullptr, 0) & 1)
                            found = 1;
                    }
                }
            }
        }
    }
    return found & 1;
}


// ---- 7100545138

void fn_7100daa92c(UnkManager_71726af468* self, CStrId name, u32 value);
bool fn_7100daaba0(UnkManager_71726af468* self, u32 value, CStrId name);
extern CStrId g_unk_71726e2090 __attribute__((visibility("hidden")));
struct UnkEmmyActor_b55;

struct UnkEmmyActor2_b0193 {
    u8 unk_0x0[0x2a254];
    u32 field_0x2a254;
};

static inline UnkManager_71726af468* AIManager_b0193() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

void fn_7100545138(UnkEmmyActor_b55* actor) {
    UnkEmmyActor2_b0193* self = reinterpret_cast<UnkEmmyActor2_b0193*>(actor);
    if (fn_7100daaba0(AIManager_b0193(), 0, CStrId(g_unk_71726e2090)))
        fn_7100daa92c(AIManager_b0193(), CStrId(g_unk_71726e2090), 0);
    self->field_0x2a254 = 0;
}

// ---- 71005592ac

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkRangeVtbl_b0193 {
    void* unk_0x0[27];
    void* (*vf_0xd8)(void* self);
};
struct UnkRangeObj_b0193 {
    UnkRangeVtbl_b0193* vtbl;
};
struct UnkRangeAI_b0193 {
    u8 unk_0x0[0x30];
    UnkActor* field_0x30;
    u8 unk_0x38[0x149b0 - 0x38];
    f32 field_0x149b0;
    u8 unk_0x149b4[0x29ee0 - 0x149b4];
    UnkRangeObj_b0193* field_0x29ee0;
};

struct UnkEmmyAIView;
struct UnkEmmyAI2_b62a3;
struct UnkEmmyAIComponent_b46a1;

f32 fn_71005592ac(UnkRangeAI_b0193* self) {
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    f32 result = -1.0f;
    if (player) {
        f32 px = player->field_0x44, py = player->field_0x54, pz = player->field_0x64;
        UnkActor* owner = self->field_0x30;
        f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
        f32 dx = px - ox;
        f32 dy = py - oy;
        f32 dz = pz - oz;
        self->field_0x29ee0->vtbl->vf_0xd8(self->field_0x29ee0);
        self->field_0x29ee0->vtbl->vf_0xd8(self->field_0x29ee0);
        u8* base = (u8*)&self->field_0x149b0;
        f32 radius;
        if (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(self)))
            radius = *(f32*)base;
        else if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)))
            radius = *(f32*)(base + 0x150);
        else if (fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(self)))
            radius = *(f32*)(base + 0x70);
        else if (fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self)))
            radius = *(f32*)(base + 0xe0);
        else
            radius = 0.0f;
        f32 distance_squared = dx * dx + dy * dy + dz * dz;
        if (distance_squared < radius * radius)
            result = __builtin_sqrtf(distance_squared);
    }
    return result;
}
