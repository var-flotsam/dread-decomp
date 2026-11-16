// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): the scenario id of a map-name id
// ("MAP_CAV", ...), the inverse of fn_7100b22e50, or the empty id for any other name. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2564.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringData;

// The id the lookup returns: starts as the pool's empty entry (making the pool if there is none yet)
// and is released and assigned like a CStrId.
struct UnkScenarioId {
    UnkStringData* data;

    UnkScenarioId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkScenarioId() { reinterpret_cast<CStrId*>(this)->Reset(); }
    UnkScenarioId& operator=(const UnkStrIdBase& id) {
        *reinterpret_cast<CStrId*>(this) = static_cast<const CStrId&>(id);
        return *this;
    }
};

UnkScenarioId fn_7100b22910(const CStrId& map) {
    UnkScenarioId result;
    const UnkStrIdBase* scenario;
    if (map == CStrId("MAP_CAV", true))
        scenario = &g_unk_71727c7fe8;
    else if (map == CStrId("MAP_MAG", true))
        scenario = &g_unk_71727c7ff0;
    else if (map == CStrId("MAP_LAB", true))
        scenario = &g_unk_71727c7ff8;
    else if (map == CStrId("MAP_AQUA", true))
        scenario = &g_unk_71727c8000;
    else if (map == CStrId("MAP_FOR", true))
        scenario = &g_unk_71727c8008;
    else if (map == CStrId("MAP_QUAR", true))
        scenario = &g_unk_71727c8010;
    else if (map == CStrId("MAP_SANC", true))
        scenario = &g_unk_71727c8018;
    else if (map == CStrId("MAP_SHIP", true))
        scenario = &g_unk_71727c8020;
    else if (map == CStrId("MAP_SKYB", true))
        scenario = &g_unk_71727c8028;
    else
        return result;
    result = *scenario;
    return result;
}
