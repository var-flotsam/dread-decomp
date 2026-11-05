// Move-assignment of an array of records (base::UnkVector's operator=(UnkVector&&)): the
// elements are destroyed, the other's array and settings are taken over and the other is left
// empty. A record is a word, a file-path id and a counted resource reference in
// fn_710000e4dc (base::animation::CBlendSpaceResource's unit 14); fn_7100268074 is the same code
// in navmesh::CDynamicSmartLinkGroup's unit (351) for a record without the word. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id released in two steps (decrement, then reload and test), as CStrId::Reset.
struct UnkPathId_b58 {
    UnkStringData* data;

    ~UnkPathId_b58() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkCountedRef_b58m {
    UnkCountedResource* ptr;

    ~UnkCountedRef_b58m() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

struct UnkPathRefRecord_b58 {
    u64 field_0x0;
    UnkPathId_b58 path;
    UnkCountedRef_b58m ref;
};

// navmesh's record has no leading word.
struct UnkPathRefPair_b58 {
    UnkPathId_b58 path;
    UnkCountedRef_b58m ref;
};

base::UnkVector<UnkPathRefRecord_b58>* fn_710000e4dc(
    base::UnkVector<UnkPathRefRecord_b58>* self,
    base::UnkVector<UnkPathRefRecord_b58>* other) {
    return &(*self = static_cast<base::UnkVector<UnkPathRefRecord_b58>&&>(*other));
}

base::UnkVector<UnkPathRefPair_b58>* fn_7100268074(
    base::UnkVector<UnkPathRefPair_b58>* self,
    base::UnkVector<UnkPathRefPair_b58>* other) {
    return &(*self = static_cast<base::UnkVector<UnkPathRefPair_b58>&&>(*other));
}

// The same arrays' Clear: destroys the elements, keeping the array (fn_710000df84 in unit 14,
// fn_7100267bbc in unit 351).
void fn_710000df84(base::UnkVector<UnkPathRefRecord_b58>* self) {
    self->Clear();
}

void fn_7100267bbc(base::UnkVector<UnkPathRefPair_b58>* self) {
    self->Clear();
}

// And their Free: destroys the elements and frees the array (fn_710000e3ac, fn_7100267f40).
void fn_710000e3ac(base::UnkVector<UnkPathRefRecord_b58>* self) {
    self->Free();
}

void fn_7100267f40(base::UnkVector<UnkPathRefPair_b58>* self) {
    self->Free();
}
