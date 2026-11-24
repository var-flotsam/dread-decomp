// CHangableGrappleSurfaceComponent unit (tools/tu_map.py). fn_7100825538 takes a grapple message's
// point (+0x14) as the hang point (+0x88), then, when the definition (+0x98) names a grapple point
// (+0x70, not the empty id) and the message comes from it, the point fn_7100825258 works out for
// the message instead. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkHangableGrappleSurface.h"
#include "types.h"

using base::global::UnkStringData;

UnkHangPoint_b50 fn_7100825258(UnkHangable_b50* self, UnkHangMessage_b50* msg);

void CHangableGrappleSurfaceComponent_SetHangPointFromMessage(UnkHangable_b50* self, UnkHangMessage_b50* msg) {
    self->point = msg->point;
    UnkStringData* id = self->def->field_0x70;
    if (id == &g_unk_7101d47108->empty || msg->sender->field_0x10->field_0x20 != id)
        return;
    self->point = fn_7100825258(self, msg);
}
