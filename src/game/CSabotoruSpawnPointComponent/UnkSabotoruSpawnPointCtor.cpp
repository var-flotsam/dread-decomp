// Constructor of a component in the CSabotoruSpawnPointComponent unit: the spawn point base
// constructor fn_71011258e4, the vtable, two containers at +0x3b8 and +0x3f8 (base vtable, empty
// array with step 10 and flags 0xf, allocator, then their own final vtables), then the
// post-construction virtual call (slot 3). The container is laid out as in
// src/game/CAutsharpSpawnPointComponent/CAutsharpSpawnPointComponent.cpp. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU2704.h"
#include "types.h"

extern void* g_unk_7101a8cc00[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a20950[];

// The containers' fields are written out flat: as sub-objects, Clang addresses one byte store
// from the container's base instead of the component's.
struct UnkSabotoruSpawnPointComponent {
    void** vtable;
    u8 pad_0x8[0x3b0];
    void** field_0x3b8;
    u64 field_0x3c0;
    u64 field_0x3c8;
    u32 field_0x3d0;
    bool field_0x3d4;
    u32 field_0x3d8;
    base::UnkAllocator* field_0x3e0;
    u32 field_0x3e8;
    u64 field_0x3f0;
    void** field_0x3f8;
    u64 field_0x400;
    u64 field_0x408;
    u32 field_0x410;
    bool field_0x414;
    u32 field_0x418;
    base::UnkAllocator* field_0x420;
    u32 field_0x428;
    u64 field_0x430;
};

void fn_7100a1c52c(UnkSabotoruSpawnPointComponent* self) {
    fn_71011258e4(self);
    self->vtable = g_unk_7101a8cc00 + 2;
    self->field_0x3b8 = g_unk_7101a20998 + 2;
    self->field_0x3c0 = 0;
    self->field_0x3c8 = 0;
    self->field_0x3d0 = 10;
    self->field_0x3d4 = true;
    self->field_0x3d8 = 0xf;
    self->field_0x3e0 = fn_7100083990();
    self->field_0x3e8 = 0;
    self->field_0x3b8 = g_unk_7101a33fa0 + 2;
    self->field_0x3f0 = 0;
    self->field_0x3f8 = g_unk_7101a20998 + 2;
    self->field_0x400 = 0;
    self->field_0x408 = 0;
    self->field_0x410 = 10;
    self->field_0x414 = true;
    self->field_0x418 = 0xf;
    self->field_0x420 = fn_7100083990();
    self->field_0x428 = 0;
    self->field_0x3f8 = g_unk_7101a20950 + 2;
    self->field_0x430 = 0;
    reinterpret_cast<void (*)(UnkSabotoruSpawnPointComponent*)>(self->vtable[3])(self);
}
