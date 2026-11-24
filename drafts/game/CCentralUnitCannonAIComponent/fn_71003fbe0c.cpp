// CCentralUnitCannonAIComponent unit (tools/tu_map.py): after the base's part (fn_71010f97ec), when the
// owner (+0x30) tracks an object (+0x220), hooks the callback fn_71003fe80c (link at +0x14a28, handle at
// +0x14a40) up to it, dropping any earlier handle first. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

u32 fn_71003fe80c(char* p0, char* p1);

struct UnkCannonOwner_l3fbe0c {
    u8 pad_0x0[0x220];
    UnkWeakTarget* track;
};

struct UnkCannonCallback_l3fbe0c {
    void* context;
    u32 (*callback)(char*, char*);
};

// A listener registered with the tracked object: a callback and the handle it is registered through.
struct UnkCannonTrackLink_l3fbe0c {
    u8 pad_0x0[0x8];
    UnkCannonCallback_l3fbe0c callback;
    UnkWeakHandle* handle;

    void Attach(void* context, u32 (*fn)(char*, char*), UnkWeakTarget* track) {
        callback = UnkCannonCallback_l3fbe0c{context, fn};
        if (UnkWeakHandle* h = handle) {
            if (UnkWeakTarget* target = h->target)
                fn_710117b3f0(target, this);
            ResetRef(&handle);
        }
        CAbilityGhostAura_AssignWeakHandleToRef(track, &handle);
        fn_710117b3e8(handle ? handle->target : nullptr, this);
    }
};

struct UnkCannonAI_l3fbe0c {
    u8 pad_0x0[0x30];
    UnkCannonOwner_l3fbe0c* owner;
    u8 pad_0x38[0x149f0];
    UnkCannonTrackLink_l3fbe0c link;  // 0x14a28
};
static_assert(__builtin_offsetof(UnkCannonAI_l3fbe0c, link) == 0x14a28, "link");

void fn_71010f97ec(UnkCannonAI_l3fbe0c* self);

void fn_71003fbe0c(UnkCannonAI_l3fbe0c* self) {
    fn_71010f97ec(self);
    UnkWeakTarget* track = self->owner->track;
    if (!track)
        return;
    self->link.Attach(self, fn_71003fe80c, track);
}
