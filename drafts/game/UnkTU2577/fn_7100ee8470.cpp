// Unit 2577 (CSubAreaManager): rebuilds the two id lists of a pair of keys and reports whether
// either differs from the lists collected into temporaries (or `force` is set), storing the new
// lists in that case. Names and layouts are placeholders.

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"



// A string id released in two steps (the count is read back after the decrement).
struct UnkListId : base::global::UnkStrIdBase {
    ~UnkListId() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

using UnkIdList = base::UnkVector<UnkListId>;

inline bool Contains(const UnkIdList& list, const UnkListId& id) {
    for (const UnkListId *it = list.data, *end = it + list.count; it != end; ++it) {
        if (it->data == id.data)
            return true;
    }
    return false;
}



void fn_7100eeb87c(void* self, const void* key, UnkIdList* out);
void fn_71000ce19c(void*, const void*);

bool fn_7100ee8470(void* self, const void* second_key, const void* first_key, UnkIdList* first_out,
                   UnkIdList* second_out, bool force) {
    first_out->Clear();
    second_out->Clear();

    UnkIdList first(0xf);
    fn_7100eeb87c(self, first_key, &first);
    UnkIdList second(0xf);
    fn_7100eeb87c(self, second_key, &second);

    bool changed = force;
    for (const UnkListId *it = first.data, *end = it + first.count; it != end; ++it) {
        if (!Contains(second, *it)) {
            changed = true;
            break;
        }
    }
    for (const UnkListId *it = second.data, *end = it + second.count; it != end && !changed; ++it) {
        if (!Contains(first, *it))
            changed = true;
    }

    if (changed) {
        fn_71000ce19c(first_out, &first);
        fn_71000ce19c(second_out, &second);
    }
    second.Free();
    first.Free();
    return changed;
}
