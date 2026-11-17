// CReturnAreaSmartObjectComponent's unit (tools/tu_map.py, unit 1513): turns the glow (+0x18e8) of the
// owner's model fully on, fading over the time (+0x28) of the player's +0x22b8 entry, or over 2.5 when
// the player has none. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSlots.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::color::CColor4F;

struct UnkStruct_7101176ea8;

struct UnkReturnAreaKey_ovb0123 {
    u8 pad_0x0[0x30];
    u64 key;
};

inline UnkGameManagerIdTable& ReturnAreaIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkReturnAreaModel_ovb0123 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkReturnAreaRender_ovb0123 {
    u8 pad_0x0[0x50];
    UnkReturnAreaModel_ovb0123* model;
};

struct UnkReturnAreaOwner_ovb0123 {
    u8 pad_0x0[0x228];
    UnkReturnAreaRender_ovb0123* render;
};

struct UnkReturnAreaSmartObject_la064c4 {
    u8 pad_0x0[0x30];
    UnkReturnAreaOwner_ovb0123* owner;  // 0x30
};

// The player as this unit reads it: its actions (+0x220).
struct UnkReturnAreaPlayer_la064c4 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkReturnAreaEntry_la064c4 {
    u8 pad_0x0[0x28];
    f32 time;  // 0x28
};

// The entries fn_7101176ce8 finds: a count and, packed after it, the entries.
#pragma pack(push, 4)
struct UnkReturnAreaEntries_la064c4 {
    u32 count;
    UnkReturnAreaEntry_la064c4* items[8];
};
#pragma pack(pop)

bool fn_7101176ce8(UnkStruct_7101176ea8* self, UnkReturnAreaEntries_la064c4* out, void* key);

void fn_7100a064c4(UnkReturnAreaSmartObject_la064c4* self) {
    UnkReturnAreaModel_ovb0123* model = self->owner->render->model;
    if (!model)
        return;
    f32 time = 2.5f;
    UnkReturnAreaPlayer_la064c4* player =
        reinterpret_cast<UnkReturnAreaPlayer_la064c4*>(fn_7100dec788(g_unk_7101d47310));
    UnkStruct_7101176ea8* actions;
    if (player && (actions = player->actions)) {
        UnkReturnAreaEntries_la064c4 entries;
        entries.count = 0;
        u64 key = reinterpret_cast<UnkReturnAreaKey_ovb0123*>(ReturnAreaIdTable().field_0x22b8.data)->key;
        if (fn_7101176ce8(actions, &entries, reinterpret_cast<void*>(key)))
            time = entries.items[0]->time;
    }
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    u64 key = reinterpret_cast<UnkReturnAreaKey_ovb0123*>(ReturnAreaIdTable().field_0x18e8.data)->key;
    fn_71001c2bac(model->slots, key, 0, &color, 1);
    color.a = 1.0f;
    key = reinterpret_cast<UnkReturnAreaKey_ovb0123*>(ReturnAreaIdTable().field_0x18e8.data)->key;
    fn_71001c3040(model->slots, key, 0, &color.r, 1, time);
}
