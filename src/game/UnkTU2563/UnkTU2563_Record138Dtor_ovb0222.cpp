// CMinimapData's unit (tools/tu_map.py unit 2563, no reliable name): the destructor of the 0x138-byte area
// record (src/game/CMinimapData/CMinimapData_Record138EraseAt_A6_b76.cpp): the paths' arrays are freed in
// reverse, then the item (its base vtable set, its array of virtual elements destroyed and freed), then the
// string id. Names and layouts are placeholders.
#include "base/UnkFreedVectorPair.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkReleasedStrIdDtor_ovb0222 {
    UnkStringData* data;
    __attribute__((always_inline)) ~UnkReleasedStrIdDtor_ovb0222() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// An element destroyed through its vtable (slot 1).
struct UnkPolyElemDtor_ovb0222 {
    void** vtable;
    u8 unk_0x8[0x18];
    __attribute__((always_inline)) ~UnkPolyElemDtor_ovb0222() {
        reinterpret_cast<void (*)(UnkPolyElemDtor_ovb0222*)>(vtable[1])(this);
    }
};
static_assert(sizeof(UnkPolyElemDtor_ovb0222) == 0x20, "layout");

struct UnkItemDtor_ovb0222 {
    void** vtable;
    base::UnkVector<UnkPolyElemDtor_ovb0222> elems;
    u8 unk_0x30[0x20];
    __attribute__((always_inline)) ~UnkItemDtor_ovb0222() {
        vtable = g_unk_7101cac938 + 2;
        elems.Free();
    }
};

struct alignas(8) UnkRecord138Dtor_ovb0222 {
    UnkReleasedStrIdDtor_ovb0222 name;
    u32 field_0x8;
    UnkItemDtor_ovb0222 item;  // 0x10
    u32 unk_0x60[6];
    UnkFreedVector<void*> points0;   // 0x78
    UnkFreedVector<void*> indices0;  // 0xa0
    u32 unk_0xc8[4];
    UnkFreedVector<void*> points1;   // 0xd8
    UnkFreedVector<void*> indices1;  // 0x100
    __attribute__((always_inline)) ~UnkRecord138Dtor_ovb0222() {}
};

void fn_7100e4e310(char* self) { reinterpret_cast<UnkRecord138Dtor_ovb0222*>(self)->~UnkRecord138Dtor_ovb0222(); }
