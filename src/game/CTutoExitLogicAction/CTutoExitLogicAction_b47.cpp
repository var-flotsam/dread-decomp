// CTutoExitLogicAction unit (tools/tu_map.py): after the base step (fn_7100a55ac8), switches the entity at
// +8 off, resolves the lazily resolved handle at +0x48 (source at +0x18; state at +0x40 is 1 until the
// handle takes its value and 2 after; the shape of CMagmaKraidScenarioControllerComponent_b45.cpp's
// fn_710091793c) and switches the entity the handle names off too. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkEntityState;

struct UnkCountedTuto_b47 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRef {
    UnkCountedTuto_b47* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkTutoExitAction_b47 {
    u8 pad_0x0[0x8];
    UnkEntityState* field_0x8;
    u8 pad_0x10[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x44;
    base::UnkRefHandle handle;
};

void fn_7100a597f4(UnkTutoExitAction_b47* self) {
    fn_7100a55ac8(self);
    fn_7100232864(self->field_0x8, false);
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedTuto_b47*>(self->handle.ptr);
    if (!c || !c->raw_0x10)
        return;
    // the original reloads the object after the test
    fn_7100232864(reinterpret_cast<UnkEntityState*>(*reinterpret_cast<void* volatile*>(&c->field_0x10)), false);
}
