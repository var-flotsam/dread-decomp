// CChozoCommanderLandingSlamAttack's unit (tools/tu_map.py): on an event of kind 1 whose id (+0x30 of the
// object at +0x38 of the object at +0x10 of its +0x8) is a fixed hash, with the owner's (+0x8) character
// class (+0x220) and the actor the owner acts against: stops layer 5 and plays layer 4 when that actor's x
// is below the owner's (the other way round otherwise), sends the class a request (descriptors as
// UnkSpitSend) and drops the weak handle at +0x78 after unlinking +0x60 from its target. Returns true.
// Names are placeholders.
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, bool, bool, f32);
void fn_710117662c(UnkActorTarget*, u32, u32, u32, u32, f32);

struct UnkSlamEventId_b59 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkSlamEventInner_b59 {
    u8 pad_0x0[0x38];
    UnkSlamEventId_b59* field_0x38;
};

struct UnkSlamEventData_b59 {
    u8 pad_0x0[0x10];
    UnkSlamEventInner_b59* field_0x10;
};

struct UnkSlamEvent_b59 {
    u32 kind;
    u8 pad_0x4[4];
    UnkSlamEventData_b59* field_0x8;
};

struct UnkSlamEventSelf_b59 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x50];
    u8 field_0x60[0x18];
    UnkWeakHandle* field_0x78;
};

bool fn_7100437dd0(UnkSlamEventSelf_b59* self, UnkSlamEvent_b59* event) {
    if (event->kind == 1 && event->field_0x8->field_0x10->field_0x38->hash == 0xb4589aae3884b4be) {
        auto* anim = static_cast<UnkActorTarget*>(self->owner->field_0x220);
        UnkActor* actor = fn_7100d7c990(self->owner);
        if (anim && actor) {
            bool below = !(actor->field_0x44 >= self->owner->field_0x44);
            fn_7101178e08(anim, below ? 5 : 4, 0, 0, -1.0f);
            fn_710117662c(anim, below ? 4 : 5, 1, 0, 0, -1.0f);
            UnkSpitSend(anim, 0xf94afb5ec7557bd5, 0xf4061ff25a2163d6);
            if (UnkWeakHandle* handle = self->field_0x78) {
                if (handle->target)
                    fn_710117b3f0(handle->target, self->field_0x60);
                ResetRef(&self->field_0x78);
            }
        }
    }
    return true;
}
