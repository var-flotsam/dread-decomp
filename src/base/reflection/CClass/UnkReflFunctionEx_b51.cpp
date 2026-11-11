// Unit 72 (base::reflection::CClass): the constructor and destructor of a reflected function (UnkReflFunction,
// 0xb8 bytes) extended with a list of reflected values (+0xc0), one more value (+0xe8) and a
// list of words (+0x100); vtable unk_7101a1b260. They start from the function's own constructor
// (fn_710009a5b4) and end in its destructor (fn_710009aae8; UnkReflFunctionData's in
// src/base/reflection/fn_7100097550.cpp). The constructor copies the value and finishes in
// fn_7100094fc0. Names and layouts are placeholders inferred from the code.

#include <new>

#include "base/UnkVector.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

extern char unk_7101a1b260[];
// UnkReflFunctionData's constructor and destructor, called on the base subobject.
void fn_710009a5b4(void* self);
void fn_7100094fc0(void* self, void* owner, void* x2);

struct UnkOwnedReflValue_7100095bdc : UnkReflValue {
    ~UnkOwnedReflValue_7100095bdc() { fn_71000a1c60(this); }
};

struct UnkReflFunctionEx_7100095bdc {
    char* vtable;
    u8 base[0xb0];
    void* owner;
    base::UnkVector<UnkOwnedReflValue_7100095bdc> values;
    UnkOwnedReflValue_7100095bdc value;
    base::UnkVector<void*> words;
};
static_assert(__builtin_offsetof(UnkReflFunctionEx_7100095bdc, words) == 0x100, "");

// The value's address: in place when it fits in 8 bytes and isn't flagged, else through field_0x0.
static inline const void* ValueAddress(const UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1)
        return v->field_0x0;
    if (*reinterpret_cast<const u64*>(v->type + 0x18) > 8)
        return v->field_0x0;
    return v;
}

void fn_7100095684(UnkReflFunctionEx_7100095bdc* self, void* owner, const UnkReflValue* value, void* x3) {
    fn_710009a5b4(self);
    self->vtable = unk_7101a1b260 + 0x10;
    self->owner = owner;
    new (&self->values) base::UnkVector<UnkOwnedReflValue_7100095bdc>(0x59);
    u32 by_ref = value->field_0x14;
    self->value.field_0x0 = nullptr;
    self->value.type = nullptr;
    // A plain word store: it may alias the source's flag, which is read again.
    *reinterpret_cast<u32*>(&self->value.kind) = value->kind;
    fn_71000a18d4(&self->value, reinterpret_cast<const base::reflection::CType*>(value->type), ValueAddress(value),
                  by_ref);
    new (&self->words) base::UnkVector<void*>(0x59);
    fn_7100094fc0(self, owner, x3);
}

void fn_7100095bdc(UnkReflFunctionEx_7100095bdc* self) {
    self->vtable = unk_7101a1b260 + 0x10;
    self->words.Free();
    self->value.~UnkOwnedReflValue_7100095bdc();
    self->values.Free();
    fn_710009aae8(self);
}
