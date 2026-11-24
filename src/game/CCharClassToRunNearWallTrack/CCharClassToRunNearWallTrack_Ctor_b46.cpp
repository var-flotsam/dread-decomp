// CCharClassToRunNearWallTrack unit (tools/tu_map.py unit 2135): a constructor: the class getter
// (refl_GetClass, run for its side effect), the track base constructor (fn_7100d2a844) given the
// unit's static descriptor (g_unk_717278e3a8), the vtable, three copies of the empty string id
// and the defaults -1 and 1. Names and the layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassToRunNearWallTrack.h"
#include "types.h"

using base::global::CStrId;

struct UnkNearWallTrack_b46a3 {
    void** vtable;
    u8 unk_0x8[0x30];
    CStrId sHighAction;
    CStrId sLowAction;
    CStrId sNoHandAction;
    f32 field_0x50;
    f32 fCheckDistance;
};

extern char g_unk_717278e3a8[] __attribute__((visibility("hidden")));
void fn_7100d2a844(void* self, void* descriptor);

void fn_7100c54548(UnkNearWallTrack_b46a3* self) {
    fn_7100c53d88();
    fn_7100d2a844(self, g_unk_717278e3a8);
    self->vtable = &g_unk_7101aa4890[2];
    new (&self->sHighAction) CStrId(base::global::CStrId::Empty());
    new (&self->sLowAction) CStrId(base::global::CStrId::Empty());
    new (&self->sNoHandAction) CStrId(base::global::CStrId::Empty());
    self->field_0x50 = -1.0f;
    self->fCheckDistance = 1.0f;
}
