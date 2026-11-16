// CBreakableTileGroupComponent's unit (tools/tu_map.py): reads the group's tile states ("TileStates", a
// string of '0', '1' and '2', one per tile) from the current scenario's blackboard section and sets each
// tile's state, then stores the "FakeHusks" flag there unless the blackboard is locked.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CBreakableTileGroup.h"
#include "game/CBreakableTileGroupComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

#include "../../game_aca57c/UnkBufferString.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkEntryInfo;

struct UnkTileGroupValue_l759b34 {
    CStrId id;
    u64 field_0x8;
};

struct UnkTileGroupComponent_l759b34 {
    u8 pad_0x0[0x4c];
    bool fake_husks;  // 0x4c
    u8 pad_0x4d[0xa0 - 0x4d];
    void* group;  // 0xa0
    u8 pad_0xa8[0xd0 - 0xa8];
    UnkTileGroupValue_l759b34* values;  // 0xd0
    u64* keys;                          // 0xd8
    u32 count;                          // 0xe0

    // The value stored under `property`'s name (one past the end when there is none).
    UnkTileGroupValue_l759b34* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* ids = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (ids[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};

// The character at `index` (an int, as the game indexes strings).
inline char CharAt(const CRntString* text, s32 index) {
    return text->data[index];
}

// Stores the flag at `value` under `name` in `section`, unless the blackboard is locked.
inline void SetSectionBool(CBlackboard* blackboard, CBlackboard::CSection* section, UnkPropertyName* name, bool* value) {
    if (!blackboard->field_0xe8) {
        UnkTypedValue typed;
        typed.type = fn_710000d228();
        typed.value = value;
        fn_7100332c80(section, name, &typed);
    }
}

void fn_7100759b34(UnkTileGroupComponent_l759b34* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    UnkBufferString<0x100, g_unk_7101a194b8> states;
    if (fn_7100395c38(section, &self->Find(g_unk_7172711538)->id, states.Str())) {
        for (u32 i = 0; i < states.Str()->length; ++i) {
            UnkQueryObj* tile = reinterpret_cast<UnkQueryObj*>(fn_7100ca1ea8(self->group, i));
            if (!tile)
                continue;
            switch (CharAt(states.Str(), i)) {
            case '0':
                fn_7100c9e6ac(tile, 0, true, false);
                break;
            case '1':
                fn_7100c9e6ac(tile, 1, true, false);
                break;
            case '2':
                fn_7100c9e6ac(tile, 2, true, false);
                break;
            }
        }
    }
    UnkTileGroupValue_l759b34* husks = self->Find(g_unk_7172711560);
    SetSectionBool(blackboard, section, reinterpret_cast<UnkPropertyName*>(&husks->id), &self->fake_husks);
}
