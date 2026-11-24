// CBTObserverComponent unit (unit 2216 of tools/tu_map.py): clears the slot at +0xd0 of the game manager's
// object at +0x1af8 when it points at this object's member at +0x30. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkObserverOwner {
    u8 pad_0x0[0x30];
    u8 member_0x30[0x8];
};

void fn_7100cab1fc(UnkObserverOwner* self) {
    UnkGameManagerSub_0x1af8* holder = g_unk_7101d47310->field_0x1af8;
    void* member = self->member_0x30;
    if (holder->field_0xd0 == member)
        holder->field_0xd0 = nullptr;
}
