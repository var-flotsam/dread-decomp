// CActorDef's unit (tools/tu_map.py unit 345): one of two flags of the definition, +0x6a when the
// game manager's check fn_7100deabe4 holds, else +0x69. Names are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkActorDefFlags345 {
    u8 pad_0x0[0x69];
    bool field_0x69;
    bool field_0x6a;
};

bool fn_7100255268(const UnkActorDefFlags345* self) {
    return fn_7100deabe4(g_unk_7101d47310) ? self->field_0x6a : self->field_0x69;
}
