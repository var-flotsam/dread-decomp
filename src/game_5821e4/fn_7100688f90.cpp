// CScorpiusMovingPoisonousGasAttack (reflection: size 0x98, parent CScorpiusPoisonousGasAttack,
// whose parent is CScorpiusAttack): its copy constructor (fn_7100688f90) and move constructor
// (fn_71006890f0), both the implicit copy.
#include "CAttack.h"

#include <new>

struct CScorpiusAttack : CAttack {
    ~CScorpiusAttack() override;
    // Two words: a single 64-bit member would let the store of this class's vtable be removed.
    u32 field_0x28;
    u32 field_0x2c;
};

struct CScorpiusPoisonousGasAttack : CScorpiusAttack {
    ~CScorpiusPoisonousGasAttack() override;
    u64 field_0x30;
    UnkAttackRefPtr field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    UnkAttackWords<3> field_0x50;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
};

struct CScorpiusMovingPoisonousGasAttack : CScorpiusPoisonousGasAttack {
    ~CScorpiusMovingPoisonousGasAttack() override;
    u64 field_0x68;
    u32 field_0x70;
    UnkAttackListener field_0x78;
};

void fn_7100688f90(CScorpiusMovingPoisonousGasAttack* self, const CScorpiusMovingPoisonousGasAttack* other) {
    new (self) CScorpiusMovingPoisonousGasAttack(*other);
}

void fn_71006890f0(CScorpiusMovingPoisonousGasAttack* self, const CScorpiusMovingPoisonousGasAttack* other) {
    new (self) CScorpiusMovingPoisonousGasAttack(*other);
}
