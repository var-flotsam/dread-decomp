// CScorpiusMovingPoisonousGasAttack's unit (tools/tu_map.py), fn_71006892bc: a copy assignment. The base part
// is fn_7100689410's (CScorpiusMovingPoisonousGasAttack_BaseAssign_A3_b76.cpp); then the 12 bytes at +0x68, the 16
// bytes at +0x80 and, unless assigning to itself, the weak reference at +0x90 (made to refer to the weak handle of
// the other's target, game_s2/UnkWeakHandle.h). Names and layouts are placeholders.
#include "CScorpiusMovingPoisonousGasAttack_Gas_A3_b76.h"

struct UnkScorpiusGas_A3_b76 : UnkScorpiusGasBase_A3_b76 {
    UnkScorpiusGas12_A3_b76 field_0x68;
    u8 pad_0x74[0xc];
    UnkScorpiusGas16_A3_b76 field_0x80;
    UnkScorpiusGasWeakRef_A3_b76 ref;  // 0x90
};

UnkScorpiusGas_A3_b76* fn_71006892bc(UnkScorpiusGas_A3_b76* self, const UnkScorpiusGas_A3_b76* other) {
    fn_7100689410(self, other);
    self->field_0x68 = other->field_0x68;
    self->field_0x80 = other->field_0x80;
    if (self != other)
        CopyWeakRef(&self->ref.handle, other->ref.handle);
    return self;
}
