// CBeamBoxComponent's unit (tools/tu_map.py unit 1355): the set-up. After the base's, it reads from the
// scenario's blackboard section whether the box was already placed (and where). Placed: it asks the
// track holder for the named track (or puts the owner at the saved place), sets three timeline ids and
// marks the navigation item open. Otherwise it registers with the player's set, points along its path
// and marks the item closed. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CLifeComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU372.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"
#include "../../../src/game_s2/UnkSpitSend.h"
#include "types.h"
#include <math.h>

struct UnkWeakTarget;
struct UnkClone_fn_71008ac458;
extern template bool base::refl_GetKeyedValue<UnkClone_fn_71008ac458>(char*, const base::UnkStrIdMember*, void*);

extern const u64 g_unk_710164af88;
extern const u64 g_unk_710164af90;
extern const u64 g_unk_710164af98;

// The property whose saved value names the section entry the box reads.
struct UnkBeamBoxProperty_l8a6730 {
    u8 pad_0x0[0x10];
    base::UnkStrIdMember id;  // 0x10
};
extern UnkBeamBoxProperty_l8a6730 g_unk_717272e168;

struct UnkBeamBoxVec2_l8a6730 {
    f32 x, y;
};

struct UnkBeamBoxVec3_l8a6730 {
    f32 x, y, z;
};

struct UnkBeamBoxOwner_l8a6730 : UnkActor {
    u8 pad_0x380[0x30];
    bool field_0x3b0;  // the transform changed
};

// A path node: its children by index and its position.
struct UnkBeamBoxPathNode_l8a6730 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkBeamBoxVec3_l8a6730* vf_0x18();  // position
    virtual void vf_0x20();
    virtual s32 vf_0x28();                                // child count
    virtual UnkBeamBoxPathNode_l8a6730* vf_0x30(s32 index);  // child
};

struct UnkBeamBoxPathHolder_l8a6730 {
    u8 pad_0x0[0x30];
    UnkBeamBoxPathNode_l8a6730 path;  // 0x30
};

struct UnkBeamBoxPlayer_l8a6730 {
    u8 pad_0x0[0x2a8];
    char* field_0x2a8;  // its +0x198 is the set of boxes
};

struct UnkBeamBoxEntry_l8a6730 {
    base::UnkStrIdMember id;
    u64 field_0x8;
};

struct UnkBeamBoxNamesTag_l8a6730;

struct UnkBeamBox_l8a6730 {
    u8 pad_0x0[0x30];
    UnkBeamBoxOwner_l8a6730* owner;  // 0x30
    u8 pad_0x38[0x3c0 - 0x38];
    base::UnkKeyedValueLookup<UnkBeamBoxNamesTag_l8a6730, UnkBeamBoxEntry_l8a6730> names;  // 0x3c0
    u8 pad_0x3e8[0x438 - 0x3e8];
    bool placed;                  // 0x438
    UnkBeamBoxVec3_l8a6730 dir;   // 0x43c
    u8 pad_0x448[0x460 - 0x448];
    base::UnkDeferredSet<void>* set;  // 0x460
    u8 pad_0x468[0x478 - 0x468];
    base::global::UnkStrIdBase track;  // 0x478
};
static_assert(__builtin_offsetof(UnkBeamBox_l8a6730, set) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkBeamBox_l8a6730, track) == 0x478, "layout");

void fn_71008a6d40(UnkBeamBox_l8a6730* self, bool flag);

static inline void SetPosition_l8a6730(UnkBeamBoxOwner_l8a6730* owner, const UnkBeamBoxVec3_l8a6730& pos) {
    owner->field_0x44 = pos.x;
    owner->field_0x54 = pos.y;
    owner->field_0x64 = pos.z;
    owner->field_0x3b0 = true;
}

// Sets an id on a timeline, with every flag.
static inline void SetId_l8a6730(UnkTimelineComponent* timeline, const u64* id,
                                 base::reflection::UnkReflTypeIdArg& mask) {
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

static inline UnkBeamBoxPlayer_l8a6730* Player_l8a6730() {
    return reinterpret_cast<UnkBeamBoxPlayer_l8a6730*>(fn_7100dec788(g_unk_7101d47310));
}

void fn_71008a6730(UnkBeamBox_l8a6730* self) {
    fn_710115fe1c(reinterpret_cast<char*>(self));
    self->placed = false;
    UnkBeamBoxVec2_l8a6730 saved = {0.0f, 0.0f};
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x608) {
        CBlackboard* blackboard = &manager->field_0x968;
        base::global::CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        UnkBeamBoxEntry_l8a6730* entry = self->names.Find(g_unk_717272e168.id);
        CBlackboard::CSection* section = fn_7101047270(blackboard, section_id);
        self->placed = section && base::refl_GetKeyedValue<UnkClone_fn_71008ac458>(
                                      reinterpret_cast<char*>(section), &entry->id, &saved);
    }
    void* item = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner));
    if (self->placed) {
        base::reflection::UnkReflTypeIdArg timeline_mask;
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        UnkBeamBoxOwner_l8a6730* owner = self->owner;
        void* holder = owner->field_0x220;
        if (holder && self->track.data != &g_unk_7101d47108->empty) {
            mode.id = 0;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(holder, self->track.data->Key(), 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_717276974c, &mask);
        } else {
            UnkBeamBoxVec3_l8a6730 pos = {saved.x, saved.y, 0.0f};
            SetPosition_l8a6730(owner, pos);
        }
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            SetId_l8a6730(timeline, &g_unk_710164af88, timeline_mask);
            SetId_l8a6730(timeline, &g_unk_710164af90, timeline_mask);
            SetId_l8a6730(timeline, &g_unk_710164af98, timeline_mask);
        }
        fn_71008a6d40(self, false);
        if (item) {
            fn_71009c0f1c(item, 0xb15b31b835c17820);
            fn_71009c0974(item, true);
        }
    } else {
        if (Player_l8a6730() && Player_l8a6730()->field_0x2a8) {
            auto* set = reinterpret_cast<base::UnkDeferredSet<void>*>(Player_l8a6730()->field_0x2a8 + 0x198);
            if (self->set) {
                fn_71004f3ea4(self->set, &self->set);
                self->set = nullptr;
            }
            self->set = set;
            fn_71004f3cf0(set, &self->set);
        }
        auto* holder = reinterpret_cast<UnkBeamBoxPathHolder_l8a6730*>(fn_7100382150(self->owner));
        if (holder) {
            UnkBeamBoxPathNode_l8a6730* path = &holder->path;
            if (path->vf_0x28()) {
                UnkBeamBoxPathNode_l8a6730* segment = path->vf_0x30(0);
                const UnkBeamBoxVec3_l8a6730* end = segment->vf_0x30(1)->vf_0x18();
                const UnkBeamBoxVec3_l8a6730* start = segment->vf_0x30(0)->vf_0x18();
                f32 dx = end->x - start->x;
                f32 dy = end->y - start->y;
                f32 dz = end->z - start->z;
                self->dir.x = dx;
                self->dir.y = dy;
                self->dir.z = dz;
                f32 scale = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
                self->dir.x *= scale;
                self->dir.y *= scale;
                self->dir.z *= scale;
                UnkBeamBoxVec3_l8a6730 pos = {self->owner->field_0x44, self->owner->field_0x54,
                                              self->owner->field_0x64};
                pos.x = segment->vf_0x30(0)->vf_0x18()->x;
                SetPosition_l8a6730(self->owner, pos);
            }
        }
        if (item) {
            fn_71009c0f1c(item, 0x13ae275a1bcd6558);
            fn_71009c0974(item, false);
        }
    }
}
