// CSpawnPointComponent's unit (tools/tu_map.py unit 2704): after the base setup (a virtual of the component, given
// the game manager's blackboard), records whether the owner's position lies on the shape of the "LS_EmmyZone"
// object, tested with the shared shape tester. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "types.h"

struct UnkRefl_fn_7100a48b60;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(void*);

// Returned in memory (not trivially copyable); only bit 0 of the first word is read.
struct UnkShapeHit_A5_b68 {
    u32 flags;  // bit 0: hit
    u8 pad_0x4[0x4];  // 8 bytes here (the frame the original has)
    UnkShapeHit_A5_b68(const UnkShapeHit_A5_b68& other);
};

// The zone object's logic shape component.
struct UnkZoneLogicShape_l126c34 {
    u8 pad_0x0[0x30];
    void* shape;  // 0x30
};

// The owning actor: its translation is at +0x44, +0x54 and +0x64.
struct UnkSpawnOwner_l126c34 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 z;
};

static inline UnkShapeTester_A5_b68* GetTester_l126c34() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}

void fn_7101126c34(CSpawnPointComponent* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    void* blackboard = &manager->field_0x968;
    // vtable slot 0x140: the base setup, given the game manager's blackboard
    reinterpret_cast<void (*)(CSpawnPointComponent*, void*)>(self->vtable[0x140 / 8])(self, blackboard);
    self->field_0x350 = false;

    void* zone;
    {
        UnkGameManager* current = g_unk_7101d47310;
        u32 camera_kind = g_unk_71727bebb8;
        u64 id = fn_7100001570("LS_EmmyZone", 11);
        u32 kind = camera_kind;
        zone = fn_7100deba8c(current, &id, &kind);
    }
    if (!zone)
        return;

    auto* logic = reinterpret_cast<UnkZoneLogicShape_l126c34*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(zone));
    void* shape = logic->shape;
    UnkShapeTester_A5_b68* tester = GetTester_l126c34();
    const UnkSpawnOwner_l126c34* owner = self->Owner<UnkSpawnOwner_l126c34>();
    struct {
        f32 x, z;
    } pos = {owner->x, owner->z};
    self->field_0x350 = fn_7100d572f8(tester, shape, reinterpret_cast<const base::math::CVector2D*>(&pos),
                                      &g_unk_710164779c, 0).flags & 1;
}
