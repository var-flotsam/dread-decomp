// CRodotukAIComponent unit (tools/tu_map.py unit 992): after the base update (fn_7100369650), finds the
// position of the wanted id (+0x14860) among the 0x30-byte entries of the definition's array at +0x518
// (the definition at +0x14858), counting it in +0x14864. Names and layouts are placeholders.
#include "types.h"

#include "game/UnkBehaviorTree.h"

struct UnkRodotukEntry_rai {
    u32 id;
    u8 pad_0x4[0x2c];
};

struct UnkRodotukDef_rai {
    u8 pad_0x0[0x518];
    UnkRodotukEntry_rai* entries;  // 0x518
    u32 count;  // 0x520
};

// The count is stored through a may_alias type: the original reloads the array after each store.
typedef u32 __attribute__((may_alias)) AliasU32_rai;

struct UnkRodotukAI_rai {
    u8 pad_0x0[0x14858];
    UnkRodotukDef_rai* def;  // 0x14858
    u32 id;  // 0x14860
    u32 index;  // 0x14864
};

void fn_71006722ac(UnkRodotukAI_rai* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkRodotukDef_rai* def = self->def;
    if (!def)
        return;
    u32 id = self->id;
    if (!id)
        return;
    AliasU32_rai* index = &self->index;
    *index = 0;
    for (u64 i = 0; i < def->count; ++i) {
        if (def->entries[i].id == id)
            return;
        *index = i + 1;
    }
}
