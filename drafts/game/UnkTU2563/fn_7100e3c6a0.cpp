// Unit 2563 (near CMinimapData): places a minimap marker at its owner's position and picks its icon:
// missile or power bomb pickups by item, a generic item otherwise, or a life pickup by its kind.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::global::CStrId;

struct UnkVec3 {
    u32 x, y, z;
};

// The icons (string ids of this unit).
extern CStrId g_unk_71727c8320;
extern CStrId g_unk_71727c8328;
extern CStrId g_unk_71727c8330;
extern CStrId g_unk_71727c8338;
extern CStrId g_unk_71727c8340;
extern CStrId g_unk_71727c8348;

inline const CStrId& StaticName_71726a47a8() {
    static CStrId s("LIFE", true);
    return s;
}

struct UnkMinimapOwner_e3c6a0 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkMinimapSource_e3c6a0 {
    u8 pad_0x0[0x18];
    UnkMinimapOwner_e3c6a0* owner;  // 0x18
    u8 pad_0x20[0x78];
    CStrId item;  // 0x98
    u8 pad_0xa0[0x8];
    f32 amount;  // 0xa8
    u8 pad_0xac[0x4];
    s32 kind;  // 0xb0
    u8 pad_0xb4[0x4];
    CStrId pickup;  // 0xb8
    u8 pad_0xc0[0x69];
    bool is_item;  // 0x129
};

struct UnkMinimapMarker_e3c6a0 {
    f32 x, y;        // 0x0
    f32 min_x, min_y;  // 0x8
    f32 max_x, max_y;  // 0x10
    CStrId icon;     // 0x18

    // A box 2 * half wide and height tall, standing on the marker's position.
    void SetBounds(f32 half, f32 height) {
        f32 px = x;
        f32 py = y;
        f32 left = px - half;
        f32 right = px + half;
        f32 top = py + height;
        min_y = py;
        min_x = left;
        max_x = right;
        max_y = top;
    }
};

static inline const CStrId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid.id;
}

void fn_7100e3c6a0(UnkMinimapSource_e3c6a0* self, UnkMinimapMarker_e3c6a0* marker) {
    UnkMinimapOwner_e3c6a0* owner = self->owner;
    u32 a = owner->field_0x44;
    u32 b = owner->field_0x54;
    u32 c = owner->field_0x64;
    UnkVec3 pos = {a, b, c};
    fn_710007b444(marker, &pos);
    marker->SetBounds(50.0f, 100.0f);

    if (self->is_item) {
        if (self->item == ItemId(g_unk_717275a218)) {
            marker->icon = self->amount > 3.0f ? g_unk_71727c8340 : g_unk_71727c8338;
        } else if (self->item == ItemId(g_unk_717275a298)) {
            marker->icon = g_unk_71727c8348;
        } else {
            marker->icon = g_unk_71727c8320;
            marker->SetBounds(100.0f, 200.0f);
        }
    } else {
        if (!(self->pickup == StaticName_71726a47a8()))
            return;
        if (self->kind == 2)
            marker->icon = g_unk_71727c8328;
        else
            marker->icon = g_unk_71727c8330;
    }
}
