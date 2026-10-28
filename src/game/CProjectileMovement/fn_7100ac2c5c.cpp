// Constructor of a component (shared by 25 derived constructors): runs the base constructor,
// sets the vtable and default-initialises the members (empty ids, unit scales, -1 markers)
// and a callback bound to the object.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::math::CVector3D;

struct UnkSub_0x48 {
    u64 field_0x0;
    u64 field_0x8;
    s32 field_0x10;
    CVector3D field_0x14[4];
};

struct UnkComponent_0x5b8 {
    void** vtable;
    u8 unk_0x8[0x100];
    f32 field_0x108;
    u8 unk_0x10c[0x374];
    f32 field_0x480;
    f32 field_0x484;
    CFilePathStrId field_0x488;
    CFilePathStrId field_0x490;
    CFilePathStrId field_0x498;
    f32 field_0x4a0;
    f32 field_0x4a4;
    u64 field_0x4a8;
    u64 field_0x4b0;
    u64 field_0x4b8;
    u64 field_0x4c0;
    u64 field_0x4c8;
    u64 field_0x4d0;
    f32 field_0x4d8[4];
    bool field_0x4e8;
    u32 field_0x4ec[12];
    f32 field_0x51c;
    f32 field_0x520;
    u16 field_0x524;
    UnkSub_0x48 field_0x528;
    bool field_0x570;
    bool field_0x571;
    bool field_0x572;
    u32 field_0x574;
    u16 field_0x578;
    u32 field_0x57c;
    UnkStringData* field_0x580;  // CStrId
    UnkStringData* field_0x588;  // CFilePathStrId
    u64 field_0x590;
    UnkStringData* field_0x598;  // CStrId
    u64 field_0x5a0;
    void* field_0x5a8;
    void (*field_0x5b0)();
};
static_assert(sizeof(UnkComponent_0x5b8) == 0x5b8, "layout");

extern void* g_unk_7101a96b30[];
void fn_7100ac7224(UnkComponent_0x5b8* self);
void fn_7100ac6a44();

static inline void AddRef(UnkStringData* d, base::global::UnkStringPool* pool) {
    if (d != &pool->empty)
        ++*reinterpret_cast<s32*>(reinterpret_cast<char*>(d) + 8);
}

void fn_7100ac2c5c(UnkComponent_0x5b8* self) {
    fn_7100ac7224(self);
    self->vtable = &g_unk_7101a96b30[2];
    self->field_0x480 = -1.0f;
    self->field_0x484 = -1.0f;
    new (&self->field_0x488) CFilePathStrId("", true);
    new (&self->field_0x490) CFilePathStrId("", true);
    new (&self->field_0x498) CFilePathStrId("", true);
    self->field_0x4a0 = 0.0f;
    self->field_0x4a4 = 1.0f;
    self->field_0x4a8 = 0;
    self->field_0x4b0 = 0;
    self->field_0x4b8 = 0;
    self->field_0x4c0 = 0;
    self->field_0x4c8 = 0;
    self->field_0x4d0 = 0;
    self->field_0x4d8[0] = 1.0f;
    self->field_0x4d8[1] = 1.0f;
    self->field_0x4d8[2] = 1.0f;
    self->field_0x4d8[3] = 1.0f;
    self->field_0x4e8 = false;
    for (u32& v : self->field_0x4ec)
        v = 0;
    self->field_0x51c = 0.0f;
    self->field_0x520 = -1.0f;
    self->field_0x524 = 0;
    self->field_0x528.field_0x0 = 0;
    self->field_0x528.field_0x8 = 0;
    self->field_0x528.field_0x10 = -1;
    for (CVector3D& v : self->field_0x528.field_0x14) {
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
    }
    self->field_0x570 = true;
    self->field_0x571 = true;
    self->field_0x572 = false;
    self->field_0x574 = 0;
    self->field_0x578 = 0;
    self->field_0x57c = 0;
    UnkStringData* id = g_unk_7101d470e0.data;
    self->field_0x580 = id;
    AddRef(id, g_unk_7101d47108);
    UnkStringData* path = g_unk_7101d470e8.data;
    self->field_0x588 = path;
    AddRef(path, g_unk_7101d47200);
    self->field_0x590 = 0;
    UnkStringData* id2 = g_unk_7101d470e0.data;
    self->field_0x598 = id2;
    AddRef(id2, g_unk_7101d47108);
    self->field_0x5a0 = 0;
    self->field_0x5a8 = self;
    self->field_0x5b0 = &fn_7100ac6a44;
    self->field_0x108 = -1.0f;
}
