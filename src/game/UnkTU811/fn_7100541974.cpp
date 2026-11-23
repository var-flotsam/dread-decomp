// UnkTU811 (CEmmyAIComponent's unit): resets the E.M.M.I.'s AI: drops its registrations and navigation-mesh
// listeners, unregisters it from the E.M.M.I. registry and the game manager, switches the alerts off and
// clears both sightings, restarts its animation state and timeline, and clears the marks of every dirty
// navigation group, handing each mark to the mesh group the AI names. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAnimationComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkMeshListener.h"
#include "game/UnkGameManager.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU39.h"
#include "game/UnkTU55.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRecord_54715c;
struct UnkEmmyAI_b60;
struct UnkEmmyRelaxAI_b59;
struct UnkHandleAI_b0193;
void fn_7100dacb50(UnkEmmyRegistry_b55* registry, void* ai);
void fn_71002b1de0(void* group, void* entry);

extern UnkSpawnGroupKey g_unk_71726e1f60 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_71726e1fa0 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_71726e20b0 __attribute__((visibility("hidden")));
extern const UnkVec3 g_unk_7101649ed8 __attribute__((visibility("hidden")));

// An entry registered in a deferred set (the entry is the registration's address).
struct UnkRegistration_l541974 {
    base::UnkDeferredSet<void>* set;
    u8 pad_0x8[0xc0];

    void Unregister() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
    }
};

// What the alerts remember of a sighting: the object, when, how strongly, where and in which cell.
struct UnkSighting_l541974 {
    UnkRefObject* object;
    u32 time;          // 0x8
    f32 score;         // 0xc
    u8 position[0x8];  // 0x10
    u32 cell;          // 0x18
    u32 field_0x1c;

    void Reset() {
        if (UnkRefObject* p = object) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            object = nullptr;
        }
        time = 0;
        score = -3.4028235e38f;
        fn_710007b444(position, &g_unk_7101649ed8);
        cell = 0xffff;
        field_0x1c = 0;
    }
};

struct UnkSwitch_l541974 {
    void** vtable;

    void vf_0x20(bool on) { reinterpret_cast<void (*)(UnkSwitch_l541974*, bool)>(vtable[0x20 / 8])(this, on); }
};

struct UnkAlerts_l541974 {
    UnkSwitch_l541974 first;
    u8 pad_0x8[0xf8];
    UnkSwitch_l541974 second;  // 0x100
    u8 pad_0x108[0x2b8];
    UnkSighting_l541974 sighting;  // 0x3c0
    UnkSighting_l541974 saved;     // 0x3e0
    bool active;                   // 0x400
};

struct UnkContext_l541974 {
    u8 pad_0x0[0x610];
    bool showsEffects;  // 0x610
};

struct UnkOwner_l541974 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x280 - 0x228];
    UnkWorld* effects;  // 0x280
};

struct UnkGameManagerView_l541974 {
    u8 pad_0x0[0x1fa8];
    base::UnkDeferredSet<base::UnkDeferredCallback<bool>> onEmmyStop;  // 0x1fa8
};

// A navigation mark: cleared on reset, then handed back to the mesh's group.
struct UnkNavMark_l541974 {
    bool set;
    u8 pad_0x1[0xff];
};

struct UnkNavGroup_l541974 {
    u8 pad_0x0[0xb0];
    bool dirty;  // 0xb0
    u8 pad_0xb1[0x7];
    base::global::CRntVector<UnkNavMark_l541974> marks;  // 0xb8
};

struct UnkNavGroupName_l541974 {
    u8 pad_0x0[0x30];
    base::global::CStrId name;  // 0x30
};

struct UnkEmmyAI_l541974 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l541974* owner;  // 0x30
    u8 pad_0x38[0x14760 - 0x38];
    void* field_0x14760;
    void* field_0x14768;
    u8 pad_0x14770[0x10];
    UnkAlerts_l541974 alerts;  // 0x14780
    u8 pad_0x14b88[0x14c68 - 0x14b88];
    UnkMeshListener meshListeners[2];  // 0x14c68
    u8 pad_0x14c98[0x27720 - 0x14c98];
    bool alerted;  // 0x27720
    u8 pad_0x27721[0x29400 - 0x27721];
    UnkRegistration_l541974 registrations[2];  // 0x29400
    u8 pad_0x29590[0x29ee0 - 0x29590];
    UnkContext_l541974* context;  // 0x29ee0
    u8 pad_0x29ee8[0x2a254 - 0x29ee8];
    u32 field_0x2a254;  // 0x2a254
    u8 pad_0x2a258[0x2a368 - 0x2a258];
    bool notifyOnStop;  // 0x2a368
    u8 pad_0x2a369[0x2a688 - 0x2a369];
    base::global::CRntVector<UnkNavGroup_l541974*> navGroups;  // 0x2a688

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkEmmyAI_l541974*)>(vtable[0])(this);
    }
    void vf_0x7d0() { reinterpret_cast<void (*)(UnkEmmyAI_l541974*)>(vtable[0x7d0 / 8])(this); }
    void vf_0x858() { reinterpret_cast<void (*)(UnkEmmyAI_l541974*)>(vtable[0x858 / 8])(this); }

    void SetAlerted(bool on) {
        if (alerted != on) {
            alerted = on;
            vf_0x858();
        }
    }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l541974, alerts) == 0x14780, "UnkEmmyAI_l541974 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l541974, registrations) == 0x29400, "UnkEmmyAI_l541974 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l541974, notifyOnStop) == 0x2a368, "UnkEmmyAI_l541974 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l541974, navGroups) == 0x2a688, "UnkEmmyAI_l541974 layout");

