// lua_bind::ResetEmmyPhase1Mode: resets every listed entity (the list fn_7100552d7c returns).
#include "../../lib/lua/lua.h"
#include "base/UnkVector.h"
#include "game/UnkTU811.h"
#include "types.h"

// Declared as in src/game/CBreakableTileGroup.cpp.
struct UnkListedItem;

struct UnkResetEmmyItem_A6_b70 {
    u8 pad_0x0[0x10];
    void* entity;
};

void fn_7100544a24(void* entity);

int fn_7100fbb670(lua_State* L) {
    base::UnkVector<UnkListedItem*>* items = fn_7100552d7c();
    UnkListedItem** it = items->data;
    UnkListedItem** end = it + items->count;
    for (; it != end; ++it) {
        auto* item = reinterpret_cast<UnkResetEmmyItem_A6_b70*>(*it);
        if (item && item->entity)
            fn_7100544a24(item->entity);
    }
    return 0;
}
