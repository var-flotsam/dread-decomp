// CCharClassSetActionToOtherEntityEvent's unit (tools/tu_map.py): its constructor, on the event base
// (fn_7100067ea4): the vtable, a copy of the empty string id (base::global::CStrId::Empty()), a set flag, a cleared
// word, another copy of the empty id, a cleared flag and a float of 1. Names and the layout are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa1918[];

struct UnkSetActionToOtherEntityEvent_b42a3 {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    bool bForce;
    u32 iFrame;
    CStrId id_0x20;
    bool bIsAttack;
    f32 fProbability;
};

void fn_7100bd009c(UnkSetActionToOtherEntityEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1918 + 0x10;
    new (&self->id_0x10) CStrId(base::global::CStrId::Empty());
    self->bForce = true;
    self->iFrame = 0;
    new (&self->id_0x20) CStrId(base::global::CStrId::Empty());
    self->bIsAttack = false;
    self->fProbability = 1.0f;
}
