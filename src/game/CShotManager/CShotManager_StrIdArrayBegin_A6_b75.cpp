// Begin iterators of the fixed string-id arrays whose end iterators are in CShotManager_CTunableShotManager.cpp
// (8 file-path ids in CShotManager::CTunableShotManager's unit, 9 plain ids in unit 2612): a temporary iterator at
// (0, the first element), set to refer to it only when the element is non-null, assigned to the result (re-pointed out
// of line), then destroyed. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkTU2612.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id on the given pool whose release decrements and tests the count in two steps.
template <UnkStringPool*& Pool>
struct UnkPathElem_A6_b75 {
    UnkStringData* data;

    ~UnkPathElem_A6_b75() {
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
struct UnkPathIterValue_A6_b75 {
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

    __attribute__((always_inline)) ~UnkPathIterValue_A6_b75() {
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
struct UnkPathIter_A6_b75 {
    u64 index;
    Elem* element;
    UnkPathIterValue_A6_b75<Elem> current;

    __attribute__((always_inline)) UnkPathIter_A6_b75(u64 index, Elem* element) : index(index), element(element) {
        current.state = 0;
        if (element) {
            current.state = 2;
            current.ref.index = &this->index;
            current.ref.element = element;
        }
    }

    __attribute__((always_inline)) UnkPathIter_A6_b75& operator=(const UnkPathIter_A6_b75& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            Sync(this);
        }
        return *this;
    }
};

using UnkFilePathElem_A6_b75 = UnkPathElem_A6_b75<g_unk_7101d47200>;
using UnkStrIdElem_A6_b75 = UnkPathElem_A6_b75<g_unk_7101d47108>;

template <class Elem, u32 N>
struct UnkPathArray_A6_b75 {
    Elem paths[N];
};

void fn_7100edc458(UnkPathArray_A6_b75<UnkFilePathElem_A6_b75, 8>* self,
                   UnkPathIter_A6_b75<UnkFilePathElem_A6_b75, fn_7100ed8144>* out) {
    *out = UnkPathIter_A6_b75<UnkFilePathElem_A6_b75, fn_7100ed8144>(0, self->paths);
}

void fn_7100fb1af0(UnkPathArray_A6_b75<UnkStrIdElem_A6_b75, 9>* self,
                   UnkPathIter_A6_b75<UnkStrIdElem_A6_b75, fn_7100fb0000>* out) {
    *out = UnkPathIter_A6_b75<UnkStrIdElem_A6_b75, fn_7100fb0000>(0, self->paths);
}
