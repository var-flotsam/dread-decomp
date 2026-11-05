// CEmmyAttackComponent's unit (tools/tu_map.py unit 812): the second collision handler, next to
// fn_710056ab74 (CEmmyAttackComponent_TileHit_A3_b64.cpp). When the other side's object (+0x10, +0x10,
// +0x10) is a CBreakableTileGroup the hit is taken (true), and when the owner's part at +0x260 says no
// (fn_710113d83c), the tile at the hit's +0x24 (fn_7100ca2048), unless of kind 2, is broken (state 2)
// when its state is 1, 2 or 7. Any other object goes to the base handler (fn_7101137df8). Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

// As src/game/CEmmyAttackComponent/CEmmyAttackComponent_TileHit_A3_b64.cpp declares them.
struct UnkQueryObj;
u32 fn_7100c9e960(UnkQueryObj* obj);
void fn_7100c9ea08(UnkQueryObj* obj);
void fn_7100c9e6ac(UnkQueryObj* obj, u32 state, bool a, bool b);
bool fn_710113d83c(void*);
// As src/game/CBTObserverComponent/CBTObserverComponent_Find.cpp declares it.
UnkQueryObj* fn_7100ca2048(void*, void*, u32);

struct UnkTileQuery_A3_b67 {
    u8 pad_0x0[0x38];
    u32 kind;
};

struct UnkTileHitLink1_A3_b67 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;
};
struct UnkTileHitLink0_A3_b67 {
    u8 pad_0x0[0x10];
    UnkTileHitLink1_A3_b67* link;
};

struct UnkTileHit_A3_b67 {
    u8 pad_0x0[0x10];
    UnkTileHitLink0_A3_b67* link;
    u8 pad_0x18[0xc];
    u8 at[1];  // 0x24
};

struct UnkTileHitOwner_A3_b67 {
    u8 pad_0x0[0x260];
    void* field_0x260;
};

struct UnkEmmyAttackComponent_A3_b67 {
    u8 pad_0x0[0x30];
    UnkTileHitOwner_A3_b67* owner;
};

bool fn_7101137df8(UnkEmmyAttackComponent_A3_b67* self, UnkTileHit_A3_b67* hit);

bool fn_710056ace0(UnkEmmyAttackComponent_A3_b67* self, UnkTileHit_A3_b67* hit) {
    base::reflection::UnkReflObject* group = hit->link->link->object;
    if (!group || !fn_710009d50c(group->vf_0x0(), fn_7100ca0484()))
        return fn_7101137df8(self, hit);
    void* part = self->owner->field_0x260;
    if (part && !fn_710113d83c(part)) {
        if (UnkQueryObj* obj = fn_7100ca2048(group, hit->at, 1)) {
            u32 kind = reinterpret_cast<UnkTileQuery_A3_b67*>(obj)->kind;
            u32 state = fn_7100c9e960(obj);
            if (kind != 2 && (state == 1 || state == 2 || state == 7)) {
                fn_7100c9ea08(obj);
                fn_7100c9e6ac(obj, 2, false, false);
            }
        }
    }
    return true;
}
