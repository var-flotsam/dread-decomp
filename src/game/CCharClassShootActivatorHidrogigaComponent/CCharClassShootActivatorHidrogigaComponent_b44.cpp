// CCharClassShootActivatorHidrogigaComponent (tools/tu_map.py unit 1347; the name from
// reflection): resolves the lazily resolved handle at +0x5c8 (a 0x28-byte source, a state that is
// 1 until the handle takes its value from the source and 2 after, and the handle; the layout of
// src/game/CKraidAIComponent/CKraidAIComponent_b40.cpp), then, when the handle names an object,
// passes that object's component (fn_7100897b0c) to fn_71008a1598 with true. Names and layout are
// placeholders.
#include "base/UnkRefHandle.h"
#include "game/CCharClassShootActivatorHidrogigaComponent.h"
#include "types.h"

// The handle's target (base::UnkRefCounted's layout); the object is at +0x10 (tested as a word and
// reloaded as a pointer, as in the Kraid file).
struct UnkCounted_b44a2 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

// A counted reference; the destructor decrements and tests in one step.
struct UnkRef {
    UnkCounted_b44a2* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkHidrogigaPart_b44a2;
void fn_71008a1598(UnkHidrogigaPart_b44a2* part, bool on);

struct UnkHidrogigaActivator_b44a2 {
    u8 pad_0x0[0x5c8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x5f4;
    base::UnkRefHandle handle;
};

void fn_7100897458(UnkHidrogigaActivator_b44a2* self) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCounted_b44a2*>(self->handle.ptr);
    if (!c || !c->raw_0x10)
        return;
    asm("" ::: "memory");  // the original reloads the object after the test
    if (UnkHidrogigaPart_b44a2* part = fn_7100897b0c(c->field_0x10))
        fn_71008a1598(part, true);
}
