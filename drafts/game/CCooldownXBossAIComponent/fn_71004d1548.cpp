// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): the start step (fn_71004d1548). After the
// boss start it copies the target's four values, resolves the two entity references and switches their
// entities off, recreates the colliders, hooks the track callback up, draws the first timer between the
// tunable bounds and sends the camera its start request. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU437.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkCooldownXMsgAI_ovb0088;
struct UnkTentacleCollider_ovb0132;
struct UnkTentacleOwner_ovb0132;
struct UnkCollisionObject;

struct UnkCoreXSource_A2_b73 {
    virtual void vf_0x0();
    u8 pad_0x8[0x38];
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    u8 pad_0x50[0x10];
    bool field_0x60;
};

struct UnkCoreXQuery_A2_b73 {
    u8 pad_0x0[0x8];
    UnkWeakHandle** field_0x8;
    u32 field_0x10;
};
UnkCoreXQuery_A2_b73* fn_710110c438(UnkCoreXAI_A2_b73* self);

void fn_71004d1b38(UnkCooldownXMsgAI_ovb0088* self, bool flag);
UnkCollisionObject* fn_7100709c8c(UnkTentacleCollider_ovb0132* self, UnkTentacleOwner_ovb0132* owner,
                                  const CStrId* group, u32 flags);
void fn_71004d57d0();
void fn_7100030fc0(void* list, const void* key_ptr, f32 value);

extern base::reflection::UnkTunable g_unk_71726d8260;
extern char g_unk_7101649b58[];  // the model flag the start sets
extern const u64 g_unk_7101649b60;  // the timeline the camera starts
extern const u64 g_unk_7101649b68;  // the timeline the owner starts
__attribute__((visibility("hidden"))) extern u64 g_unk_71726d8708;

struct UnkCooldownXCameraKey_l4d1548 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};
__attribute__((visibility("hidden"))) extern UnkCooldownXCameraKey_l4d1548* g_unk_71726d8748;

// The tunable fields the start reads.
struct UnkCooldownXStartTunable_l4d1548 {
    u8 pad_0x0[0x24];
    s32 start_state;  // 0x24
};

// A collider the boss recreates, named by its group.
struct UnkCooldownXCollider_l4d1548 {
    u8 pad_0x0[0x8];
    CStrId group;  // 0x8
};

// The boss's data: its colliders and the bounds of the first timer.
struct UnkCooldownXData_l4d1548 {
    u8 pad_0x0[0xc40];
    UnkCooldownXCollider_l4d1548** colliders;  // 0xc40
    u32 collider_count;                        // 0xc48
    u8 pad_0xc4c[0x1c];
    f32 timer_min;  // 0xc68
    f32 timer_max;  // 0xc6c

    UnkCooldownXCollider_l4d1548** begin() { return colliders; }
    UnkCooldownXCollider_l4d1548** end() { return colliders + collider_count; }
};

// What a resolved entity reference holds: the entity's state.
struct UnkCooldownXEntity_l4d1548 : base::UnkRefCounted {
    UnkEntityState* state;  // 0x10
};

// An entity reference resolved on start.
struct UnkCooldownXEntityRef_l4d1548 {
    u8 source[0x28];
    s32 status;                          // 0x28: 1 until resolved, then 2
    UnkCooldownXEntity_l4d1548* entity;  // 0x30

    // Resolves the reference once, then switches its entity off.
    void Start() {
        if (status == 1) {
            {
                base::UnkRefPtrMember resolved = fn_7100353be8(source);
                fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&entity),
                              reinterpret_cast<const base::UnkRefHandle*>(&resolved));
            }
            status = 2;
        }
        if (entity && entity->state)
            fn_7100232864(entity->state, false);
    }
};

// The callback the track calls: an object and a function.
struct UnkCooldownXDelegate_l4d1548 {
    void* context;
    void (*callback)();
};

// The boss's hook on the owner's track: the link the track keeps and the handle it gives back.
struct UnkCooldownXLink_l4d1548 {
    u8 pad_0x0[0x8];
    UnkCooldownXDelegate_l4d1548 delegate;  // 0x8
};

