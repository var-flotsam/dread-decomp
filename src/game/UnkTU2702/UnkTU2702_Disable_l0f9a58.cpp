// Unit 2702 (hint CAIComponent): the AI is disabled: tells the owner's timeline "ondisabled", takes the
// owner off the team manager and the AI manager (unless the game manager says not to), drops the two
// deferred entries, stops the owner's navigation, resets the target state at +0x68 and leaves the list
// at +0x18a8. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkEmptyDtorElem.h"
#include "base/global/CName.h"
#include "base/global/CRntVector.h"
#include "game/CActorComponent.h"
#include "game/CInputComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkActor.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU108.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkParams.h"
#include "types.h"

void fn_7100da9f60(UnkManager_71726af468* self, UnkActor* actor);

static inline char* TeamManager() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return instance;
}

static inline UnkManager_71726af468* AIManager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

// A 4-byte element whose out-of-line destructor is empty.
struct UnkStateKey_l0f9a58 {
    u32 value;
    ~UnkStateKey_l0f9a58() { fn_71002874dc(this); }
};

// The target state at +0x68 (the object fn_7100119f98 enables, game/UnkTU108.h).
struct UnkTargetState_l0f9a58 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u8 pad_0xc[0x1c];
    u32 field_0x28;
    bool enabled;  // 0x2c
    bool field_0x2d;
    u8 pad_0x2e[0x22];
    u32 field_0x50;
    u8 pad_0x54[0x1c];
    UnkStateKey_l0f9a58* keys;  // 0x70
    u32 key_count;              // 0x78
    u8 pad_0x7c[0x1c];
    u32 field_0x98;

    void Reset() {
        enabled = false;
        field_0x2d = true;
        field_0x8 = 0;
        field_0x28 = 0;
        field_0x50 = 0;
        for (u64 i = 0; i < key_count; ++i)
            keys[i].~UnkStateKey_l0f9a58();
        key_count = 0;
        field_0x98 = 0;
        SetPolylineClosed(reinterpret_cast<UnkTileOutline*>(this), false);
    }
};
static_assert(__builtin_offsetof(UnkTargetState_l0f9a58, field_0x98) == 0x98, "layout");

struct UnkDisableAI_l0f9a58 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkTargetState_l0f9a58 target_state;  // 0x68
    u8 pad_0x108[0x1000];
    u64 field_0x1108;
    u8 pad_0x1110[0x6e8];
    void* deferred_a;  // 0x17f8
    u8 pad_0x1800[0x10];
    void* deferred_b;  // 0x1810
    u8 pad_0x1818[0x90];
    u8 list[0x8];  // 0x18a8
};
static_assert(__builtin_offsetof(UnkDisableAI_l0f9a58, field_0x1108) == 0x1108, "layout");
static_assert(__builtin_offsetof(UnkDisableAI_l0f9a58, deferred_b) == 0x1810, "layout");
static_assert(__builtin_offsetof(UnkDisableAI_l0f9a58, list) == 0x18a8, "layout");

void fn_71010f9a58(UnkDisableAI_l0f9a58* self) {
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        u64 id = base::global::CName("ondisabled").hash;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    }

    if (!fn_7100deabe4(g_unk_7101d47310)) {
        fn_71011a73bc(TeamManager(), reinterpret_cast<UnkRemovedValue_b60*>(self->owner));
        fn_7100da9f60(AIManager(), self->owner);
    }

    if (self->deferred_a) {
        fn_71002ae87c(self->deferred_a, &self->deferred_a);
        self->deferred_a = nullptr;
    }
    if (self->deferred_b) {
        fn_71002ae87c(self->deferred_b, &self->deferred_b);
        self->deferred_b = nullptr;
    }

    if (self->owner->field_0x298 && fn_710024fbc8(self->owner->field_0x298)) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x34 = 5;
        params.pad_0x38 = 1;
        params.field_0x39 = 1;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        fn_71006f4b0c(self->owner->field_0x298, &params, 1);
    }

    self->target_state.Reset();
    self->field_0x1108 = 0;
    fn_7100874460(self->list, reinterpret_cast<UnkWeakTarget**>(&self->owner));
}
