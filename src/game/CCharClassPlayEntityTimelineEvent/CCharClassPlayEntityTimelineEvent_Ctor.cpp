// CCharClassPlayEntityTimelineEvent's default constructor (the name from reflection): on the event
// base (fn_7100067ea4), two empty string ids at +0x10 and +0x18. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa1648[];

struct UnkPlayEntityTimelineEvent {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    CStrId id_0x18;
};

void fn_7100bc58a4(UnkPlayEntityTimelineEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1648 + 0x10;
    new (&self->id_0x10) CStrId("", true);
    new (&self->id_0x18) CStrId("", true);
}
