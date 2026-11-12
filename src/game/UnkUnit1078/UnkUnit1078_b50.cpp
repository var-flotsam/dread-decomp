// Unit 1078 of tools/tu_map.py (no class name). fn_71006d3750 picks an id for an actor: the
// unit's own id (g_unk_71727099e8) when the state at +0x6f8 of the actor's component at +0x218 is
// 1 or 6, else the entry at +0x23a8 of the id table (fn_7100031894). Names and layouts are
// placeholders.
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

extern char g_unk_71727099e8[] __attribute__((visibility("hidden")));

struct UnkActorState_b50 {
    u8 pad_0x0[0x6f8];
    u32 state;
};

struct UnkActor_b50 {
    u8 pad_0x0[0x218];
    UnkActorState_b50* field_0x218;
};

const void* fn_71006d3750(void* self, UnkActor_b50* actor) {
    if (actor) {
        if (UnkActorState_b50* st = actor->field_0x218) {
            if (st->state == 1 || st->state == 6)
                return g_unk_71727099e8;
        }
    }
    return &GameManagerIdTable().field_0x23a8;
}
