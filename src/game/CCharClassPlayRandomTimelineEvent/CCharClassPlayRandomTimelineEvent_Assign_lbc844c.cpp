// CCharClassPlayRandomTimelineEvent's unit (tools/tu_map.py, unit 1828): the event's copy-assignment, which
// copies the base's word, the block of four words and the eight string ids.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTimelineEventParams_lbc844c.h"
#include "types.h"

using base::global::CStrId;

// The event: the timeline event base (vtable and one word), a block of four words and eight string ids.
struct UnkPlayRandomTimelineEvent {
    void** vtable;
    u32 field_0x8;
    UnkTimelineEventParams_lbc844c params;  // 0xc
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    CStrId field_0x58;
};
static_assert(__builtin_offsetof(UnkPlayRandomTimelineEvent, params) == 0xc, "layout");
static_assert(__builtin_offsetof(UnkPlayRandomTimelineEvent, field_0x20) == 0x20, "layout");

UnkPlayRandomTimelineEvent* fn_7100bc844c(UnkPlayRandomTimelineEvent* self, const UnkPlayRandomTimelineEvent* other) {
    self->field_0x8 = other->field_0x8;
    self->params = other->params;
    self->field_0x20 = other->field_0x20;
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x38 = other->field_0x38;
    self->field_0x40 = other->field_0x40;
    self->field_0x48 = other->field_0x48;
    self->field_0x50 = other->field_0x50;
    self->field_0x58 = other->field_0x58;
    return self;
}
