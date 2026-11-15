#pragma once

// base::core::AssetName (the reflected name of the element type of the CRntVector descriptor built
// by fn_7100019dcc): a string id at 0 and a file-path id at 8, both released in two steps and
// sharing one base holding the string (base/UnkPooledStrId.h).
//
// base/UnkStrIdPathPair.h has the same layout under a placeholder name, with ids that hold the
// string as their own member; it is not made an alias of this yet (see the journal, 2026-11-15).

#include "base/UnkPooledStrId.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

namespace base::core {

struct AssetName {
    UnkPooledStrId<g_unk_7101d47108> id;
    UnkPooledStrId<g_unk_7101d47200> path;

    // Both ids compare equal (fn_710001e1d0).
    bool operator==(const AssetName& other) const { return id == other.id && path == other.path; }
};
static_assert(sizeof(AssetName) == 0x10, "AssetName layout");

}  // namespace base::core
