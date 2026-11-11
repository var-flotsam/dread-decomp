// CAreaFXComponent unit (tools/tu_map.py unit 1263): the base area component's constructor
// (fn_71007fe1e4: vtable g_unk_7101a74498, a listener with two vtables at +0x3d0 pointing back at the
// object, a default string id at +0x408 and an empty reference list at +0x438 with flags 0xf) and its
// destructor (fn_710080260c: the list's references dropped and the list freed, the id released, the
// reference at +0x3b0 dropped, then the base destructor fn_71007f8d80). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CFrozenComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern void* g_unk_7101a74498[];

struct UnkRefObj_ovb0081 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
};

// An owning reference: drops the count on destruction and destroys the object at zero.
struct UnkRefPtr_ovb0081 {
    UnkRefObj_ovb0081* ptr;
    ~UnkRefPtr_ovb0081() {
        if (ptr) {
            u32* count = &ptr->ref_count;
            if (--*count == 0)
                ptr->vf_0x10();
        }
    }
};

struct UnkAreaFXBase_ovb0081 {
    void** vtable;
    u8 pad_0x8[0x3a8];
    UnkRefObj_ovb0081* field_0x3b0;
    u8 pad_0x3b8[0x50];
    u8 field_0x408[0x8];  // a CStrId
    u8 pad_0x410[0x28];
    base::UnkVector<UnkRefPtr_ovb0081> refs;  // 0x438
};
static_assert(__builtin_offsetof(UnkAreaFXBase_ovb0081, refs) == 0x438, "UnkAreaFXBase_ovb0081 layout");

void fn_71007f8d80(UnkAreaFXBase_ovb0081* self);

void fn_710080260c(UnkAreaFXBase_ovb0081* self) {
    self->vtable = g_unk_7101a74498 + 2;
    base::UnkVector<UnkRefPtr_ovb0081>& refs = self->refs;
    if (refs.data) {
        for (u64 i = 0; i < refs.count; ++i)
            refs.data[i].~UnkRefPtr_ovb0081();
        refs.count = 0;
        refs.allocator->vf_0x48(refs.data);
        refs.capacity = 0;
    }
    reinterpret_cast<CStrId*>(self->field_0x408)->Reset();
    if (UnkRefObj_ovb0081* ref = self->field_0x3b0) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
    fn_71007f8d80(self);
}


extern void* g_unk_7101a746b0[];

struct UnkAreaFXCtor_ovb0081 {
    void** vtable;
    u8 pad_0x8[0x3a8];
    void* field_0x3b0;
    void* field_0x3b8;
    void* field_0x3c0;
    bool field_0x3c8;
    u8 pad_0x3c9[0x7];
    void** listener_vtable;   // 0x3d0
    void** listener_vtable2;  // 0x3d8
    UnkAreaFXCtor_ovb0081* listener_owner;  // 0x3e0
    void* field_0x3e8;
    u32 field_0x3f0;
    u8 pad_0x3f4[0x14];
    UnkStringData* field_0x408;  // a CStrId
    u8 pad_0x410[0x8];
    void* field_0x418;
    u32 field_0x420;
    void* field_0x428;
    u32 field_0x430;
    base::UnkVector<void*> refs;  // 0x438
};
static_assert(__builtin_offsetof(UnkAreaFXCtor_ovb0081, refs) == 0x438, "UnkAreaFXCtor_ovb0081 layout");

void fn_71007fe1e4(UnkAreaFXCtor_ovb0081* self) {
    fn_71007f8bc0(self);
    self->vtable = g_unk_7101a74498 + 2;
    self->field_0x3b0 = nullptr;
    self->field_0x3b8 = nullptr;
    self->field_0x3c0 = nullptr;
    self->field_0x3c8 = false;
    self->listener_vtable = g_unk_7101a746b0 + 2;
    self->listener_vtable2 = g_unk_7101a746b0 + 8;
    self->listener_owner = self;
    self->field_0x3e8 = nullptr;
    self->field_0x3f0 = 0;
    self->field_0x408 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x408 = &g_unk_7101d47108->empty;
    self->field_0x418 = nullptr;
    self->field_0x420 = 0;
    self->field_0x428 = nullptr;
    self->field_0x430 = 0;
    new (&self->refs) base::UnkVector<void*>(0xf);
}
