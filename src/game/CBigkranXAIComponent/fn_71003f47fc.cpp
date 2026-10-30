// Initialisation: runs fn_71003e59f8, keeps the owner's object at +0x10 when it is a Bigkran X AI
// component (null otherwise) and registers two embedded entries in the list at 0x161c, each one
// initialised from its own fixed offset into that component. Returns bit 0 of the first step's
// result.
#include "base/reflection/UnkClassGetters.h"
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
void fn_71003ea558(void* entry, u8* source);
void fn_71003f14c4(void* entry, u8* source);

__attribute__((always_inline)) static inline void Register(UnkSelf* self, u8* entry) {
    // Addressed by hand: an array member would be materialised ahead of the first store.
    char* list = reinterpret_cast<char*>(self) + 0x161c;
    u32 index = self->field_0x1618;
    *reinterpret_cast<u64_a4*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    self->field_0x1618 = index + 1;
}

// The step, the component's class getter and the two entries' initialisers, and the offsets of the
// component pointer, the entries and their sources in the component.
template <u32 (*Step)(UnkSelf*), const CClass* (*Getter)(), void (*Init1)(void*, u8*), void (*Init2)(void*, u8*),
          u32 Component, u32 Entry1, u32 Entry2, u32 Source1, u32 Source2>
__attribute__((always_inline)) static inline u32 Initialise(UnkSelf* self) {
    char* base = reinterpret_cast<char*>(self);
    u32 result = Step(self);
    UnkComponent* object = self->field_0x10;
    UnkComponent* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), Getter()))
        component = object;
    u64* slot = reinterpret_cast<u64*>(base + Component);
    *slot = reinterpret_cast<u64>(component);
    Register(self, reinterpret_cast<u8*>(base + Entry1));
    Init1(base + Entry1, reinterpret_cast<u8*>(*slot + Source1));
    Register(self, reinterpret_cast<u8*>(base + Entry2));
    Init2(base + Entry2, reinterpret_cast<u8*>(*slot + Source2));
    return result & 1;
}

u32 fn_71003f47fc(UnkSelf* self) {
    return Initialise<fn_71003e59f8, fn_71003f5604, fn_71003ea558, fn_71003f14c4, 0x149b8, 0x149c0, 0x14a20, 0x628,
                      0x730>(self);
}

// The same initialisation in the Goliath X, Quetzoa and Shakernaut AI components' units, with their
// own steps, class getters, initialisers and offsets.
u32 fn_7100590c14(UnkSelf* self);
u32 fn_71003ac8f4(UnkSelf* self);
u32 fn_710066193c(UnkSelf* self);
void fn_7100583e14(void* entry, u8* source);
void fn_71005869d0(void* entry, u8* source);
void fn_7100643ed0(void* entry, u8* source);
void fn_71006453a8(void* entry, u8* source);
void fn_71006a4200(void* entry, u8* source);
void fn_71006a6e4c(void* entry, u8* source);

u32 fn_710058835c(UnkSelf* self) {
    return Initialise<fn_7100590c14, fn_710058793c, fn_7100583e14, fn_71005869d0, 0x14b58, 0x14b60, 0x14bf0, 0x628,
                      0x720>(self);
}

u32 fn_7100649f88(UnkSelf* self) {
    return Initialise<fn_71003ac8f4, fn_7100648ce0, fn_7100643ed0, fn_71006453a8, 0x14a28, 0x149a0, 0x149e0, 0x528,
                      0x618>(self);
}

u32 fn_71006ad75c(UnkSelf* self) {
    return Initialise<fn_710066193c, fn_71006ac5f4, fn_71006a4200, fn_71006a6e4c, 0x147f8, 0x146d0, 0x14738, 0x520,
                      0x620>(self);
}
