// CStartCentralUnitCombatLogicAction (tools/tu_map.py unit 2597): after the base step (fn_7100a55ac8),
// resolves the lazily resolved handle at +0x48 (source at +0x18, state at +0x40: 1 until the handle takes
// its value, 2 after; as src/game/CTutoExitLogicAction/CTutoExitLogicAction_b47.cpp) and, when the actor
// it names is a central unit (fn_71007d74b0), starts it (fn_710077561c). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// As src/game/CTutoExitLogicAction/CTutoExitLogicAction_b47.cpp declares it.
struct UnkTutoExitAction_b47;
void fn_7100a55ac8(UnkTutoExitAction_b47* self);

// As src/game/CCharClassStartCentralUnitProtoEvent/CCharClassStartCentralUnitProtoEvent.cpp declares it.
struct UnkCentralUnitProto;
void fn_710077561c(UnkCentralUnitProto* unit);

UnkCentralUnitProto* fn_71007d74b0(void* actor);

struct UnkCentralUnitTarget_b49a6 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkStartCentralUnitAction_b49a6 {
    u8 pad_0x0[0x18];
    u8 source[0x28];
    u32 state;
    u32 pad_0x44;
    base::UnkRefHandle handle;
};

void fn_7100f182c8(UnkStartCentralUnitAction_b49a6* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (self->state == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->source);
            fn_7100234f78(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->state = 2;
    }
    auto* target = static_cast<UnkCentralUnitTarget_b49a6*>(self->handle.ptr);
    if (target && target->field_0x10) {
        // the original reloads the actor after the test
        if (UnkCentralUnitProto* unit = fn_71007d74b0(*reinterpret_cast<void* volatile*>(&target->field_0x10)))
            fn_710077561c(unit);
    }
}
