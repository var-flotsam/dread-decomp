// CCharClassChozoCommanderAIComponent's unit (tools/tu_map.py): the id an actor's Chozo Commander AI (its
// AI component at +0x240, cast to CChozoCommanderAIComponent) answers with: g_unk_71726c9250 while its
// phase (+0x1140) is 1, otherwise the id at +0x23a8 of the id table. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

// The id table (fn_7100031894 builds it; as src/game/CEmmyAIComponent/CEmmyAIComponent_IsStateId_b56.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x23a8];
    u8 field_0x23a8[8];
    u8 pad_0x23b0[0x2578 - 0x23b0];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

extern u8 g_unk_71726c9250[] __attribute__((visibility("hidden")));

struct UnkChozoCommanderAI_ovb0088 {
    u8 pad_0x0[0x1140];
    s32 phase;  // 0x1140
};

struct UnkChozoCommanderCastTag_ovb0088;
template <>
struct base::reflection::UnkCastTraits<UnkChozoCommanderCastTag_ovb0088> {
    static const base::reflection::CClass* type() { return fn_710044e708(); }
};

const void* fn_710044e264(void* self, UnkActor* actor) {
    auto* ai = reinterpret_cast<UnkChozoCommanderAI_ovb0088*>(
        base::reflection::refl_CastTo<UnkChozoCommanderCastTag_ovb0088>(
            static_cast<base::reflection::UnkReflObject*>(actor->field_0x240)));
    if (ai->phase == 1)
        return g_unk_71726c9250;
    return GameManagerIdTable().field_0x23a8;
}
