// CCharClassSetEntityVisibilityStateEvent (unit 1852 of tools/tu_map.py, the name from reflection): applies
// the event's flag (+0x18) to an entity with fn_71002334fc. The entity is the one named by the
// event's id (+0x10, looked up in the game manager with the global kind) unless the id is -1;
// then it is the event's own entity (`*owner`). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkWeakTarget;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

struct UnkStruct_71002334fc;

struct UnkSetEntityVisibilityStateEvent {
    u8 unk_0x0[0x10];
    u64 id;
    bool value;
};

void fn_7100bd69d8(UnkSetEntityVisibilityStateEvent* self, UnkStruct_71002334fc** owner) {
    if (self->id != (u64)-1) {
        UnkGameManager* manager = g_unk_7101d47310;
        alignas(8) u32 kind = g_unk_71727bebb8;
        UnkWeakTarget* entity = fn_7100debbc8(manager, &self->id, &kind);
        if (entity)
            fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(entity), self->value);
        return;
    }
    fn_71002334fc(*owner, self->value);
}
