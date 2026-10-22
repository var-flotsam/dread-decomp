// Construction of cached iterator values (base::UnkIterValue) from another one: an owned pair is
// moved or copied, a referenced pair is read through the reference (the value then owns it).
// Moves with string-id elements and 4-byte indices (fn_71000cd580; CFilePathStrId's pool
// fn_71000cfd3c), with string-id keys and 4- or 8-byte elements (fn_71000de484, fn_71000de9bc;
// fn_7100120758, fn_7100120c90, fn_71002424e8, fn_7100242a20, fn_7100251208, fn_71002515ac),
// with two-word elements (fn_7100121610, fn_7100121984) and with 8-byte indices and elements
// (fn_71001f1cd8, fn_71001f1fc8), with string-id keys and CFilePathStrId elements (moved in
// fn_71000da264, copied in fn_71000da8dc); copies with string-id elements (fn_71000cda10; CFilePathStrId's
// pool fn_71000d01cc).
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkIterValue;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id moved out by taking its data and leaving the pool's empty string.
template <UnkStringPool** Pool>
struct UnkMovedStrIdT {
    UnkStringData* data;

    UnkMovedStrIdT(UnkMovedStrIdT&& other) : data(other.data) { other.data = &(*Pool)->empty; }
};

// A string id copied by adding a reference (unless it is the empty string).
template <UnkStringPool** Pool>
struct UnkCopiedStrIdT {
    UnkStringData* data;

    UnkCopiedStrIdT(const UnkCopiedStrIdT& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
};

using UnkMovedStrId = UnkMovedStrIdT<&g_unk_7101d47108>;
using UnkMovedFilePathStrId = UnkMovedStrIdT<&g_unk_7101d47200>;
using UnkCopiedStrId = UnkCopiedStrIdT<&g_unk_7101d47108>;
using UnkCopiedFilePathStrId = UnkCopiedStrIdT<&g_unk_7101d47200>;

// Elements moved member by member.
struct UnkWordPair {
    u32 first;
    u32 second;

    UnkWordPair(UnkWordPair&& other) : first(other.first), second(other.second) {}
};

struct UnkWord {
    u32 value;

    UnkWord(UnkWord&& other) : value(other.value) {}
};

template <class T, class K>
static inline void CopyConstruct(UnkIterValue<T, K>* self, const UnkIterValue<T, K>* other) {
    using Value = typename UnkIterValue<T, K>::Value;
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) Value(other->value);
    } else if (other->state == 2) {
        self->state = 1;
        typename UnkIterValue<T, K>::Ref r = other->ref;
        new (&self->value) Value{*r.index, *r.element};
    }
}

#define UNK_ITER_MOVE(addr, T, K)                                                                 \
    __attribute__((flatten)) void fn_##addr(void* dst, const void* src) {                         \
        base::refl_IterValueMove<T, K>(dst, src);                                                 \
    }

UNK_ITER_MOVE(71000cd580, UnkMovedStrId, u32)
UNK_ITER_MOVE(71000cfd3c, UnkMovedFilePathStrId, u32)
UNK_ITER_MOVE(71000de484, UnkWord, UnkMovedStrId)
UNK_ITER_MOVE(71000de9bc, UnkWord, UnkMovedStrId)
UNK_ITER_MOVE(7100120758, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100120c90, u64, UnkMovedStrId)
UNK_ITER_MOVE(71002424e8, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100242a20, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100251208, u64, UnkMovedStrId)
UNK_ITER_MOVE(71002515ac, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100121610, UnkWordPair, u32)
UNK_ITER_MOVE(7100121984, UnkWordPair, u32)
UNK_ITER_MOVE(71001f1cd8, u64, u64)
UNK_ITER_MOVE(71001f1fc8, u64, u64)
UNK_ITER_MOVE(71000da264, UnkMovedFilePathStrId, UnkMovedStrId)
UNK_ITER_MOVE(71000da8dc, UnkCopiedFilePathStrId, UnkMovedStrId)

void fn_71000cda10(UnkIterValue<UnkCopiedStrId>* self, const UnkIterValue<UnkCopiedStrId>* other) {
    CopyConstruct(self, other);
}

void fn_71000d01cc(UnkIterValue<UnkCopiedFilePathStrId>* self, const UnkIterValue<UnkCopiedFilePathStrId>* other) {
    CopyConstruct(self, other);
}
