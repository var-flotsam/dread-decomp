// End iterators of fixed arrays of string ids (8 file-path ids in CShotManager::CTunableShotManager's
// unit, 9 plain string ids in another unit): a temporary iterator at (count, one past the last element), assigned to
// the result, whose current value is re-pointed out of line (fn_7100ed8144, fn_7100fb0000), then
// the temporary's value destroyed (an owned id is released in two steps). The shape of
// base::refl_End (include/base/UnkCollectionIter.h). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id on the given pool whose release decrements and tests the count in two steps.
template <UnkStringPool*& Pool>
struct UnkPathElem {
    UnkStringData* data;

    ~UnkPathElem() {
        if (UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

template <class Elem>
struct UnkPathIterValue {
    struct Value {
        u64 index;
        Elem element;
    };
    struct Ref {
        const u64* index;
        Elem* element;
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;  // 0 empty, 1 owns `value`, 2 refers through `ref`

    __attribute__((always_inline)) ~UnkPathIterValue() {
        if (state == 2) {
        } else if (state == 1) {
            value.~Value();
        } else {
            return;
        }
        state = 0;
    }
};

template <class Elem, void (*Sync)(void*)>
struct UnkPathIter {
    u64 index;
    Elem* element;
    UnkPathIterValue<Elem> current;

    __attribute__((always_inline)) UnkPathIter(u64 index, Elem* element) : index(index), element(element) {
        current.state = 2;
        current.ref.index = &this->index;
        current.ref.element = element;
    }

    __attribute__((always_inline)) UnkPathIter& operator=(const UnkPathIter& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            Sync(this);
        }
        return *this;
    }
};

void fn_7100ed8144(void* it);
void fn_7100fb0000(void* it);

using UnkFilePathElem = UnkPathElem<g_unk_7101d47200>;
using UnkStrIdElem = UnkPathElem<g_unk_7101d47108>;

template <class Elem, u32 N>
struct UnkPathArray {
    Elem paths[N];
};

void fn_7100edc524(UnkPathArray<UnkFilePathElem, 8>* self, UnkPathIter<UnkFilePathElem, fn_7100ed8144>* out) {
    *out = UnkPathIter<UnkFilePathElem, fn_7100ed8144>(8, self->paths + 8);
}

void fn_7100fb1bbc(UnkPathArray<UnkStrIdElem, 9>* self, UnkPathIter<UnkStrIdElem, fn_7100fb0000>* out) {
    *out = UnkPathIter<UnkStrIdElem, fn_7100fb0000>(9, self->paths + 9);
}
