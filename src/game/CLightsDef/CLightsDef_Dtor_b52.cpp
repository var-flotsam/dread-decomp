// CLightsDef unit (tools/tu_map.py unit 2529): the destructors of the object inside
// src/game/CLightManager/fn_7100e0ccf8.cpp's pair (the 0x58-byte base, fn_7100016f88, then a
// file-path id at +0x58 and an array at +0x60): the vtable, the array cleared (fn_7100da0870) and
// freed, the id released, then the base destructor; fn_7100d9ef14 also frees the object. Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

// Signatures as src/base/file/nx/CDeviceNX/fn_710002ddc0.cpp and
// src/game/CLightManager/fn_7100e0ccf8.cpp declare them.
void fn_7100016f88(void* self);
struct UnkClearedArray;
void fn_7100da0870(UnkClearedArray* self, u32 count);  // resizes

extern void* g_unk_7101aa9140[];

struct UnkLightsDefBase_b52 {
    void** vtable;
    u8 unk_0x8[0x50];
    __attribute__((always_inline)) ~UnkLightsDefBase_b52() { fn_7100016f88(this); }
};

// base::UnkVector's layout; cleared out of line before its storage is freed.
struct UnkLightsDefArray_b52 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkLightsDefArray_b52() {
        if (data) {
            fn_7100da0870(reinterpret_cast<UnkClearedArray*>(this), 0);
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkLightsDefObj_b52 : UnkLightsDefBase_b52 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x58
    UnkLightsDefArray_b52 array;                    // 0x60

    __attribute__((always_inline)) ~UnkLightsDefObj_b52() { vtable = &g_unk_7101aa9140[2]; }
};

void fn_7100d9ee70(UnkLightsDefObj_b52* self) {
    self->~UnkLightsDefObj_b52();
}

// By reference: the object is known to exist, so the delete has no null check.
void fn_7100d9ef14(UnkLightsDefObj_b52& self) {
    delete &self;
}