static inline UnkNavMeshManager* NavMeshManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

void fn_7100541974(UnkEmmyAI_l541974* self) {
    self->registrations[0].Unregister();
    self->registrations[1].Unregister();
    self->meshListeners[0].Detach();
    self->meshListeners[1].Detach();
    fn_7100dacb50(g_unk_71726af0a8, self);
    fn_7100de9730(g_unk_7101d47310, &g_unk_71726e1f60, self);
    self->field_0x14760 = nullptr;
    self->field_0x14768 = nullptr;

    UnkAlerts_l541974* alerts = &self->alerts;
    if (alerts->active) {
        alerts->active = false;
        if (!fn_710009d50c(self->vf_0x0(), fn_71004fa488()))
            fn_7100552324(reinterpret_cast<UnkEmmyRelaxAI_b59*>(self));
    }
    alerts->first.vf_0x20(false);
    alerts->second.vf_0x20(false);
    const UnkRecord_54715c* current = reinterpret_cast<UnkRecord_54715c*>(&alerts->sighting);
    UnkRecord_54715c* saved = reinterpret_cast<UnkRecord_54715c*>(&alerts->saved);
    fn_710054715c(saved, current);
    alerts->sighting.time = 0;
    self->vf_0x7d0();

    self->SetAlerted(false);
    if (UnkAnimationComponent* animation = self->owner->animation)
        fn_710117ab64(animation, false, &g_unk_71726e1fa0);
    fn_71003695fc(reinterpret_cast<UnkNode*>(self));
    fn_7100541e64(reinterpret_cast<UnkHandleAI_b0193*>(self));
    self->SetAlerted(true);
    fn_7100df5cec(g_unk_7101d47310, &g_unk_7101d470e0);

    if (self->context->showsEffects) {
        UnkWorld* effects = self->owner->effects;
        fn_710114fe8c(effects, 0x7ff940855c402436, 0, 0);
        fn_710114fe8c(effects, 0xe83eac0c8c30776c, 0, 0);
        fn_710114fe8c(effects, 0xaf057066b81fccd4, 0, 0);
    }
    self->field_0x2a254 = 0;
    if (self->notifyOnStop) {
        self->notifyOnStop = false;
        UnkGameManagerView_l541974* manager = reinterpret_cast<UnkGameManagerView_l541974*>(g_unk_7101d47310);
        bool value = true;
        fn_71000303c0(&manager->onEmmyStop, &value);
    }

    alerts->saved.Reset();
    alerts->sighting.Reset();
    if (UnkAnimationComponent* animation = self->owner->animation)
        fn_710117ab64(animation, false, &g_unk_71726e20b0);
    fn_7100541fd4(reinterpret_cast<UnkEmmyAI_b60*>(self));

    UnkNavGroup_l541974** end = self->navGroups.data + self->navGroups.count;
    for (UnkNavGroup_l541974** it = self->navGroups.data; it != end; ++it) {
        if (!(*it)->dirty)
            continue;
        (*it)->dirty = false;
        UnkNavGroup_l541974* group = *it;
        UnkNavMark_l541974* marksEnd = group->marks.data + group->marks.count;
        for (UnkNavMark_l541974* mark = group->marks.data; mark != marksEnd; ++mark) {
            mark->set = false;
            auto* info = reinterpret_cast<UnkNavGroupName_l541974*>(
                fn_710110c3d8(reinterpret_cast<UnkNavComponent*>(self)));
            if (!info)
                continue;
            if (void* meshGroup = fn_7100ec13cc(NavMeshManager(), &info->name))
                fn_71002b1de0(meshGroup, mark);
        }
    }
}
