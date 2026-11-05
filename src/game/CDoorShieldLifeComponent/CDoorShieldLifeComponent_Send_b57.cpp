// CDoorShieldLifeComponent's unit (tools/tu_map.py): fn_71008debb0 calls fn_710024fa0c(0) on the
// owner's (+0x30) object at +0x2a0, if any; sets the delay at +0x404 to 0.5 and the time at +0x400
// from the tunable (+0x488, +0x120); when the owner's sub-object at +0x220 has an entry under the
// request's hash (fn_7101176d34), the delay becomes that entry's length (+0x28) minus the time, at
// least 0.1, and the request is sent with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h); then
// gives the owner's weak target (fn_71002b3b84) a second hash (fn_71009c0f1c) and, when the time is
// not positive, calls fn_71008de71c with the delay. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);

struct UnkStruct_7101176ea8;
void* fn_7101176d34(UnkStruct_7101176ea8* self, void* arg);

struct UnkWeakTarget;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);

struct UnkDoorShieldOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2a0 - 0x228];
    UnkStruct_710024fa0c* field_0x2a0;
};

struct UnkDoorShieldTunable_b57 {
    u8 pad_0x0[0x120];
    f32 field_0x120;
};

struct UnkDoorShieldEntry_b57 {
    u8 pad_0x0[0x28];
    f32 length;
};

struct UnkDoorShieldLife_b57 {
    u8 pad_0x0[0x30];
    UnkDoorShieldOwner_b57* owner;
    u8 pad_0x38[0x400 - 0x38];
    f32 field_0x400;
    f32 field_0x404;
    u8 pad_0x408[0x488 - 0x408];
    UnkDoorShieldTunable_b57* field_0x488;
};

void fn_71008de71c(UnkDoorShieldLife_b57* self, f32 delay);

void fn_71008debb0(UnkDoorShieldLife_b57* self) {
    if (UnkStruct_710024fa0c* part = self->owner->field_0x2a0)
        fn_710024fa0c(part, false);
    self->field_0x404 = 0.5f;
    self->field_0x400 = self->field_0x488->field_0x120;
    if (void* target = self->owner->field_0x220) {
        const u64 hash = 0x7531238cb72381d8;
        if (UnkDoorShieldEntry_b57* entry = static_cast<UnkDoorShieldEntry_b57*>(fn_7101176d34(
                static_cast<UnkStruct_7101176ea8*>(target), reinterpret_cast<void*>(hash)))) {
            f32 delay = entry->length - self->field_0x400;
            if (!(delay > 0.1f))
                delay = 0.1f;
            self->field_0x404 = delay;
            UnkSpitSend(target, hash, 0xf4061ff25a2163d6);
        }
    }
    if (void* weak = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(weak, 0xa6a4b8f65b760be7);
    if (self->field_0x400 <= 0.0f)
        fn_71008de71c(self, self->field_0x404);
}
