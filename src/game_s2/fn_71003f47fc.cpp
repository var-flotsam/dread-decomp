// Initialisation: runs fn_71003e59f8, keeps the owner's object at +0x10 when it is a Bigkran X AI
// component (null otherwise) and registers two embedded entries in the list at 0x161c, each one
// initialised from its own fixed offset into that component. Returns bit 0 of the first step's
// result.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkComponent {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkComponent*)>(vtable[0])(this); }
};

struct UnkSelf {
    u8 pad_0x0[0x10];
    UnkComponent* field_0x10;
    u8 pad_0x18[0x1600];
    u32 field_0x1618;
    u8 pad_0x161c[0x1339c];
    u64 field_0x149b8;
    u8 field_0x149c0[1];
    u8 pad_0x149c1[0x5f];
    u8 field_0x14a20[1];
};

u32 fn_71003e59f8(UnkSelf* self);
const CClass* fn_71003f5604();  // CCharClassBigkranXAIComponent::refl_GetClass
void fn_71003ea558(void* entry, u8* source);
void fn_71003f14c4(void* entry, u8* source);

__attribute__((always_inline)) static inline void Register(UnkSelf* self, u8* entry) {
    // Addressed by hand: an array member would be materialised ahead of the first store.
    char* list = reinterpret_cast<char*>(self) + 0x161c;
    u32 index = self->field_0x1618;
    *reinterpret_cast<u64_a4*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    self->field_0x1618 = index + 1;
}

u32 fn_71003f47fc(UnkSelf* self) {
    u32 result = fn_71003e59f8(self);
    UnkComponent* object = self->field_0x10;
    UnkComponent* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_71003f5604()))
        component = object;
    self->field_0x149b8 = reinterpret_cast<u64>(component);
    Register(self, self->field_0x149c0);
    fn_71003ea558(self->field_0x149c0, reinterpret_cast<u8*>(self->field_0x149b8 + 0x628));
    Register(self, self->field_0x14a20);
    fn_71003f14c4(self->field_0x14a20, reinterpret_cast<u8*>(self->field_0x149b8 + 0x730));
    return result & 1;
}
