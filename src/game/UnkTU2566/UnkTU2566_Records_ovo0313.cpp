// Unit 2566 (tools/tu_map.py; hint CMinimapManager::CTunableMinimapManager): a refresh of the owner's
// items for a map id when its scenario is the current one (fn_7100e95130), and the erase of a record
// table of ids and words (fn_7100e98348). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2565.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

#define HIDDEN __attribute__((visibility("hidden")))

struct UnkMinimapOwner;
struct UnkMapData;

// The minimap owner's layout as fn_7100e95130 uses it.
struct UnkMmOwner_o0313 {
    u8 unk_0x0[0x110];
    CStrId field_0x110;
};

const base::global::CStrId* fn_7100de9bf8(UnkGameManager* self);

void fn_7100e95130(UnkMapData* map, u64 id) {
    UnkMmOwner_o0313* self = (UnkMmOwner_o0313*)map;
    if (self->field_0x110.data == &g_unk_7101d47108->empty)
        return;
    const CStrId* current = fn_7100de9bf8(g_unk_7101d47310);
    if (self->field_0x110.data != current->data)
        return;
    u32 count = fn_7100e791a4((u8*)self + 0x118, (void*)id, 0, true, g_unk_71727cc99c);
    fn_7100e951d4((UnkMinimapOwner*)self, count, g_unk_71727cc99c, 1);
}

// A string id (released in two steps) and five words; 0x20 bytes.
struct UnkIdRecord_o0313 {
    UnkStringData* id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 pad_0x1c;

    void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    fn_71000803a4(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }
};

// Records and the words that belong to them, in parallel arrays.
struct UnkIdRecordTable_o0313 {
    UnkIdRecord_o0313* records;
    u32* words;
    union {
        u32 field_0x10;
    };
};

// Removes `it` by moving the last record (and its word) into its place.
UnkIdRecord_o0313* fn_7100e98348(UnkIdRecordTable_o0313* self, UnkIdRecord_o0313* it) {
    u32 index = it - self->records;
    self->records[index].Release();
    u32 last = self->field_0x10 - 1;
    if (last != index) {
        UnkIdRecord_o0313* dst = &self->records[index];
        UnkIdRecord_o0313* src = &self->records[last];
        dst->id = src->id;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&src->id, &empty, sizeof(empty));
        dst->field_0x8 = src->field_0x8;
        dst->field_0xc = src->field_0xc;
        dst->field_0x10 = src->field_0x10;
        dst->field_0x14 = src->field_0x14;
        dst->field_0x18 = src->field_0x18;
        self->records[self->field_0x10 - 1].Release();
        self->words[index] = self->words[self->field_0x10 - 1];
    }
    --self->field_0x10;
    return &self->records[index];
}
