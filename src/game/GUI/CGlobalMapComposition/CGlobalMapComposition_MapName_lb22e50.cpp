// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): the map-name id ("MAP_CAV", ...) of a
// scenario id, or the empty id for any other scenario. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2564.h"
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

UnkMapNameId fn_7100b22e50(const CStrId& scenario) {
    UnkMapNameId result;
    const char* name;
    if (scenario.data == g_unk_71727c7fe8.data)
        name = "MAP_CAV";
    else if (scenario.data == g_unk_71727c7ff0.data)
        name = "MAP_MAG";
    else if (scenario.data == g_unk_71727c7ff8.data)
        name = "MAP_LAB";
    else if (scenario.data == g_unk_71727c8000.data)
        name = "MAP_AQUA";
    else if (scenario.data == g_unk_71727c8008.data)
        name = "MAP_FOR";
    else if (scenario.data == g_unk_71727c8010.data)
        name = "MAP_QUAR";
    else if (scenario.data == g_unk_71727c8018.data)
        name = "MAP_SANC";
    else if (scenario.data == g_unk_71727c8020.data)
        name = "MAP_SHIP";
    else if (scenario.data == g_unk_71727c8028.data)
        name = "MAP_SKYB";
    else
        return result;
    result = CStrId(name, true);
    return result;
}
