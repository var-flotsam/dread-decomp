// CMagmaKraidScenarioControllerComponent (tools/tu_map.py unit 1403): functions over its two lazily
// resolved handles (the shape of CMagmaKraidScenarioControllerComponent_b45.cpp's fn_710091793c): the
// first at +0x78 (source +0x48, state +0x70), the second at +0xb8 (source +0x88, state +0xb0), whose
// object fn_7100917acc turns into the scenario's Kraid. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkCountedMagma_b47 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRef {
    UnkCountedMagma_b47* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

extern u64 g_unk_710164b210;

struct UnkMagmaKraid_b47;
UnkMagmaKraid_b47* fn_7100917acc(void* object);
void fn_7100918a78(UnkMagmaKraid_b47* kraid);
bool fn_7100918ef4(UnkMagmaKraid_b47* kraid);
bool fn_71009185d0(UnkMagmaKraid_b47* kraid);

struct UnkLazyHandle_b47 {
    u8 source[0x28];
    u32 state;
    u32 pad;
    base::UnkRefHandle handle;

    // Resolves the handle first; whether it names an object.
    __attribute__((always_inline)) bool Resolve() {
        if (state == 1) {
            {
                UnkRef fresh = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        auto* c = static_cast<UnkCountedMagma_b47*>(handle.ptr);
        return c && c->raw_0x10;
    }
    // The object (the original reloads it after the test).
    __attribute__((always_inline)) void* Object() {
        auto* c = static_cast<UnkCountedMagma_b47*>(handle.ptr);
        return *reinterpret_cast<void* volatile*>(&c->field_0x10);
    }
};


struct UnkMagmaKraidController_b47 {
    u8 pad_0x0[0x48];
    UnkLazyHandle_b47 first;   // 0x48
    u8 pad_0x80[8];
    UnkLazyHandle_b47 second;  // 0x88
};
static_assert(__builtin_offsetof(UnkMagmaKraidController_b47, second) == 0x88, "layout");

void fn_71009179cc(UnkMagmaKraidController_b47* self) {
    if (self->second.Resolve()) {
        if (UnkMagmaKraid_b47* kraid = fn_7100917acc(self->second.Object()))
            fn_7100918a78(kraid);
    }
    if (self->first.Resolve())
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->first.Object()), &g_unk_710164b210, false);
}

bool fn_7100917ba8(UnkMagmaKraidController_b47* self) {
    if (self->second.Resolve()) {
        if (UnkMagmaKraid_b47* kraid = fn_7100917acc(self->second.Object())) {
            if (fn_7100918ef4(kraid) || !fn_71009185d0(kraid))
                return false;
        }
    }
    return true;
}

bool fn_7100917c5c(UnkMagmaKraidController_b47* self) {
    if (self->second.Resolve()) {
        if (UnkMagmaKraid_b47* kraid = fn_7100917acc(self->second.Object())) {
            if (fn_71009185d0(kraid))
                return true;
        }
    }
    return false;
}
