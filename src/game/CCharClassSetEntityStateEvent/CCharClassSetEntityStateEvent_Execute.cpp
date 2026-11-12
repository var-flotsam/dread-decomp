// CCharClassSetEntityStateEvent (unit 1851 of tools/tu_map.py, the name from reflection): applies
// the event's flag (+0x18) to an entity with fn_7100232864. The entity is the one named by the
// event's id (+0x10, looked up in the game manager with the global kind) unless the id is -1;
// then it is the event's own entity (`*owner`). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkWeakTarget;

struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);

struct UnkSetEntityStateEvent {
    u8 unk_0x0[0x10];
    u64 id;
    bool value;
};

void fn_7100bd6248(UnkSetEntityStateEvent* self, UnkEntityState** owner) {
    if (self->id != (u64)-1) {
        UnkGameManager* manager = g_unk_7101d47310;
        alignas(8) u32 kind = g_unk_71727bebb8;
        UnkWeakTarget* entity = fn_7100debbc8(manager, &self->id, &kind);
        if (entity)
            fn_7100232864(reinterpret_cast<UnkEntityState*>(entity), self->value);
        return;
    }
    fn_7100232864(*owner, self->value);
}
