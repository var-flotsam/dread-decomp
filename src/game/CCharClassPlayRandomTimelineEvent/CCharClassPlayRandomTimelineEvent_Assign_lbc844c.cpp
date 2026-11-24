// CCharClassPlayRandomTimelineEvent's unit (tools/tu_map.py, unit 1828): the event's copy-assignment, which
// copies the base's word, the block of four words and the eight string ids.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkPlayRandomTimelineEvent.h"
#include "game/UnkTimelineEventParams_lbc844c.h"
#include "types.h"

using base::global::CStrId;

static_assert(__builtin_offsetof(UnkPlayRandomTimelineEvent, params) == 0xc, "layout");
static_assert(__builtin_offsetof(UnkPlayRandomTimelineEvent, sName0) == 0x20, "layout");

UnkPlayRandomTimelineEvent* fn_7100bc844c(UnkPlayRandomTimelineEvent* self, const UnkPlayRandomTimelineEvent* other) {
    self->field_0x8 = other->field_0x8;
    self->params = other->params;
    self->sName0 = other->sName0;
    self->sName1 = other->sName1;
    self->sName2 = other->sName2;
    self->sName3 = other->sName3;
    self->sName4 = other->sName4;
    self->sName5 = other->sName5;
    self->sName6 = other->sName6;
    self->sName7 = other->sName7;
    return self;
}
