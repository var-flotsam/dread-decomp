// CCharClassEmmyCheckSetSuccessActionEvent unit (tools/tu_map.py): when the actor's component at +0x240
// is of the class fn_710053b934 describes, the actor has its +0x220 sub-object and its +0x278 object
// passes fn_7101136308, sends that sub-object the hash at +0x30 of the event's +0x10 object with mode 0
// (UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttackComponent.h"
#include "types.h"

struct UnkEmmyCheckActor_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    base::reflection::UnkReflObject* field_0x240;
    u8 pad_0x248[0x30];
    void* field_0x278;
};

struct UnkEmmyCheckData_b56 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkEmmyCheckEvent_b56 {
    u8 pad_0x0[0x10];
    UnkEmmyCheckData_b56* field_0x10;
};

void fn_7100ba7f7c(UnkEmmyCheckEvent_b56* self, UnkEmmyCheckActor_b56** actor) {
    if (!*actor)
        return;
    base::reflection::UnkReflObject* comp = (*actor)->field_0x240;
    if (!comp || !fn_710009d50c(comp->vf_0x0(), fn_710053b934()))
        return;
    if (!(*actor)->field_0x220 || !(*actor)->field_0x278 || !fn_7101136308((*actor)->field_0x278))
        return;
    UnkSpitSend((*actor)->field_0x220, self->field_0x10->hash, 0xf4061ff25a2163d6);
}
