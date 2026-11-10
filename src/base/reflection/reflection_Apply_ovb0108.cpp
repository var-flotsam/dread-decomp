// Unit 271 of tools/tu_map.py (base::reflection names): hands an object (unless its vtable slot
// 0x48 says 1) this record's target (+0x20, through slot 0x20) and then its three members (+0x8,
// +0x10, +0x18), its two flags (+0x158, +0x159) and the caller's arguments through slot 0x28, then
// releases the members at +0x18 (fn_71001ea5a8) and +0x10 (fn_71001ebe58). The vector is passed by
// value (its copy is written out). Names and layouts are placeholders.
#include "types.h"

struct UnkApplyVec_ovb0108 {
    f32 x, y, z;
    UnkApplyVec_ovb0108(const UnkApplyVec_ovb0108& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkApplyRecord_ovb0108 {
    u8 pad_0x0[0x8];
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    void* target;  // 0x20
    u8 pad_0x28[0x130];
    bool field_0x158;
    bool field_0x159;
};

struct UnkApplyObject_ovb0108 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(void* target);
    virtual void vf_0x28(void* a, void* b, void* c, void* d, void* e, bool f, bool g, bool h, bool i,
                         UnkApplyVec_ovb0108 v, f32 t);
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual s32 vf_0x48();
};

void fn_71001ea5a8(void* p);
void fn_71001ebe58(void* p);

void fn_71001ef898(UnkApplyRecord_ovb0108* self, UnkApplyObject_ovb0108* obj, void* a, void* b, bool c, bool d,
                   const UnkApplyVec_ovb0108* v, f32 t) {
    if (!obj || !self->target)
        return;
    if (obj->vf_0x48() == 1)
        return;
    obj->vf_0x20(self->target);
    obj->vf_0x28(self->field_0x8, self->field_0x10, self->field_0x18, a, b, self->field_0x158, self->field_0x159, c, d,
                 *v, t);
    if (self->field_0x18)
        fn_71001ea5a8(self->field_0x18);
    if (self->field_0x10)
        fn_71001ebe58(self->field_0x10);
}
