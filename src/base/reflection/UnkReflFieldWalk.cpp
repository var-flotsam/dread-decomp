// A walk over a reflected object's fields (tools/tu_map.py: unit 85, next to the state checksum
// src/game_s1/fn_71000e81e8.cpp). A field record (0xb0 bytes) has the field's offset (or ~0 when
// it is reached through its accessor), its type, an accessor object and a small attribute map
// (keys and values side by side). A field with the attribute the reflection singleton keeps at
// +0xda8 is handed to fn_71000e7a80 with that attribute's value; a field of a class type is
// entered and its own fields walked with the field's object. Holds fn_71000e7b5c.
#include <new>

#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

// An object and its type, as the walk passes it on.
struct UnkReflFieldObject {
    char* type;
    void* ptr;
};

// What the accessor's read takes besides the object (cleared by the walk).
struct UnkReflReadArgs {
    void* field_0x0;
    void* field_0x8;
};

struct UnkReflAccessor {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkReflValue Read(const UnkReflFieldObject* obj, const UnkReflReadArgs& args,
                              bool flag);
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual bool HasValue();
};

struct UnkReflField;

// A class type as the walk sees it: its fields at +0xe8 and their count at +0xf8.
struct UnkReflFieldClass : UnkReflObject {
    u8 pad_0x8[0xe0];
    UnkReflField* fields;
    u8 pad_0xf0[0x8];
    u32 field_count;
};

struct UnkReflField {
    u8 pad_0x0[0x8];
    u32 offset;  // ~0: reached through the accessor
    u8 pad_0xc[0x4];
    UnkReflFieldClass* type;
    UnkReflAccessor* accessor;
    u8 pad_0x20[0x8];
    void* field_0x28;
    u8 pad_0x30[0x8];
    UnkReflValue* attr_values;
    u64* attr_keys;
    u32 attr_count;
    u8 pad_0x4c[0x64];

    UnkReflValue* AttrEnd() { return attr_values + attr_count; }
    UnkReflValue* FindAttr(const u64& key) {
        for (u64 i = 0; i < attr_count; ++i) {
            if (attr_keys[i] == key)
                return attr_values + i;
        }
        return AttrEnd();
    }
};
static_assert(sizeof(UnkReflField) == 0xb0, "UnkReflField layout");

// The value argument fn_71000e7a80 takes, built from a reflected value by fn_71000a1710.
struct UnkReflValueArg {
    u8 storage[0x18];
};
void fn_71000a1710(UnkReflValueArg* out, const UnkReflValue* value);
bool fn_71000e7a80(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);

// Where a reflected value's data is: inline when it fits in 8 bytes, else through field_0x0.
static inline void* UnkReflValueData(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
        return v->field_0x0;
    return v;
}

// The accessor's value is declared for the whole walk: once destroyed, its storage still holds the
// value an inline field points into, and the field's own fields are walked over it, as in the
// original (whose slot for it is apart from the walk object's).
// NON_MATCHING(minor): stack slots and block order: the attribute value and the handler's argument
// trade slots, the accessor's argument pair shares the walk object's slot (the frame is 0x10
// smaller), and the default value's block comes after the attribute copy's.
bool fn_71000e7b5c(UnkReflField* self, const UnkReflFieldObject* obj) {
    if (!self->field_0x28 && self->offset == 0xFFFFFFFF &&
        !(self->accessor && self->accessor->HasValue()))
        return false;
    const u64& key = *reinterpret_cast<const u64*>(
        reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0xda8);
    UnkReflValue* it = self->FindAttr(key);
    UnkReflValue attr;
    if (it == self->AttrEnd()) {
        attr.field_0x0 = nullptr;
        attr.type = nullptr;
        attr.kind = 2;
        attr.field_0x14 = 1;
    } else {
        attr.field_0x0 = nullptr;
        attr.type = nullptr;
        attr.kind = it->kind;
        fn_71000a18d4(&attr, reinterpret_cast<const CType*>(it->type), UnkReflValueData(it), 0);
    }
    if (attr.type) {
        UnkReflValueArg arg;
        fn_71000a1710(&arg, &attr);
        bool result = fn_71000e7a80(self, obj, &arg);
        fn_71000a1c60(&attr);
        return result;
    }
    UnkReflFieldClass* cls = self->type;
    if (cls && fn_710009d50c(cls->vf_0x0(), fn_7100096100())) {
        if (self->offset != 0xFFFFFFFF || (self->accessor && self->accessor->HasValue())) {
            char* type;
            void* ptr;
            UnkReflValue v;
            if (self->offset != 0xFFFFFFFF) {
                ptr = static_cast<char*>(obj->ptr) + self->offset;
                type = reinterpret_cast<char*>(self->type);
                if (ptr)
                    type = fn_710009dcd8(type, ptr);
            } else {
                UnkReflReadArgs args = {nullptr, nullptr};
                new (&v) UnkReflValue(self->accessor->Read(obj, args, false));
                type = const_cast<char*>(v.type);
                ptr = UnkReflValueData(&v);
                fn_71000a1c60(&v);
            }
            UnkReflFieldObject inner = {fn_710009dcd8(type, ptr), ptr};
            for (UnkReflField *f = cls->fields, *end = f + cls->field_count; f != end; ++f)
                fn_71000e7b5c(f, &inner);
        }
    }
    fn_71000a1c60(&attr);
    return false;
}
