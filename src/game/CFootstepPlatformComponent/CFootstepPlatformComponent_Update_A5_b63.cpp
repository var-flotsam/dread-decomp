// CFootstepPlatformComponent's unit (tools/tu_map.py unit 1255): resolves the lazily resolved handle at +0x50
// (state at +0x78, handle at +0x80) and returns when it names nothing. When what it names has a door lock,
// runs fn_71008d0ef4 on it and sets an id on the timeline of the owner's part (fn_71003c6c3c on +0x30) to the
// all-flags mask. Then, when the owner has its collision container (+0x2a0), turns on one of the two unit
// statics' collision objects by which side of the owner (x, +0x44) the named object stands. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "game/CAutoolAIComponent.h"
#include "game/CFootstepPlatformComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1376.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkCountedFootstep_A5_b63 : base::UnkRefCounted {
    UnkActor* field_0x10;
};

struct UnkRef {
    UnkCountedFootstep_A5_b63* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkWeakTarget;
struct UnkDoorLock;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);
struct UnkEvacPart_b61;
struct UnkEvacTimeline_b61;

struct UnkFootstepStatic_b45a2;
extern u64 g_unk_710164ab58;

struct UnkFootstepOwner_A5_b63 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x2a0 - 0x48];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkFootstepPlatform_A5_b63 {
    u8 pad_0x0[0x30];
    UnkFootstepOwner_A5_b63* owner;
    u8 pad_0x38[0x50 - 0x38];
    u8 source[0x28];  // 0x50
    u32 state;        // 0x78
    u32 pad_0x7c;
    base::UnkRefHandle handle;  // 0x80
};

__attribute__((always_inline)) static inline void Resolve_A5_b63(UnkFootstepPlatform_A5_b63* self) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
}

void fn_71007e7c08(UnkFootstepPlatform_A5_b63* self) {
    Resolve_A5_b63(self);
    auto* c = static_cast<UnkCountedFootstep_A5_b63*>(self->handle.ptr);
    if (!c || !c->field_0x10)
        return;
    asm("" ::: "memory");  // the original reloads the object after the test
    if (UnkDoorLock* lock = fn_71002af860(reinterpret_cast<UnkWeakTarget*>(c->field_0x10))) {
        fn_71008d0ef4(lock);
        auto* timeline =
            reinterpret_cast<UnkTimelineComponent*>(fn_71003c6c3c(reinterpret_cast<UnkEvacPart_b61*>(self->owner)));
        if (timeline) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_710164ab58, &mask.id, false, -1.0f);
        }
    }
    if (UnkCollisionContainer* collision = self->owner->collision) {
        Resolve_A5_b63(self);
        f32 dx = static_cast<UnkCountedFootstep_A5_b63*>(self->handle.ptr)->field_0x10->field_0x44 - self->owner->x;
        if (auto* a = fn_71008640a4(collision, reinterpret_cast<const base::global::CStrId*>(&g_unk_717271fae0)))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(a), !(dx > 0.0f));
        if (auto* b = fn_71008640a4(collision, reinterpret_cast<const base::global::CStrId*>(&g_unk_717271fad8)))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(b), dx > 0.0f);
    }
}
