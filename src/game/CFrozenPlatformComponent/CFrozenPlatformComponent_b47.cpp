// CFrozenPlatformComponent unit (tools/tu_map.py): resolves the handle at +0x90 (source +0x60, state +0x88)
// and switches the entity it names to the opposite of `on`, then the owner's entity (+0x30) to `on`;
// returns false. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "types.h"

struct UnkCounted_FrozenB47 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRef {
    UnkCounted_FrozenB47* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

// A lazily resolved handle (the shape of CMagmaKraidScenarioControllerComponent_b45.cpp's fn_710091793c):
// the state is 1 until the handle takes its value from the source and 2 after.
struct UnkLazyHandle_FrozenB47 {
    u8 source[0x28];
    u32 state;
    u32 pad;
    base::UnkRefHandle handle;

    __attribute__((always_inline)) bool Resolve() {
        if (state == 1) {
            {
                UnkRef fresh = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        auto* c = static_cast<UnkCounted_FrozenB47*>(handle.ptr);
        return c && c->raw_0x10;
    }
    // The object (the original reloads it after the test).
    __attribute__((always_inline)) void* Object() {
        auto* c = static_cast<UnkCounted_FrozenB47*>(handle.ptr);
        return *reinterpret_cast<void* volatile*>(&c->field_0x10);
    }
};

struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);

struct UnkFrozenPlatform_b47 {
    u8 pad_0x0[0x30];
    UnkEntityState* field_0x30;
    u8 pad_0x38[0x60 - 0x38];
    UnkLazyHandle_FrozenB47 link;  // 0x60
};

bool fn_71009d5e84(UnkFrozenPlatform_b47* self, bool on) {
    if (self->link.Resolve())
        fn_7100232864(static_cast<UnkEntityState*>(self->link.Object()), !on);
    fn_7100232864(self->field_0x30, on);
    return false;
}
