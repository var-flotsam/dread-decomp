// From unit 49 of tools/tu_map.py (0x71000710b0-0x7100071dac): the move assignment of a reflected
// 0x20-byte value (its descriptor's fn_0x50): a counted resource reference at +0x0 (copied), a
// 0xc-byte block at +0x8 and a file-path id at +0x18 (moved: the source gets the pool's empty id).
// Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace {
// A counted reference whose assignment reads the source pointer first.
struct UnkUnit49Ref {
    UnkCountedResource* ptr;

    __attribute__((always_inline)) UnkUnit49Ref& operator=(const UnkUnit49Ref& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
};

// A file-path id moved in: the old one is released (decremented and tested in two steps) and the
// source left with the empty id of the pool loaded before the release.
struct UnkUnit49PathId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkUnit49PathId& operator=(UnkUnit49PathId&& other) {
        base::global::UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &pool->empty;
        return *this;
    }
};

struct UnkUnit49Value {
    UnkUnit49Ref field_0x0;
    struct {
        u32 words[3];
    } field_0x8;
    UnkUnit49PathId field_0x18;

    UnkUnit49Value& operator=(UnkUnit49Value&& other) = default;
};
static_assert(sizeof(UnkUnit49Value) == 0x20, "UnkUnit49Value layout");
}  // namespace

void* fn_7100071a98(void* dst, const void* src) {
    UnkUnit49Value* d = static_cast<UnkUnit49Value*>(dst);
    UnkUnit49Value* s = static_cast<UnkUnit49Value*>(const_cast<void*>(src));
    if (d != s)
        *d = static_cast<UnkUnit49Value&&>(*s);
    return d;
}
