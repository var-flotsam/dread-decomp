// CGoliathXAIComponent's unit (tools/tu_map.py): on defeat, logs the defeat message, runs the base handler,
// detonates every burst projection bomb still out, takes the core's handle once and moves the core to the
// owner's position, telling its AI component and its collision about it. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAISmartObjectComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkMlogMessage_b42a1;

// A message for the log: its kind, its text id and the text's arguments.
struct UnkMlogMessage_l588ba8 {
    u32 kind;
    base::global::CStrId text;
    base::global::CRntVector<base::UnkStrIdMember> args;

    UnkMlogMessage_l588ba8(u32 k, CStrId t) : kind(k), text(t), args(0xc) {}
};

// A projectile in the weapon movement list: its movement and what that movement is attached to.
struct UnkBombEntry_l588ba8 {
    u8 pad_0x0[0x10];
    UnkReflObject* movement;  // 0x10
};

struct UnkBombMovement_l588ba8 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* field_0x30;
};

struct UnkBombList_l588ba8 {
    UnkBombEntry_l588ba8** items;
    u32 count;

    UnkBombEntry_l588ba8** begin() { return items; }
    UnkBombEntry_l588ba8** end() { return items + count; }
};

// The core's actor, as far as this handler uses it.
struct UnkCoreActor_l588ba8 {
    void** vtable;
    u8 pad_0x8[0x238];
    UnkReflObject* aiComponent;  // 0x240
    u8 pad_0x248[0x58];
    UnkStruct_7100861148* collision;  // 0x2a0

    // vtable slot 0xd0: places the actor at `pos`.
    void SetPosition(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkCoreActor_l588ba8*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};
static_assert(__builtin_offsetof(UnkCoreActor_l588ba8, collision) == 0x2a0, "layout");

// The core's handle target (base::UnkRefCounted's layout); the object it stands for is at +0x10.
struct UnkCoreTarget_l588ba8 : base::UnkRefCounted {
    void* object;
};

struct UnkGoliathX_l588ba8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14c50];
    u8 field_0x14c88[0x28];
    u32 coreState;  // 0x14cb0
    u8 pad_0x14cb4[4];
    base::UnkRefHandle core;  // 0x14cb8
};
static_assert(__builtin_offsetof(UnkGoliathX_l588ba8, coreState) == 0x14cb0, "layout");

struct UnkObj_7101129240;

// The other declarations of this symbol drop the result; it is the actor the object stands for.
static inline UnkCoreActor_l588ba8* PickActor(UnkObj_7101129240* object, bool primary) {
    return reinterpret_cast<UnkCoreActor_l588ba8* (*)(UnkObj_7101129240*, bool)>(fn_7101129b08)(object, primary);
}

struct UnkBigFistAI_b59;
void fn_71003e5ef8(UnkBigFistAI_b59* self, void* arg);
void fn_71004db440(UnkReflObject* component, UnkActor* owner);

static inline UnkCoreTarget_l588ba8* CoreTarget(UnkGoliathX_l588ba8* self) {
    return static_cast<UnkCoreTarget_l588ba8*>(self->core.ptr);
}

void fn_7100588ba8(UnkGoliathX_l588ba8* self, void* arg) {
    UnkMlogMessage_l588ba8 message(0, CStrId("#MLOG_DEFEAT_BOSS_SUPER_GOLIATH", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                  reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
    fn_71003e5ef8(reinterpret_cast<UnkBigFistAI_b59*>(self), arg);

    UnkBombList_l588ba8* bombs = reinterpret_cast<UnkBombList_l588ba8*>(fn_7100ac73b0());
    for (UnkBombEntry_l588ba8* entry : *bombs) {
        if (!entry)
            continue;
        UnkReflObject* movement = entry->movement;
        if (movement && fn_710009d50c(movement->vf_0x0(), fn_7100589df4()))
            fn_7100232c68(static_cast<UnkBombMovement_l588ba8*>(movement)->field_0x30);
    }

    if (self->coreState == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x14c88);
            // The reference's pointer is the handle's.
            fn_7100234f78(&self->core, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->coreState = 2;
    }
    UnkCoreTarget_l588ba8* target = CoreTarget(self);
    if (target && target->object) {
        if (UnkSpawnRegistered_ovb0065* registered = fn_7100360044(target->object)) {
            UnkCoreActor_l588ba8* actor = PickActor(reinterpret_cast<UnkObj_7101129240*>(registered), false);
            if (actor) {
                UnkActor* owner = self->owner;
                UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
                actor->SetPosition(&pos, true);
                if (UnkReflObject* component = actor->aiComponent) {
                    if (fn_710009d50c(component->vf_0x0(), fn_71004d94cc()))
                        fn_71004db440(component, self->owner);
                }
                if (actor->collision)
                    fn_7100861148(actor->collision);
            }
        }
    }
    message.args.Free();
}
