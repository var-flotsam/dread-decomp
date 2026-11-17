// Move-assignment of an array of records (base::global::CRntVector's operator=(CRntVector&&)): the
// elements are destroyed, the other's array and settings are taken over and the other is left
// empty. A record is a word, a file-path id and a counted resource reference in
// fn_710000e4dc (base::animation::CBlendSpaceResource's unit 14); fn_7100268074 is the same code
// in navmesh::CDynamicSmartLinkGroup's unit (351) for a record without the word. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

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
    UnkPathIdTwoStep path;
    UnkCountedRef_b58m ref;
};

// navmesh's record has no leading word.
struct UnkPathRefPair_b58 {
    UnkPathIdTwoStep path;
    UnkCountedRef_b58m ref;
};

base::global::CRntVector<UnkPathRefRecord_b58>* fn_710000e4dc(
    base::global::CRntVector<UnkPathRefRecord_b58>* self,
    base::global::CRntVector<UnkPathRefRecord_b58>* other) {
    return &(*self = static_cast<base::global::CRntVector<UnkPathRefRecord_b58>&&>(*other));
}

base::global::CRntVector<UnkPathRefPair_b58>* fn_7100268074(
    base::global::CRntVector<UnkPathRefPair_b58>* self,
    base::global::CRntVector<UnkPathRefPair_b58>* other) {
    return &(*self = static_cast<base::global::CRntVector<UnkPathRefPair_b58>&&>(*other));
}

// The same arrays' Clear: destroys the elements, keeping the array (fn_710000df84 in unit 14,
// fn_7100267bbc in unit 351).
void fn_710000df84(base::global::CRntVector<UnkPathRefRecord_b58>* self) {
    self->Clear();
}

void fn_7100267bbc(base::global::CRntVector<UnkPathRefPair_b58>* self) {
    self->Clear();
}

// And their Free: destroys the elements and frees the array (fn_710000e3ac, fn_7100267f40).
void fn_710000e3ac(base::global::CRntVector<UnkPathRefRecord_b58>* self) {
    self->Free();
}

void fn_7100267f40(base::global::CRntVector<UnkPathRefPair_b58>* self) {
    self->Free();
}
