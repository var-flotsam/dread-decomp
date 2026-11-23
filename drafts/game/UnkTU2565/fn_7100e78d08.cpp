// UnkTU2565 (tools/tu_map.py unit 2565): loads the map grid's tile kinds from the game blackboard. The rows
// are kept as strings of "<count><char>" runs under the grid's (remapped) section; each run sets that many
// cells to the char's index in the tile alphabet. Then every cell is marked and the grid's bounds are
// recomputed from the cells that are set and solid. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2565.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector2D;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

// A row's text, kept with inline storage (0x248 bytes).
struct UnkGridRowText_e78d08 {
    const char* text;
    u8 storage[0x240];

    const char* CStr() const { return text ? text : g_unk_710162e2ed; }
    void Destroy() {
        CRntString_Clear(reinterpret_cast<CRntString*>(this));
        CRntString_Destroy(reinterpret_cast<CRntString*>(this));
    }
};

// The rows as the blackboard holds them: the texts and the row index each one is for.
struct UnkGridRows_e78d08 {
    UnkGridRowText_e78d08* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkGridRows_e78d08()
        : values(nullptr), keys(nullptr), count(0), capacity(0), step(10), flags(0xe), allocator(GetDefaultAllocator()) {}
    ~UnkGridRows_e78d08() {
        if (values) {
            for (u64 i = 0; i < count; ++i)
                values[i].Destroy();
        }
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// The tile alphabet: a cell's kind is its char's index.
struct UnkCharList_e78d08 {
    const u8* chars;
    u32 count;
};

struct UnkGridCell_e78d08 {
    f32 x;
    f32 y;
    u8 kind;  // 0x8
    u8 pad_0x9[0x2f];
    bool field_0x38;
    u8 pad_0x39[0x9];
    bool field_0x42;
    u8 pad_0x43[0x5];
};
static_assert(sizeof(UnkGridCell_e78d08) == 0x48, "UnkGridCell_e78d08");

struct UnkGridBounds_e78d08 {
    CVector2D min;
    CVector2D max;
};

struct UnkGrid_e78d08 {
    UnkGridCell_e78d08 cells[300][650];
    u8 pad_0xd63bc0[0x8];
    CStrId section;  // 0xd63bc8
    u8 pad_0xd63bd0[0x10];
    UnkGridBounds_e78d08 bounds;  // 0xd63be0
};
static_assert(__builtin_offsetof(UnkGrid_e78d08, bounds) == 0xd63be0, "UnkGrid_e78d08");

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The value slot the id names in the section's properties, or the slot past the last one.
static inline UnkReflValue* FindSlot(UnkPropertyArrays* properties, const CStrId& id) {
    for (u64 i = 0; i < properties->count; ++i) {
        if (*reinterpret_cast<const u64*>(&properties->ids[i]) == *reinterpret_cast<const u64*>(&id))
            return &static_cast<UnkReflValue*>(properties->values)[i];
    }
    return &static_cast<UnkReflValue*>(properties->values)[properties->count];
}

// Reads the rows property of the section into `rows`.
static inline bool ReadRows(CBlackboard::CSection* section, const CStrId& id, UnkGridRows_e78d08* rows) {
    UnkPropertyArrays* properties = &section->data.properties;
    UnkReflValue* v = FindSlot(properties, id);
    if (v == &static_cast<UnkReflValue*>(properties->values)[properties->count])
        return false;
    const char* type = v->type;
    if (!type)
        return false;
    void* p = v->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? v->field_0x0 : v;
    if (!p)
        return false;
    const base::reflection::CType* t = reinterpret_cast<const base::reflection::CType*>(&g_unk_71727c9208);
    UnkTypedRef dst{t, rows};
    UnkTypedRef src;
    fn_71000a16e0(&src, type, p);
    return fn_710009d7a8(t, &dst, &src);
}

// The index of `c` in the alphabet, or -1.
static inline s32 IndexOf(const UnkCharList_e78d08* list, u8 c) {
    for (u32 i = 0; i < list->count; ++i) {
        if (list->chars[i] == c)
            return (s32)i;
    }
    return -1;
}

static inline bool IsDigit(u8 c) {
    return (u32)(c - '0') < 10;
}

void fn_7100e78d08(UnkGrid_e78d08* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId name = fn_7100dfbda4(manager, self->section);
    UnkGridRows_e78d08 rows;
    const UnkQualifiedId& id = GetQualifiedId(g_unk_71727c94c0);
    CBlackboard::CSection* section = fn_7101047270(blackboard, name);
    if (!section || !ReadRows(section, id.id, &rows))
        return;

    UnkGridBounds_e78d08* bounds = &self->bounds;
    UnkCharList_e78d08* alphabet = reinterpret_cast<UnkCharList_e78d08*>(&g_unk_71727c8f58);
    UnkGridCell_e78d08* cells = &self->cells[0][0];
    UnkGridRowText_e78d08* row = rows.values;
    for (u32 *key = rows.keys, *end = rows.keys + rows.count; key != end; ++key, ++row) {
        const char* str = row->CStr();
        s32 base = *key * 650;
        s32 col = 0;
        s32 pos = 0;
        while (str[pos]) {
            s32 n = atoi(str + pos);
            // Past the count's digits to the run's char.
            const char* p = row->text + pos;
            ++pos;
            u8 c;
            do {
                c = *++p;
                ++pos;
            } while (IsDigit(c));
            s32 kind = IndexOf(alphabet, c);
            if (n > 0 && kind >= 0 && kind < 7) {
                for (s32 i = 0; i < n; ++i)
                    cells[base + col++].kind = (u8)kind;
            }
        }
    }

    bounds->min.x = 3.4028235e38f;
    bounds->min.y = 3.4028235e38f;
    bounds->max.x = -3.4028235e38f;
    bounds->max.y = -3.4028235e38f;
    for (s32 r = 0; r < 300; ++r) {
        for (s32 c = 0; c < 650; ++c) {
            UnkGridCell_e78d08* cell = &self->cells[r][c];
            cell->field_0x42 = true;
            if (cell->field_0x38 && cell->kind >= 2) {
                f32 x = cell->x;
                f32 y = cell->y;
                bounds->min.x = bounds->min.x < x ? bounds->min.x : x;
                bounds->min.y = bounds->min.y < y ? bounds->min.y : y;
                bounds->max.x = bounds->max.x > x + 100.0f ? bounds->max.x : x + 100.0f;
                bounds->max.y = bounds->max.y > y + 100.0f ? bounds->max.y : y + 100.0f;
            }
        }
    }
}
