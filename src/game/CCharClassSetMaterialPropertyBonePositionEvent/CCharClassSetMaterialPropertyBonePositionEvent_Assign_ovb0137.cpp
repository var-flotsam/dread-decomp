// CCharClassSetMaterialPropertyBonePositionEvent's unit (tools/tu_map.py): the event's copy assignment (a word, four string ids and 0x15 bytes of plain values).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkMaterialBoneEvent_ovb0137 {
    virtual ~UnkMaterialBoneEvent_ovb0137();
    u32 field_0x8;
    CStrId sNode;
    CStrId sActor;
    CStrId sFXId;
    CStrId sMaterialName;
    u8 field_0x30[0x15];  // plain values, copied as bytes
};

// The event's copy assignment.
UnkMaterialBoneEvent_ovb0137* fn_7100bd8864(UnkMaterialBoneEvent_ovb0137* self,
                                            const UnkMaterialBoneEvent_ovb0137* other) {
    self->field_0x8 = other->field_0x8;
    self->sNode = other->sNode;
    self->sActor = other->sActor;
    self->sFXId = other->sFXId;
    self->sMaterialName = other->sMaterialName;
    __builtin_memcpy(self->field_0x30, other->field_0x30, sizeof(self->field_0x30));
    return self;
}
