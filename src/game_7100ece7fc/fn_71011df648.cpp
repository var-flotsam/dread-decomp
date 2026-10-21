// Constructor of a GUI object: runs the base constructor (fn_71011d8d24), sets the vtable and
// default-initialises the rest, including two arrays, ten callbacks bound to itself and two
// string ids copied from the global defaults.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::UnkStringData;

// A callback bound to an object: {unused, object, function}.
struct UnkBoundCall {
    void* field_0x0;
    void* object;
    void (*fn)();
};

struct UnkGuiObject_71011df648 {
    void** vtable;
    u8 unk_0x8[0x228];
    u32 field_0x230;
    u16 field_0x234;
    u8 unk_0x236;
    bool field_0x237;
    s32 field_0x238;
    u8 unk_0x23c[0x4];
    UnkBoundCall field_0x240;
    bool field_0x258;
    void* field_0x260[18];
    base::UnkVector<void*> field_0x2f0;
    void* field_0x318;
    void* field_0x320;
    void* field_0x328;
    void* field_0x330;
    s64 field_0x338;
    base::UnkVector<void*> field_0x340;
    UnkBoundCall field_0x368;
    UnkBoundCall field_0x380;
    void* field_0x398[9];
    UnkBoundCall field_0x3e0;
    UnkBoundCall field_0x3f8;
    UnkBoundCall field_0x410;
    UnkBoundCall field_0x428;
    UnkBoundCall field_0x440;
    UnkBoundCall field_0x458;
    UnkBoundCall field_0x470;
    UnkBoundCall field_0x488;
    u32 field_0x4a0;
    bool field_0x4a4;
    UnkStringData* field_0x4a8;  // CStrId
    UnkStringData* field_0x4b0;  // CFilePathStrId
    void* field_0x4b8;
};

extern void* g_unk_7101cb6040[];
void fn_71011d8d24(UnkGuiObject_71011df648* self);
void fn_71011e7f14();
void fn_71011e7bf8();
void fn_71011e7c10();
void fn_71011e7c18();
void fn_71011e7cbc();
void fn_71011e7d60();
void fn_71011e7e10();
void fn_71011e7e2c();
void fn_71011e7e48();
void fn_71011e7e8c();
void fn_71011e7ed8();

static inline void Bind(UnkBoundCall* call, void* object, void (*fn)()) {
    call->field_0x0 = nullptr;
    call->object = object;
    call->fn = fn;
}

void fn_71011df648(UnkGuiObject_71011df648* self) {
    fn_71011d8d24(self);
    self->vtable = &g_unk_7101cb6040[2];
    self->field_0x230 = 0x10000;
    self->field_0x234 = 0;
    self->field_0x237 = false;
    self->field_0x238 = -1;
    Bind(&self->field_0x240, self, fn_71011e7f14);
    self->field_0x258 = false;
    for (s32 i = 0; i < 18; ++i)
        self->field_0x260[i] = nullptr;
    new (&self->field_0x2f0) base::UnkVector<void*>(0xf);
    self->field_0x318 = nullptr;
    self->field_0x320 = nullptr;
    self->field_0x328 = nullptr;
    self->field_0x330 = nullptr;
    self->field_0x338 = -1;
    new (&self->field_0x340) base::UnkVector<void*>(0xf);
    Bind(&self->field_0x368, self, fn_71011e7bf8);
    Bind(&self->field_0x380, self, fn_71011e7c10);
    for (s32 i = 0; i < 9; ++i)
        self->field_0x398[i] = nullptr;
    Bind(&self->field_0x3e0, self, fn_71011e7c18);
    Bind(&self->field_0x3f8, self, fn_71011e7cbc);
    Bind(&self->field_0x410, self, fn_71011e7d60);
    Bind(&self->field_0x428, self, fn_71011e7e10);
    Bind(&self->field_0x440, self, fn_71011e7e2c);
    Bind(&self->field_0x458, self, fn_71011e7e48);
    Bind(&self->field_0x470, self, fn_71011e7e8c);
    Bind(&self->field_0x488, self, fn_71011e7ed8);
    self->field_0x4a0 = 0;
    self->field_0x4a4 = false;
    UnkStringData* id = g_unk_7101d470e0.data;
    self->field_0x4a8 = id;
    if (&g_unk_7101d47108->empty != id)
        ++id->ref_count;
    UnkStringData* path = g_unk_7101d470e8.data;
    self->field_0x4b0 = path;
    if (&g_unk_7101d47200->empty != path)
        ++path->ref_count;
    self->field_0x4b8 = nullptr;
}
