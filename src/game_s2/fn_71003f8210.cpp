// When the component hasn't started yet (the flags at 0x1471a, 0x1471b and 0x1471c all clear), marks
// it started and registers its id with the global manager (created on first use), then calls the
// target's slot-0x178 callback unless that is disabled (0x1472c). Finally unregisters the id from the
// manager and releases the counted reference at 0x146d0.
#include "game/UnkManager_71726af468.h"
#include "types.h"

struct UnkTarget {
    void** vtable;
    void vf_0x178(void* id) { reinterpret_cast<void (*)(UnkTarget*, void*)>(vtable[47])(this, id); }
};

struct UnkCounted {
    void** vtable;
    u32 count;
    void vf_0x10() { reinterpret_cast<void (*)(UnkCounted*)>(vtable[2])(this); }
};

struct UnkState {
    UnkCounted* field_0x0;
    u8 pad_0x8[0x18];
    UnkTarget* field_0x20;
    u8 pad_0x28[0x22];
    u8 field_0x4a;
    u8 field_0x4b;
    u8 field_0x4c;
    u8 pad_0x4d[0xf];
    u8 field_0x5c;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x14698];
    UnkState state;
};

void* fn_71000839a0(u32 size, u32 align_mask);
void fn_71003fa71c(void* registry, void* id);
void fn_71003f9388(void* registry, void* id, u32 flag);
void fn_7100a1782c(UnkTarget* target, u32 flag);

void fn_71003f8210(UnkSelf* self) {
    UnkState* state = &self->state;
    if (!state->field_0x4a && !state->field_0x4b && !state->field_0x4c) {
        state->field_0x4c = 1;
        UnkManager_71726af468* manager = g_unk_71726af468;
        if (!manager) {
            manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
            fn_7100da7e2c(manager);
            g_unk_71726af468 = manager;
        }
        fn_71003fa71c(manager->field_0x330, self->field_0x30);
        if (state->field_0x20)
            fn_7100a1782c(state->field_0x20, 2);
    }
    if (!state->field_0x5c && state->field_0x20)
        state->field_0x20->vf_0x178(self->field_0x30);
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_71003f9388(manager->field_0x330, self->field_0x30, 0);
    if (UnkCounted* ref = state->field_0x0) {
        if (--ref->count == 0)
            ref->vf_0x10();
        state->field_0x0 = nullptr;
    }
}
