// CCharClassEventScenarioComponent's unit (1251): for each of the links at +0x108, takes a copy of it,
// resolves the copy, and enables or disables the linked entity (fn_7100232864). Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The resolved link's weak handle: its target at +0x10.
struct UnkScenarioLinkHandle_l7e4a00 {
    u8 pad_0x0[0x10];
    UnkEntityState* target;  // 0x10
};

struct UnkScenarioLinks_l7e4a00 {
    u8 pad_0x0[0x108];
    base::global::CRntVector<UnkCallbackSet_7101a20950> links;  // 0x108
};

// Resolves the link once (while its state is 1) and returns its handle, or null.
inline UnkScenarioLinkHandle_l7e4a00* ResolveScenarioLink_l7e4a00(UnkCallbackSet_7101a20950& link) {
    if (link.field_0x30 == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link.owner),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.field_0x30 = 2;
    }
    return reinterpret_cast<UnkScenarioLinkHandle_l7e4a00*>(link.owner);
}

void fn_71007e4a00(UnkScenarioLinks_l7e4a00* self, bool enable) {
    for (UnkCallbackSet_7101a20950* it = self->links.data, *end = it + self->links.count; it != end; ++it) {
        UnkCallbackSet_7101a20950 link(*it);
        UnkScenarioLinkHandle_l7e4a00* handle = ResolveScenarioLink_l7e4a00(link);
        if (!handle || !handle->target)
            continue;
        fn_7100232864(handle->target, enable);
    }
}
