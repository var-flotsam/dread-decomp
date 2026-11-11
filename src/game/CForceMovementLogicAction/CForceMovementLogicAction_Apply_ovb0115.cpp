// CForceMovementLogicAction's unit (tools/tu_map.py unit 1568): applies the forced movement to the player
// (fn_7100dec788): with the flag at +0x10 its input (+0x1d8, fn_7100872460) and movement (+0x200,
// fn_71010db848 with the parameters at +0x14) take the actor's name (+0x30); without it both release it
// (fn_71010db850) and the actor is told fn_7100232864. The "ForcedMovementAreaComponent" id is built on the
// way (unused). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInputComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2689.h"
#include "types.h"

using base::global::CStrId;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkInputComponent;
struct UnkForcedMovementTarget_A5_b68;

struct UnkForcedMovementPlayer_ovb0115 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* field_0x1d8;
    u8 pad_0x1e0[0x20];
    UnkForcedMovementTarget_A5_b68* field_0x200;
};

struct UnkForcedMovementActor_ovb0115 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkForcedMovementAction_ovb0115 {
    void** vtable;
    UnkForcedMovementActor_ovb0115* actor;  // 0x8
    bool enable;                             // 0x10
    u8 pad_0x11[3];
    u8 field_0x14[4];
};

struct UnkInputComponent;
struct UnkForcedMovementTarget_A5_b68;
void fn_71010db848(UnkForcedMovementTarget_A5_b68* self, const CStrId* name, const void* params);
struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);

void fn_7100a4fd80(UnkForcedMovementAction_ovb0115* self) {
    UnkForcedMovementPlayer_ovb0115* player =
        reinterpret_cast<UnkForcedMovementPlayer_ovb0115*>(fn_7100dec788(g_unk_7101d47310));
    UnkForcedMovementTarget_A5_b68* movement = player->field_0x200;
    if (!movement)
        return;
    UnkInputComponent* input = player->field_0x1d8;
    if (!input)
        return;
    if (self->enable) {
        fn_7100872460(input, true, false, &self->actor->name);
        fn_71010db848(movement, &self->actor->name, self->field_0x14);
    } else {
        fn_7100872460(input, false, false, &self->actor->name);
        fn_71010db850(movement, &self->actor->name);
        static CStrId s_component("ForcedMovementAreaComponent", true);
        fn_7100232864(reinterpret_cast<UnkEntityState*>(self->actor), false);
    }
}
