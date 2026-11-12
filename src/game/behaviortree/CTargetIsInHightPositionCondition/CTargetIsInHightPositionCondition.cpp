// behaviortree::CTargetIsInHightPositionCondition (tools/tu_map.py, the name from reflection): true when
// the owner (+0x18) has a target, its AI component is a CShakernautAIComponent and the target stands
// higher than the owner (the y of fn_7100d7b7a8's positions). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "types.h"

struct UnkShakernautAI_tih {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkShakernautAI_tih*)>(vtable[0])(this);
    }
};

// A position returned in memory (x8): 16 bytes, not trivially copyable.
struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

struct UnkHighCondition_tih {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
};

bool fn_71006ac408(UnkHighCondition_tih* self) {
    if (!self->owner)
        return false;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    UnkShakernautAI_tih* ai = static_cast<UnkShakernautAI_tih*>(self->owner->field_0x240);
    if (!ai)
        return false;
    if (!fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()))
        return false;
    UnkActorPosition own = fn_7100d7b7a8(self->owner);
    UnkActorPosition other = fn_7100d7b7a8(target);
    return other.y > own.y;
}
