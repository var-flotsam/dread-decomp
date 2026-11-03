// CMinimapManager::CTunableMinimapManager unit (tools/tu_map.py, the name from reflection): a tunable
// flag setter. The byte picked from the argument's holder (inline, or behind a pointer for 9 or more
// entries or kind 1) is stored at +0xd641c4 of the object at 8 when it differs; then the unit's cached
// flag (g_unk_71727cc998, also read by src/game/CMinimapManager.cpp) and the byte two after it are
// cleared. Returns an empty result of kind 2. Names and layouts are placeholders.
#include "types.h"

extern bool g_unk_71727cc998 __attribute__((visibility("hidden")));

struct UnkValueSet_mmt {
    u8 unk_0x0[0x18];
    u64 count;
};

struct UnkValueSource_mmt {
    u8* holder;
    UnkValueSet_mmt* set;
    u8 unk_0x10[4];
    s32 kind;
};

struct UnkResult_0x18_mmt {
    u64 empty[2];
    u64 kind_and_flag;
};

struct UnkMinimapTunableFlags_mmt {
    u8 field_0x0;
    u8 field_0x1;
    u8 field_0x2;
};

struct UnkMinimapTunableOwner_mmt {
    u8 unk_0x0[8];
    u8* target;
};

UnkResult_0x18_mmt fn_7100eaa590(void* unused, UnkMinimapTunableOwner_mmt* owner, UnkValueSource_mmt** source) {
    UnkMinimapTunableFlags_mmt* flags = reinterpret_cast<UnkMinimapTunableFlags_mmt*>(owner->target + 0xd641c4);
    UnkValueSource_mmt* src = *source;
    u8* slot;
    if (src->kind == 1 || src->set->count >= 9)
        slot = src->holder;
    else
        slot = reinterpret_cast<u8*>(src);
    u8 value = *slot;
    if (flags->field_0x0 != value) {
        flags->field_0x0 = value;
        g_unk_71727cc998 = false;
        flags->field_0x2 = 0;
    }
    UnkResult_0x18_mmt result = {};
    result.kind_and_flag = 0x100000002;
    return result;
}
