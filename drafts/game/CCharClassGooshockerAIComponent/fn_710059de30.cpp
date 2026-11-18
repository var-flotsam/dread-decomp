// CCharClassGooshockerAIComponent's unit (tools/tu_map.py unit 853): a state code, 4 when the object at
// +0x60 of the fourth argument has the id table's +0xec8 id, else 1. Names and layouts are placeholders.
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

struct UnkIdObj_l59de30 {
    u8 pad_0x0[0x20];
    base::global::CStrId id;  // 0x20
};

struct UnkIdHolder_l59de30 {
    u8 pad_0x0[0x60];
    UnkIdObj_l59de30* obj;  // 0x60
};

u32 fn_710059de30(void* self, void* arg, void* actor, UnkIdHolder_l59de30* holder) {
    UnkIdObj_l59de30* obj = holder->obj;
    if (obj == nullptr)
        return 1;
    if (GameManagerIdTable().field_0xec8 == obj->id)
        return 4;
    return 1;
}
