// Unit 2702: an AI component's event handler: events of kind 1 and 2 clear a stored pair of floats while a flag
// is set; an event of kind 1 named with one id drops the weak handle, any other one tells a push track
// to act. Always reports handled. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

struct UnkEventId_l111744 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

// A string id: its data pointer.
struct UnkEventName_l111744 {
    UnkEventId_l111744* data;
};

struct UnkEventObject_l111744 {
    u8 pad_0x0[0x38];
    UnkEventName_l111744 name;  // 0x38
};

struct UnkEventPayload_l111744 {
    u8 pad_0x0[0x10];
    UnkEventObject_l111744* object;  // 0x10
};

struct UnkEvent_l111744 {
    u32 kind;
    u8 pad_0x4[4];
    UnkEventPayload_l111744* payload;  // 0x8
};

struct UnkEventConfig_l111744 {
    u8 pad_0x0[0x41a];
    bool field_0x41a;
};

struct UnkEventOwner_l111744 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* track;  // 0x220
};

struct UnkEventAI_l111744;
struct UnkEventVtable_l111744 {
    void* pad_0x0[0x83];
    void (*vf_0x418)(UnkEventAI_l111744* self);
    void* pad_0x420[0x6e];
    void (*vf_0x788)(UnkEventAI_l111744* self);
};

struct UnkEventAI_l111744 {
    UnkEventVtable_l111744* vtable;
    u8 pad_0x8[0x28];
    UnkEventOwner_l111744* owner;  // 0x30
    u8 pad_0x38[0x15d8];
    UnkEventConfig_l111744* config;  // 0x1610
    u8 pad_0x1618[0x12e80];
    u8 field_0x14498[0x18];
    UnkWeakHandle* handle;  // 0x144b0
    u8 pad_0x144b8[0x48];
    u32 field_0x14500;
    u32 field_0x14504;
};
static_assert(__builtin_offsetof(UnkEventAI_l111744, handle) == 0x144b0, "layout");

bool fn_7101111744(UnkEventAI_l111744* self, UnkEvent_l111744* event) {
    const UnkEventName_l111744* name = reinterpret_cast<const UnkEventName_l111744*>(&g_unk_7101d470e0);
    if (UnkEventPayload_l111744* payload = event->payload) {
        UnkEventObject_l111744* object = payload->object;
        name = object ? &object->name : name;
    }
    bool clear;
    if (self->config->field_0x41a) {
        clear = false;
        if (event->kind - 1 <= 1) {
            self->field_0x14500 = 0;
            self->field_0x14504 = 0xbf800000;
        }
    } else {
        clear = true;
    }
    if (event->kind == 1) {
        if (name->data->key == 0x3951592af38cb532ull) {
            self->vtable->vf_0x418(self);
            if (clear) {
                if (UnkWeakHandle* handle = self->handle) {
                    if (UnkWeakTarget* target = handle->target)
                        fn_710117b3f0(target, self->field_0x14498);
                    ResetRef(&self->handle);
                }
            }
        } else if (UnkAnimationComponent* track = self->owner->track) {
            if (fn_710117b348(track, fn_7100d253f8()))
                self->vtable->vf_0x788(self);
        }
    }
    return true;
}
