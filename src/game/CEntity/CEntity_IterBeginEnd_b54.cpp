// CEntity's unit (tools/tu_map.py unit 2523): Begin/End of reflected collections, copies of CActor's
// (src/game/CActor/CActor_ArrayMapIter_b49.cpp: fn_71002488f4, fn_7100248a18; and
// src/game/CActor/CActor_CallbackSetEnd_b49.cpp: fn_71002476ec; CActor_CountedListBegin_b49.cpp:
// fn_7100240c44) with this unit's out-of-line iterator
// re-pointers (fn_7100d81b0c, fn_7100d80b84). Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU341.h"
#include "types.h"

// --- a map from string ids to arrays of 0x40-byte polymorphic elements (copy of fn_71002488f4) ---

// Destroyed through its vtable's first slot.
struct UnkEntityPolyElem_b54 {
    virtual ~UnkEntityPolyElem_b54();
    u8 unk_0x8[0x38];
};
static_assert(sizeof(UnkEntityPolyElem_b54) == 0x40, "UnkEntityPolyElem_b54 layout");

// An array that destroys its elements and frees itself.
struct UnkEntityPolyArray_b54 {
    UnkEntityPolyElem_b54* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkEntityPolyArray_b54() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkEntityPolyElem_b54();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkEntityArrayMapNode_b54;

struct UnkEntityArrayMap_b54 {
    u8 unk_0x0[0x40];
    UnkEntityArrayMapNode_b54* first;
};

struct UnkEntityArrayMapIter_b54;
void fn_7100d81b0c(UnkEntityArrayMapIter_b54* it);  // re-points `current` at the node

struct UnkEntityArrayMapIter_b54 {
    UnkEntityArrayMapNode_b54* node;
    base::UnkIterValue<UnkEntityPolyArray_b54, base::UnkStrIdMember> current;

    explicit UnkEntityArrayMapIter_b54(UnkEntityArrayMapNode_b54* node) : node(node) { fn_7100d81b0c(this); }
    UnkEntityArrayMapIter_b54& operator=(const UnkEntityArrayMapIter_b54& other) {
        node = other.node;
        fn_7100d81b0c(this);
        return *this;
    }
};

void fn_7100d8a158(const UnkEntityArrayMap_b54* map, UnkEntityArrayMapIter_b54* out) {
    *out = UnkEntityArrayMapIter_b54(map->first);
}

void fn_7100d8a27c(const UnkEntityArrayMap_b54* map, UnkEntityArrayMapIter_b54* out) {
    *out = UnkEntityArrayMapIter_b54(nullptr);
}

// --- an array of callback lists (copy of fn_71002476ec) ---

struct UnkEntityCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkEntityCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

using UnkEntityCallbackList_b54 = base::UnkCallbackListShort<UnkEntityCallbacks_b54>;
static_assert(sizeof(UnkEntityCallbackList_b54) == 0x40, "UnkEntityCallbackList_b54 layout");

struct UnkEntityCallbackIter_b54 {
    u32 index;
    UnkEntityCallbackList_b54* element;
    base::UnkIterValue<UnkEntityCallbackList_b54, u32> current;

    UnkEntityCallbackIter_b54(u32 index, UnkEntityCallbackList_b54* element) : index(index), element(element) {
        fn_7100d80b84(this);
    }
    UnkEntityCallbackIter_b54& operator=(const UnkEntityCallbackIter_b54& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_7100d80b84(this);
        }
        return *this;
    }
};

// The braces evaluate the count before the data, as the original does.
void fn_7100d88f50(const base::UnkVector<UnkEntityCallbackList_b54>* v, UnkEntityCallbackIter_b54* out) {
    *out = UnkEntityCallbackIter_b54{v->count, v->data + v->count};
}

// --- a linked list (first node at +0x10), iterated with a count (copy of fn_7100240c44 in
// src/game/CActor/CActor_CountedListBegin_b49.cpp) ---

struct UnkEntityListNode_b54;

struct UnkEntityList_b54 {
    u8 unk_0x0[0x10];
    UnkEntityListNode_b54* first;
};

struct UnkEntityListIter_b54 {
    UnkEntityListNode_b54* node;
    u32 index;
    union {
        u8 value[0x10];
        struct {
            const u32* index;
            UnkEntityListNode_b54* node;
        } ref;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
    __attribute__((always_inline)) void SetRef(const u32* i, UnkEntityListNode_b54* n) {
        s32* p = &state;
        switch (*p) {
        case 1:
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        ref.index = i;
        ref.node = n;
    }

    UnkEntityListIter_b54(UnkEntityListNode_b54* node, u32 index) : node(node), index(index), state(0) {
        if (node)
            SetRef(&this->index, node);
    }
    ~UnkEntityListIter_b54() { Reset(); }
    UnkEntityListIter_b54& operator=(const UnkEntityListIter_b54& other) {
        if (this != &other) {
            index = other.index;
            node = other.node;
            if (node)
                SetRef(&index, node);
            else
                Reset();
        }
        return *this;
    }
};

void fn_7100d837dc(const UnkEntityList_b54* list, UnkEntityListIter_b54* out) {
    *out = UnkEntityListIter_b54(list->first, 0);
}
