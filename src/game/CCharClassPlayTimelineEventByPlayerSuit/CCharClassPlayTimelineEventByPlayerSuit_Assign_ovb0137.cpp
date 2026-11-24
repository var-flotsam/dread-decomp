// CCharClassPlayTimelineEventByPlayerSuit's unit (tools/tu_map.py): the event's copy assignment (a word, four string ids and a word).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkTimelineEventBySuit_ovb0137 {
    virtual ~UnkTimelineEventBySuit_ovb0137();
    u32 field_0x8;
    CStrId sNamePower;
    CStrId sNameVaria;
    CStrId sNameGravity;
    CStrId sNameHyper;
    u32 ePlayOnly;
};

UnkTimelineEventBySuit_ovb0137* fn_7100bca6f0(UnkTimelineEventBySuit_ovb0137* self,
                                              const UnkTimelineEventBySuit_ovb0137* other) {
    self->field_0x8 = other->field_0x8;
    self->sNamePower = other->sNamePower;
    self->sNameVaria = other->sNameVaria;
    self->sNameGravity = other->sNameGravity;
    self->sNameHyper = other->sNameHyper;
    self->ePlayOnly = other->ePlayOnly;
    return self;
}
