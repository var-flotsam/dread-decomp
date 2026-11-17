// Unit 2565 (tools/tu_map.py): writes the run-length texts of the given rows of the minimap grid into
// the game blackboard's section for the grid's (remapped) id: the texts stored there are read, each
// listed row's text is added or rewritten, and the result is stored back unless the blackboard is
// locked. Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2565.h"
#include "types.h"

using base::global::CStrId;
using UnkRowText_le785e4 = UnkBufferString<0x200, g_unk_7101a1a5a8>;

struct UnkRowCell_b0238;
struct UnkString200Array;
void fn_7100e7bc3c(UnkString200Array* self, u32 capacity);
void fn_7100e7a110(UnkRowCell_b0238* cells, s32 row, base::global::CRntString* out);

// The row texts by row: a text per row and the rows alongside.
struct UnkRowTexts_le785e4 {
    UnkRowText_le785e4* values = nullptr;
    u32* keys = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;
    u32 flags = 0xe;
    base::UnkAllocator* allocator = fn_7100083990();

    ~UnkRowTexts_le785e4() {
        if (values && count) {
            for (u32 i = 0; i < count; ++i)
                values[i].~UnkRowText_le785e4();
        }
        u32* k = keys;
        count = 0;
        if (k)
            allocator->vf_0x48(k);
        if (values)
            allocator->vf_0x48(values);
    }

    UnkRowText_le785e4* End() { return &values[count]; }
    UnkRowText_le785e4* Find(u32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    // Appends an empty text for `key` and returns it.
    UnkRowText_le785e4* Add(u32 key) {
        if (count == capacity)
            fn_7100e7bc3c(reinterpret_cast<UnkString200Array*>(this), step + count);
        keys[count] = key;
        new (&values[count]) UnkRowText_le785e4();
        u32 n = ++count;
        UnkRowText_le785e4* end = &values[n];
        return n ? &end[-1] : end;
    }
};

// The texts' reflected type.
struct UnkRowTextsTag_le785e4;
template <>
struct base::reflection::UnkFieldTraits<UnkRowTextsTag_le785e4> {
    static const CType* type() { return reinterpret_cast<const CType*>(&g_unk_71727c9208); }
};

// The grid as this function sees it: the cells, then its id.
struct UnkMinimapGrid_le785e4 {
    u8 cells[0xd63bc8];
    CStrId id;  // 0xd63bc8
};

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// `rows` is a count followed by that many row numbers.
void fn_7100e785e4(UnkMinimapGrid_le785e4* self, const u32* rows) {
    if (rows[0] == 0)
        return;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId key = fn_7100dfbda4(manager, self->id);
    UnkRowTexts_le785e4 texts;
    {
        const UnkQualifiedId& name = Qid(g_unk_71727c94c0);
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, key))
            base::refl_GetKeyedValue<UnkRowTextsTag_le785e4>(
                reinterpret_cast<char*>(section), reinterpret_cast<const base::UnkStrIdMember*>(&name.id), &texts);
    }
    const u32* it = rows;
    u32 n = *it++;
    for (const u32* end = it + n; it != end; ++it) {
        u32 row = *it;
        UnkRowText_le785e4* text = texts.Find(row);
        if (text == texts.End())
            text = texts.Add(row);
        fn_7100e7a110(reinterpret_cast<UnkRowCell_b0238*>(self), row, text->Str());
    }
    {
        const UnkQualifiedId& name = Qid(g_unk_71727c94c0);
        CBlackboard::CSection* section = fn_71010472e0(blackboard, key);
        if (section && !manager->field_0x968.field_0xe8) {
            base::reflection::UnkTypedRef value{base::reflection::UnkFieldTraits<UnkRowTextsTag_le785e4>::type(),
                                                &texts};
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)),
                          reinterpret_cast<const UnkTypedValue*>(&value));
        }
    }
}
