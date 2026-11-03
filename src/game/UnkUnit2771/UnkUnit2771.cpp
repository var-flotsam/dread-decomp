// A constructor in unit 2771 (near copy of fn_71006eede8): the base constructor fn_7101206894, the
// vtable (0x7101cb7200), the members at +0x2a0 and +0x2e8 (fn_71012106c8, fn_7101235040), two
// cleared fields, then a recursive mutex at +0x5f0. Names and layouts are placeholders.
#include <nn/os.h>
#include "types.h"

void fn_7101206894(void* self);
void fn_71012106c8(void* self);
void fn_7101235040(void* self);
extern char g_unk_7101cb7200[];

struct UnkUnit2771Obj {
    void** vtable;
    u8 pad_0x8[0x298];
    u8 field_0x2a0[0x48];
    u8 field_0x2e8[0x80];
    u16 field_0x368;
    u8 pad_0x36a[0x2];
    u32 field_0x36c;
    u8 pad_0x370[0x280];
    nn::os::MutexType field_0x5f0;
};

void fn_71012046c0(UnkUnit2771Obj* self) {
    fn_7101206894(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101cb7200) + 2;
    fn_71012106c8(self->field_0x2a0);
    fn_7101235040(self->field_0x2e8);
    self->field_0x368 = 0;
    self->field_0x36c = 0;
    nn::os::InitializeMutex(&self->field_0x5f0, true, 0);
}
