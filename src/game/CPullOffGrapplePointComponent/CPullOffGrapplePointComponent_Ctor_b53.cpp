// Constructor of a component in the CPullOffGrapplePointComponent unit: the base constructor
// fn_710082ca34, the vtable, the member at +0x90 (fn_710082eba4), an allocator-backed container at
// +0xc8 (base vtable, then its own), two cleared words, a listener at +0x110 (vtable and cleared
// words), then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "types.h"
#include "base/UnkVector.h"

extern void* g_unk_7101a76dc0[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a77000[];

void fn_710082ca34(char*);
void fn_710082eba4(char*);

struct UnkPullOffContainerBase_b53 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u8 field_0x1c;
    u32 field_0x20;
};

struct UnkPullOffContainer_b53 : UnkPullOffContainerBase_b53 {
    base::UnkAllocator* field_0x28;
    u32 field_0x30;
};

struct UnkPullOffListener_b53 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkPullOffGrapplePointComponent_b53 {
    void** vtable;
    u8 pad_0x8[0x88];
    u8 field_0x90[0x38];
    UnkPullOffContainer_b53 field_0xc8;
    u64 field_0x100;
    u64 field_0x108;
    UnkPullOffListener_b53 field_0x110;
};

void fn_710082edf8(UnkPullOffGrapplePointComponent_b53* self) {
    fn_710082ca34(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a76dc0 + 2;
    fn_710082eba4(reinterpret_cast<char*>(self->field_0x90));
    UnkPullOffContainer_b53& c = self->field_0xc8;
    c.vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    c.field_0x8 = 0;
    c.field_0x10 = 0;
    c.field_0x18 = 10;
    c.field_0x1c = 1;
    c.field_0x20 = 15;
    c.field_0x28 = fn_7100083990();
    c.field_0x30 = 0;
    c.vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    self->field_0x100 = 0;
    self->field_0x108 = 0;
    self->field_0x110.vtable = g_unk_7101a77000 + 2;
    self->field_0x110.field_0x8 = 0;
    self->field_0x110.field_0x10 = 0;
    self->field_0x110.field_0x18 = 0;
    reinterpret_cast<void (*)(UnkPullOffGrapplePointComponent_b53*)>(self->vtable[3])(self);
}
