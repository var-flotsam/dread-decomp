// CCharClassSteamJetComponent's unit: handles a message that has flag 0x80 and one of 0xe set (+0xb4) while
// the component's flag at +0xf4 is clear: marks the owner (+0x30) with g_unk_710164af20 (fn_7100d7c828), moves
// the component's listener (+0x198) onto the owner's timeline (+0x2e8), clears a byte of what fn_710035379c
// returns for the owner, and sets 0x1000 in the flags of the owner's "collision" object. Returns false.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CTimelineComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern u64 g_unk_710164af20;
void fn_7100a2d4c8(UnkWeakTarget* target, void* listener);
void fn_710082efe8(UnkWeakTarget* self, UnkWeakHandle** out);

struct UnkSteamJetNode_l89a7c0 {
    u8 pad_0x0[0x3d];
    u8 field_0x3d;
};
UnkSteamJetNode_l89a7c0* fn_710035379c(UnkActor* owner);

struct UnkSteamJetFlagged_l89a7c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
    virtual void vf_0x60(u32 flags);
};

struct UnkSteamJetMessage_l89a7c0 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

struct UnkSteamJetOwner_l89a7c0 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[0x40];
    UnkWeakTarget* timeline;  // 0x2e8
};

struct UnkSteamJetComponent_l89a7c0 {
    u8 pad_0x0[0x30];
    UnkSteamJetOwner_l89a7c0* owner;  // 0x30
    u8 pad_0x38[0xbc];
    bool field_0xf4;
    u8 pad_0xf5[0xa3];
    u8 listener[0x18];  // 0x198
    UnkWeakHandle* handle;  // 0x1b0
};

bool fn_710089a7c0(UnkSteamJetComponent_l89a7c0* self, UnkActor* unused, UnkSteamJetMessage_l89a7c0* message) {
    u32 flags = message->flags;
    if (!(flags & 0x80) || !(flags & 0xe) || self->field_0xf4)
        return false;
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164af20, false);
    if (UnkWeakTarget* timeline = self->owner->timeline) {
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target) {
                fn_7100a2d4d0(target, self->listener);
                if (!self->handle)
                    goto attach;
            }
            ResetRef(&self->handle);
        }
    attach:
        fn_710082efe8(timeline, &self->handle);
        fn_7100a2d4c8(self->handle ? self->handle->target : nullptr, self->listener);
    }
    if (UnkSteamJetNode_l89a7c0* node = fn_710035379c(reinterpret_cast<UnkActor*>(self->owner)))
        node->field_0x3d = 0;
    if (UnkCollisionContainer* collision = self->owner->collision) {
        UnkSteamJetFlagged_l89a7c0* object;
        {
            CStrId name("collision", true);
            object = reinterpret_cast<UnkSteamJetFlagged_l89a7c0*>(fn_71008640a4(collision, &name));
        }
        if (object)
            object->vf_0x60(object->vf_0x58() | 0x1000);
    }
    return false;
}
