#pragma once

// base::global::CFilePathStrId: a string id like CStrId, but over its own pool
// (g_unk_7101d47200). The name comes from reflection (fn_710000ad9c describes it). The
// constructor from a string is out of line (fn_71000166c8); copying and destruction are
// inline and follow CStrId's, on the other pool.

#include "base/global/CStrId.h"
#include "types.h"

// The file-path string pool (reached through the GOT).
extern base::global::UnkStringPool* g_unk_7101d47200;

namespace base::global {

class CFilePathStrId {
public:
    UnkStringData* data;

    CFilePathStrId(const char* str, bool flag);  // fn_71000166c8
    CFilePathStrId(const CFilePathStrId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~CFilePathStrId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

}  // namespace base::global

// A global CFilePathStrId that default values copy (possibly the empty id).
extern base::global::CFilePathStrId g_unk_7101d470e8;
