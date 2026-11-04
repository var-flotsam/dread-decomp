// CEmmyAttackComponent's unit (tools/tu_map.py): a collision handler. When the other side's object
// (+0x10, +0x10, +0x10) is a CBreakableTileGroup, the hit is taken (true); and when the owner (+0x30) has a
// part at +0x260 that fn_710113d83c says no to, and a CEmmyAIComponent (+0x240) that may act (part
// +0x29418 not active, flag +0x29f26 clear), the tile at the hit's +0x24 (fn_7100ca1ecc), unless of kind
// 2, is broken (state 2) when its state is 1, 2 or 7. Any other object goes to the base handler
// (fn_7101137c8c). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkQueryObj;
u32 fn_7100c9e960(UnkQueryObj* obj);
void fn_7100c9ea08(UnkQueryObj* obj);
void fn_7100c9e6ac(UnkQueryObj* obj, u32 state, bool a, bool b);
bool fn_710113d83c(void*);
bool fn_710002fb70(void*);

struct UnkTileQuery_A3_b64 {
    u8 pad_0x0[0x38];
    u32 kind;
};

UnkTileQuery_A3_b64* fn_7100ca1ecc(base::reflection::UnkReflObject* group, const void* at, void* arg, bool flag);

struct UnkTileHitLink1_A3_b64 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;
};
struct UnkTileHitLink0_A3_b64 {
    u8 pad_0x0[0x10];
    UnkTileHitLink1_A3_b64* link;
};

struct UnkTileHit_A3_b64 {
    u8 pad_0x0[0x10];
    UnkTileHitLink0_A3_b64* link;
    u8 pad_0x18[0xc];
    u8 at[1];  // 0x24
};

struct UnkTileHitOwner_A3_b64 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;
    u8 pad_0x248[0x18];
    void* field_0x260;
};

struct UnkEmmyAttackComponent_A3_b64 {
    u8 pad_0x0[0x30];
    UnkTileHitOwner_A3_b64* owner;
};

bool fn_7101137c8c(UnkEmmyAttackComponent_A3_b64* self, UnkTileHit_A3_b64* hit);

bool fn_710056ab74(UnkEmmyAttackComponent_A3_b64* self, UnkTileHit_A3_b64* hit) {
    base::reflection::UnkReflObject* group = hit->link->link->object;
    if (!group || !fn_710009d50c(group->vf_0x0(), fn_7100ca0484()))
        return fn_7101137c8c(self, hit);
    void* part = self->owner->field_0x260;
    base::reflection::UnkReflObject* ai = self->owner->ai;
    if (ai)
        ai = fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? ai : nullptr;
    if (part && !fn_710113d83c(part) && ai) {
        char* emmy = reinterpret_cast<char*>(ai);
        if (!fn_710002fb70(emmy + 0x29418) && !*reinterpret_cast<bool*>(emmy + 0x29f26)) {
            if (UnkTileQuery_A3_b64* tile = fn_7100ca1ecc(group, hit->at, nullptr, true)) {
                u32 kind = tile->kind;
                UnkQueryObj* obj = reinterpret_cast<UnkQueryObj*>(tile);
                u32 state = fn_7100c9e960(obj);
                if (kind != 2 && (state == 1 || state == 2 || state == 7)) {
                    fn_7100c9ea08(obj);
                    fn_7100c9e6ac(obj, 2, false, false);
                }
            }
        }
    }
    return true;
}
