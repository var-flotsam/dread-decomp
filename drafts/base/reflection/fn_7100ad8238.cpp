// From unit 1634 of data/tu_map.csv (base::reflection): resolves one step of a path in a class
// object: the step names a field (a string id), which is looked up in the object's class; the
// field's value is the result at the path's last step, else the resolution goes on into it.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU85.h"
#include "game/UnkTunableFieldHandle.h"
#include "game/UnkTunableReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

UnkReflTypedPtr fn_71000e7614(UnkReflField* self, const UnkReflTypedPtr* obj);
bool fn_71000e750c(UnkTunableFieldHandle* field);  // whether the field can be read

// Two words assigned only from another object.
struct UnkResolvedPair {
    u64 a;
    u64 b;

    UnkResolvedPair& operator=(const UnkResolvedPair& o) {
        if (this != &o) {
            a = o.a;
            b = o.b;
        }
        return *this;
    }
};

// A reflected value that owns what it holds: copied by fn_71000a179c, released by fn_71000a1c60.
struct UnkHeldReflValue : UnkReflValue {
    UnkHeldReflValue() {}
    UnkHeldReflValue(const UnkReflValue& o) { fn_71000a179c(this, &o); }
    UnkHeldReflValue(const UnkHeldReflValue& o) { fn_71000a179c(this, &o); }
    ~UnkHeldReflValue() { fn_71000a1c60(this); }
};

// A resolved value (0x30 bytes): the reflected value and what came with it.
struct UnkResolvedValue {
    UnkReflValue value;
    u64 field_0x18;
    UnkResolvedPair field_0x20;

    UnkResolvedValue() {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        field_0x18 = 0;
        field_0x20.a = 0;
        field_0x20.b = 0;
    }
    UnkResolvedValue(UnkHeldReflValue v, u64 field, const UnkTypedRef& object)
        : field_0x18(field), field_0x20{(u64)object.type, (u64)object.ptr} {
        fn_71000a179c(&value, &v);
    }
    UnkResolvedValue& operator=(const UnkResolvedValue& o) {
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(&value),
                      reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&o.value));
        field_0x18 = o.field_0x18;
        field_0x20 = o.field_0x20;
        return *this;
    }
    ~UnkResolvedValue() { fn_71000a1c60(&value); }
};

UnkResolvedValue fn_7100ad7db8(const UnkTypedRef* ref, void* a, u32 b);

// A value of any reflected type (0x10 bytes): copied and destroyed through its type's functions.
struct UnkAnyValue_ad8238;
struct UnkAnyType_ad8238 {
    u8 pad_0x0[0x30];
    void (*copy)(UnkAnyValue_ad8238* dst, const UnkAnyValue_ad8238* src);  // 0x30
    u8 pad_0x38[8];
    void (*destroy)(UnkAnyValue_ad8238* self);  // 0x40
};

struct UnkAnyValue_ad8238 {
    u64 value;
    const UnkAnyType_ad8238* type;

    UnkAnyValue_ad8238(const UnkAnyValue_ad8238& o) : type(nullptr) {
        const UnkAnyType_ad8238* t = o.type;
        if (auto copy = t->copy) {
            type = t;
            copy(this, &o);
        }
    }
    ~UnkAnyValue_ad8238() {
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
    }

    template <class T, const CType* (*GetType)()>
    T* As() {
        return fn_710009d50c(reinterpret_cast<const CType*>(type), GetType()) ? reinterpret_cast<T*>(this)
                                                                              : nullptr;
    }
};

// The path: its steps, each a field's name.
struct UnkReflPath_ad8238 {
    UnkAnyValue_ad8238* steps;
    u32 count;
};

// A class's field (0xb0 bytes).
struct UnkClassField_ad8238 {
    UnkTunableFieldHandle handle;
    u8 pad_0x20[0x90];
};
static_assert(sizeof(UnkClassField_ad8238) == 0xb0, "");

// A class descriptor as the lookup reads it: its fields by name hash.
struct UnkClassFields_ad8238 {
    u8 pad_0x0[0xe8];
    UnkClassField_ad8238* fields;  // 0xe8
    u64* keys;                     // 0xf0
    u32 count;                     // 0xf8

    UnkClassField_ad8238* End() { return &fields[count]; }
    UnkClassField_ad8238* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
};

// A type descriptor's size, at +0x18.
struct UnkSizedType_ad8238 {
    u8 pad_0x0[0x18];
    u64 size;
};

static inline bool IsClass(const CType* type) {
    return fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), fn_7100096100());
}

static inline UnkHeldReflValue ValueOf(const UnkTypedRef* ref) {
    UnkHeldReflValue v;
    fn_71000a1ae4(&v, ref, 1, 2);
    return v;
}

// Whether the field is read through its accessor or can be read at its offset.
static inline bool CanRead(UnkTunableFieldHandle* field) {
    return field->accessor || fn_71000e750c(field);
}

// The field's entry, or null when the class has none of that name.
static inline UnkClassField_ad8238* FindField(UnkClassFields_ad8238* cls, u64 key) {
    UnkClassField_ad8238* field = cls->Find(key);
    return field != cls->End() ? field : nullptr;
}

// What a field of `object` refers to.
static inline UnkReflTypedPtr Target(UnkTunableFieldHandle* field, const UnkTypedRef* object) {
    UnkTypedRef self;
    fn_71000a174c(&self, object);
    return fn_71000e7614(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflTypedPtr*>(&self));
}

UnkResolvedValue fn_7100ad8238(const UnkTypedRef* object, void* a, u32 b) {
    const UnkReflPath_ad8238* path = static_cast<const UnkReflPath_ad8238*>(a);
    UnkAnyValue_ad8238 step = path->steps[b];
    CStrId name = *step.As<CStrId, fn_7100009658>();
    const CType* type = object->type;
    UnkClassFields_ad8238* cls = type && IsClass(type) ? (UnkClassFields_ad8238*)type : nullptr;
    UnkClassField_ad8238* field = FindField(cls, name.data->Key());
    if (field) {
        if (fn_71000e750c(&field->handle)) {
            UnkReflTypedPtr found = Target(&field->handle, object);
            const UnkTypedRef& target = reinterpret_cast<const UnkTypedRef&>(found);
            if (target.type && target.ptr) {
                if (b + 1 == path->count)
                    return UnkResolvedValue(ValueOf(&target), (u64)field, *object);
                return fn_7100ad7db8(&target, a, b + 1);
            }
        } else if (CanRead(&field->handle)) {
            UnkTunableReflValue v =
                fn_71000e76dc(&field->handle, reinterpret_cast<const UnkReflTypedPtr*>(object));
            if (v.type) {
                if (b + 1 == path->count)
                    return UnkResolvedValue(v, (u64)field, *object);
                UnkTypedRef inner;
                inner.type = reinterpret_cast<const CType*>(v.type);
                inner.ptr = v.field_0x14 == 1 || reinterpret_cast<const UnkSizedType_ad8238*>(v.type)->size > 8
                                ? v.field_0x0
                                : &v;
                return fn_7100ad7db8(&inner, a, b + 1);
            }
            return UnkResolvedValue();
        }
    }
    return UnkResolvedValue();
}
