// Constructor of a component: runs the base constructor, sets its vtable and builds a callback record
// at +0x2a6f8 (an empty string id and the component with fn_71004fd50c as the function).
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a45710[];

struct UnkComponent {
    void** vtable;
    u8 pad_0x8[0x2a6f0];
};

void fn_71004fd50c();

struct UnkCallback {
    u64 field_0x0;
    base::global::UnkStringData* field_0x8;
    u32 field_0x10;
    u64 field_0x18;
    UnkComponent* field_0x20;
    void (*field_0x28)();
    u32 field_0x30;

    __attribute__((always_inline)) explicit UnkCallback(UnkComponent* owner) : field_0x0(0), field_0x8(nullptr) {
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        field_0x8 = &g_unk_7101d47108->empty;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = owner;
        field_0x28 = fn_71004fd50c;
        field_0x30 = 0;
    }
};

struct UnkRecord : UnkComponent {
    UnkCallback field_0x2a6f8;
};

void fn_710053e5c0(UnkComponent* self);
void fn_71004fd50c();

void fn_71004faad4(UnkRecord* self) {
    fn_710053e5c0(self);
    self->vtable = g_unk_7101a45710 + 2;
    new (&self->field_0x2a6f8) UnkCallback(self);
}
