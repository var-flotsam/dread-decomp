#pragma once

// Whether a point lies in the shape of the central unit's tracked component: the central unit comes from
// the lazily created registry g_unk_71726bee10 (fn_7100ddb768), its target from its part at +0x5a8
// (fn_71002a76a4), the component from the target's actor (refl_FindTyped, fn_710028aca8) with its shape at
// +0x30; the point is projected by fn_710007b430 and tested by the lazily created shape tester (as
// src/game/CLogicShapeComponent/CLogicShapeComponent_Test_A5_b68.cpp). Used by
// CFulmiteBellyMineAIComponent_InShape_A3_b70.cpp; fn_710057d6f0 and fn_71005d09a8 (CInfesterBallAIComponent)
// are the same test of a moved point. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAutsharpAIComponent.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU365.h"
#include "types.h"

struct UnkDoorRegistry_b61;
struct UnkUnit2547;
struct UnkCentralUnit;
struct UnkBreakablePart;
struct UnkBreakableTarget;

struct UnkRefl_fn_710028aca8;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710028aca8>(void*);

struct UnkShapeTester_A5_b68;

// Returned in memory (not trivially copyable); only bit 0 of the first word is read.
struct UnkShapeHit_A5_b68 {
    u32 flags;
    u8 pad_0x4[0xc];
    UnkShapeHit_A5_b68(const UnkShapeHit_A5_b68& other);
};
UnkShapeHit_A5_b68 fn_7100d572f8(UnkShapeTester_A5_b68* tester, void* shape, const base::math::CVector2D* pos,
                                 const base::math::CVector3D* offset, u64 flags);

// The owner's matrix translation (+0x44/+0x54/+0x64).
struct UnkShapeOwner_A3_b70 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkShapeTarget_A3_b70 {
    u8 pad_0x0[0x10];
    void* actor;
};

template <class Point>
__attribute__((always_inline)) inline bool CentralShapeTest_A3_b70(Point point) {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    char* unit = reinterpret_cast<char*>(fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(reg)));
    if (!unit)
        return false;
    auto* target = reinterpret_cast<UnkShapeTarget_A3_b70*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(unit + 0x5a8)));
    if (!target)
        return false;
    if (!target->actor)
        return false;
    char* comp = reinterpret_cast<char*>(base::reflection::refl_FindTyped<UnkRefl_fn_710028aca8>(target->actor));
    if (!comp)
        return false;
    if (!*reinterpret_cast<void**>(comp + 0x30))
        return false;
    UnkShapeTester_A5_b68* tester = g_unk_71726a47a0;
    if (!tester) {
        tester = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(tester);
        g_unk_71726a47a0 = tester;
    }
    base::math::CVector2D pos;
    {
        base::math::CVector3D v = point();
        fn_710007b430(&pos, &v);
    }
    return fn_7100d572f8(tester, *reinterpret_cast<void**>(comp + 0x30), &pos, &g_unk_710164779c, 0).flags & 1;
}
