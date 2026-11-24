// CCharClassBillboardComponent unit: fn_710072f3cc is the destructor of the component fn_710072f1a4
// builds: hands its two ids' keys to the billboard manager (made on first use), takes its items out of
// the render manager, frees the arrays and releases the ids, then runs the component base's
// destructor. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassBillboardComponent.h"
#include "game/CCharClassComponent.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkBillboardComponent_l72f1a4.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2560.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkItemBase_71011a91c4;

void fn_7100e1d0a8(void* manager, const u64* key);
static_assert(__builtin_offsetof(UnkBillboardComponent_l72f1a4, fSoundImpactRadiusMaxAtt) == 0x160, "layout");

// The billboard manager, made on first use.
inline void* UnkBillboardManager_l72f3cc() {
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    return manager;
}

inline bool UnkIsEmpty_l72f3cc(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

inline void UnkRemoveItems_l72f3cc(base::global::CRntVector<UnkItemBase_71011a91c4*>& items) {
    for (UnkItemBase_71011a91c4 **it = items.data, **end = items.data + items.count; it != end;) {
        UnkItemBase_71011a91c4* item = *it++;
        fn_71002199d8(g_unk_7101d47300, item, 0);
    }
}

void fn_710072f3cc(UnkBillboardComponent_l72f1a4* self) {
    self->vtable = g_unk_7101a6d578 + 0x10;
    if (!UnkIsEmpty_l72f3cc(self->field_0xd0)) {
        void* manager = UnkBillboardManager_l72f3cc();
        u64 key = self->field_0xd0.data->Key();
        fn_7100e1ceb0(manager, &key);
    }
    if (!UnkIsEmpty_l72f3cc(self->field_0xd8)) {
        void* manager = UnkBillboardManager_l72f3cc();
        u64 key = self->field_0xd8.data->Key();
        fn_7100e1d0a8(manager, &key);
    }
    UnkRemoveItems_l72f3cc(self->field_0xe0);
    UnkRemoveItems_l72f3cc(self->field_0x108);
    self->field_0xe0.count = 0;
    self->field_0x108.count = 0;
    self->sSoundImpactBase.~CStrId();
    self->sSoundLoopLit.~CFilePathStrId();
    self->sSoundLoopLot.~CStrId();
    self->field_0x108.Free();
    self->field_0xe0.Free();
    self->field_0xd8.~CStrId();
    self->field_0xd0.~CStrId();
    self->sGroupMotionDefaultLoop.~CStrId();
    self->sBillboardMotionDefaultLoop.~CStrId();
    self->sBillboardDefaultLoop.~CStrId();
    self->sBgPath.~CFilePathStrId();
    self->sPath.~CFilePathStrId();
    fn_71011acd74(reinterpret_cast<UnkIdObject_8da5c0*>(self));
}