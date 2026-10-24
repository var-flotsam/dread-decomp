// Initialisation: runs fn_71003ac8f4, keeps the owner's object at +0x10 when it is a Chozo Warrior
// AI component (null otherwise) and registers three embedded entries in the list at 0x161c, each
// one initialised from its own fixed offset into that component. Returns bit 0 of the first
// step's result.
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
    u8 pad_0x161c[0x133b4];
    u64 field_0x149d0;
    u8 pad_0x149d8[0x58];
    u8 field_0x14a30[1];
    u8 pad_0x14a31[0x47];
    u8 field_0x14a78[1];
    u8 pad_0x14a79[0x67];
    u8 field_0x14ae0[1];
};

u32 fn_71003ac8f4(UnkSelf* self);
const CClass* fn_7100496fbc();  // CCharClassChozoWarriorAIComponent::refl_GetClass
void fn_7100494c30(void* entry, u8* source);
void fn_7100495474(void* entry, u8* source);
void fn_710049418c(void* entry, u8* source);

__attribute__((always_inline)) static inline void Register(UnkSelf* self, u8* entry) {
    // Addressed by hand: an array member would be materialised ahead of the first store.
    char* list = reinterpret_cast<char*>(self) + 0x161c;
    u32 index = self->field_0x1618;
    *reinterpret_cast<u64_a4*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    self->field_0x1618 = index + 1;
}

u32 fn_7100498a38(UnkSelf* self) {
    u32 result = fn_71003ac8f4(self);
    UnkComponent* object = self->field_0x10;
    UnkComponent* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_7100496fbc()))
        component = object;
    self->field_0x149d0 = reinterpret_cast<u64>(component);
    Register(self, self->field_0x14a30);
    fn_7100494c30(self->field_0x14a30, reinterpret_cast<u8*>(self->field_0x149d0 + 0x528));
    Register(self, self->field_0x14a78);
    fn_7100495474(self->field_0x14a78, reinterpret_cast<u8*>(self->field_0x149d0 + 0x618));
    Register(self, self->field_0x14ae0);
    fn_710049418c(self->field_0x14ae0, reinterpret_cast<u8*>(self->field_0x149d0 + 0x708));
    return result & 1;
}
