// base::reflection's unit (tools/tu_map.py, unit 2613): erasing an owned item from an object's array at
// +0x10 (fn_7100fb5200). Names and layouts are placeholders.
#include <new>
#include "base/global/CStrId.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

// The id table (fn_7100031894 builds it; as src/game/CAbilityEnergyWave/CAbilityEnergyWave_Effect_ovb0022.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// An owned pointer that moves out of its slot.
struct UnkMovedPtr_ovb0130 : UnkOwnedPtr {
    UnkMovedPtr_ovb0130(UnkMovedPtr_ovb0130&& other) {
        UnkOwned* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
};

struct UnkReflOwnedSet_ovb0130 {
    u8 pad_0x0[0x10];
    base::UnkVector<UnkMovedPtr_ovb0130> items;  // 0x10
};

static inline void EraseAt_ovb0130(base::UnkVector<UnkMovedPtr_ovb0130>* v, u32 index) {
    if (index < v->count) {
        u64 i = index;
        v->data[i].~UnkMovedPtr_ovb0130();
        for (; i < (u32)(v->count - 1); ++i) {
            UnkMovedPtr_ovb0130* cur = &v->data[i];
            new (cur) UnkMovedPtr_ovb0130(static_cast<UnkMovedPtr_ovb0130&&>(cur[1]));
            (&v->data[i])[1].~UnkMovedPtr_ovb0130();
        }
        v->count = v->count - 1;
    }
}

// fn_7100fb5200: removes the item at `index` (false when there is none).
bool fn_7100fb5200(UnkReflOwnedSet_ovb0130* self, u32 index) {
    if (index >= self->items.count)
        return false;
    GameManagerIdTable();
    EraseAt_ovb0130(&self->items, index);
    return true;
}
