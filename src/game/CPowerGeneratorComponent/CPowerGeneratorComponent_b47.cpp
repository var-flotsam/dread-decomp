// CPowerGeneratorComponent unit (tools/tu_map.py): unless the flag at +0x138 is set, resolves the lazily
// resolved handle at +0xf0 (source at +0xc0; state at +0xe8 is 1 until the handle takes its value and 2
// after; the shape of CMagmaKraidScenarioControllerComponent_b45.cpp's fn_710091793c) and clears the
// byte at +0x568 of what fn_7100a7eba0 finds for the object the handle names. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkDestructors.h"
#include "game/UnkTU437.h"
#include "types.h"

struct UnkCountedPowerGen_b47 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

using UnkRef = base::UnkRefPtrMember;

struct UnkPowerGenTarget_b47 {
    u8 pad_0x0[0x568];
    bool field_0x568;
};
UnkPowerGenTarget_b47* fn_7100a7eba0(void* object);

struct UnkPowerGenerator_b47 {
    u8 pad_0x0[0xc0];
    u8 source[0x28];
    u32 state;
    u32 pad_0xec;
    base::UnkRefHandle handle;
    u8 pad_0xf8[0x138 - 0xf8];
    bool field_0x138;
};

void fn_7100a7eb00(UnkPowerGenerator_b47* self) {
    if (self->field_0x138)
        return;
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedPowerGen_b47*>(self->handle.ptr);
    if (!c || !c->raw_0x10)
        return;
    // the original reloads the object after the test
    if (UnkPowerGenTarget_b47* target = fn_7100a7eba0(*reinterpret_cast<void* volatile*>(&c->field_0x10)))
        target->field_0x568 = false;
}

// The same resolution, then stores `value` at +0x568 of what fn_7100a7eba0 finds.
void fn_7100a7f0c4(UnkPowerGenerator_b47* self, bool value) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedPowerGen_b47*>(self->handle.ptr);
    if (!c || !c->raw_0x10)
        return;
    if (UnkPowerGenTarget_b47* target = fn_7100a7eba0(*reinterpret_cast<void* volatile*>(&c->field_0x10)))
        target->field_0x568 = value;
}
