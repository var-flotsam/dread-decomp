// Initialisation: runs fn_71003ac8f4, keeps the owner's object at +0x10 when it is a Cooldown
// X-boss AI component (null otherwise) and registers five embedded entries in the list at 0x161c,
// each one initialised from its own fixed offset into that component. Returns bit 0 of the
// first step's result.
#include "types.h"

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkComponent {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkComponent*)>(vtable[0])(this); }
};

struct UnkSelf {
    u8 pad_0x0[0x10];
    UnkComponent* field_0x10;
    u8 pad_0x18[0x1600];
    u32 field_0x1618;
    // Pointers stored from 4-byte alignment.
    u64_a4 field_0x161c[1];
    u8 pad_0x1624[0x1337c];
    u64 field_0x149a0;
    u8 field_0x149a8[1];
    u8 pad_0x149a9[0x787];
    u8 field_0x15130[1];
    u8 pad_0x15131[0x34f];
    u8 field_0x15480[1];
    u8 pad_0x15481[0x8f];
    u8 field_0x15510[1];
    u8 pad_0x15511[0x97];
    u8 field_0x155a8[1];
};

u32 fn_71003ac8f4(UnkSelf* self);
void* fn_71004cde00();  // CCharClassCooldownXBossAIComponent::refl_GetClass
bool fn_710009d50c(const void* from, void* to);
void fn_71004c3604(void* entry, u8* source);
void fn_71004c6564(void* entry, u8* source);
void fn_71004c987c(void* entry, u8* source);
void fn_71004cb4f4(void* entry, u8* source);
void fn_71004c26ec(void* entry, u8* source);

__attribute__((always_inline)) static inline void Register(UnkSelf* self, u8* entry) {
    // Addressed by hand: an array member would be materialised ahead of the first store.
    char* list = reinterpret_cast<char*>(self) + 0x161c;
    u32 index = self->field_0x1618;
    *reinterpret_cast<u64_a4*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    self->field_0x1618 = index + 1;
}

u32 fn_71004d0344(UnkSelf* self) {
    u32 result = fn_71003ac8f4(self);
    UnkComponent* object = self->field_0x10;
    UnkComponent* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_71004cde00()))
        component = object;
    self->field_0x149a0 = reinterpret_cast<u64>(component);
    Register(self, self->field_0x149a8);
    fn_71004c3604(self->field_0x149a8, reinterpret_cast<u8*>(self->field_0x149a0 + 0x528));
    Register(self, self->field_0x15130);
    fn_71004c6564(self->field_0x15130, reinterpret_cast<u8*>(self->field_0x149a0 + 0x658));
    Register(self, self->field_0x15480);
    fn_71004c987c(self->field_0x15480, reinterpret_cast<u8*>(self->field_0x149a0 + 0x7e8));
    Register(self, self->field_0x15510);
    fn_71004cb4f4(self->field_0x15510, reinterpret_cast<u8*>(self->field_0x149a0 + 0x9f0));
    Register(self, self->field_0x155a8);
    fn_71004c26ec(self->field_0x155a8, reinterpret_cast<u8*>(self->field_0x149a0 + 0xb38));
    return result & 1;
}
