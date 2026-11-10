// CShelmitPlasmaRayAttack (reflection: size 0xb0): its copy assignment (fn_71006b6b5c). CAttack's fields, the
// attack's own plain fields, the listener at +0x38 (its words, then, unless it is assigned to itself, the
// weak handle of whatever the other's owner refers to, created if needed), four more words and thirteen
// u32s copied one by one. Names and layouts are placeholders.
#include "game/CAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The listener (vtable 0x7101a22d48) as its assignment sees it.
struct UnkPlasmaRayListener_ovb0126 {
    void* vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkWeakHandle* owner;

    __attribute__((always_inline)) UnkPlasmaRayListener_ovb0126& operator=(const UnkPlasmaRayListener_ovb0126& other) {
        __builtin_memcpy(&field_0x8, &other.field_0x8, 0x10);
        if (this != &other)
            CopyWeakRef(&owner, other.owner);
        return *this;
    }
};

struct UnkPlasmaRayWords_ovb0126 {
    u32 w[13];

    UnkPlasmaRayWords_ovb0126& operator=(const UnkPlasmaRayWords_ovb0126& other) {
        for (int i = 0; i < 13; i++)
            w[i] = other.w[i];
        return *this;
    }
};

struct UnkPlasmaRayAttackBase_ovb0126 {
    void* vtable;
    UnkActor* field_0x8;
    u32 field_0x10;
    bool field_0x14;
    s64 field_0x18;
    u64 field_0x20;

    UnkPlasmaRayAttackBase_ovb0126& operator=(const UnkPlasmaRayAttackBase_ovb0126& other) {
        __builtin_memcpy(&field_0x8, &other.field_0x8, 0x20);
        return *this;
    }
};

struct UnkPlasmaRayAttackCopy_ovb0126 : UnkPlasmaRayAttackBase_ovb0126 {
    u64 field_0x28;
    u32 field_0x30;
    UnkPlasmaRayListener_ovb0126 field_0x38;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u64 field_0x70;
    UnkPlasmaRayWords_ovb0126 field_0x78;
};
static_assert(sizeof(UnkPlasmaRayAttackCopy_ovb0126) == 0xb0, "layout");

__attribute__((flatten)) UnkPlasmaRayAttackCopy_ovb0126* fn_71006b6b5c(UnkPlasmaRayAttackCopy_ovb0126* self,
                                              const UnkPlasmaRayAttackCopy_ovb0126* other) {
    return &(*self = *other);
}
