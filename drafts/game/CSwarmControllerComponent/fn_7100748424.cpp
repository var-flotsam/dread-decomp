// CSwarmControllerComponent's unit: the controller's start. It resets every group (its +0x68 flag and its
// position), sizes the member list at +0x208 for all members, sets the fill ratios, resolves the linked actor
// (state 1 at +0x70), seeds each member's +0x184 from the tunables and links +0x260 into the manager
// g_unk_7101d47360 (created on first use; its list at +0xa60). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU883.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

struct UnkActor;
struct UnkMagnetPathHolder_b52;
UnkMagnetPathHolder_b52* fn_7100382150(UnkActor* actor);

struct UnkSwarmMember_l748424 {
    u8 pad_0x0[0x184];
    f32 field_0x184;
};

// One group of swarm members (0x148 bytes).
struct UnkSwarmGroup_l748424 {
    u8 pad_0x0[0x68];
    u8 field_0x68;
    u8 pad_0x69[7];
    CVector3D position;  // 0x70
    u8 pad_0x7c[0x3c];
    UnkSwarmMember_l748424** members;  // 0xb8
    u32 count;                          // 0xc0
    u8 pad_0xc4[0x1c];
    u32 field_0xe0;
    u8 pad_0xe4[0x64];

    // Back to its start: the flag off and the position at the origin.
    void Reset() {
        field_0x68 = 0;
        position.x = g_unk_710164779c.x;
        position.y = g_unk_710164779c.y;
        position.z = g_unk_710164779c.z;
    }
};
static_assert(__builtin_offsetof(UnkSwarmGroup_l748424, members) == 0xb8, "UnkSwarmGroup_l748424 layout");
static_assert(sizeof(UnkSwarmGroup_l748424) == 0x148, "UnkSwarmGroup_l748424 layout");

struct UnkSwarmGroups_l748424 {
    u8 pad_0x0[0x50];
    UnkSwarmGroup_l748424* groups;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110

    // Group i, or null past the end.
    UnkSwarmGroup_l748424* Get(s64 i) {
        if (i < count && groups)
            return &groups[i];
        return nullptr;
    }
};

struct UnkSwarmDef_l748424 {
    u8 pad_0x0[0x10];
    UnkSwarmGroups_l748424* groups;  // 0x10
};

struct UnkSwarmTunables_l748424 {
    u8 pad_0x0[0x330];
    s32 maxMembers;  // 0x330
};

// What the link at +0x78 refers to: its actor.
struct UnkSwarmLinkTarget_l748424 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

// What fn_7100382150 finds for that actor.
struct UnkSwarmActorData_l748424 {
    u8 pad_0x0[0x30];
    u8 field_0x30[1];
};

struct UnkSwarmController_l748424 {
    virtual void vf_0x0();
    u8 pad_0x8[0x40];
    u8 field_0x48[0x28];  // the link's source property
    u32 linkState;        // 0x70
    u8 pad_0x74[4];
    base::UnkRefHandle link;  // 0x78
    void* linkData;           // 0x80
    u8 pad_0x88[0x60];
    UnkSwarmDef_l748424* def;  // 0xe8
    u8 pad_0xf0[0x10];
    u32 field_0x100;
    u8 pad_0x104[0x9c];
    u32 field_0x1a0;
    u32 field_0x1a4;
    u8 pad_0x1a8[0x38];
    UnkSwarmTunables_l748424* tunables;  // 0x1e0
    u8 activeGroups;                     // 0x1e8
    u8 pad_0x1e9[3];
    f32 fill;      // 0x1ec
    f32 lastFill;  // 0x1f0
    u8 pad_0x1f4[0x14];
    base::global::CRntVector<UnkSwarmMember_l748424*> members;  // 0x208
    u8 pad_0x230[0x28];
    u32 field_0x258;
    u8 pad_0x25c[4];
    void* managerLink;  // 0x260

    UnkSwarmGroups_l748424* Groups() { return def ? def->groups : nullptr; }

    typedef void (*StepFn)(UnkSwarmController_l748424*);
    void Slot(u32 offset) { (*reinterpret_cast<StepFn**>(this))[offset / 8](this); }
};
static_assert(__builtin_offsetof(UnkSwarmController_l748424, def) == 0xe8, "UnkSwarmController_l748424 layout");
static_assert(__builtin_offsetof(UnkSwarmController_l748424, tunables) == 0x1e0, "UnkSwarmController_l748424 layout");
static_assert(__builtin_offsetof(UnkSwarmController_l748424, members) == 0x208, "UnkSwarmController_l748424 layout");
static_assert(__builtin_offsetof(UnkSwarmController_l748424, managerLink) == 0x260, "UnkSwarmController_l748424 layout");

f32 fn_71007376d8(UnkSwarmTunables_l748424* tunables);
void fn_7100748708(UnkSwarmController_l748424* self);

void fn_7100748424(UnkSwarmController_l748424* self) {
    self->field_0x100 = 0;
    self->field_0x258 = 0;
    if (self->def && self->def->groups) {
        u32 total = 0;
        for (s32 i = 0; i < self->def->groups->count; ++i) {
            UnkSwarmGroup_l748424* group = self->Groups()->Get(i);
            total += group->count;
            group->Reset();
        }
        self->members.Clear();
        self->members.Reserve(total);

        s32 max = self->tunables->maxMembers;
        self->fill = max;
        self->fill = max > 0 ? (s32)total / self->fill : -1.0f - self->fill;
        self->lastFill = self->fill;

        if (self->linkState == 1) {
            {
                UnkRefPtr reference = fn_710023b580(self->field_0x48);
                fn_7100234f78(&self->link, reinterpret_cast<const base::UnkRefHandle*>(&reference));
            }
            self->linkState = 2;
        }
        UnkSwarmLinkTarget_l748424* target = reinterpret_cast<UnkSwarmLinkTarget_l748424*>(self->link.ptr);
        if (target && target->actor)
            self->linkData =
                reinterpret_cast<UnkSwarmActorData_l748424*>(fn_7100382150(target->actor))->field_0x30;
        if (self->linkData)
            fn_7100748708(self);

        self->field_0x1a4 = self->field_0x1a0;
        for (s32 i = 0; i < self->def->groups->count; ++i) {
            UnkSwarmGroup_l748424* group = self->def->groups->Get(i);
            group->field_0xe0 = group->count;
            for (u64 j = 0; j < group->count; ++j) {
                UnkSwarmMember_l748424* member = group->members[j];
                member->field_0x184 = fn_71007376d8(self->tunables);
            }
        }
        self->activeGroups = self->def->groups->count;
        self->Slot(0x1f8);
    }

    void** link = &self->managerLink;
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    if (*link) {
        fn_71005c4828(*link, link);
        *link = nullptr;
    }
    void* list = static_cast<char*>(manager) + 0xa60;
    *link = list;
    fn_71005c41bc(list, reinterpret_cast<UnkCallback*>(link));
}
