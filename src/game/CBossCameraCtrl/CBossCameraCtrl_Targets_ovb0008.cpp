// CBossCameraCtrl's unit (tools/tu_map.py, unit 1164): the boss camera's list of targets (counted
// weak handles at +0x2a0) and what uses it: the membership test (fn_7100750ab4) and adding a target
// (fn_7100750804); the start of a boss camera (fn_710074f104: base start, cleared framing box,
// approach speeds, each boss AI target's fn_71003b02ec, the first distance and the position from
// the argument's node); its end (fn_710074f80c) and the music and area name it sets
// (fn_710074f6c4), both through the music manager singleton; and whether every target's boss AI
// lets the camera finish (fn_710074f91c). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkXformNode.h"
#include "game/sound/CMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

extern void* g_unk_71726bc560;  // the music manager (game/sound/CMusicManager.h)
extern CStrId g_unk_7172710f30;
extern CStrId g_unk_7172710f50 __attribute__((visibility("hidden")));

void fn_7100e03a78(UnkGameManager*, void*);
void fn_7100e8f9a0(char* self, bool remove, base::global::CStrId* key);
struct UnkMgr_0xf18;
void fn_7100fdb9e0(UnkMgr_0xf18* manager, u32 kind, u32 flag, f32 a, f32 b);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkFixedCamera_fc;
struct UnkArg_fc;
void fn_71010ed98c(UnkFixedCamera_fc* self, UnkArg_fc* arg, u32 flag);

// A counted reference to a weak handle: the elements of the target list.
struct UnkHandlePtr_ovb0008 {
    UnkWeakHandle* p = nullptr;
};

struct UnkBossCameraCtrl_ovb0008 {
    u8 pad_0x0[0x10];
    bool active;  // 0x10
    u8 pad_0x11[0x227];
    f32 pos_x;     // 0x238
    f32 pos_y;     // 0x23c
    u8 pad_0x240[0xc];
    f32 target_z;  // 0x24c
    u8 pad_0x250[4];
    f32 distance;         // 0x254
    f32 target_distance;  // 0x258
    f32 distance_speed;   // 0x25c
    u8 pad_0x260[0x40];
    base::UnkVector<UnkHandlePtr_ovb0008> targets;  // 0x2a0
    u8 pad_0x2c8[0x60];
    u64 field_0x328;
    u64 field_0x330;
    u64 field_0x338;
    u8 pad_0x340[4];
    f32 field_0x344;
    u8 pad_0x348[0xc];
    f32 speed;  // 0x354

    bool Contains(UnkWeakTarget* target) {
        UnkHandlePtr_ovb0008* end = targets.data + targets.count;
        for (UnkHandlePtr_ovb0008* it = targets.data; it != end; ++it) {
            UnkWeakHandle* h = it->p;
            if ((h ? h->target : nullptr) == target)
                return true;
        }
        return false;
    }
};
static_assert(__builtin_offsetof(UnkBossCameraCtrl_ovb0008, speed) == 0x354, "UnkBossCameraCtrl_ovb0008 layout");

void fn_710074f550(UnkBossCameraCtrl_ovb0008* self);  // drops the targets that are gone
f32 fn_710074f2f0(UnkBossCameraCtrl_ovb0008* self);   // the framing distance
void fn_710031da18(base::UnkVector<UnkHandlePtr_ovb0008>* v, u32 capacity);

static inline void* MusicManager() {
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = instance;
    }
    return instance;
}

bool fn_7100750ab4(UnkBossCameraCtrl_ovb0008* self, UnkWeakTarget* target) {
    return self->Contains(target);
}

