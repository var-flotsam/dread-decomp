// CCharClassEnemyMovement's unit (tools/tu_map.py): the value at +0x18 of the actor's character
// class (+0x220) record for the class fn_7100c43508 names, else the movement's own value at +0x6c.
// Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkTrackRecordValue_b45a2 {
    u8 pad_0x0[0x18];
    f32 value;
};

struct UnkEnemyMovement_b45a2 {
    u8 pad_0x0[0x6c];
    f32 field_0x6c;
};

f32 fn_7100964f8c(UnkEnemyMovement_b45a2* self, UnkActor* actor) {
    if (actor->field_0x220) {
        UnkTrackQuery query = {fn_7100c43508(), nullptr, -1.0f};
        fn_710117b08c(static_cast<UnkAnimationComponent*>(actor->field_0x220), &query);
        if (query.result)
            return reinterpret_cast<UnkTrackRecordValue_b45a2*>(query.result)->value;
    }
    return self->field_0x6c;
}
