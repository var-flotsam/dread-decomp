// CCharClassGroundShockerAIComponent unit: answers 7 unless the owner's AI component is a GroundShocker AI that is
// busy and the string id taken from the argument differs from the id table's id at +0xec8. Names and layouts
// are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;

struct UnkGameManagerIdTable {
    u8 pad_0x0[0xec8];
    CStrId field_0xec8;
    u8 pad_0xed0[0x2578 - 0xed0];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};

inline const UnkGameManagerIdTable& IdTable_l5a3a60() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

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
u32 fn_71005a4be8(UnkShockerAI_b62a3* self);

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
        const UnkGameManagerIdTable& table = IdTable_l5a3a60();
        const UnkGameManagerIdTable* t = &table;
        // keeps the id table's address loaded before the id's data, as the original does
        asm("" : "+r"(t));
        return id->data == t->field_0xec8.data ? 7 : 1;
    }
    return 7;
}
