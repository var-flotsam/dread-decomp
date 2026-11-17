// Unit 1629: whether an entity can be interacted with in a mode. Modes 1 and 2 look at the doors linked to
// its model and at its own breakable tile (types 1 and 7); any other mode asks whether a tile of its tile's
// group is in state 2. Names and layouts are placeholders.
#include "game/CBreakableTileGroup.h"
#include "game/UnkTU367.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTileGroup;

// One tile of a group.
struct UnkQueryObj {
    u32 field_0x0;  // index
    u32 field_0x4;  // type
    u32 field_0x8;  // cell x
    u32 field_0xc;  // cell y
    u8 pad_0x10[0x18];
    f32 field_0x28;  // x
    f32 field_0x2c;  // y
    UnkTileGroup* field_0x30;
    s32 state;  // 0x38
    bool field_0x3c;
    u8 pad_0x3d[0xb];
    u64 field_0x48;
    u8 pad_0x50[0x58];
};
static_assert(sizeof(UnkQueryObj) == 0xa8, "UnkQueryObj placeholder layout");

struct UnkTileCollider;

struct UnkTileGroup {
    u8 pad_0x0[0x14];
    bool field_0x14;
    u8 pad_0x15[0x3];
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    u8 pad_0x2c[0x14];
    u32 field_0x40;
    UnkQueryObj* field_0x48;
    u8 pad_0x50[0x8];
    u32* field_0x58;
    u8 pad_0x60[0x68];
    u32 field_0xc8;
    UnkTileCollider* field_0xd0;
};

// A link of the model to another object (a door), held through a weak handle.
struct UnkModelLink_ad00ac {
    void* key;
    UnkWeakHandle* handle;  // 0x8
    u32 kind;               // 0x10
};

struct UnkModel_ad00ac {
    u8 pad_0x0[0x124];
    u32 link_count;               // 0x124
    UnkModelLink_ad00ac links[1];  // 0x128, link_count of them

    UnkModelLink_ad00ac* begin() { return links; }
    UnkModelLink_ad00ac* end() { return links + link_count; }
};
static_assert(__builtin_offsetof(UnkModel_ad00ac, links) == 0x128, "layout");

struct UnkEntityObj_ad00ac {
    u8 pad_0x0[0x60];
    bool field_0x60;
    u8 pad_0x61[0x7];
    UnkModel_ad00ac* model;  // 0x68
};

struct UnkEntity_ad00ac {
    u8 pad_0x0[0x10];
    bool enabled;  // 0x10
    u8 pad_0x11[0x7];
    UnkEntityObj_ad00ac* obj;  // 0x18
};

UnkQueryObj* fn_71002b37d8(void* tile);
bool fn_71008d0db4(UnkDoorLock* door);

bool fn_7100ad00ac(UnkEntity_ad00ac* entity, u32 mode) {
    if (!entity->enabled)
        return false;
    UnkEntityObj_ad00ac* obj = entity->obj;
    if (!obj || obj->field_0x60)
        return true;
    UnkModel_ad00ac* model = obj->model;
    if (mode == 1 || mode == 2) {
        if (!model)
            return true;
        bool found = false;
        for (UnkModelLink_ad00ac& link : *model) {
            UnkDoorLock* door = nullptr;
            if (link.handle && link.handle->target)
                door = fn_71002af860(link.handle->target);
            if (mode == 2 || (door && fn_71008d0db4(door)))
                found = true;
        }
        if (!found) {
            if (UnkQueryObj* tile = fn_71002b37d8(entity->obj->model))
                found = fn_7100c9e960(tile) == 1 || fn_7100c9e960(tile) == 7;
        }
        return found;
    }
    UnkQueryObj* tile = fn_71002b37d8(model);
    if (!tile)
        return true;
    UnkTileGroup* group = tile->field_0x30;
    if (!group)
        return false;
    for (u32 i = 0; i < group->field_0x40; ++i) {
        UnkQueryObj* other = reinterpret_cast<UnkQueryObj*>(fn_7100ca1ea8(group, i));
        if (other && other->state == 2)
            return true;
    }
    return false;
}
