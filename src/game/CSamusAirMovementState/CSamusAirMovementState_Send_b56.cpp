// CSamusAirMovementState unit (tools/tu_map.py): when the object at +0x38 passes fn_71009e75e8, sends the
// owner's (+0x30) sub-object at +0x220 the id hash of the singleton's (fn_7100031894) part at +0xfd0 with
// mode 1 (as UnkSpitSend, src/game_s2/UnkSpitSend.h, with the mode as a parameter), then stops the
// object at +0x38 with a zero vector (fn_71010d08d4). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CMovementComponent.h"
#include "game/UnkTU1501.h"
#include "types.h"

struct UnkFlagged;

// Declared as src/game/CMovementComponent/fn_71010d08d4.cpp defines it.
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;

struct UnkAirSendId_b56 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAirSendOwner_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkAirSend_b56 {
    u8 pad_0x0[0x30];
    UnkAirSendOwner_b56* owner;
    void* field_0x38;
};

struct UnkAirSendVec_b56 {
    f32 x, y, z;
};

void fn_710098d8b8(UnkAirSend_b56* self) {
    if (!fn_71009e75e8(static_cast<UnkFlagged*>(self->field_0x38)))
        return;
    u64 hash = (*reinterpret_cast<UnkAirSendId_b56**>(
                    reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0xfd0))
                   ->hash;
    void* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    UnkAirSendVec_b56 v;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    UnkMovement_71010d08d4* movement = static_cast<UnkMovement_71010d08d4*>(self->field_0x38);
    v = {0.0f, 0.0f, 0.0f};
    fn_71010d08d4(movement, reinterpret_cast<const UnkMovementVec_71010d08d4*>(&v), true);
}
