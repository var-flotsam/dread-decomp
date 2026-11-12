// UnkTU2566 (minimap tunables): looks up the entry of a keyed table for a type id, with a few ids
// that never have one. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

extern u64 g_unk_71727c8198;
extern u64 g_unk_71727c81a8;
extern u64 g_unk_71727c81c0;
extern u64 g_unk_71727c81d0;
extern u64 g_unk_71727c81b0;
extern u64 g_unk_71727c81e0;
extern u64 g_unk_71727c8108;
extern u64 g_unk_71727c80f8;
extern u64 g_unk_71727c8100;

struct UnkEntryTable_ea1b94 {
    u8 pad_0x0[0xa8];
    u8* entries;
    u64* keys;
    u32 count;
};

struct UnkOwner_ea1b94 {
    u8 pad_0x0[0x60];
    UnkEntryTable_ea1b94* table;
};

template <u64& Id>
static inline u8* Search(UnkEntryTable_ea1b94* t) {
    u64 id = Id;
    u32 count = t->count;
    u64* key = t->keys;
    u64 off = 0;
    if (count != 0) {
        u64 end = u64(count) * 200;
        do {
            // keeps the byte offset as the loop counter instead of a negated induction variable
            asm("" : "+r"(off));
            if (*key == id)
                return t->entries + off;
            off += 200;
            key++;
        } while (off != end);
    }
    return t->entries + u64(count) * 200;
}

u8* fn_7100ea1b94(UnkOwner_ea1b94* self, u64* type) {
    u64 t = *type;
    u64 empty = reinterpret_cast<u64>(&g_unk_7101d47108->empty);
    if ((t == g_unk_71727c81a8) | (t == g_unk_71727c8198) | (t == empty))
        return nullptr;
    UnkEntryTable_ea1b94* table = self->table;
    u8* found;
    if (t == g_unk_71727c81c0) {
        found = Search<g_unk_71727c80f8>(table);
    } else if (t == g_unk_71727c81d0) {
        found = Search<g_unk_71727c8100>(table);
    } else if (t == g_unk_71727c81b0) {
        found = Search<g_unk_71727c80f8>(table);
    } else if (t == g_unk_71727c81e0) {
        return nullptr;
    } else {
        found = Search<g_unk_71727c8108>(table);
    }
    return found == table->entries + u64(table->count) * 200 ? nullptr : found;
}
