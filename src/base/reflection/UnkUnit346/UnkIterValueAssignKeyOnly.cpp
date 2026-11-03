// From unit 346 of tools/tu_map.py: two copy assignments of reflected iterator values
// (base/UnkCollectionIter.h) whose element's assignment does nothing, so only the index is
// copied: a u64 index with a 0xc8-byte element (fn_7100260c50) and a string-id key with a
// 0x60-byte element (fn_710025f084). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace {
template <u64 Size>
struct UnkUnassignedElem {
    u64 words[Size / 8];
    UnkUnassignedElem& operator=(const UnkUnassignedElem&) { return *this; }
};
}  // namespace

void* fn_7100260c50(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkUnassignedElem<0xc8>, u64>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
void* fn_710025f084(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkUnassignedElem<0x60>, base::global::CStrId>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
