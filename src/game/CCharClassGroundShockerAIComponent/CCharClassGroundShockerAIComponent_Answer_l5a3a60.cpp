// CCharClassGroundShockerAIComponent unit: answers 7 unless the owner's AI component is a GroundShocker AI that is
// busy and the string id taken from the argument differs from the id table's id at +0xec8. Names and layouts
// are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CGroundShockerAIComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdHolder_l5a3a60 {
    u8 pad_0x0[0x60];
    u8* field_0x60;
};

struct UnkShockerComponent_l5a3a60 {
    virtual const base::reflection::CType* vf_0x0();
};

struct UnkShockerOwner_l5a3a60 {
    u8 pad_0x0[0x240];
    UnkShockerComponent_l5a3a60* field_0x240;
};

struct UnkShockerAI_b62a3;

u32 fn_71005a3a60(void* unused0, UnkShockerOwner_l5a3a60* owner, void* unused1, UnkIdHolder_l5a3a60* holder) {
    UnkShockerComponent_l5a3a60* comp = owner->field_0x240;
    if (comp && !fn_710009d50c(comp->vf_0x0(), fn_71005a4004()))
        comp = nullptr;
    CStrId* id = &g_unk_7101d470e0;
    if (holder) {
        u8* inner = holder->field_0x60;
        if (inner)
            id = reinterpret_cast<CStrId*>(inner + 0x20);
    }
    if (comp && id->data != &g_unk_7101d47108->empty && __builtin_expect(fn_71005a4be8(reinterpret_cast<UnkShockerAI_b62a3*>(comp)) & 1, 1)) {
        const UnkGameManagerIdTable& table = GameManagerIdTable();
        const UnkGameManagerIdTable* t = &table;
        // keeps the id table's address loaded before the id's data, as the original does
        asm("" : "+r"(t));
        return id->data == t->field_0xec8.data ? 7 : 1;
    }
    return 7;
}
