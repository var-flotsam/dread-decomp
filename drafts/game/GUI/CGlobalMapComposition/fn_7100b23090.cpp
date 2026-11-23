// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): the teleporter actor of a transport
// (its name holds the two areas it links, as "MAG-CAV1"), or the empty id for a transport with none.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// The id the lookup returns: starts as the pool's empty entry (making the pool if there is none yet)
// and is released like a CStrId.
struct UnkMapNameId {
    UnkStringData* data;

    UnkMapNameId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkMapNameId() { reinterpret_cast<CStrId*>(this)->Reset(); }
    // Releases the string held and takes the one `id` holds, leaving `id` empty.
    UnkMapNameId& operator=(CStrId&& id) {
        reinterpret_cast<CStrId*>(this)->ReleaseAndTake(id);
        return *this;
    }
};

// Whether the id's text contains `str` (an empty id's does not); reading it counts a read of the id.
static inline bool NameHas(const CStrId& id, const char* str) {
    UnkStringData* string = id.data;
    UnkStringData* empty = &g_unk_7101d47108->empty;
    ++string->reads;
    if (id.data == empty)
        return false;
    return CRntString_Contains(&id.data->String(), str);
}

UnkMapNameId fn_7100b23090(const CStrId& transport) {
    UnkMapNameId result;
    const char* name;
    if (NameHas(transport, "AQUA-SANC"))
        name = "UsableTeleportX";
    else if (NameHas(transport, "LAB-CAV"))
        name = "UsableTeleportI";
    else if (NameHas(transport, "MAG-CAV1"))
        name = "UsableTeleportA";
    else if (NameHas(transport, "MAG-CAV2"))
        name = "UsableTeleportU";
    else if (NameHas(transport, "MAG-LAB"))
        name = "UsableTeleportE";
    else if (NameHas(transport, "MAG-FOR"))
        name = "UsableTeleportY";
    else if (NameHas(transport, "AQUA-FOR"))
        name = "UsableTeleportO";
    else
        return result;
    result = CStrId(name, true);
    return result;
}
