// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): fn_7100519b48. When the
// actor's AI component (+0x240) is a CEmmyAIComponent with the flag at +0x29f24 clear, it resets it
// (fn_7100541de4) and enters state g_unk_71726ded60; otherwise it hands the cast component (or null)
// and the actor's target to fn_7100547428. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkEmmyActor_b55;
extern char g_unk_71726ded60[];
void fn_7100547428(UnkEmmyActor_b55* self, bool a, UnkActor* target, bool b);

struct UnkEmmyAICast_b63a3 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEmmyAICast_b63a3*)>(vtable[0])(this);
    }
};

struct UnkEmmySearchingAction_b63a3 {
    u8 pad_0x0[0x18];
    UnkActor* actor;
};

void fn_7100519b48(UnkEmmySearchingAction_b63a3* self) {
    auto* ai = static_cast<UnkEmmyAICast_b63a3*>(self->actor->field_0x240);
    if (ai) {
        if (!fn_710009d50c(ai->vf_0x0(), fn_710053b934())) {
            ai = nullptr;
        } else if (!reinterpret_cast<char*>(ai)[0x29f24]) {
            auto* emmy = reinterpret_cast<UnkEmmyActor_b55*>(ai);
            fn_7100541de4(emmy);
            fn_7100544a34(emmy, g_unk_71726ded60, true);
            return;
        }
    }
    fn_7100547428(reinterpret_cast<UnkEmmyActor_b55*>(ai), true, fn_7100d7c990(self->actor), false);
}
