// behaviortree::CEmmyPatrolSearchingAction's unit (tools/tu_map.py): when the owner's (+0x18) AI
// component (+0x240) is a CEmmyAIComponent whose search time (+0x14c10) is positive, adds `dt` to the
// elapsed time (+0x160) and, once it reaches that time, enters the state g_unk_71726ded60; then runs
// the base update (fn_7100515904). Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkEmmyActor_b55;
void fn_7100544a34(UnkEmmyActor_b55* self, char* data, bool on);
extern char g_unk_71726ded60[];

struct UnkEmmySearchingAI_A3_b64 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x14c10 - 0x8];
    f32 field_0x14c10;
};

struct UnkEmmySearchingOwner_A3_b64 {
    u8 pad_0x0[0x240];
    UnkEmmySearchingAI_A3_b64* ai;
};

struct UnkEmmySearchingAction_A3_b64 {
    u8 pad_0x0[0x18];
    UnkEmmySearchingOwner_A3_b64* owner;
    u8 pad_0x20[0x160 - 0x20];
    f32 field_0x160;
};

void fn_7100515904(UnkEmmySearchingAction_A3_b64* self, f32 dt);

void fn_7100516964(UnkEmmySearchingAction_A3_b64* self, f32 dt) {
    UnkEmmySearchingAI_A3_b64* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934())) {
        f32* time = &ai->field_0x14c10;
        if (*time > 0.0f) {
            self->field_0x160 += dt;
            if (self->field_0x160 >= *time)
                fn_7100544a34(reinterpret_cast<UnkEmmyActor_b55*>(ai), g_unk_71726ded60, true);
        }
    }
    fn_7100515904(self, dt);
}
