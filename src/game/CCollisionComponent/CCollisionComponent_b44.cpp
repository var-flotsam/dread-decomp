// CCollisionComponent (tools/tu_map.py; the name from reflection): the first collider whose shape
// is enabled (+0x6a) and whose layer mask (vf_0x58) meets `mask`; an entry flagged at +0x18 is
// handed to vf_0x170 first. Null when none. Names and layout are placeholders.
#include "types.h"

struct UnkColliderShape_b44a2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
    u8 pad_0x8[0x62];
    bool field_0x6a;
};

struct UnkColliderEntry_b44a2 {
    u8 pad_0x0[0x18];
    bool field_0x18;
    u8 pad_0x19[0xf];
    UnkColliderShape_b44a2* shape;
};
static_assert(sizeof(UnkColliderEntry_b44a2) == 0x30, "entry");

struct UnkCollisionComponent_b44a2 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkColliderEntry_b44a2* entries;
    u32 count;
};

UnkColliderShape_b44a2* fn_71008642c4(UnkCollisionComponent_b44a2* self, u32 mask) {
    for (u64 i = 0; i < self->count; ++i) {
        UnkColliderEntry_b44a2* e = &self->entries[i];
        if (e->shape->field_0x6a && (e->shape->vf_0x58() & mask)) {
            if (e->field_0x18)
                reinterpret_cast<void (*)(UnkCollisionComponent_b44a2*, UnkColliderEntry_b44a2*, u32)>(
                    self->vtable[0x170 / 8])(self, e, 0);
            return e->shape;
        }
    }
    return nullptr;
}
