// Pilot (2026-11-15), not built and not indexed: CRntVector<base::core::AssetName>'s out-of-line
// helpers instantiated from the members of include/base/global/CRntVector.h. The functions map to
// their addresses only once data/functions/ names them after the members (a local, uncommitted
// rename was used for the trial):
//   0x710001e4e8 Reserve   0x710001e3b4 CRntVector (copy)   0x710001e1d0 operator==
//   0x710001e62c Truncate  0x710001e870 InsertCopy          0x710001ead8 InsertMove
// State: Truncate matches; Reserve (84%), the copy constructor (93.5%) and operator== (56%) differ in
// registers only; InsertCopy (39%) and InsertMove (6%) differ in block placement and an early load of
// the path pool. AssetName's two ids release in two steps (decrement, then re-read the count) and
// share one base holding the string, unlike CStrId and CFilePathStrId.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// Both ids' common base: the string they hold (a store through one may alias the other).
struct UnkStringRef {
    UnkStringData* data;
};

template <UnkStringPool*& Pool>
struct UnkPooledId : UnkStringRef {
    UnkPooledId(const UnkPooledId& other) : UnkStringRef(other) {
        if (&Pool->empty != data)
            ++data->ref_count;
    }
    UnkPooledId(UnkPooledId&& other) : UnkStringRef(other) { other.data = &Pool->empty; }
    bool operator==(const UnkPooledId& other) const { return data == other.data; }
    ~UnkPooledId() {
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

namespace base::core {
struct AssetName {
    UnkPooledId<g_unk_7101d47108> id;
    UnkPooledId<g_unk_7101d47200> path;
    bool operator==(const AssetName& other) const { return id == other.id && path == other.path; }
};
}  // namespace base::core

using Vec = base::global::CRntVector<base::core::AssetName>;
template void Vec::Reserve(u32);
template Vec::CRntVector(const Vec&);
template bool Vec::operator==(const Vec&) const;
template void Vec::Truncate(u32);
template void Vec::InsertCopy(const base::core::AssetName&, u32);
template void Vec::InsertMove(base::core::AssetName&&, u32);