// A reference held for the duration of the add.
struct UnkHandleRef_ovb0008 {
    UnkWeakHandle* p;
    UnkHandleRef_ovb0008(UnkWeakHandle* h) : p(h) {
        if (p)
            ++p->ref_count;
    }
    ~UnkHandleRef_ovb0008() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

void fn_7100750804(UnkBossCameraCtrl_ovb0008* self, UnkWeakTarget* target) {
    if (!target || self->Contains(target))
        return;
    UnkHandleRef_ovb0008 ref(fn_7100d78378(target));
    base::UnkVector<UnkHandlePtr_ovb0008>& v = self->targets;
    u32 n = v.count;
    u32 capacity = v.capacity;
    if (capacity <= n && v.step != 0)
        capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_710031da18(&v, capacity);
    UnkWeakHandle** slot = &v.data[v.count].p;
    *slot = nullptr;
    AssignRef(slot, ref.p);
    ++v.count;
}

// The music state the manager reports (+0x2f8); kind 7 is the boss music.
struct UnkMusicState_ovb0008 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkMusicManagerView_ovb0008 {
    u8 pad_0x0[0x2f8];
    UnkMusicState_ovb0008* state;  // 0x2f8
};

void fn_710074f80c(UnkBossCameraCtrl_ovb0008* self) {
    g_unk_7101d47310->field_0x2230 = g_unk_7101d470e0;
    UnkMusicState_ovb0008* state = static_cast<UnkMusicManagerView_ovb0008*>(MusicManager())->state;
    if (state && state->kind == 7)
        fn_7100fdb9e0(reinterpret_cast<UnkMgr_0xf18*>(MusicManager()), 1, 0, -1.0f, 0.5f);
    fn_7100e03a78(g_unk_7101d47310, &g_unk_7172710f30);
}

// The first target's object at +0x1b8 and the child it makes on demand (fn_710115fe20).
struct UnkBossChild_ovb0008 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBossHolder_ovb0008 {
    u8 pad_0x0[0x48];
    UnkBossChild_ovb0008* field_0x48;
};
struct UnkHolder;
void fn_710115fe20(UnkHolder* holder);
// Declared without its argument in src/game/CBossAIComponent/fn_71003ad8e4.cpp; this caller passes
// the holder's child object.
CStrId* fn_710112bf20();

struct UnkBossHolderActor_ovb0008 {
    u8 pad_0x0[0x1b8];
    UnkBossHolder_ovb0008* holder;  // 0x1b8
};

static inline void* ChildObject(UnkBossHolder_ovb0008* holder) {
    if (!holder->field_0x48 || !holder->field_0x48->field_0x10)
        fn_710115fe20(reinterpret_cast<UnkHolder*>(holder));
    return holder->field_0x48 ? holder->field_0x48->field_0x10 : nullptr;
}

void fn_710074f6c4(UnkBossCameraCtrl_ovb0008* self) {
    if (self->targets.count && self->targets.data[0].p) {
        auto* actor = reinterpret_cast<UnkBossHolderActor_ovb0008*>(self->targets.data[0].p->target);
        if (actor && actor->holder && ChildObject(actor->holder)) {
            UnkGameManager* manager = g_unk_7101d47310;
            void* child = ChildObject(actor->holder);
            manager->field_0x2230 = *reinterpret_cast<CStrId* (*)(void*)>(fn_710112bf20)(child);
        }
    }
    fn_7100fdb9e0(reinterpret_cast<UnkMgr_0xf18*>(MusicManager()), 7, 0, -1.0f, 0.5f);
}

struct UnkBossState_ovb0008 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkBossAI_ovb0008 {
    void** vtable;
    const CClass* GetClass() { return reinterpret_cast<const CClass* (*)(UnkBossAI_ovb0008*)>(vtable[0])(this); }
    bool vf_0x7c8() { return reinterpret_cast<bool (*)(UnkBossAI_ovb0008*)>(vtable[0x7c8 / 8])(this); }
};
void fn_71003b02ec(UnkBossAI_ovb0008* ai);

struct UnkBossActor_ovb0008 {
    u8 pad_0x0[0x1b8];
    UnkBossState_ovb0008* state;  // 0x1b8
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkBossAI_ovb0008* ai;  // 0x240
};

struct UnkManagerSub_ovb0008 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

static inline UnkManagerSub_ovb0008* ManagerSub() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return *reinterpret_cast<UnkManagerSub_ovb0008**>(reinterpret_cast<char*>(manager) + 0x2410);
}

bool fn_710074f91c(UnkBossCameraCtrl_ovb0008* self) {
    if (!self->active)
        return true;
    fn_710074f550(self);
    UnkHandlePtr_ovb0008* end = self->targets.data + self->targets.count;
    for (UnkHandlePtr_ovb0008* it = self->targets.data; it != end; ++it) {
        auto* actor = reinterpret_cast<UnkBossActor_ovb0008*>(it->p->target);
        bool check_ai = true;
        if (actor->state && actor->state->state == 2) {
            UnkManagerSub_ovb0008* sub = ManagerSub();
            check_ai = sub && sub->field_0x10;
        }
        if (check_ai) {
            UnkBossAI_ovb0008* ai = actor->ai;
            if (!ai || !fn_710009d50c(ai->GetClass(), fn_71003ab524()) || ai->vf_0x7c8())
                return false;
        }
        if (actor && actor->animation && fn_710117b348(actor->animation, fn_7100d06178()))
            return false;
    }
    return true;
}

struct UnkCameraMotion_ovb0008 {
    u8 pad_0x0[0xb4];
    f32 field_0xb4;
    f32 field_0xb8;
};

struct UnkCameraArg_ovb0008 {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
    u8 pad_0x20[0x130 - 0x20 - sizeof(UnkXformNode)];
    UnkCameraMotion_ovb0008* motion;  // 0x130
};

void fn_710074f104(UnkBossCameraCtrl_ovb0008* self, UnkCameraArg_ovb0008* arg) {
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), 1);
    self->field_0x328 = 0;
    self->field_0x330 = 0;
    self->field_0x338 = 0;
    self->speed = 0.015f;
    self->distance_speed = 1.0f;
    self->field_0x344 = 1.0f;
    UnkHandlePtr_ovb0008* end = self->targets.data + self->targets.count;
    for (UnkHandlePtr_ovb0008* it = self->targets.data; it != end; ++it) {
        if (!it->p || !it->p->target)
            continue;
        UnkBossAI_ovb0008* ai = reinterpret_cast<UnkBossActor_ovb0008*>(it->p->target)->ai;
        if (ai && fn_710009d50c(ai->GetClass(), fn_71003ab524()))
            fn_71003b02ec(ai);
    }
    f32 distance = fn_710074f2f0(self);
    self->target_distance = distance;
    self->distance = distance;
    UnkXformNode* node = &arg->node;
    if (fn_7100206154(node).z < 3000.0f) {
        self->pos_x = fn_7100206154(node).x;
        self->pos_y = fn_7100206154(node).y;
        self->distance = fn_7100206154(node).z - self->target_z;
    }
    if (self->distance < 1000.0f)
        self->distance_speed = 20.0f;
    UnkCameraMotion_ovb0008* motion = arg->motion;
    if (fabsf(motion->field_0xb4) > 0.0001f || fabsf(motion->field_0xb8) > 0.0001f)
        self->distance_speed = 20.0f;
    fn_7100e8f9a0(static_cast<char*>(g_unk_7101d47310->field_0x2228), false, &g_unk_7172710f50);
}
