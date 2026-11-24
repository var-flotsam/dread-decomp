// Unit 82 (base::reflection): walks a reflected object for a visitor. When the visitor walks
// classes, each field it accepts is handed to the field type's visit slot with the field's value,
// then every function of the class is reported; the object itself is reported last.
// Names and layouts are placeholders.
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "base/UnkCloneHelpers.h"
#include "game/UnkTU85.h"
#include "game/UnkTunableReflValue.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct UnkReflFieldInfo;
struct UnkFieldOwner;

bool fn_71000e74bc(CVariable* field);  // whether a field without an accessor can be set
bool fn_71000e750c(UnkTunableFieldHandle* field);  // whether the field can be read
UnkReflTypedPtr GetReflectedFieldTypedPtr(const UnkReflFieldInfo* field, UnkFieldOwner* owner);

struct UnkObjectVisitor_l0a23f0;

// A field's type as the walk sees it: its class (slot 0) and its visit slot.
struct UnkVisitedType_l0a23f0 {
    virtual const CClass* GetClass() const;
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void Visit(UnkTypedRef* value, UnkObjectVisitor_l0a23f0* visitor);
};

// A reflected field (0xb0 bytes).
struct UnkVisitedField_l0a23f0 {
    u8 pad_0x0[0x10];
    UnkVisitedType_l0a23f0* type;  // +0x10
    void* accessor;                // +0x18
    u8 pad_0x20[0x90];
};
static_assert(sizeof(UnkVisitedField_l0a23f0) == 0xb0, "");

// A reflected function (0xb8 bytes).
struct UnkVisitedFunction_l0a23f0 {
    u8 pad_0x0[0xb8];
};

struct UnkObjectVisitor_l0a23f0 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual s32 WalksClasses();
    virtual void VisitObject(const UnkReflTypedPtr* obj);
    virtual s32 AcceptsField(UnkVisitedField_l0a23f0* field);
    virtual void EndField(UnkVisitedField_l0a23f0* field);
    virtual void VisitFunction(UnkVisitedFunction_l0a23f0* function);

    u32 field_0x8;  // nonzero: only fields of pointer type are read through values
};

static inline const CClass* ClassOf(const UnkReflTypedPtr* obj) {
    auto* object = reinterpret_cast<UnkReflObject*>(obj->type);
    return object && fn_710009d50c(object->vf_0x0(), fn_7100096100()) ? reinterpret_cast<const CClass*>(object)
                                                                      : nullptr;
}

static inline bool IsPointerType(UnkVisitedType_l0a23f0* type) {
    return fn_710009d50c(type->GetClass(), fn_710009bedc());
}

// Hands the field's type the value at `ptr` of `value_type`, if there is one.
static inline void VisitAt(UnkObjectVisitor_l0a23f0* self, UnkVisitedField_l0a23f0* field, char* value_type,
                           void* ptr) {
    if (!ptr)
        return;
    UnkVisitedType_l0a23f0* type = field->type;
    UnkTypedRef ref;
    ref.type = reinterpret_cast<const CType*>(fn_710009dcd8(value_type, ptr));
    ref.ptr = ptr;
    type->Visit(&ref, self);
}

// Reads the field into `value` when it can be read.
static inline bool ReadField(UnkTunableReflValue* value, UnkVisitedField_l0a23f0* field, const UnkReflTypedPtr* obj) {
    auto* handle = reinterpret_cast<UnkTunableFieldHandle*>(field);
    if (!field->accessor && !fn_71000e750c(handle))
        return false;
    UnkTunableReflValue read = fn_71000e76dc(handle, obj);
    fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(value),
                  reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&read));
    return true;
}

// Hands the value's object, if it has one, to the field's type.
static inline void VisitValue(UnkObjectVisitor_l0a23f0* self, UnkVisitedField_l0a23f0* field,
                              UnkTunableReflValue* value) {
    if (!value->type)
        return;
    void* ptr = value->field_0x14 == 1 || reinterpret_cast<const CType*>(value->type)->m_size > 8 ? value->field_0x0
                                                                                                   : value;
    VisitAt(self, field, const_cast<char*>(value->type), ptr);
}

bool fn_71000a23f0(UnkObjectVisitor_l0a23f0* self, const UnkReflTypedPtr* obj) {
    if (self->WalksClasses() == 1) {
        const CClass* cls = ClassOf(obj);
        auto* fields = static_cast<UnkVisitedField_l0a23f0*>(cls->m_fields.data);
        for (u64 i = 0, n = cls->m_fields.count; i != n; ++i) {
            UnkVisitedField_l0a23f0* field = &fields[i];
            if (self->AcceptsField(field) == 1) {
                if (fn_71000e74bc(reinterpret_cast<CVariable*>(field))) {
                    UnkReflTypedPtr at = GetReflectedFieldTypedPtr(reinterpret_cast<const UnkReflFieldInfo*>(field),
                                                       reinterpret_cast<UnkFieldOwner*>(const_cast<UnkReflTypedPtr*>(obj)));
                    if (at.type)
                        VisitAt(self, field, at.type, at.ptr);
                } else if (self->field_0x8 == 0) {
                    UnkTunableReflValue value(base::reflection::refl_EmptyValue());
                    if (!ReadField(&value, field, obj))
                        fn_71000a2014(&value, reinterpret_cast<const CType*>(field->type));
                    VisitValue(self, field, &value);
                } else if (IsPointerType(field->type)) {
                    UnkTunableReflValue value(base::reflection::refl_EmptyValue());
                    ReadField(&value, field, obj);
                    VisitValue(self, field, &value);
                }
            }
            self->EndField(field);
        }
        auto* functions = static_cast<UnkVisitedFunction_l0a23f0*>(cls->m_functions.data);
        for (auto* function = functions; function != functions + cls->m_functions.count; ++function)
            self->VisitFunction(function);
    }
    self->VisitObject(obj);
    return true;
}
