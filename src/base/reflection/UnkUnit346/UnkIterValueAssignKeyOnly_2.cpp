// Copies of fn_710025f084 (src/base/reflection/UnkUnit346/UnkIterValueAssignKeyOnly.cpp): copy
// assignments of reflected iterator values (base/UnkCollectionIter.h) whose element's assignment
// does nothing, so only the string-id key is copied. fn_7101041c3c keys by CStrId; fn_7100e0dd5c
// keys by a file-path id, whose assignment follows CStrId's on its own pool. Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace {
template <u64 Size>
struct UnkUnassignedElem {
    u64 words[Size / 8];
    UnkUnassignedElem& operator=(const UnkUnassignedElem&) { return *this; }
};
}  // namespace

// A file-path id's copy assignment, as CStrId's on g_unk_7101d47200 (outside an anonymous
// namespace: there it is inlined with the loads in the other order).
struct UnkFilePathIterKey : base::global::UnkFilePathStrIdBase {
    UnkFilePathIterKey& operator=(const UnkFilePathIterKey& other) {
        if (data != other.data) {
            base::global::UnkStringData* d = other.data;
            base::global::UnkStringPool* pool = g_unk_7101d47200;
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
};

void* fn_7100e0dd5c(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkUnassignedElem<0x90>, UnkFilePathIterKey>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
void* fn_7101041c3c(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkUnassignedElem<0x5d0>, base::global::CStrId>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
// A copy of fn_7100260c50 with a u32 index and a 0x60-byte element.
void* fn_7100da0d68(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkUnassignedElem<0x60>, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
