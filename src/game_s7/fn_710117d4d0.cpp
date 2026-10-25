// libc++ std::sort helpers instantiated for a list of ref-counted object handles, sorted by the
// u32 at +0x60 in descending order. The comparator takes its arguments by value, so every
// comparison copies (and releases) both handles. std::__sort itself (fn_710117c430) differs from
// libc++ 9.0.1, so its body is the adapted copy in lib/libcxx/sort.h.
#include <algorithm>
#include "../../lib/libcxx/sort.h"
#include "base/UnkCountedResource.h"
#include "types.h"

// The resource of base/UnkCountedResource.h with the sort key at +0x60. The handles hold the
// resource type itself (a conversion at the free call folds a known-null argument differently
// and changes both non-matching sorts); the comparator casts for the key.
struct UnkObj : UnkCountedResource {
    u8 pad_0x58[8];
    u32 field_0x60;
};

struct UnkRef {
    UnkCountedResource* p;
    UnkRef(const UnkRef& o) : p(o.p) {
        if (p)
            ++p->ref_count;
    }
    UnkRef& operator=(const UnkRef& o) {
        UnkCountedResource* np = o.p;
        if (p != np) {
            if (np)
                ++np->ref_count;
            UnkCountedResource* old = p;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            p = np;
        }
        return *this;
    }
    ~UnkRef() {
        UnkCountedResource* old = p;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
    }
};

struct UnkSortCmp {
    bool operator()(UnkRef a, UnkRef b) const {
        return static_cast<UnkObj*>(b.p)->field_0x60 < static_cast<UnkObj*>(a.p)->field_0x60;
    }
};

template void libcxx_adapted::__sort<UnkSortCmp, UnkRef*>(UnkRef*, UnkRef*, UnkSortCmp);
template unsigned std::__sort3<UnkSortCmp, UnkRef*>(UnkRef*, UnkRef*, UnkRef*, UnkSortCmp);
template unsigned std::__sort4<UnkSortCmp, UnkRef*>(UnkRef*, UnkRef*, UnkRef*, UnkRef*, UnkSortCmp);
template unsigned std::__sort5<UnkSortCmp, UnkRef*>(UnkRef*, UnkRef*, UnkRef*, UnkRef*, UnkRef*,
                                                    UnkSortCmp);
template bool std::__insertion_sort_incomplete<UnkSortCmp, UnkRef*>(UnkRef*, UnkRef*, UnkSortCmp);

unsigned fn_710117d4d0(UnkRef*, UnkRef*, UnkRef*, UnkSortCmp)
    __attribute__((alias("_ZNSt3__17__sort3I10UnkSortCmpP6UnkRefEEjT0_S4_S4_T_")));
unsigned fn_710117dbf0(UnkRef*, UnkRef*, UnkRef*, UnkRef*, UnkSortCmp)
    __attribute__((alias("_ZNSt3__17__sort4I10UnkSortCmpP6UnkRefEEjT0_S4_S4_S4_T_")));
unsigned fn_710117e030(UnkRef*, UnkRef*, UnkRef*, UnkRef*, UnkRef*, UnkSortCmp)
    __attribute__((alias("_ZNSt3__17__sort5I10UnkSortCmpP6UnkRefEEjT0_S4_S4_S4_S4_T_")));
// NON_MATCHING(minor): passes the known-null handle in a register (mov x0, x21) where ours folds it
// to xzr; same behaviour.
bool fn_710117e5b0(UnkRef*, UnkRef*, UnkSortCmp)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteI10UnkSortCmpP6UnkRefEEbT0_S4_T_")));
// NON_MATCHING(minor): same algorithm and operations, but the original lays the blocks out in
// another order (partition loops and handle releases) and duplicates a few (17 more instructions);
// same behaviour.
void fn_710117c430(UnkRef*, UnkRef*, UnkSortCmp)
    __attribute__((alias("_ZN14libcxx_adapted6__sortI10UnkSortCmpP6UnkRefEEvT0_S4_T_")));
