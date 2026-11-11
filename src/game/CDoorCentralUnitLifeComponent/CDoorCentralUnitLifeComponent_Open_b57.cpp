// CDoorCentralUnitLifeComponent's unit (tools/tu_map.py): fn_71008ca700 sends the owner's (+0x30)
// sub-object at +0x220, when there is one, the hash of "shieldopened" (flag set) or "shieldopen" with
// mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h), then calls fn_710024fa0c(0) on the owner's
// object fn_71005012bc finds, if any. Names are placeholders. The flag is a bool (only a bool
// parameter gives the original's branch order); the older declarations in
// CCentralUnitComponent_b46.cpp and CDoorCentralUnitLifeComponent_Send_b56.cpp say u32.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/CActorComponent.h"
#include "game/UnkTU794.h"
#include "types.h"

struct UnkStruct_710024fa0c;

struct UnkDoorLifeOwner_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkDoorLifePart_b56;

struct UnkCentralUnitDoor_b46a2 {
    u8 pad_0x0[0x30];
    UnkDoorLifeOwner_b56* owner;
};

void fn_71008ca700(UnkCentralUnitDoor_b46a2* door, bool open) {
    if (void* target = door->owner->field_0x220) {
        u64 hash;
        if (open)
            hash = base::global::CName("shieldopened").hash;
        else
            hash = base::global::CName("shieldopen").hash;
        UnkSpitSend(target, hash, 0xf4061ff25a2163d6);
    }
    if (UnkDoorLifePart_b56* part = fn_71005012bc(door->owner))
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(part), false);
}