struct UnkCooldownXOwner_l4d1548 {
    u8 pad_0x0[0x220];
    void* track;  // 0x220
    void* model;  // 0x228
    u8 pad_0x230[0xb8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkCooldownXStartAI_l4d1548 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkCooldownXOwner_l4d1548* owner;  // 0x30
    u8 pad_0x38[0x14968];
    UnkCooldownXData_l4d1548* data;  // 0x149a0
    u8 pad_0x149a8[0xc98];
    f32 target_values[4];  // 0x15640
    u8 pad_0x15650[0x34];
    f32 timer;  // 0x15684
    u8 pad_0x15688[0x10];
    UnkCooldownXEntityRef_l4d1548 first;  // 0x15698
    u8 pad_0x156d0[0x48];
    UnkCooldownXEntityRef_l4d1548 second;  // 0x15718
    u8 pad_0x15750[0x4];
    bool field_0x15754;
    u8 pad_0x15755[0x1b];
    UnkCooldownXLink_l4d1548 link;  // 0x15770
    UnkWeakHandle* handle;          // 0x15788
    u8 pad_0x15790[0x8c];
    s32 field_0x1581c;
    u8 pad_0x15820[0x15];
    bool field_0x15835;
    bool field_0x15836;

    void SetState(s32 next, bool flag) {
        reinterpret_cast<void (*)(UnkCooldownXStartAI_l4d1548*, s32, bool)>(vtable[0x208 / 8])(this, next, flag);
    }
};
static_assert(__builtin_offsetof(UnkCooldownXStartAI_l4d1548, second) == 0x15718, "layout");
static_assert(__builtin_offsetof(UnkCooldownXStartAI_l4d1548, link) == 0x15770, "layout");
static_assert(__builtin_offsetof(UnkCooldownXStartAI_l4d1548, field_0x1581c) == 0x1581c, "layout");
static_assert(__builtin_offsetof(UnkCooldownXStartAI_l4d1548, field_0x15836) == 0x15836, "layout");

// Starts a timeline on `timeline` with every flag.
static inline void PlayTimeline(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

void fn_71004d1548(UnkCooldownXStartAI_l4d1548* self) {
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    self->field_0x15754 = false;
    self->field_0x15835 = false;
    self->field_0x15836 = false;
    if (UnkCoreXQuery_A2_b73* query = fn_710110c438(reinterpret_cast<UnkCoreXAI_A2_b73*>(self))) {
        if (query->field_0x10 == 1) {
            if (UnkWeakHandle* handle = *query->field_0x8) {
                if (auto* source = reinterpret_cast<UnkCoreXSource_A2_b73*>(handle->target)) {
                    if (source->field_0x60)
                        (*reinterpret_cast<void (***)(UnkCoreXSource_A2_b73*)>(source))[0x98 / 8](source);
                    self->target_values[0] = source->field_0x40;
                    self->target_values[1] = source->field_0x44;
                    self->target_values[2] = source->field_0x48;
                    self->target_values[3] = source->field_0x4c;
                }
            }
        }
    }
    self->first.Start();
    self->second.Start();
    for (UnkCooldownXCollider_l4d1548* collider : *self->data) {
        if (collider)
            fn_7100709c8c(reinterpret_cast<UnkTentacleCollider_ovb0132*>(collider),
                          reinterpret_cast<UnkTentacleOwner_ovb0132*>(self->owner), &collider->group, 0x180);
    }
    UnkCooldownXOwner_l4d1548* owner = self->owner;
    if (owner->model) {
        fn_7101172780(static_cast<UnkFlagTarget*>(owner->model), g_unk_7101649b58);
        owner = self->owner;
    }
    if (owner->track) {
        fn_710117662c(static_cast<UnkActorTarget*>(owner->track), 4, 1, false, false, -1.0f);
        UnkCooldownXLink_l4d1548* link = &self->link;
        UnkWeakHandle* old = self->handle;
        link->delegate = UnkCooldownXDelegate_l4d1548{self, fn_71004d57d0};
        UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->track);
        if (UnkWeakHandle* handle = old) {
            if (UnkWeakTarget* t = handle->target)
                fn_710117b3f0(t, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        if (track) {
            CAbilityGhostAura_AssignWeakHandleToRef(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
    fn_71004d1b38(reinterpret_cast<UnkCooldownXMsgAI_ovb0088*>(self), false);
    UnkCooldownXData_l4d1548* data = self->data;
    f32 min = data->timer_min;
    f32 max = data->timer_max;
    f32 draw = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    self->timer = min + (max - min) * draw;
    UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    fn_7100030fc0(reinterpret_cast<u8*>(ability) + 0x5a8, &g_unk_71726d8708, 2.3f);
    UnkCooldownXStartTunable_l4d1548* tunable = reinterpret_cast<UnkCooldownXStartTunable_l4d1548*>(&g_unk_71726d8260);
    if (tunable->start_state)
        self->SetState(tunable->start_state, true);
    u64 key = g_unk_71726d8748->key;
    u32 kind = g_unk_71727bebb8;
    UnkCooldownXOwner_l4d1548* camera =
        reinterpret_cast<UnkCooldownXOwner_l4d1548*>(fn_7100debbc8(g_unk_7101d47310, &key, &kind));
    if (camera && camera->track)
        UnkSpitSend(camera->track, 0x3d73722bcf353a54, 0xf4061ff25a2163d6);
    // The camera's timeline is read even when there is no camera, as in the original.
    if (UnkTimelineComponent* timeline = camera->timeline)
        PlayTimeline(timeline, &g_unk_7101649b60);
    self->field_0x1581c = 0;
    if (UnkTimelineComponent* timeline = self->owner->timeline)
        PlayTimeline(timeline, &g_unk_7101649b68);
}
