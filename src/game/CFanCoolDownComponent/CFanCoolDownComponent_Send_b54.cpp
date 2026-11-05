// Four "send a named request" functions of one shape: when the owner (+0x30) has the sub-object at
// its +0x220, they send it the hash of a short name with mode 0 under the same kind and
// descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). fn_7100880938 ("start"),
// fn_7100880a5c ("block") and fn_7100880b80 ("relax") are in the CCharClassFanCoolDownComponent
// unit; fn_71007ddb9c ("open") in the CProfessorDoorComponent unit (tools/tu_map.py). Names are
// placeholders. fn_71007dda00 (CProfessorDoorComponent's unit) first gives the owner's weak target
// (fn_71002b3b84) the id 0xb0c1699f8b57b14b (fn_71009c0f1c), then sends "open" with the other
// second descriptor (UnkNamedSend_b54.h).
#include "../../game_s2/UnkSpitSend.h"
#include "../UnkNamedSend_b54.h"
#include "base/global/CName.h"
#include "types.h"

struct UnkWeakTarget;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);

struct UnkSendOwner_b54 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkSendComponent_b54 {
    u8 pad_0x0[0x30];
    union {
        UnkSendOwner_b54* owner;
        UnkWeakTarget* weak_owner;
    };
};

static inline __attribute__((always_inline)) void SendNamed_b54(UnkSendComponent_b54* self, const char* name) {
    if (self->owner)
        if (void* target = self->owner->field_0x220)
            UnkSpitSend(target, base::global::CName(name).hash, 0xf4061ff25a2163d6);
}

void fn_7100880938(UnkSendComponent_b54* self) { SendNamed_b54(self, "start"); }
void fn_7100880a5c(UnkSendComponent_b54* self) { SendNamed_b54(self, "block"); }
void fn_7100880b80(UnkSendComponent_b54* self) { SendNamed_b54(self, "relax"); }
void fn_71007ddb9c(UnkSendComponent_b54* self) { SendNamed_b54(self, "open"); }

void fn_71007dda00(UnkSendComponent_b54* self) {
    if (void* target = fn_71002b3b84(self->weak_owner))
        fn_71009c0f1c(target, 0xb0c1699f8b57b14b);
    if (self->owner)
        if (void* target = self->owner->field_0x220)
            UnkSpitSend_974c(target, base::global::CName("open").hash, 0xf4061ff25a2163d6);
}
