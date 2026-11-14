// CCharClassShootActivatorHidrogigaComponent unit (tools/tu_map.py): resolves the lazily resolved handle at
// +0x5f8 (source at +0x5c8; state at +0x5f0 is 1 until the handle takes its value and 2 after; the shape of
// CMagmaKraidScenarioControllerComponent_b45.cpp's fn_710091793c) and stores `value` at +0x2f8 of what
// fn_7100897b0c finds for the object the handle names. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CCharClassShootActivatorHidrogigaComponent.h"
#include "base/UnkDestructors.h"
#include "game/UnkTU437.h"
#include "types.h"

struct UnkCountedHidrogiga_b47 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

using UnkRef = base::UnkRefPtrMember;

struct UnkHidrogigaTarget_b47 {
    u8 pad_0x0[0x2f8];
    bool field_0x2f8;
};
struct UnkHidrogigaPart_b44a2;  // CCharClassShootActivatorHidrogigaComponent_b44.cpp

struct UnkShootActivatorHidrogiga_b47 {
    u8 pad_0x0[0x5c8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x5f4;
    base::UnkRefHandle handle;
};

void fn_7100898498(UnkShootActivatorHidrogiga_b47* self, bool value) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedHidrogiga_b47*>(self->handle.ptr);
    if (!c || !c->raw_0x10)
        return;
    // the original reloads the object after the test
    if (auto* target = reinterpret_cast<UnkHidrogigaTarget_b47*>(
            fn_7100897b0c(*reinterpret_cast<void* volatile*>(&c->field_0x10))))
        target->field_0x2f8 = value;
}
