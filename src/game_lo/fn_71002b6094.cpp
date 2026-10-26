// Destructor of an object holding a string id, four growable arrays and a refcounted
// pointer: releases them in reverse order of declaration.
#include "base/UnkDestructors.h"
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkVector.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

struct UnkStruct_71002b6094_Elem {
    u32 field_0x0;
    ~UnkStruct_71002b6094_Elem() { fn_71002874dc(this); }
};


struct UnkStruct_71002b6094 {
    u8 pad_0x0[0x38];
    base::UnkStrIdMember field_0x38;
    u8 pad_0x40[0x18];
    base::UnkVector<u32> field_0x58;
    base::UnkVector<UnkStruct_71002b6094_Elem> field_0x80;
    base::UnkVector<UnkStruct_71002b6094_Elem> field_0xa8;
    base::UnkVector<UnkStruct_71002b6094_Elem> field_0xd0;
    base::UnkRefPtrMember field_0xf8;
};

void fn_71002b6094(UnkObject_0x100* object) {
    auto* self = reinterpret_cast<UnkStruct_71002b6094*>(object);
    self->field_0xf8.base::UnkRefPtrMember::~UnkRefPtrMember();
    self->field_0xd0.Free();
    self->field_0xa8.Free();
    self->field_0x80.Free();
    self->field_0x58.Free();
    self->field_0x38.~UnkStrIdMemberOf();
}
