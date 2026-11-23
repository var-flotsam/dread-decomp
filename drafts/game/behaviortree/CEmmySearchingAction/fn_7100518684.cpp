// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): when the CEmmyMovement is in mode 1,
// picks the search animation (the id the AI's blackboard holds, cleared after use, else one from the movement),
// plays it and listens to the animation through a weak handle. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyName g_unk_71726df1b0;
// The animation ids by the movement's direction.
extern const u64 g_unk_7101649d88[] __attribute__((visibility("hidden")));

bool fn_710051d464(CBlackboard::CSection* section, const void* name, CStrId* out);
void fn_710051d568();

// An empty string id, built in place (CStrId has no default constructor; the original's is inline).
struct UnkEmptyStrId_l518684 {
    alignas(8) u8 storage[sizeof(CStrId)];

    UnkEmptyStrId_l518684() {
        Id().data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        Id().data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrId_l518684() { Id().~CStrId(); }
    CStrId& Id() { return *reinterpret_cast<CStrId*>(storage); }
};

struct UnkEmmyMovement_l518684 {
    void** vtable;
    u8 pad_0x8[0x9dc];
    u8 field_0x9e4[0x4];

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEmmyMovement_l518684*)>(vtable[0])(this);
    }
    UnkNavMeshCell* vf_0x2d0() {
        return reinterpret_cast<UnkNavMeshCell* (*)(UnkEmmyMovement_l518684*)>(vtable[0x2d0 / 8])(this);
    }
};

s32 fn_71005717d4(UnkEmmyMovement_l518684* self, void* arg);

struct UnkEmmyAI_l518684 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1258];
    CBlackboard blackboard;  // 0x1260
};

struct UnkEmmyActor_l518684 {
    u8 pad_0x0[0x1f0];
    UnkEmmyMovement_l518684* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkWeakTarget* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkEmmyAI_l518684* ai;  // 0x240
};

struct UnkEmmyAnimLink_l518684 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

// The slot that holds a counted reference to an object's weak handle. fn_7100316018 takes the slot itself,
// not the pointer inside it: with UnkWeakHandle** (include/game/CAbilityGhostAura.h today) the compiler shares
// one address for the slot and folds it into a pre-indexed load. Belongs in include/game_s2/UnkWeakHandle.h.
struct UnkWeakRef_l518684 {
    UnkWeakHandle* handle;
};
void fn_7100316018(UnkWeakTarget* self, UnkWeakRef_l518684* out);

struct UnkEmmySearchingAction_l518684 {
    u8 pad_0x0[0x18];
    UnkEmmyActor_l518684* actor;  // 0x18
    u8 pad_0x20[0x10];
    u64 animation;  // 0x30
    u8 pad_0x38[0x8];
    UnkEmmyAnimLink_l518684 link;  // 0x40
    UnkWeakRef_l518684 ref;        // 0x58
};

// Stops listening to the animation the handle points at, and drops the handle.
inline void DetachAnimation_l518684(UnkEmmySearchingAction_l518684* self, UnkEmmyAnimLink_l518684* link) {
    if (UnkWeakHandle* handle = self->ref.handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        ResetRef(&self->ref.handle);
    }
}

void fn_7100518684(UnkEmmySearchingAction_l518684* self) {
    UnkEmmyMovement_l518684* movement = self->actor->movement;
    if (movement && !fn_710009d50c(movement->vf_0x0(), fn_710057134c()))
        movement = nullptr;
    if (fn_71010cc3a4(movement) != 1)
        return;

    UnkWeakTarget* animation = self->actor->animation;
    UnkEmmyAI_l518684* ai = self->actor->ai;
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        ai = nullptr;

    UnkEmptyStrId_l518684 empty;
    CStrId& id = empty.Id();
    // The cleared id's typed value keeps a slot of its own for the whole function.
    UnkTypedValue value;
    CBlackboard* blackboard = &ai->blackboard;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71726b7cb8))
        fn_710051d464(section, &g_unk_71726df1b0, &id);
    if (id.data != &g_unk_7101d47108->empty) {
        self->animation = id.data->Key();
        if (CBlackboard::CSection* section = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
            if (!blackboard->field_0xe8) {
                value.type = fn_7100009658();
                value.value = &g_unk_7101d470e0;
                fn_7100332c80(section, &g_unk_71726df1b0, &value);
            }
        }
    } else {
        UnkNavMeshCell* cell = movement->vf_0x2d0();
        if (cell && fn_71002b005c(cell))
            self->animation = 0xff7922a845d1be30;
        else
            self->animation = g_unk_7101649d88[fn_71005717d4(movement, movement->field_0x9e4)];
    }

    u64 hash = self->animation;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask)) {
        UnkEmmyAnimLink_l518684* link = &self->link;
        link->context = self;
        link->callback = fn_710051d568;
        DetachAnimation_l518684(self, link);
        if (animation) {
            fn_7100316018(animation, &self->ref);
            fn_710117b3e8(self->ref.handle ? self->ref.handle->target : nullptr, link);
        }
    } else {
        self->animation = ~0ull;
        DetachAnimation_l518684(self, &self->link);
    }
}
