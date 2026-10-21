#pragma once

// base::global::CFilePathStrId: a string id like CStrId, but over its own pool
// (g_unk_7101d47200). The name comes from reflection (fn_710000ad9c describes it). The
// constructor from a string is out of line (fn_71000166c8); copying and destruction are
// inline and follow CStrId's, on the other pool.

#include "base/global/CStrId.h"
#include "types.h"

// The file-path string pool (reached through the GOT).
extern base::global::UnkStringPool* g_unk_7101d47200;
u32 fn_7100080570();

namespace base::global {

// The file-path pool's counterpart of UnkEnsureStringPool (the same holder type, another pool).
inline void UnkEnsureFilePathPool() {
    static UnkStringPoolHolder s_holder(fn_7100080570(), &g_unk_7101d47200);
    fn_710007fdb4();
}

}  // namespace base::global

namespace base::global {

// The pointer, copied as a whole (a defaulted copy, as UnkStrIdBase's).
struct UnkFilePathStrIdBase {
    UnkStringData* data;
};

class CFilePathStrId : public UnkFilePathStrIdBase {
public:
    CFilePathStrId(const char* str, bool flag);  // fn_71000166c8
    // The same constructor defined inline, on this pool (as UnkStrIdBase's; reflected default
    // values inline it for the empty path, so only the pool check and the empty id remain).
    __attribute__((always_inline)) CFilePathStrId(UnkStrIdInline, const char* str, bool flag) {
        data = nullptr;
        if (!g_unk_7101d47200)
            UnkEnsureFilePathPool();
        if (str) {
            u64 length = __builtin_strlen(str);
            if (length) {
                u64 hash = fn_7100001570(str, length);
                data = fn_71000804f8(g_unk_7101d47200, hash);
                if (data)
                    ++data->ref_count;
                else
                    data = fn_71000801e4(g_unk_7101d47200, hash, str, length, flag);
            }
        }
        if (!data)
            data = &g_unk_7101d47200->empty;
    }
    CFilePathStrId(const CFilePathStrId& other) : UnkFilePathStrIdBase(other) {
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
