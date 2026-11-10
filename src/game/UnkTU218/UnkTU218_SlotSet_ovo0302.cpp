// Unit 218: setting one vector of a slot in the slot table (game/UnkSlots.h). Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkSlots.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

// The table's own copy of a slot's values: ten vectors per id.
struct UnkSlotRow_ovo0302 {
    CVector4D values[10];
};

static inline u8* RecordOf(void* table, u32 id) {
    return reinterpret_cast<u8**>(*reinterpret_cast<u8**>(static_cast<u8*>(table) + 0x48))[id];
}

bool fn_71001996e0(UnkSlotTable* table, u32 id, u32 kind, UnkVector4D value, u32 mode) {
    u8* base = reinterpret_cast<u8*>(table);
    u8* record = RecordOf(table, id);
    switch (mode) {
    case 0: {
        CVector4D* stored = reinterpret_cast<CVector4D*>(reinterpret_cast<u8*>(&reinterpret_cast<UnkSlotRow_ovo0302*>(base)[id].values[kind]) + 0x15b0);
        fn_710007b518(stored, &value);
        fn_710007b518(reinterpret_cast<CVector4D*>(record + (u64)kind * 16 + 0x240), stored);
        if (u8* target = *reinterpret_cast<u8**>(record + (u64)kind * 8 + 0x2e0))
            __builtin_memcpy(*reinterpret_cast<u8**>(target + 0x30), stored, 16);
        return true;
    }
    case 1: {
        CVector4D* stored = reinterpret_cast<CVector4D*>(reinterpret_cast<u8*>(&reinterpret_cast<UnkSlotRow_ovo0302*>(base)[id].values[kind]) + 0x1b0);
        fn_710007b518(stored, &value);
        fn_710007b518(reinterpret_cast<CVector4D*>(record + (u64)kind * 16 + 0x14c), stored);
        if (u8* target = *reinterpret_cast<u8**>(record + (u64)kind * 8 + 0x1f0))
            __builtin_memcpy(*reinterpret_cast<u8**>(target + 0x30), stored, 16);
        return true;
    }
    }
    return false;
}
