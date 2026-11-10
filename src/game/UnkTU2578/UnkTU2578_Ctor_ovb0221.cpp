// Unit 2578 (no reflected class): the sound-id manager's constructor (the 0x150-byte object at g_unk_7101d47320, whose
// layout src/game/UnkSoundIdRegistry.cpp describes, created by src/engine/UnkStartupManagers.cpp): a chained map of
// registrations with 11 buckets, three lists of (id, instance) nodes and one of instances, each behind a vtable
// pointer (the objects of src/game_7100ece7fc/fn_7100f02528.cpp), the two item pointers, the blend colour (1, 1),
// the enabled flag and the stack of blend ids. Names and layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101caa208[];
extern void* g_unk_7101caa228[];

struct UnkSoundIdNodeA;
struct UnkSoundIdNodeB;

// A list of reference nodes behind a vtable pointer.
template <void** Vtable, class Node>
struct UnkSoundIdListOwner {
    void** vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkSoundIdListOwner()
        : vtable(&Vtable[2]), count(0), head(nullptr), tail(nullptr), allocator(fn_7100083990()), flags(0x13) {}
};

struct UnkMap11 : UnkChainedMap {
    __attribute__((always_inline)) UnkMap11() { ResizeBuckets(11); }
};

struct UnkStruct_0x128 {
    UnkMap11 map;
    UnkSoundIdListOwner<g_unk_7101caa208, UnkSoundIdNodeA> field_0x48;
    UnkSoundIdListOwner<g_unk_7101caa208, UnkSoundIdNodeA> field_0x78;
    UnkSoundIdListOwner<g_unk_7101caa208, UnkSoundIdNodeA> field_0xa8;
    UnkSoundIdListOwner<g_unk_7101caa228, UnkSoundIdNodeB> field_0xd8;
    void* field_0x108;
    void* field_0x110;
    f32 field_0x118;
    f32 field_0x11c;
    u16 field_0x120;
    bool field_0x122;
    base::UnkVector<u32> field_0x128;

    UnkStruct_0x128()
        : field_0x108(nullptr), field_0x110(nullptr), field_0x118(1.0f), field_0x11c(1.0f), field_0x120(0),
          field_0x122(true), field_0x128(0xf) {}
};
static_assert(sizeof(UnkStruct_0x128) == 0x150, "UnkStruct_0x128 layout");

__attribute__((flatten)) void fn_7100ef3494(UnkStruct_0x128* self) {
    new (self) UnkStruct_0x128;
}
