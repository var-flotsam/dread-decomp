// Unit 2669: releases one reference to the script named `name` (ignoring case). When the last goes, its
// Lua global is cleared, its entry removed and the script deleted. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CRntString.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU57.h"
#include "types.h"

#include "../../../src/game_aca57c/UnkBufferString.h"

using base::global::CRntString;

struct UnkScript_l09d890 {
    CRntString name;     // 0x0
    CRntString path;     // 0x30
    // A bit-field in the original: its decrement is reloaded (bit-fields carry no type-based aliasing).
    s32 references : 32;  // 0x60
};

struct UnkScriptEntry_l09d890 {
    CRntString name;              // 0x0
    UnkScript_l09d890* script;    // 0x30
    u8 pad_0x38[0x10];
    UnkScriptEntry_l09d890* next;  // 0x48
};

struct UnkScriptTable_l09d890 {
    u8 pad_0x0[0x10];
    UnkScriptEntry_l09d890** buckets;  // 0x10
    u32 count;                         // 0x18
    u32 bucket_count;                  // 0x1c

    UnkScriptEntry_l09d890* Find(const CRntString& key) {
        if (!count)
            return nullptr;
        u32 hash = (u32)(key.key >> 32) ^ (u32)key.key;
        u32 index = hash % bucket_count;
        UnkScriptEntry_l09d890** bucket = &buckets[index];
        if (!bucket)
            return nullptr;
        for (UnkScriptEntry_l09d890* entry = *bucket; entry; entry = entry->next) {
            if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&entry->name),
                              reinterpret_cast<const UnkCompared_0x30*>(&key)))
                return entry;
        }
        return nullptr;
    }
};

struct UnkScriptTable_ovb0245;
struct UnkScriptItem_ovb0245;
UnkScriptItem_ovb0245* fn_710109da60(UnkScriptTable_ovb0245* self, UnkScriptItem_ovb0245* key);

using UnkScriptName_l09d890 = UnkBufferString<0x100, g_unk_7101a194b8>;

// Copies `src` lower-cased (ASCII) into `dst`.
static inline void ToLower(char* dst, const char* src) {
    dst[0] = '\0';
    u32 length = __builtin_strlen(src);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            dst[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        dst[length] = '\0';
    }
}

void fn_710109d890(UnkScriptManager_ovb0085* self, base::global::CRntString* name) {
    char buffer[0x100];
    ToLower(buffer, name->data);
    UnkScriptName_l09d890 key(buffer);
    UnkScriptTable_l09d890* table = reinterpret_cast<UnkScriptTable_l09d890*>(self);
    if (UnkScriptEntry_l09d890* entry = table->Find(*key.Str())) {
        entry->script->references--;
        if (entry->script->references == 0) {
            lua_pushnil(g_unk_7172a01d18);
            lua_setglobal(g_unk_7172a01d18, entry->name.data);
            UnkScript_l09d890* script = entry->script;
            fn_710109da60(reinterpret_cast<UnkScriptTable_ovb0245*>(table),
                          reinterpret_cast<UnkScriptItem_ovb0245*>(entry));
            delete script;
            fn_710109d0a0();
        }
    }
}
