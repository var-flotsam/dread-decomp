// CActorDef's unit (tools/tu_map.py unit 345): the first entry of the definition's array of entry
// pointers (+0xa0, count +0xa8) whose descriptor (+0x10) has the hash (+0x30) of `name` built as a
// base::global::CRntString; each descriptor tested has its counter at +0x44 bumped. Null when none
// does. Names are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

typedef s32 __attribute__((may_alias)) UnkAliasS32_A1b71;

struct UnkActorDefInfo_A1b71 {
    u8 pad_0x0[0x30];
    s64 hash;  // 0x30
    u8 pad_0x38[0xc];
    UnkAliasS32_A1b71 counter;  // 0x44
};

struct UnkActorDefEntry_A1b71 {
    u8 pad_0x0[0x10];
    UnkActorDefInfo_A1b71* info;  // 0x10
};

struct UnkActorDef_A1b71 {
    u8 pad_0x0[0xa0];
    UnkActorDefEntry_A1b71** entries;  // 0xa0
    u32 count;                         // 0xa8
};

UnkActorDefEntry_A1b71* fn_71002552a4(const UnkActorDef_A1b71* self, const char* name) {
    UnkActorDefEntry_A1b71** it = self->entries;
    UnkActorDefEntry_A1b71** end = it + self->count;
    for (; it != end; ++it) {
        UnkActorDefEntry_A1b71* entry = *it;
        s64 hash, key;
        {
            base::global::CRntString str(name);
            ++*reinterpret_cast<UnkAliasS32_A1b71*>(&entry->info->counter);
            hash = entry->info->hash;
            key = str.key;
        }
        UnkActorDefEntry_A1b71* found = *it;
        asm volatile("" : "+r"(found));
        if (hash == key)
            return found;
    }
    return nullptr;
}
