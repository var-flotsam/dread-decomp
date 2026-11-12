// UnkTU2566 (minimap tunables): finds the entry for an id in a keyed table, adding a zeroed one with the
// default 3-float value for that id when missing. Names and layouts are placeholders.
#include "game/UnkTU2566.h"
#include "types.h"

char* fn_7100ea9dcc(char* p0);
void fn_7100ea9c1c(char* p0, u32 p1);

struct UnkEntry_le98090 {
    u64 field_0x0[6];
    u32 field_0x30;
    f32 value[3];
    u64 field_0x40[2];
};
static_assert(sizeof(UnkEntry_le98090) == 0x50, "x");

struct UnkEntryTable_le98090 {
    UnkEntry_le98090* entries;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 grow;
};

static inline UnkEntry_le98090* FindEntry(UnkEntryTable_le98090* self, u32 id) {
    u32 count = self->count;
    const u32* key = self->keys;
    if (count != 0) {
        u64 end = u64(count) * sizeof(UnkEntry_le98090);
        u64 off = 0;
        do {
            // keeps the byte offset as the loop counter instead of a negated induction variable
            asm("" : "+r"(off));
            if (*key == id)
                return reinterpret_cast<UnkEntry_le98090*>(reinterpret_cast<u8*>(self->entries) + off);
            off += sizeof(UnkEntry_le98090);
            key++;
        } while (off != end);
    }
    return &self->entries[count];
}

// NON_MATCHING(minor): same instructions; register assignment differs (x8/x9 swapped in the search and insert path)
void fn_7100e98090(UnkEntryTable_le98090* self, u32 id) {
    UnkEntry_le98090* entry = FindEntry(self, id);
    u32 count = self->count;
    u32 index = count;
    if (entry == &self->entries[self->count]) {
        if (count == self->capacity) {
            fn_7100ea9c1c(reinterpret_cast<char*>(self), self->grow + count);
            index = self->count;
        }
        self->keys[index] = id;
        UnkEntry_le98090* slot = self->entries + self->count;
        for (int k = 0; k < 5; k++)
            slot->field_0x0[k * 2] = slot->field_0x0[k * 2 + 1] = 0;
        fn_7100ea9dcc(reinterpret_cast<char*>(slot));
        u32 n = self->count + 1;
        self->count = n;
        entry = n == 0 ? self->entries : &self->entries[n] - 1;

        const f32* src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x24);
        switch (id) {
        case 1: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x12c); break;
        case 2: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x138); break;
        case 3: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x144); break;
        case 4: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x150); break;
        case 5: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x15c); break;
        case 6: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x168); break;
        case 7: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x174); break;
        case 9: src = reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71727ca410) + 0x180); break;
        default: break;
        }
        entry->value[0] = src[0];
        entry->value[1] = src[1];
        entry->value[2] = src[2];
    }
    entry->field_0x30 = id;
}
