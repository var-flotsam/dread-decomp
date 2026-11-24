// A constructor in CCharClassSetMaterialPropertyTransitionEvent's unit: the base constructor
// (fn_7100067ea4), the vtable, a string id built from "" and the value fields, ending in two
// floats of 1 (as the FX material events in
// src/game/CCharClassFXSetMaterialPropertyEvent/CCharClassFXSetMaterialPropertyEvent.cpp, with one
// id). Names and the layout are placeholders.
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU0.h"
#include "types.h"

struct UnkMaterialTransitionEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    u64 id_0x10;
    u32 ePropertyName;
    u32 eStage;
    u64 field_0x20;
    u64 field_0x28;
    f32 fHDR;
    f32 fTime;
};

extern void* g_unk_7101aa6138[];

void fn_7100cd5c4c(UnkMaterialTransitionEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa6138 + 2;
    CStrId_Construct(&self->id_0x10, "", true);
    self->ePropertyName = 0;
    self->eStage = 1;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->fHDR = 1.0f;
    self->fTime = 1.0f;
}
