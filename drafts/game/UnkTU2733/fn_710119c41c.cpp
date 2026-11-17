// The language singleton's unit (tools/tu_map.py, unit 2733): the game's language index for the
// system's desired language code, or 0x7fffffff for a code it does not know (fn_710119c41c).
// Names and layouts are placeholders.
#include <nn/oe.h>
#include <nn/settings.h>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CRntSmallDictionary;

// The key of a language code: its string's hash.
struct LanguageKey_l19c41c {
    u64 hash;

    explicit LanguageKey_l19c41c(const nn::settings::LanguageCode& code) { fn_710006663c(&hash, code.code); }
    bool operator==(const LanguageKey_l19c41c& other) const { return hash == other.hash; }
};

using LanguageTable_l19c41c = CRntSmallDictionary<LanguageKey_l19c41c, s32>;

// Appends an entry, growing both arrays by the table's step when they are full.
static inline void Add_l19c41c(LanguageTable_l19c41c& table, const LanguageKey_l19c41c& key, const s32& value) {
    if (table.count == table.capacity) {
        u32 new_capacity = table.capacity + table.step;
        if (table.count < new_capacity) {
            s32* values = static_cast<s32*>(
                table.allocator->vf_0x40((u64)new_capacity * sizeof(s32), 8, table.flags));
            LanguageKey_l19c41c* keys = static_cast<LanguageKey_l19c41c*>(
                table.allocator->vf_0x40((u64)new_capacity * sizeof(LanguageKey_l19c41c), 8, table.flags));
            if (table.values) {
                __builtin_memcpy(values, table.values, (u64)table.count * sizeof(s32));
                table.allocator->vf_0x48(table.values);
            }
            if (table.keys) {
                for (u32 i = 0; i < table.count; ++i)
                    keys[i] = table.keys[i];
                table.allocator->vf_0x48(table.keys);
            }
            table.values = values;
            table.keys = keys;
            table.capacity = new_capacity;
        }
    }
    table.keys[table.count] = key;
    table.values[table.count] = value;
    ++table.count;
}

static inline bool Contains_l19c41c(const LanguageTable_l19c41c& table, const LanguageKey_l19c41c& key) {
    for (u32 i = 0; i < table.count; ++i)
        if (table.keys[i] == key)
            return true;
    return false;
}

s32 fn_710119c41c() {
    LanguageTable_l19c41c table(0xe);
    Add_l19c41c(table, LanguageKey_l19c41c({"en-GB"}), 7);
    Add_l19c41c(table, LanguageKey_l19c41c({"es"}), 2);
    Add_l19c41c(table, LanguageKey_l19c41c({"fr"}), 3);
    Add_l19c41c(table, LanguageKey_l19c41c({"de"}), 4);
    Add_l19c41c(table, LanguageKey_l19c41c({"nl"}), 5);
    Add_l19c41c(table, LanguageKey_l19c41c({"it"}), 6);
    Add_l19c41c(table, LanguageKey_l19c41c({"en-US"}), 7);
    Add_l19c41c(table, LanguageKey_l19c41c({"es-419"}), 8);
    Add_l19c41c(table, LanguageKey_l19c41c({"fr-CA"}), 9);
    Add_l19c41c(table, LanguageKey_l19c41c({"ja"}), 10);
    Add_l19c41c(table, LanguageKey_l19c41c({"ko"}), 11);
    Add_l19c41c(table, LanguageKey_l19c41c({"zh-Hant"}), 12);
    Add_l19c41c(table, LanguageKey_l19c41c({"zh-Hans"}), 13);
    Add_l19c41c(table, LanguageKey_l19c41c({"ru"}), 14);

    LanguageKey_l19c41c key(nn::oe::GetDesiredLanguage());
    s32 language = 0x7fffffff;
    if (Contains_l19c41c(table, key))
        language = *table.Find(key);

    table.allocator->vf_0x48(table.keys);
    table.allocator->vf_0x48(table.values);
    return language;
}
