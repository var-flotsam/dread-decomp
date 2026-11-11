// CForcedMovementAreaComponent's unit: tells the emmy registry (g_unk_71726af0a8, fn_7100daec80) about the actor at
// +0x18 walking through an emmy door, then calls the scenario's Lua callback with the actor and the flags (on: true
// and the registry's flag at +0x124; off: false, false, then fn_71007ea408). Returns true. Names are placeholders.
#include "game/CForcedMovementAreaComponent.h"
#include "game/UnkTU0.h"
#include "types.h"

struct UnkEmmyRegistry_b55;
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;

struct UnkForcedMovementArea_A5_b68e;
void fn_7100daec80(UnkEmmyRegistry_b55* registry, void* actor, bool on);

struct UnkForcedMovementArea_A5_b68e {
    u8 pad_0x0[0x18];
    void* field_0x18;
};

bool fn_71007ecfe8(UnkForcedMovementArea_A5_b68e* self, bool on) {
    void* actor = self->field_0x18;
    UnkEmmyRegistry_b55* registry = g_unk_71726af0a8;
    if (on) {
        u8 flag = reinterpret_cast<u8*>(registry)[0x124];
        fn_7100daec80(registry, actor, true);
        fn_7100000790("CurrentScenario.OnWalkThroughEmmyDoor", "obb", actor, true, flag);
    } else {
        fn_7100daec80(registry, actor, false);
        fn_7100000790("CurrentScenario.OnWalkThroughEmmyDoor", "obb", actor, false, false);
        fn_71007ea408(self);
    }
    return true;
}
