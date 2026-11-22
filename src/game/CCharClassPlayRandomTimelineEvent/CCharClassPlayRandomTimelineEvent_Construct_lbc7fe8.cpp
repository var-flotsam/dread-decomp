// CCharClassPlayRandomTimelineEvent's unit (tools/tu_map.py, unit 1828): the event's constructor: the
// timeline event base (fn_7100067ea4), the vtable, then each string id empty. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkPlayRandomTimelineEvent.h"
#include "game/UnkTimelineEventParams_lbc844c.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa16a8[];

static_assert(__builtin_offsetof(UnkPlayRandomTimelineEvent, params) == 0xc, "layout");
static_assert(__builtin_offsetof(UnkPlayRandomTimelineEvent, field_0x20) == 0x20, "layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100bc7fe8(UnkPlayRandomTimelineEvent* self) {
    fn_7100067ea4(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa16a8 + 0x10);
    MakeEmpty(&self->field_0x20);
    MakeEmpty(&self->field_0x28);
    MakeEmpty(&self->field_0x30);
    MakeEmpty(&self->field_0x38);
    MakeEmpty(&self->field_0x40);
    MakeEmpty(&self->field_0x48);
    MakeEmpty(&self->field_0x50);
    MakeEmpty(&self->field_0x58);
}
