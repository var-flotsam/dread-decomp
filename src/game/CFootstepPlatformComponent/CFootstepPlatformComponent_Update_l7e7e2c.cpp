// CFootstepPlatformComponent's unit (tools/tu_map.py unit 1255): the update. After the base update, a
// pending step (+0xc8) waits 3 seconds, then unlocks the door lock of the object the handle at +0x80 names,
// sets an id on its timeline and the owner's, and runs the step's script (+0xe0) on the owner; a second
// timer (+0xd4, limit +0xd8) clears both. Names and layouts are placeholders.
#include "game/CFootstepPlatformComponent.h"
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CAutoolAIComponent.h"
#include "game/CComponent.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU367.h"
#include "game/UnkTU437.h"
#include "game/UnkTimelineCalls.h"
#include "game/lua_lib.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkDoorLock;
struct UnkEvacPart_b61;
struct UnkEvacTimeline_b61;
struct UnkTimelineComponent;
struct UnkParent;

bool fn_71008d5e08(UnkDoorLock* lock);

struct UnkCountedStep_l7e7e2c : base::UnkRefCounted {
    void* object;  // 0x10
};

struct UnkFootstepPlatform_l7e7e2c {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x50 - 0x38];
    u8 source[0x28];  // 0x50
    u32 state;        // 0x78
    u32 pad_0x7c;
    base::UnkRefHandle handle;  // 0x80
    u8 pad_0x88[0xc8 - 0x88];
    bool pending;     // 0xc8
    f32 wait;         // 0xcc
    bool resetting;   // 0xd0
    f32 reset_time;   // 0xd4
    f32 reset_limit;  // 0xd8
    u32 pad_0xdc;
    base::global::UnkStrIdBase script;  // 0xe0
};
static_assert(__builtin_offsetof(UnkFootstepPlatform_l7e7e2c, script) == 0xe0, "layout");

// Resolves the lazily resolved handle once.
static inline void Resolve_l7e7e2c(UnkFootstepPlatform_l7e7e2c* self) {
    if (self->state == 1) {
        {
            base::UnkRefPtrMember fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
}

static inline void* Target_l7e7e2c(UnkFootstepPlatform_l7e7e2c* self) {
    auto* c = static_cast<UnkCountedStep_l7e7e2c*>(self->handle.ptr);
    return c ? c->object : nullptr;
}

// Sets the step's id on a part's timeline, with every flag.
static inline void MarkTimeline_l7e7e2c(void* part) {
    auto* timeline = reinterpret_cast<UnkTimelineComponent*>(fn_71003c6c3c(static_cast<UnkEvacPart_b61*>(part)));
    if (timeline) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &g_unk_710164ab58, &mask.id, false, -1.0f);
    }
}

void fn_71007e7e2c(UnkFootstepPlatform_l7e7e2c* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (self->pending) {
        self->wait += dt;
        if (self->wait >= 3.0f) {
            Resolve_l7e7e2c(self);
            UnkDoorLock* lock = fn_71002af860(static_cast<UnkWeakTarget*>(Target_l7e7e2c(self)));
            if (lock && !fn_71008d5e08(lock)) {
                fn_71008d0ef4(lock);
                Resolve_l7e7e2c(self);
                MarkTimeline_l7e7e2c(Target_l7e7e2c(self));
                MarkTimeline_l7e7e2c(self->owner);
            }
            self->pending = false;
            self->wait = 0.0f;
            if (self->script.data != &g_unk_7101d47108->empty) {
                if (fn_71010c0bb0(g_unk_7172a01d18, self->script.Text())) {
                    fn_7100fbc4e0(g_unk_7172a01d18, self->owner);
                    char result[0x100];
                    fn_71010c0d80(g_unk_7172a01d18, 1, result, sizeof(result), nullptr, nullptr);
                }
            }
        }
    }
    if (self->resetting) {
        self->reset_time += dt;
        if (self->reset_time >= self->reset_limit) {
            self->pending = false;
            self->wait = 0.0f;
            self->reset_time = 0.0f;
            self->resetting = false;
        }
    }
}
