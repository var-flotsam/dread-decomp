// CTriggerComponent's unit: for the player, walks its list of entries (+0x328) and passes two summary flags
// of their state objects to the game manager's sub-object. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

char* fn_7100f0e208(char* p0, u32 p1);
char* fn_7100f0e21c(char* p0, u32 p1);

struct UnkEntryState_la5fccc {
    u8 pad_0x0[0x238];
    u8 field_0x238;
    u8 pad_0x239[5];
    u8 field_0x23e;
};

struct UnkEntryObj_la5fccc {
    u8 pad_0x0[0x1e8];
    UnkEntryState_la5fccc* field_0x1e8;
};

struct UnkEntryNode_la5fccc {
    UnkEntryObj_la5fccc* obj;
    UnkEntryNode_la5fccc* next;
};

struct UnkPlayer {
    u8 pad_0x0[0x328];
    UnkEntryNode_la5fccc* field_0x328;
};

void fn_7100a5fccc(UnkPlayer* self) {
    if (fn_7100dec788(g_unk_7101d47310) != self)
        return;
    bool all = true;
    bool any = false;
    UnkEntryNode_la5fccc* node = self->field_0x328;
    while (node) {
        UnkEntryState_la5fccc* state = node->obj->field_0x1e8;
        if (state) {
            if (all) {
                all = true;
                if (state->field_0x238 && !(state->field_0x23e & 0x12))
                    all = false;
            }
            any = any || state->field_0x238;
            if (!all && any) {
                all = false;
                break;
            }
        }
        node = node->next;
    }
    char* sub = reinterpret_cast<char*>(g_unk_7101d47310) + 0x38;
    fn_7100f0e208(sub, all);
    sub = reinterpret_cast<char*>(g_unk_7101d47310) + 0x38;
    fn_7100f0e21c(sub, any);
}
