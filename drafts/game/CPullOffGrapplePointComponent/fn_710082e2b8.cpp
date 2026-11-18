// CPullOffGrapplePointComponent's unit (tools/tu_map.py unit 1296): the pull-off point's pulled handler. After
// the base handler, when pulled it reports to the game blackboard, sets the attached part's weight to 1, plays
// the pull-off timeline on the owner and listens for its end, and leaves the owner without a timeline state
// when neither pull-off id is known to it. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game/UnkTU437.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

void fn_710082efe8(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_7100a2d4c8(UnkWeakTarget* target, void* listener);

struct UnkPullOffGrapplePoint_l82e2b8;
void fn_710082ef40(UnkPullOffGrapplePoint_l82e2b8* self);

struct UnkPullOffCounted_l82e2b8 : base::UnkRefCounted {
    void* field_0x10;
};

// What fn_710082e510 finds for the attached object.
struct UnkPullOffPart_l82e2b8 {
    void** vtable;

    void SetWeight(f32 weight) {
        reinterpret_cast<void (*)(UnkPullOffPart_l82e2b8*, f32)>(vtable[0x168 / 8])(this, weight);
    }
};

// The two timeline ids, by the side the point is pulled from.
struct UnkPullOffIds_l82e2b8 {
    u8 pad_0x0[0x68];
    CStrId right;  // 0x68
    CStrId left;   // 0x70
};

// A listener: the object and function it calls, and the counted link to what it listens to.
struct UnkPullOffListener_l82e2b8 {
    void** vtable;
    UnkPullOffGrapplePoint_l82e2b8* context;               // 0x8
    void (*callback)(UnkPullOffGrapplePoint_l82e2b8*);     // 0x10
    UnkWeakHandle* link;                                   // 0x18
};

struct UnkPullOffOwner_l82e2b8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38 (the UnkXform of game/UnkXformNode.h)
    u8 pad_0x78[0x2a0 - 0x78];
    UnkStruct_7100861148* collision;  // 0x2a0
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* timeline;  // 0x2e8

    UnkActor* Actor() { return reinterpret_cast<UnkActor*>(this); }
};

struct UnkPullOffGrapplePoint_l82e2b8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPullOffOwner_l82e2b8* owner;  // 0x30
    u8 pad_0x38[0x98];
    u8 source[0x28];              // 0xd0
    u32 state;                    // 0xf8
    u32 pad_0xfc;
    base::UnkRefHandle attached;  // 0x100
    UnkPullOffIds_l82e2b8* ids;   // 0x108
    UnkPullOffListener_l82e2b8 listener;  // 0x110

    void Notify(CBlackboard* blackboard, bool flag) {
        reinterpret_cast<void (*)(UnkPullOffGrapplePoint_l82e2b8*, CBlackboard*, bool)>(vtable[0x1a8 / 8])(
            this, blackboard, flag);
    }
};
static_assert(__builtin_offsetof(UnkPullOffGrapplePoint_l82e2b8, listener) == 0x110, "layout");

// Resolves the attached object's handle if it is still pending.
static inline void Resolve(UnkPullOffGrapplePoint_l82e2b8* self) {
    if (self->state == 1) {
        {
            base::UnkRefPtrMember fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->attached, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
}

// The attached object, resolving the handle first if it is still pending.
static inline void* Attached(UnkPullOffGrapplePoint_l82e2b8* self) {
    Resolve(self);
    auto* c = static_cast<UnkPullOffCounted_l82e2b8*>(self->attached.ptr);
    return c ? c->field_0x10 : nullptr;
}

// The timeline id to play: the one that is set, or, with both set, the one for the side the owner faces.
static inline const CStrId& PullOffId(UnkPullOffGrapplePoint_l82e2b8* self) {
    UnkPullOffIds_l82e2b8* ids = self->ids;
    u64 empty = g_unk_7101d470e0.data->Key();
    if (empty == ids->right.data->Key())
        return ids->left;
    if (empty == ids->left.data->Key())
        return ids->right;
    if (fn_710002614c(&self->owner->transform).m[0] < 0.0f)
        return self->ids->left;
    return self->ids->right;
}

// Listens for the end of the owner's timeline; whether the timeline knows either pull-off id.
static inline bool Listen(UnkPullOffGrapplePoint_l82e2b8* self, UnkTimelineComponent* timeline) {
    UnkPullOffListener_l82e2b8* listener = &self->listener;
    listener->context = self;
    listener->callback = fn_710082ef40;
    if (UnkWeakHandle* link = listener->link) {
        if (UnkWeakTarget* target = link->target)
            fn_7100a2d4d0(target, listener);
        if (UnkWeakHandle* p = listener->link) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            listener->link = nullptr;
        }
    }
    fn_710082efe8(reinterpret_cast<UnkWeakTarget*>(timeline), &listener->link);
    fn_7100a2d4c8(listener->link ? listener->link->target : nullptr, listener);
    u64 left = self->ids->left.data->Key();
    if (fn_7100a2c13c(timeline, &left))
        return true;
    u64 right = self->ids->right.data->Key();
    return fn_7100a2c13c(timeline, &right) != nullptr;
}

void fn_710082e2b8(UnkPullOffGrapplePoint_l82e2b8* self, void* arg, bool pulled) {
    fn_710082ccec(reinterpret_cast<UnkDoorGrapplePoint_A5_b66*>(self), arg, pulled);
    if (!pulled)
        return;
    self->Notify(&g_unk_7101d47310->field_0x968, true);
    if (Attached(self))
        static_cast<UnkPullOffPart_l82e2b8*>(fn_710082e510(Attached(self)))->SetWeight(1.0f);
    u64 id = PullOffId(self).data->Key();
    fn_7100d7c828(self->owner->Actor(), &id, false);
    fn_710082cee4(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), false);
    UnkTimelineComponent* timeline = self->owner->timeline;
    if (!timeline || !Listen(self, timeline))
        fn_7100232864(reinterpret_cast<UnkEntityState*>(self->owner), false);
    fn_710082cf08(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), true);
    fn_7100861148(self->owner->collision);
}
