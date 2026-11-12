// Unit 85 (no reflected class; 0x71000e6ce0-0x71000e8728, after src/base/reflection/UnkUnit85/): helpers over
// reflected fields. A bool attribute test of a holder (fn_71000e6db0, over the attribute fn_710009ddd4 finds); the
// pass over the attributes of a class entry that hands each one to the handler registered for its key
// (fn_71000e7420); the typed pointer of a field of an object (fn_71000e7614) and its value (fn_71000e77dc), through
// the field's accessor (+0x18) or from its storage at the offset (+0x8); a field's constructor, with the 8-byte index
// words at +0x18 and +0x28 set to -1 and two empty attribute containers (fn_71000e6e4c). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct UnkFieldArgs85 {
    void* a;
    void* b;
};

struct UnkFieldAccessor85 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkReflValue vf_0x10(const UnkTypedRef& obj, const UnkFieldArgs85& args, u32 flags);
};

const base::reflection::UnkReflAttrKey& fn_7100090910();
struct UnkVersionEntry;

// Whether the bool attribute under the key (fn_7100090910) is set; false for a null holder.
bool fn_71000e6db0(UnkVersionEntry* self) {
    u32 ok = (u64)self;
    if (self) {
        ok = 0;
        UnkReflValue v = fn_710009ddd4(self, fn_7100090910());
        const CType* type = reinterpret_cast<const CType*>(v.type);
        if (type && fn_710009d50c(type, fn_710000d228()))
            ok = *base::reflection::refl_ArgValue<u8>(&v);
        // keeps the flag's byte in a register across the destructor call, tested after it
        asm("" : "+r"(ok));
        fn_71000a1c60(&v);
    }
    return ok != 0;
}

struct UnkVersionRegistry;
struct UnkAttrHandler85 {
    u8 pad_0x0[8];
    void (*handler)(void* key, void* value, void* field, void* owner);
};
UnkAttrHandler85* fn_71000a5350(UnkVersionRegistry* registry, const u64* key);

struct UnkReflClassEntry;
struct UnkAttrOwner85 {
    u8 pad_0x0[0x38];
    char* values;  // 0x38: 0x18 bytes each
    u64* keys;     // 0x40
    u32 count;     // 0x48
};

// Hands each attribute of the entry to the handler registered for its key.
void fn_71000e7420(UnkReflClassEntry* self_, base::reflection::CClass* owner) {
    UnkAttrOwner85* self = reinterpret_cast<UnkAttrOwner85*>(self_);
    for (u64 i = 0; i < self->count; ++i) {
        UnkAttrHandler85* h = fn_71000a5350(g_unk_7101d4c8d0, &self->keys[i]);
        if (h) {
            auto fn = h->handler;
            char* value = self->values + i * 0x18;
            fn(&self->keys[i], value, self, owner);
        }
    }
}

// The reflected value's copy-assignment (declared as src/base/reflection/UnkReflResolve.cpp does, over a record of
// UnkReflValue's size).
struct UnkAssignedRecord_a1ba4 {
    u64 unk_0x0[0x18 / 8];
};
bool fn_710009d960(const CType* type, UnkTypedRef* src, UnkTypedRef* dst);

// A reflected field: its offset in the object (-1 when it has an accessor), its type and its accessor.
struct UnkReflField {
    void* vtable;
    u32 offset;  // 0x8
    u8 pad_0xc[4];
    char* type;                    // 0x10
    UnkFieldAccessor85* accessor;  // 0x18
};

// Where a reflected value's data is: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValueData85(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
        return v->field_0x0;
    return v;
}

// The typed pointer of a field of the object `obj` points to: its storage in the object, or the value its
// accessor gives.
UnkReflTypedPtr fn_71000e7614(UnkReflField* self, const UnkReflTypedPtr* obj) {
    UnkReflTypedPtr r;
    if (self->offset == 0xffffffff) {
        UnkReflTypedPtr o;
        __builtin_memcpy(&o, obj, sizeof(o));
        void* p = o.ptr;
        UnkTypedRef typed = {reinterpret_cast<const CType*>(fn_710009dcd8(o.type, p)), p};
        UnkReflValue value = self->accessor->vf_0x10(typed, UnkFieldArgs85{nullptr, nullptr}, 0);
        r.type = const_cast<char*>(value.type);
        r.ptr = ValueData85(&value);
        fn_71000a1c60(&value);
        return r;
    }
    char* type = self->type;
    char* ptr = static_cast<char*>(obj->ptr) + self->offset;
    r.type = fn_710009dcd8(type, ptr);
    r.ptr = ptr;
    return r;
}

// The value of a field of the object `obj` points to, through its accessor or copied out of its storage.
UnkReflValue fn_71000e77dc(UnkReflField* field, const UnkReflTypedPtr* obj) {
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    if (UnkFieldAccessor85* accessor = field->accessor) {
        void* p = obj->ptr;
        UnkReflValue v = accessor->vf_0x10(
            UnkTypedRef{reinterpret_cast<const CType*>(fn_710009dcd8(obj->type, p)), p}, UnkFieldArgs85{nullptr, nullptr}, 0);
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(&r), reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&v));
        fn_71000a1c60(&v);
        return r;
    }
    fn_71000a2014(&r, reinterpret_cast<const CType*>(field->type));
    char* type = field->type;
    UnkTypedRef ref;
    fn_71000a16e0(&ref, type, static_cast<char*>(obj->ptr) + field->offset);
    UnkTypedRef dst = {reinterpret_cast<const CType*>(r.type), ValueData85(&r)};
    fn_710009d960(reinterpret_cast<const CType*>(type), &ref, &dst);
    return r;
}

struct UnkTypedWord85 {
    u64 value;
    const CType* type;
};

// Makes the slot hold -1 as the 8-byte type's value: the old value destroyed (type +0x40), the new one
// constructed from a temporary (type +0x38).
static inline __attribute__((always_inline)) void InitWord85(UnkTypedWord85* slot) {
    s64 index = -1;
    slot->type = nullptr;
    const CType* t = fn_7100010e58();
    if (slot->type) {
        slot->type->m_fn_0x40(slot);
        slot->type = nullptr;
    }
    slot->type = t;
    if (t->m_fn_0x38)
        t->m_fn_0x38(slot, &index);
}

struct UnkAttrContainer85 {
    base::reflection::UnkReflArray array;
    void* owner;
    u32 flag;
    UnkAttrContainer85(void* o) : array(10, 0x3e), owner(o), flag(1) {}
};

struct UnkFieldNew85 {
    base::global::CStrId name;  // 0x0
    u32 offset;                 // 0x8
    u8 pad_0xc[4];
    void* type;  // 0x10
    UnkTypedWord85 word_0x18;
    UnkTypedWord85 word_0x28;
    UnkAttrContainer85 attrs_0x38;
    UnkAttrContainer85 attrs_0x70;
    u16 field_0xa8;
};

void fn_71000e6e4c(UnkFieldNew85* self, void* type) {
    self->name.data = nullptr;
    if (__builtin_expect(!g_unk_7101d47108, 1))
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
    self->offset = 0xffffffff;
    self->type = type;
    InitWord85(&self->word_0x18);
    InitWord85(&self->word_0x28);
    new (&self->attrs_0x38) UnkAttrContainer85(type);
    new (&self->attrs_0x70) UnkAttrContainer85(nullptr);
    self->field_0xa8 = 0;
}
