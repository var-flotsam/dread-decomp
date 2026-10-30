// The CAbilitySonar unit (tools/tu_map.py unit 434, by its reflected class): fn_7100339ea0 loads a
// resource of one registered type from a path and keeps it in two holders. Which object the holders
// belong to is inferred from the code only; names and layouts are placeholders.
#include "base/UnkResKey.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

// Loads the resource of type g_unk_716e454078 at `path`; when there is one, keeps it at +0x10 and
// +0xa0, tells the object at +0x28 (vf_0x90) and returns true.
bool fn_7100339ea0(UnkResHolder* self, const CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e454078>(self, path);
}

// The same for a model.
bool fn_7100339a50(UnkResHolder* self, const CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e453ff8>(self, path);
}
