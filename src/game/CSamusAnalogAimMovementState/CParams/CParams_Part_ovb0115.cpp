// CSamusAnalogAimMovementState::CParams's unit (tools/tu_map.py unit 1456): the function in slot 6 of one
// of two parts of the large object built once (base::reflection::refl_GetSingleton), +0xcb0 when the owner's
// flag (+0x38, its +0x3a8) is set, +0xfd8 otherwise. Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

struct UnkAimParamsOwner_ovb0115 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkAimParams_ovb0115 {
    u8 pad_0x0[0x38];
    UnkAimParamsOwner_ovb0115* owner;
};

static inline void** SingletonPart_ovb0115(u32 offset) {
    return reinterpret_cast<void**>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + offset);
}

bool fn_710098f8a0(UnkAimParams_ovb0115* self, void** out) {
    void** part = self->owner->field_0x3a8 ? SingletonPart_ovb0115(0xcb0) : SingletonPart_ovb0115(0xfd8);
    *out = reinterpret_cast<void**>(*part)[6];
    return true;
}
