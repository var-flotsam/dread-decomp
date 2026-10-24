// Initialisation: runs fn_71003ac8f4, keeps the owner's object at +0x10 when it is a Chozo Robot
// Soldier AI component (null otherwise), points the callback slot at 0x15a0 back at the component
// (with its handler fn_7100474f24), initialises three embedded blocks at 0x27628, 0x27690 and
// 0x276f8 from the actor and registers four embedded entries in the list at 0x161c, each initialised
// from its own fixed offset into the actor. Returns bit 0 of the first step's result.
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
    u8 pad_0x18[0x1588];
    UnkSelf* field_0x15a0;
    void* field_0x15a8;
    u8 pad_0x15b0[0x68];
    u32 field_0x1618;
    u8 pad_0x161c[0x13384];
    u64 field_0x149a0;
    u8 field_0x149a8[1];
    u8 pad_0x149a9[0x6f];
    u8 field_0x14a18[1];
    u8 pad_0x14a19[0x47];
    u8 field_0x14a60[1];
    u8 pad_0x14a61[0x2f];
    u8 field_0x14a90[1];
    u8 pad_0x14a91[0x12b97];
    u8 field_0x27628[1];
    u8 pad_0x27629[0x67];
    u8 field_0x27690[1];
    u8 pad_0x27691[0x67];
    u8 field_0x276f8[1];
};

u32 fn_71003ac8f4(UnkSelf* self);
const CClass* fn_710046d9e0();  // CCharClassChozoRobotSoldierAIComponent::refl_GetClass
void fn_7100474f24();
void fn_71002c550c(void* block, u8* source);
void fn_71004649f0(void* entry, u8* source);
void fn_7100468248(void* entry, u8* source);
void fn_71004696bc(void* entry, u8* source);
void fn_710046a1c8(void* entry, u8* source);

__attribute__((always_inline)) static inline void Register(UnkSelf* self, u8* entry) {
    // Addressed by hand: an array member would be materialised ahead of the first store.
    char* list = reinterpret_cast<char*>(self) + 0x161c;
    u32 index = self->field_0x1618;
    *reinterpret_cast<u64_a4*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    self->field_0x1618 = index + 1;
}

u32 fn_710046f400(UnkSelf* self) {
    u32 result = fn_71003ac8f4(self);
    UnkComponent* object = self->field_0x10;
    UnkComponent* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710046d9e0()))
        component = object;
    self->field_0x149a0 = reinterpret_cast<u64>(component);
    self->field_0x15a0 = self;
    self->field_0x15a8 = reinterpret_cast<void*>(fn_7100474f24);
    fn_71002c550c(self->field_0x27628, reinterpret_cast<u8*>(self->field_0x149a0 + 0x960));
    fn_71002c550c(self->field_0x27690, reinterpret_cast<u8*>(self->field_0x149a0 + 0x9b8));
    fn_71002c550c(self->field_0x276f8, reinterpret_cast<u8*>(self->field_0x149a0 + 0xa10));
    Register(self, self->field_0x149a8);
    fn_71004649f0(self->field_0x149a8, reinterpret_cast<u8*>(self->field_0x149a0 + 0x528));
    Register(self, self->field_0x14a18);
    fn_7100468248(self->field_0x14a18, reinterpret_cast<u8*>(self->field_0x149a0 + 0x640));
    Register(self, self->field_0x14a60);
    fn_71004696bc(self->field_0x14a60, reinterpret_cast<u8*>(self->field_0x149a0 + 0x730));
    Register(self, self->field_0x14a90);
    fn_710046a1c8(self->field_0x14a90, reinterpret_cast<u8*>(self->field_0x149a0 + 0x820));
    return result & 1;
}
