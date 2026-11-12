// CScorpiusFXComponent's unit (tools/tu_map.py, unit 1272): the component's poisonous gas effects, a
// fixed array of up to 50 effect objects (count at +0x64, items from +0x68). The constructor
// (fn_710080f8d4) runs the base constructor and clears the line at +0x48 and the count; the
// creation (fn_710080f4c8) makes the 50 gas effects from the render pool (fn_7100339d48), puts each
// one's scene node on layer 0 and switches it off; the release (inlined in fn_710080f624, the
// shutdown fn_710080f710 and the destructors fn_710080f184 and fn_710080f284) hands each one back
// to the model manager. Reading the array past its count grows it with zeroed slots. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/core/CBaseObject.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAbilitySonar.h"
#include "game/CComponent.h"
#include "game/CSceneComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern char g_unk_7101a74e88[];  // vtable

struct UnkBreakableTileGroup_b46a2;
// Signature as src/game/CBreakableTileGroupComponent/CBreakableTileGroupComponent_b46.cpp declares it.
struct UnkNode;
struct UnkMorphBallLauncher_b57;

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// The effect array: a spacing, the count and raw storage for the items.
struct UnkGasEffects_ovb0008 {
    f32 spacing;  // 0x0
    u32 count;    // 0x4
    alignas(8) u8 storage[50 * sizeof(UnkSwitchObject*)];

    UnkSwitchObject** Items() { return reinterpret_cast<UnkSwitchObject**>(storage); }

    UnkSwitchObject*& operator[](u32 index) {
        UnkSwitchObject** data = Items();
        if (index >= count) {
            __builtin_memset(data + count, 0, (u64)(index - count + 1) * sizeof(UnkSwitchObject*));
            count = index + 1;
        }
        return data[index];
    }
};

struct UnkScorpiusFX_ovb0008 {
    void** vtable;
    u8 pad_0x8[0x40];
    u64 field_0x48[3];
    UnkGasEffects_ovb0008 gas;  // 0x60
};
static_assert(__builtin_offsetof(UnkScorpiusFX_ovb0008, gas) == 0x60, "UnkScorpiusFX_ovb0008 layout");

void fn_710080f8d4(UnkScorpiusFX_ovb0008* self) {
    fn_71009f1910(reinterpret_cast<UnkBreakableTileGroup_b46a2*>(self));
    self->vtable = reinterpret_cast<void**>(g_unk_7101a74e88 + 0x10);
    for (u64& v : self->field_0x48)
        v = 0;
    self->gas.count = 0;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}

static inline void ReleaseGas(UnkScorpiusFX_ovb0008* self) {
    for (u32 i = 0; i < self->gas.count;) {
        u32 index = i++;
        fn_71011a91c4(ModelManager(), self->gas[index], 0);
    }
    self->gas.count = 0;
}

void fn_710080f184(UnkScorpiusFX_ovb0008* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a74e88 + 0x10);
    ReleaseGas(self);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

void fn_710080f284(UnkScorpiusFX_ovb0008* self) {
    fn_710080f184(self);
    fn_7100083c00(self);
}

void fn_710080f624(UnkScorpiusFX_ovb0008* self) {
    ReleaseGas(self);
}

void fn_710080f710(UnkScorpiusFX_ovb0008* self) {
    ReleaseGas(self);
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
}

void fn_710080f4c8(UnkScorpiusFX_ovb0008* self) {
    for (s32 i = 0; i < 50; ++i) {
        static const CFilePathStrId s_path("actors/characters/scorpius/fx/scorpiuspoisonousgas.bcptl", true);
        ModelManager();
        UnkRenderManager* render = g_unk_7101d47300;
        UnkSwitchObject* gas = nullptr;
        fn_7100339d48(render->field_0x218, &s_path, &gas, &g_unk_7101d470e0);
        fn_7100219ae0(render, gas ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(gas) + 8) : nullptr, 0);
        UnkSwitchObject* item = gas;
        fn_7100146dac(item, false);
        self->gas.Items()[self->gas.count] = item;
        self->gas.count = self->gas.count + 1;
    }
}
