// CSamusAction (unit 2208): whether an action id is one of six of the game's string ids (the id table built
// by fn_7100031894). Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The game's id table, with the six slots this test reads.
struct UnkSamusActionIds_lc99be8 {
    u8 pad_0x0[0x1d48];
    CStrId field_0x1d48;
    CStrId field_0x1d50;
    CStrId field_0x1d58;
    CStrId field_0x1d60;
    CStrId field_0x1d68;
    CStrId field_0x1d70;
    u8 pad_0x1d78[0x2578 - 0x1d78];
    UnkSamusActionIds_lc99be8() asm("fn_7100031894");
    ~UnkSamusActionIds_lc99be8() asm("fn_71000407f0");
};
static_assert(sizeof(UnkSamusActionIds_lc99be8) == 0x2578, "UnkSamusActionIds_lc99be8 layout");

inline const UnkSamusActionIds_lc99be8& SamusActionIds_lc99be8() {
    static UnkSamusActionIds_lc99be8 s_table asm("g_unk_7101d552d0");
    return s_table;
}

bool fn_7100c99be8(const CStrId* id) {
    if (id) {
        const CStrId& action = *id;
        if (action == SamusActionIds_lc99be8().field_0x1d70 || action == SamusActionIds_lc99be8().field_0x1d68 ||
            action == SamusActionIds_lc99be8().field_0x1d58 || action == SamusActionIds_lc99be8().field_0x1d60 ||
            action == SamusActionIds_lc99be8().field_0x1d50 || action == SamusActionIds_lc99be8().field_0x1d48)
            return true;
    }
    return false;
}
