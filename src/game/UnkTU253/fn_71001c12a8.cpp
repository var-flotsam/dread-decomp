// Unit 253: picks the three values at +0x128 from the first entry of the owner's list whose name
// contains neither "CH_" nor "DC_", or from the owner's own defaults when the global flag is clear or
// no entry qualifies. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringEntry;

// The global object as this function reads it: one more flag than game/UnkGlobal_710b372a98.h shows.
struct UnkGlobalView_l1c12a8 {
    u8 pad_0x0[0x3183660];
    bool field_0x3183660;
};

struct UnkEntry_l1c12a8 {
    u8 pad_0x0[0x24];
    u32 field_0x24;
    u8 pad_0x28[0xc];
    u32 field_0x34;
    u8 pad_0x38[0xc];
    u32 field_0x44;
    u8 pad_0x48[0x20];
    CStrId name;
    u8 pad_0x70[0x28];
};
static_assert(sizeof(UnkEntry_l1c12a8) == 0x98, "entry size");

struct UnkEntryList_l1c12a8 {
    u8 pad_0x0[8];
    UnkEntry_l1c12a8* entries;
    u8 pad_0x10[8];
    u32 count;
};

struct UnkInfo_l1c12a8 {
    u8 pad_0x0[0x80];
    UnkEntryList_l1c12a8* list;
};

struct UnkOwner_l1c12a8 {
    u8 pad_0x0[0x38];
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u8 pad_0x44[0x34];
    UnkInfo_l1c12a8* info;
    u8 pad_0x80[0xa8];
    u32 field_0x128;
    u32 field_0x12c;
    u32 field_0x130;
};
static_assert(__builtin_offsetof(UnkOwner_l1c12a8, field_0x128) == 0x128, "layout");

struct UnkSlotsOwner_ovb0176;

// Whether the id's text contains `str`; reading the text counts a use.
static inline bool NameContains(const CStrId& id, const char* str) {
    UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
    ++entry->tail.field_0x44;
    return id.data != &g_unk_7101d47108->empty && fn_710007fb9c(&entry->buffer, str);
}

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_71001c12a8(UnkSlotsOwner_ovb0176* self) {
    UnkOwner_l1c12a8* owner = reinterpret_cast<UnkOwner_l1c12a8*>(self);
    UnkEntry_l1c12a8* found = nullptr;
    if (reinterpret_cast<UnkGlobalView_l1c12a8*>(g_unk_710b372a98)->field_0x3183660) {
        UnkEntryList_l1c12a8* list = owner->info->list;
        if (list) {
            u32 count = list->count;
            for (u64 i = 0; i < count && !found; ++i) {
                UnkEntry_l1c12a8* entry = &list->entries[i];
                bool plain = !NameContains(entry->name, "CH_") && !NameContains(entry->name, "DC_");
                found = plain ? entry : nullptr;
            }
        }
    }
    if (found) {
        owner->field_0x128 = found->field_0x24;
        owner->field_0x12c = found->field_0x34;
        owner->field_0x130 = found->field_0x44;
    } else {
        owner->field_0x128 = owner->field_0x38;
        owner->field_0x12c = owner->field_0x3c;
        owner->field_0x130 = owner->field_0x40;
    }
}
