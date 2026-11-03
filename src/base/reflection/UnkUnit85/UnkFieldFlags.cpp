// From unit 85 of tools/tu_map.py (see UnkFieldPointee.cpp): a test on a reflected field
// (fn_71000e7354). True when the word at +0x28 is set, the field has a fixed offset, or its
// accessor (+0x18) answers true through vf_0x38; otherwise, when the field's type (+0x10) is a
// pointer type (base::reflection::CPointerType, fn_710009bedc), whether bit 1 of the flags of the
// type's descriptor (+0xe8, its +0x20) is clear for a field that has an offset or an accessor.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CType;

struct UnkTypeDesc_71000e7354 {
    u8 unk_0x0[0x20];
    u8 flags;
};

struct UnkFieldType_71000e7354 {
    virtual const CType* vf_0x0() const;
    u8 unk_0x8[0xe0];
    UnkTypeDesc_71000e7354* desc;  // +0xe8
};

struct UnkFieldAccessor_71000e7354 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual bool vf_0x38();
};

struct UnkFieldInfo_71000e7354 {
    void* vtable;
    u32 offset;  // +0x8: 0xffffffff when the field has an accessor
    u8 pad_0xc[4];
    UnkFieldType_71000e7354* type;           // +0x10
    UnkFieldAccessor_71000e7354* accessor;  // +0x18
    u8 pad_0x20[8];
    void* field_0x28;

    bool HasStorage() const { return accessor || offset != 0xffffffff; }
};

bool fn_71000e7354(const UnkFieldInfo_71000e7354* self) {
    if (self->field_0x28 || self->offset != 0xffffffff)
        return true;
    if (self->accessor && self->accessor->vf_0x38())
        return true;
    UnkFieldType_71000e7354* type = self->type;
    if (type && fn_710009d50c(type->vf_0x0(), fn_710009bedc()) && self->HasStorage())
        return (type->desc->flags & 2) == 0;
    return false;
}
