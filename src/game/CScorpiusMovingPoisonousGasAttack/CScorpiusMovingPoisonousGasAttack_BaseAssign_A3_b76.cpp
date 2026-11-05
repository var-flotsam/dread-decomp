// CScorpiusMovingPoisonousGasAttack's unit (tools/tu_map.py), fn_7100689410: a copy assignment of the base
// part of fn_71006892bc's type (CScorpiusMovingPoisonousGasAttack_Assign_A3_b76.cpp). Unless assigning to itself,
// the weak reference at +0x38 is made to refer to the weak handle of the other's target (game_s2/UnkWeakHandle.h).
// Names and layouts are placeholders.
#include "CScorpiusMovingPoisonousGasAttack_Gas_A3_b76.h"

// All but the vtable.
UnkScorpiusGasBase_A3_b76* fn_7100689410(UnkScorpiusGasBase_A3_b76* self, const UnkScorpiusGasBase_A3_b76* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    if (self != other)
        CopyWeakRef(&self->ref.handle, other->ref.handle);
    self->field_0x40 = other->field_0x40;
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    self->field_0x58 = other->field_0x58;
    self->field_0x5c = other->field_0x5c;
    return self;
}
