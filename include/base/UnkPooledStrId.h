#pragma once

// A string id of a given pool, released in two steps (the id and its count reloaded after the
// decrement, the pool after a release), whose string sits in a base that every pool's ids of this
// kind share: a store to one may alias another, even of another pool. base::core::AssetName's
// ids are these (its move in fn_710001e4e8 reloads the string id after writing the file-path id).
// base::UnkStrIdMemberOf (base/UnkDestructors.h) releases the same way but holds the string as its
// own member, which CActor's fn_7100248e50 needs. Placeholder names.

#include "base/global/CStrId.h"
#include "types.h"

namespace base {

struct UnkPooledString {
    global::UnkStringData* data;
};

template <global::UnkStringPool*& Pool>
struct UnkPooledStrId : UnkPooledString {
    // A copy holds another reference to the string (fn_710001e3b4).
    UnkPooledStrId(const UnkPooledStrId& other) : UnkPooledString(other) {
        if (&Pool->empty != data)
            ++data->ref_count;
    }
    // A move takes the string and leaves the pool's empty one (fn_710001e4e8).
    UnkPooledStrId(UnkPooledStrId&& other) : UnkPooledString(other) { other.data = &Pool->empty; }

    bool operator==(const UnkPooledStrId& other) const { return data == other.data; }

    ~UnkPooledStrId() {
        if (global::UnkStringPool* pool = Pool) {
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

}  // namespace base
