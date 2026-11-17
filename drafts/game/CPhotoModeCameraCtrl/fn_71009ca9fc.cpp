// CPhotoModeCameraCtrl's unit (tools/tu_map.py unit 1490): fn_71009ca9fc constructs the input member the
// camera controllers own: its name key, a flags word and an empty queue of 120 input events.
// Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "game/CPhotoModeCameraCtrl.h"
#include "types.h"

#include <new>

struct UnkInputEvent_l9ca9fc {
    u32 key = 0;
    u32 type = 0;
    u32 field_0x8 = 0;
    u32 value = 0;
    u32 field_0x10 = 0xfe;
};

struct UnkInputMember_l9ca9fc {
    base::UnkNamedEntry name;          // 0x0
    u32 flags;                         // 0x30
    bool field_0x34;                   // 0x34
    UnkInputEvent_l9ca9fc events[120];  // 0x38
    u32 event_count;                   // 0x998
    u32 field_0x99c;                   // 0x99c
};
static_assert(__builtin_offsetof(UnkInputMember_l9ca9fc, field_0x99c) == 0x99c, "layout");

void fn_71009ca9fc(void* self_, const char* name, u32 flags) {
    UnkInputMember_l9ca9fc* self = static_cast<UnkInputMember_l9ca9fc*>(self_);
    fn_710007e704(&self->name, name);
    self->flags = flags;
    self->field_0x34 = false;
    new (self->events) UnkInputEvent_l9ca9fc[120];
    self->event_count = 0;
    self->field_0x99c = 1;
}
