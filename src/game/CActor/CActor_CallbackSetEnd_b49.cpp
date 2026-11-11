// CActor's unit (tools/tu_map.py unit 341): End of a reflected array of callback lists
// (base::UnkCallbackListShort, 0x40 bytes, with the vtables src/clones/clones_00.cpp gives
// fn_710023ac4c's copy). The iterator's constructor and assignment re-point its cached value
// through fn_710023ac4c, kept out of line. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "game/UnkTU341.h"

struct UnkClone_fn_710023ac4c;
template <>
struct base::UnkCallbackTraits<UnkClone_fn_710023ac4c> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

using UnkActorCallbackList_b49a1 = base::UnkCallbackListShort<UnkClone_fn_710023ac4c>;
static_assert(sizeof(UnkActorCallbackList_b49a1) == 0x40, "UnkActorCallbackList_b49a1 layout");

struct UnkActorCallbackIter_b49a1 {
    u32 index;
    UnkActorCallbackList_b49a1* element;
    base::UnkIterValue<UnkActorCallbackList_b49a1, u32> current;

    UnkActorCallbackIter_b49a1(u32 index, UnkActorCallbackList_b49a1* element) : index(index), element(element) {
        fn_710023ac4c(this);
    }
    UnkActorCallbackIter_b49a1& operator=(const UnkActorCallbackIter_b49a1& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_710023ac4c(this);
        }
        return *this;
    }
};

// The braces evaluate the count before the data, as the original does.
void fn_71002476ec(const base::UnkVector<UnkActorCallbackList_b49a1>* v, UnkActorCallbackIter_b49a1* out) {
    *out = UnkActorCallbackIter_b49a1{v->count, v->data + v->count};
}
