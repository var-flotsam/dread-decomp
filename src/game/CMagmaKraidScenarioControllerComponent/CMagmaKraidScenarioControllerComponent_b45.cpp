// CMagmaKraidScenarioControllerComponent (tools/tu_map.py unit 1403; the name from reflection):
// resolves the lazily resolved handle at +0x48 (source; state at +0x70 is 1 until the handle at
// +0x78 takes its value from the source and 2 after; the shape of fn_7100897458), then, when the
// handle names an object, passes it to fn_7100d7c828 with a key and false. Names and layout are
// placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkTU437.h"
#include "game/UnkTimelineComponent.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkCounted_b45a2 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

using UnkRef = base::UnkRefPtrMember;

extern u64 g_unk_710164b208;

struct UnkMagmaKraidController_b45a2 {
    u8 pad_0x0[0x48];
    u8 source[0x28];
    u32 state;
    u32 pad_0x74;
    base::UnkRefHandle handle;
};

// The object the handle names (null when it names none); the handle takes its value from the
// source on first use.
static inline UnkActor* GetActor(UnkMagmaKraidController_b45a2* self) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCounted_b45a2*>(self->handle.ptr);
    return c ? static_cast<UnkActor*>(c->field_0x10) : nullptr;
}

void fn_710091793c(UnkMagmaKraidController_b45a2* self) {
    if (!GetActor(self))
        return;
    fn_7100d7c828(GetActor(self), &g_unk_710164b208, false);
}
