// CQuetzoaXAIComponent unit (tools/tu_map.py): the boss's defeat step. Sends "#MLOG_DEFEAT_BOSS_SUPER_QUETZOA"
// to the game manager's callback set at +0x2128, runs the base step, resolves the linked spawn point once, and
// places the actor it spawned at the owner's position, notifying a CCoreXAIComponent on it. Names and layouts
// are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAISmartObjectComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CCollisionComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2704.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

namespace {

// The message, laid out as src/game/CEmmyCaveAIComponent/UnkEmmyDefeatedMessage_b42.cpp lays it out.
struct UnkMlogMessage_l63fbec {
    u32 kind;
    base::global::CStrId text;
    base::global::CRntVector<base::UnkStrIdMember> args;

    UnkMlogMessage_l63fbec(const CStrId& t) : kind(0), text(t), args(0xc) {}
    explicit UnkMlogMessage_l63fbec(const char* t) : UnkMlogMessage_l63fbec(CStrId(t, true)) {}
    ~UnkMlogMessage_l63fbec() { args.Free(); }
};

}  // namespace

// The weak handle the link refers to; the spawn point it stands for is at +0x10.
struct UnkQuetzoaXLinkTarget_l63fbec : base::UnkRefCounted {
    void* spawn_point;
};

struct UnkQuetzoaXReflObject_l63fbec {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkQuetzoaXReflObject_l63fbec*)>(vtable[0])(this);
    }
};

// The actor the spawn point gives.
struct UnkQuetzoaXSpawned_l63fbec {
    void** vtable;
    u8 pad_0x8[0x238];
    UnkQuetzoaXReflObject_l63fbec* ai;  // 0x240
    u8 pad_0x248[0x58];
    UnkStruct_7100861148* collision;  // 0x2a0

    void vf_0xd0(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkQuetzoaXSpawned_l63fbec*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};
static_assert(__builtin_offsetof(UnkQuetzoaXSpawned_l63fbec, collision) == 0x2a0, "layout");

struct UnkQuetzoaXAI_l63fbec {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14b90 - 0x38];
    u8 link[0x28];  // 0x14b90
    u32 link_state;  // 0x14bb8
    u8 pad_0x14bbc[4];
    base::UnkRefHandle spawn_handle;  // 0x14bc0
};

struct UnkMlogMessage_b42a1;
void fn_71004db440(void* component, UnkActor* owner);
void fn_71004db4bc(void* component);

// The other declarations of this symbol drop the result; it is the actor the object stands for.
static inline UnkQuetzoaXSpawned_l63fbec* SpawnedActor(UnkSpawnRegistered_ovb0065* object) {
    return reinterpret_cast<UnkQuetzoaXSpawned_l63fbec* (*)(UnkObj_7101129240*, bool)>(fn_7101129b08)(
        reinterpret_cast<UnkObj_7101129240*>(object), false);
}

void fn_710063fbec(UnkQuetzoaXAI_l63fbec* self, void* arg) {
    UnkMlogMessage_l63fbec message("#MLOG_DEFEAT_BOSS_SUPER_QUETZOA");
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                  reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
    fn_71003aea40(reinterpret_cast<UnkBossAIComponent*>(self), arg);
    if (self->link_state == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->link);
            // The reference's pointer is the handle's.
            fn_7100234f78(&self->spawn_handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->link_state = 2;
    }
    UnkQuetzoaXLinkTarget_l63fbec* target = static_cast<UnkQuetzoaXLinkTarget_l63fbec*>(self->spawn_handle.ptr);
    if (target && target->spawn_point) {
        if (UnkSpawnRegistered_ovb0065* registered = fn_7100360044(target->spawn_point)) {
            if (UnkQuetzoaXSpawned_l63fbec* actor = SpawnedActor(registered)) {
                UnkActor* owner = self->owner;
                UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
                actor->vf_0xd0(&pos, true);
                if (UnkQuetzoaXReflObject_l63fbec* ai = actor->ai) {
                    if (fn_710009d50c(ai->vf_0x0(), fn_71004d94cc())) {
                        fn_71004db440(ai, self->owner);
                        fn_71004db4bc(ai);
                    }
                }
                if (actor->collision)
                    fn_7100861148(actor->collision);
            }
        }
    }
}
