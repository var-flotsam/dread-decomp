// CCharClassSteamJetComponent unit: resolves both link lists and collects each linked actor's target, notifies
// itself, then stores the offset between the two linked actors' positions and resets the scale to one.
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game/UnkTU437.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSteamJetLinkTarget_l899780 {
    u8 pad_0x0[0x10];
    void* actor;  // 0x10
};

struct UnkSteamJetLink_l899780 {
    u8 pad_0x0[8];
    u8 member[0x28];  // 0x8
    s32 state;        // 0x30
    u8 pad_0x34[4];
    UnkSteamJetLinkTarget_l899780* target;  // 0x38
};

struct UnkSteamJetLinks_l899780 {
    u8 links[8];
    u32 count;  // 0x8
};

struct UnkSteamJetVec3_l899780 {
    f32 x, y, z;
};

static inline UnkSteamJetVec3_l899780 operator-(const UnkSteamJetVec3_l899780& a, const UnkSteamJetVec3_l899780& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

struct UnkSteamJetActor_l899780 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38: rows; the last column is the position

    UnkSteamJetVec3_l899780 Position() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
};

struct UnkSteamJetEndTarget_l899780 {
    u8 pad_0x0[0x10];
    UnkSteamJetActor_l899780* actor;  // 0x10
};

// A link to an actor whose handle is fetched from `source` on first use.
struct UnkSteamJetEnd_l899780 {
    u8 pad_0x0[8];
    u8 source[0x28];  // 0x8
    s32 state;        // 0x30
    u8 pad_0x34[4];
    UnkSteamJetEndTarget_l899780* target;  // 0x38

    UnkSteamJetEndTarget_l899780* Target() {
        if (state == 1) {
            {
                base::UnkRefPtrMember ref = fn_7100353be8(source);
                fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&target),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return target;
    }
};

struct UnkSteamJet_l899780 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkSteamJetLinks_l899780 links_a;  // 0x40
    u8 pad_0x4c[0x68 - 0x4c];
    UnkSteamJetLinks_l899780 links_b;  // 0x68
    u8 pad_0x74[0x90 - 0x74];
    base::global::CRntVector<void*> targets_a;  // 0x90
    base::global::CRntVector<void*> targets_b;  // 0xb8
    UnkSteamJetVec3_l899780 scale;              // 0xe0
    u8 pad_0xec[0xf8 - 0xec];
    UnkSteamJetEnd_l899780 end_b;  // 0xf8
    UnkSteamJetEnd_l899780 end_a;  // 0x138
    u8 pad_0x178[0x1f0 - 0x178];
    UnkSteamJetVec3_l899780 offset;  // 0x1f0

    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkSteamJet_l899780*, CBlackboard*)>(vtable[0x140 / 8])(this, blackboard);
    }
};
static_assert(__builtin_offsetof(UnkSteamJet_l899780, targets_b) == 0xb8, "UnkSteamJet layout");
static_assert(__builtin_offsetof(UnkSteamJet_l899780, end_a) == 0x138, "UnkSteamJet layout");
static_assert(__builtin_offsetof(UnkSteamJet_l899780, offset) == 0x1f0, "UnkSteamJet layout");

// The link at `index` of `links`, resolved to its actor first if that is still pending.
static inline UnkSteamJetLink_l899780* ResolvedLink(UnkSteamJetLinks_l899780& links, u32 index) {
    UnkSteamJetLink_l899780* link = reinterpret_cast<UnkSteamJetLink_l899780*>(
        fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(links.links), index));
    if (link->state == 1) {
        {
            UnkRefPtr ref = fn_710023b580(link->member);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link->target),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link->state = 2;
    }
    return link;
}

// The target of the actor `link` refers to, or null.
static inline void* LinkedTarget(UnkSteamJetLink_l899780* link) {
    if (link->target && link->target->actor)
        return fn_710082e510(link->target->actor);
    return nullptr;
}

void fn_7100899780(UnkSteamJet_l899780* self) {
    for (u32 i = 0; i < self->links_a.count; ++i) {
        if (void* target = LinkedTarget(ResolvedLink(self->links_a, i)))
            self->targets_a.PushBack(target);
    }
    for (u32 i = 0; i < self->links_b.count; ++i) {
        if (void* target = LinkedTarget(ResolvedLink(self->links_b, i)))
            self->targets_b.PushBack(target);
    }

    self->vf_0x140(&g_unk_7101d47310->field_0x968);

    if (self->end_a.Target() && self->end_a.Target()->actor && self->end_b.Target() &&
        self->end_b.Target()->actor) {
        self->offset = self->end_a.Target()->actor->Position() - self->end_b.Target()->actor->Position();
    }
    self->scale.x = 1.0f;
    self->scale.y = 1.0f;
    self->scale.z = 1.0f;
}
