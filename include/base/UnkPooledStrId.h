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

    // Another reference to an id's string, such as the global empty id's (fn_710001de1c). The string
    // is stored in the body: the original loads it before it computes the member's address.
    explicit UnkPooledStrId(global::UnkStringData* const& string) {
        data = string;
        if (&Pool->empty != data)
            ++data->ref_count;
    }

    // As CStrId's: the new string gains a reference before the old one is released.
    UnkPooledStrId& operator=(const UnkPooledStrId& other) {
        if (data != other.data) {
            global::UnkStringData* d = other.data;
            global::UnkStringPool* pool = Pool;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    // Releases the string as the destructor does, then takes the other's (fn_710001dffc).
    UnkPooledStrId& operator=(UnkPooledStrId&& other) {
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
        data = other.data;
        other.data = &Pool->empty;
        return *this;
    }

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
