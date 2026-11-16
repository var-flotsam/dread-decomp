// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): for the link `index` of the component's
// links at +0x146d0, takes a copy of it, resolves the copy, and asks whether the linked object's part
// passes the two checks of this unit (fn_7100405c7c with the value at +0x14730, fn_7100405ba0).
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CAISmartObjectComponent.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCentralUnitPart_l406080;
void* fn_7100405c7c(UnkCentralUnitPart_l406080* part);
void* fn_7100405ba0(UnkCentralUnitPart_l406080* part);
bool fn_71003fc0a8(void* object, u32 value);

// The resolved link's weak handle: its target at +0x10.
struct UnkCentralUnitLinkHandle_l406080 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkCentralUnitPartHolder_l406080 {
    u8 pad_0x0[0x10];
    UnkCentralUnitPart_l406080* part;  // 0x10
};

struct UnkCentralUnitRegistered_l406080 {
    u8 pad_0x0[0x40];
    UnkCentralUnitPartHolder_l406080* holder;  // 0x40
};

struct UnkCentralUnitAI_l406080 {
    u8 pad_0x0[0x146d0];
    base::global::CRntVector<UnkCallbackSet_7101a20950> links;  // 0x146d0
    u8 pad_0x146f8[0x38];
    u32 field_0x14730;
};
static_assert(__builtin_offsetof(UnkCentralUnitAI_l406080, field_0x14730) == 0x14730, "layout");

// Resolves the link once (while its state is 1) and returns its target, or null.
inline void* CentralUnitLinkTarget_l406080(UnkCallbackSet_7101a20950& link) {
    if (link.field_0x30 == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link.owner),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.field_0x30 = 2;
    }
    UnkCentralUnitLinkHandle_l406080* handle =
        reinterpret_cast<UnkCentralUnitLinkHandle_l406080*>(link.owner);
    return handle ? handle->target : nullptr;
}

bool fn_7100406080(UnkCentralUnitAI_l406080* self, u32 index) {
    if (self->links.count <= index)
        return false;
    UnkCallbackSet_7101a20950 link(*reinterpret_cast<UnkCallbackSet_7101a20950*>(
        fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(&self->links), index)));
    UnkCentralUnitRegistered_l406080* registered = reinterpret_cast<UnkCentralUnitRegistered_l406080*>(
        fn_7100360044(CentralUnitLinkTarget_l406080(link)));
    if (!registered || !registered->holder)
        return false;
    UnkCentralUnitPart_l406080* part = registered->holder->part;
    if (!part)
        return false;
    bool hit = false;
    if (void* object = fn_7100405c7c(part))
        hit = fn_71003fc0a8(object, self->field_0x14730);
    if (fn_7100405ba0(part))
        hit = true;
    return hit;
}
