// CCharClassGoliathAIComponent's unit (tools/tu_map.py unit 843): fn_7100590414 gives 4, unless the actor's AI
// component is a CGoliathAIComponent whose vf_0x868 rejects the info's +0x60: then 5.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkGoliathAI_l590414 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkGoliathAI_l590414*)>(vtable[0])(this);
    }
    bool vf_0x868(void* arg) {
        return reinterpret_cast<bool (*)(UnkGoliathAI_l590414*, void*)>(vtable[0x868 / 8])(this, arg);
    }
};

struct UnkGoliathInfo_l590414 {
    u8 pad_0x0[0x60];
    void* field_0x60;
};

// The actor's AI component when it is a CGoliathAIComponent, otherwise null.
static inline UnkGoliathAI_l590414* GoliathAI(UnkActor* actor) {
    UnkGoliathAI_l590414* ai = static_cast<UnkGoliathAI_l590414*>(actor->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71005908ac()))
        return ai;
    return nullptr;
}

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
u32 fn_7100590414(void* self, UnkActor* actor, void* arg, UnkGoliathInfo_l590414* info) {
    if (UnkGoliathAI_l590414* ai = GoliathAI(actor)) {
        if (!ai->vf_0x868(info->field_0x60))
            return 5;
    }
    return 4;
}
