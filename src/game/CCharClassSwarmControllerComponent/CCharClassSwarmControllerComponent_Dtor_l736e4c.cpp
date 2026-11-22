// CCharClassSwarmControllerComponent's unit: the component's destructor. Empties its three id
// lists, destroys its id pairs, strings and damage factors, releases its id and runs the base
// destructor. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassSwarmControllerComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkTU32.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// Up to ten ids stored in place after their count.
struct UnkSwarmIdList_l736e4c {
    u32 count;
    u8 storage[10 * sizeof(CStrId)];

    CStrId* items() { return reinterpret_cast<CStrId*>(storage); }

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            items()[i].Reset();
        count = 0;
    }
};
static_assert(sizeof(UnkSwarmIdList_l736e4c) == 0x54, "UnkSwarmIdList_l736e4c layout");

// Three id pairs, destroyed last to first.
struct UnkSwarmIdPairs_l736e4c {
    u8 pair_0x0[0x18];
    u8 pair_0x18[0x18];
    u8 pair_0x30[0x18];

    void Destroy() {
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(pair_0x30));
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(pair_0x18));
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(pair_0x0));
    }
};

struct UnkSwarmController_l736e4c {
    void* vtable;
    u8 pad_0x8[0x68];
    CStrId field_0x70;
    u8 pad_0x78[0x98];
    u8 field_0x110[0x70];
    CRntString field_0x180;
    CRntString field_0x1b0;
    CRntString field_0x1e0;
    CRntString field_0x210;
    CRntString field_0x240;
    CRntString field_0x270;
    UnkSwarmIdPairs_l736e4c field_0x2a0;
    UnkSwarmIdPairs_l736e4c field_0x2e8;
    u8 pad_0x330[0x4];
    UnkSwarmIdList_l736e4c field_0x334;
    UnkSwarmIdList_l736e4c field_0x388;
    UnkSwarmIdList_l736e4c field_0x3dc;
};
static_assert(__builtin_offsetof(UnkSwarmController_l736e4c, field_0x180) == 0x180, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l736e4c, field_0x334) == 0x334, "layout");

void fn_7100736e4c(UnkSwarmController_l736e4c* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6d688) + 0x10;
    self->field_0x3dc.Clear();
    self->field_0x388.Clear();
    self->field_0x334.Clear();
    self->field_0x2e8.Destroy();
    self->field_0x2a0.Destroy();
    CRntString_Destroy(&self->field_0x270);
    CRntString_Destroy(&self->field_0x240);
    CRntString_Destroy(&self->field_0x210);
    CRntString_Destroy(&self->field_0x1e0);
    CRntString_Destroy(&self->field_0x1b0);
    CRntString_Destroy(&self->field_0x180);
    fn_71008c2de0(self->field_0x110);
    self->field_0x70.Reset();
    fn_71011acd74(reinterpret_cast<UnkIdObject*>(self));
}
