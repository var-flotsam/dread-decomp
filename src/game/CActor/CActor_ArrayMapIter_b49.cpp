// CActor's unit (tools/tu_map.py unit 341): Begin and End (slots 0x160 and 0x168) of a reflected
// map from string ids to arrays of 0x40-byte polymorphic elements. The iterator walks the map's
// nodes (the first at +0x40 of the map, null past the end); its constructor and assignment
// re-point its cached (key, value) through fn_710023c550, kept out of line. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

// Destroyed through its vtable's first slot.
struct UnkActorPolyElem_b49a1 {
    virtual ~UnkActorPolyElem_b49a1();
    u8 unk_0x8[0x38];
};
static_assert(sizeof(UnkActorPolyElem_b49a1) == 0x40, "UnkActorPolyElem_b49a1 layout");

// An array that destroys its elements and frees itself.
struct UnkActorPolyArray_b49a1 {
    UnkActorPolyElem_b49a1* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkActorPolyArray_b49a1() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkActorPolyElem_b49a1();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkActorArrayMapNode_b49a1;

struct UnkActorArrayMap_b49a1 {
    u8 unk_0x0[0x40];
    UnkActorArrayMapNode_b49a1* first;
};

struct UnkActorArrayMapIter_b49a1;
void fn_710023c550(UnkActorArrayMapIter_b49a1* it);  // re-points `current` at the node

struct UnkActorArrayMapIter_b49a1 {
    UnkActorArrayMapNode_b49a1* node;
    base::UnkIterValue<UnkActorPolyArray_b49a1, base::UnkStrIdMember> current;

    explicit UnkActorArrayMapIter_b49a1(UnkActorArrayMapNode_b49a1* node) : node(node) { fn_710023c550(this); }
    UnkActorArrayMapIter_b49a1& operator=(const UnkActorArrayMapIter_b49a1& other) {
        node = other.node;
        fn_710023c550(this);
        return *this;
    }
};

void fn_71002488f4(const UnkActorArrayMap_b49a1* map, UnkActorArrayMapIter_b49a1* out) {
    *out = UnkActorArrayMapIter_b49a1(map->first);
}

void fn_7100248a18(const UnkActorArrayMap_b49a1* map, UnkActorArrayMapIter_b49a1* out) {
    *out = UnkActorArrayMapIter_b49a1(nullptr);
}
