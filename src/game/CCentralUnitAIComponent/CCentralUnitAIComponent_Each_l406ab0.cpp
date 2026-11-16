// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): for each of the component's links at
// +0x146d0, takes a copy of it, resolves the copy, and hands the linked object's part to fn_7100232c68.
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CAISmartObjectComponent.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCentralUnitPart_l406ab0;

// The resolved link's weak handle: its target at +0x10.
struct UnkCentralUnitLinkHandle_l406ab0 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkCentralUnitPartHolder_l406ab0 {
    u8 pad_0x0[0x10];
    UnkCentralUnitPart_l406ab0* part;  // 0x10
};

struct UnkCentralUnitRegistered_l406ab0 {
    u8 pad_0x0[0x40];
    UnkCentralUnitPartHolder_l406ab0* holder;  // 0x40
};

struct UnkCentralUnitAI_l406ab0 {
    u8 pad_0x0[0x146d0];
    base::global::CRntVector<UnkCallbackSet_7101a20950> links;  // 0x146d0
};

// Resolves the link once (while its state is 1) and returns its target, or null.
inline void* CentralUnitLinkTarget_l406ab0(UnkCallbackSet_7101a20950& link) {
    if (link.field_0x30 == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link.owner),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.field_0x30 = 2;
    }
    UnkCentralUnitLinkHandle_l406ab0* handle =
        reinterpret_cast<UnkCentralUnitLinkHandle_l406ab0*>(link.owner);
    return handle ? handle->target : nullptr;
}

struct UnkCentralUnitAI3_ovb0125;

void fn_7100406ab0(UnkCentralUnitAI3_ovb0125* component) {
    UnkCentralUnitAI_l406ab0* self = reinterpret_cast<UnkCentralUnitAI_l406ab0*>(component);
    for (UnkCallbackSet_7101a20950* it = self->links.data, *end = it + self->links.count; it != end; ++it) {
        UnkCallbackSet_7101a20950 link(*it);
        UnkCentralUnitRegistered_l406ab0* registered = reinterpret_cast<UnkCentralUnitRegistered_l406ab0*>(
            fn_7100360044(CentralUnitLinkTarget_l406ab0(link)));
        if (registered && registered->holder) {
            if (UnkCentralUnitPart_l406ab0* part = registered->holder->part)
                fn_7100232c68(part);
        }
    }
}
