// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): for the link `index` of the component's
// links at +0x146d0, takes a resolved copy of it; when the linked object is registered, has its shot
// object (fn_7100405ba0), passes its slot 0x168 check and either has no part or the tunable allows
// simultaneous Rinkas, hands the shot object the value at +0x14730 and `value` (fn_710066008c).
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/global/CRntVector.h"
#include "game/CAISmartObjectComponent.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCentralUnitPart_l405d58;
struct UnkRinkaUnit_b61;
void* fn_7100405ba0(void* object);
void CRinkaUnitComponent_SetKindAndRequestState2(UnkRinkaUnit_b61* self, u32 a, u32 b);

// The central unit's tunable (CTunableCharClassCentralUnitAIComponent).
extern base::reflection::UnkTunable g_unk_71726c42a0;
struct UnkCentralUnitTunable_l405d58 {
    u8 pad_0x0[0x21];
    bool bAllowSimultaneousRinkas;  // 0x21
};

// The resolved link's weak handle: its target at +0x10.
struct UnkCentralUnitLinkHandle_l405d58 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkCentralUnitPartHolder_l405d58 {
    u8 pad_0x0[0x10];
    UnkCentralUnitPart_l405d58* part;  // 0x10
};

struct UnkCentralUnitRegistered_l405d58 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkCentralUnitPartHolder_l405d58* holder;  // 0x40

    bool vf_0x168(f32 value) {
        return reinterpret_cast<bool (*)(UnkCentralUnitRegistered_l405d58*, f32)>(vtable[0x168 / 8])(this, value);
    }
};

struct UnkCentralUnitAI_l405d58 {
    u8 pad_0x0[0x146d0];
    base::global::CRntVector<UnkCallbackSet_7101a20950> links;  // 0x146d0
    u8 pad_0x146f8[0x38];
    u32 field_0x14730;
};
static_assert(__builtin_offsetof(UnkCentralUnitAI_l405d58, field_0x14730) == 0x14730, "layout");

// Resolves the link once (while its state is 1) and returns its target, or null.
inline void* CentralUnitLinkTarget_l405d58(UnkCallbackSet_7101a20950& link) {
    if (link.field_0x30 == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link.owner),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.field_0x30 = 2;
    }
    UnkCentralUnitLinkHandle_l405d58* handle =
        reinterpret_cast<UnkCentralUnitLinkHandle_l405d58*>(link.owner);
    return handle ? handle->target : nullptr;
}

bool fn_7100405d58(UnkCentralUnitAI_l405d58* self, u32 index, u32 value) {
    if (self->links.count <= index)
        return false;
    UnkCallbackSet_7101a20950 link(*reinterpret_cast<UnkCallbackSet_7101a20950*>(
        fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(&self->links), index)));
    UnkCentralUnitRegistered_l405d58* registered = reinterpret_cast<UnkCentralUnitRegistered_l405d58*>(
        fn_7100360044(CentralUnitLinkTarget_l405d58(link)));
    if (!registered)
        return false;
    UnkCentralUnitPart_l405d58* part = nullptr;
    if (UnkCentralUnitPartHolder_l405d58* holder = registered->holder)
        part = holder->part;
    void* shot = fn_7100405ba0(CentralUnitLinkTarget_l405d58(link));
    if (!shot || !registered->vf_0x168(0.0f))
        return false;
    const UnkCentralUnitTunable_l405d58* tunable =
        reinterpret_cast<const UnkCentralUnitTunable_l405d58*>(&g_unk_71726c42a0);
    if (part && !tunable->bAllowSimultaneousRinkas)
        return false;
    CRinkaUnitComponent_SetKindAndRequestState2(static_cast<UnkRinkaUnit_b61*>(shot), self->field_0x14730, value);
    return true;
}
