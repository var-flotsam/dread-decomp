// CScourgeTongueSlashAttack (reflection: size 0x70): its copy constructor (fn_710069c864) and move
// constructor (fn_710069c938), both the implicit copy,
// and its copy (fn_710069ca18) and move (fn_710069cb90) assignments, both the implicit copy assignment.
#include "game/CAttack.h"

#include <new>

struct CScourgeTongueSlashAttack : CAttack {
    ~CScourgeTongueSlashAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u32 field_0x38;
    UnkAttackListener field_0x40;
    u64 field_0x60;
    u32 field_0x68;

    __attribute__((always_inline)) CScourgeTongueSlashAttack& operator=(const CScourgeTongueSlashAttack&) = default;
};

void fn_710069c864(CScourgeTongueSlashAttack* self, const CScourgeTongueSlashAttack* other) {
    new (self) CScourgeTongueSlashAttack(*other);
}

void fn_710069c938(CScourgeTongueSlashAttack* self, const CScourgeTongueSlashAttack* other) {
    new (self) CScourgeTongueSlashAttack(*other);
}

CScourgeTongueSlashAttack* fn_710069ca18(CScourgeTongueSlashAttack* self, const CScourgeTongueSlashAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

CScourgeTongueSlashAttack* fn_710069cb90(CScourgeTongueSlashAttack* self, const CScourgeTongueSlashAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}
