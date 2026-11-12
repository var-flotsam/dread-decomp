// CCharClassSetDoorLockedEvent (unit 1847 of tools/tu_map.py, the name from reflection): finds the
// entity named by the event's key (+0x10) in the game manager and, when it has a door
// (fn_71002af860), locks it (fn_71008d586c with true, 0 and an empty string id). Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU367.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;

struct UnkDoorLock;

struct UnkSetDoorLockedEvent {
    u8 unk_0x0[0x10];
    u64 key;
};

void fn_7100bd37dc(UnkSetDoorLockedEvent* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    alignas(8) u32 kind = g_unk_71727bebb8;
    UnkWeakTarget* entity = fn_7100debbc8(manager, &self->key, &kind);
    if (!entity)
        return;
    UnkDoorLock* door = fn_71002af860(entity);
    if (!door)
        return;
    CStrId name(g_unk_7101d470e0);
    fn_71008d586c(door, true, 0, &name);
}
