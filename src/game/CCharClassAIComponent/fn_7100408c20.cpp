// Member fields of CCharClassAIComponent (its class getter's x4 function, fn_7100408c20): 142
// fields with their defaults and editor descriptions, and the functions behind getter-backed
// fields. Written from tools/refl_attrs.py's draft (the parser stops at the struct-typed defaults
// of the four SDeathImpactDisplacement fields; those were added by hand), in its typed lambda
// form: typed values call their type getter after the value is stored (AddTyped), and each
// function registration sits in its own immediately called lambda.
#include "base/reflection/CCollectionType.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CEnemyPreset.h"
#include "game/SDeathImpactDisplacement.h"
#include "game/UnkTU1112.h"
#include "game/UnkTU2702.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflField;
using base::reflection::CFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
#include "base/reflection/TypeObject.h"
enum class ECutPathMode : u32;
template <> base::reflection::CType base::reflection::TypeObject<ECutPathMode>::object asm("g_unk_71726b1db0");
enum class EXpawnAngPolicy : u32;
template <> base::reflection::CType base::reflection::TypeObject<EXpawnAngPolicy>::object asm("g_unk_71726b1f10");
enum class ENavigablePathType : u32;
template <> base::reflection::CType base::reflection::TypeObject<ENavigablePathType>::object asm("g_unk_717270c618");
enum class EInFrustumMode : u32;
template <> base::reflection::CType base::reflection::TypeObject<EInFrustumMode>::object asm("g_unk_71727952a8");
enum class ELOSMode : u32;
template <> base::reflection::CType base::reflection::TypeObject<ELOSMode>::object asm("g_unk_7172a05520");
void fn_7100415b48();
void fn_7100411160(CFunction* fn, const char* name);
void fn_7100415bd8();
void fn_71004112dc(CFunction* fn, const char* name);
void fn_7100415c68();
void fn_7100411458(CFunction* fn, const char* name);
void fn_7100415cf8();
void fn_71004115d4(CFunction* fn, const char* name);
void fn_7100415d88();
void fn_7100411750(CFunction* fn, const char* name);
base::reflection::UnkReflValue fn_7100415e18(void* unused, void* argument);
void fn_71004118cc(CFunction* fn, const char* name);
void fn_7100415ea4();
void fn_7100411a40(CFunction* fn, const char* name);
void fn_7100415f34();
void fn_7100411bbc(CFunction* fn, const char* name);
void fn_7100415fc4();
void fn_7100411d38(CFunction* fn, const char* name);
void fn_7100416054();
void fn_7100411eb4(CFunction* fn, const char* name);
void fn_71004160e4();
void fn_7100412030(CFunction* fn, const char* name);
void fn_7100416174();
void fn_71004121ac(CFunction* fn, const char* name);
void fn_710041621c();
void fn_7100412328(CFunction* fn, const char* name);
void fn_71004162c4();
void fn_71004124a4(CFunction* fn, const char* name);
void fn_710041636c();
void fn_7100412620(CFunction* fn, const char* name);
void fn_7100416414();
void fn_710041279c(CFunction* fn, const char* name);
void fn_71004164bc();
void fn_7100412918(CFunction* fn, const char* name);
void fn_7100416554();
void fn_7100412a94(CFunction* fn, const char* name);
void fn_71004165e4();
void fn_7100412c10(CFunction* fn, const char* name);
void fn_7100416674();
void fn_7100412d8c(CFunction* fn, const char* name);
void fn_71004167c0();
void fn_7100412f08(CFunction* fn, const char* name);
// The value type of four fields' defaults (0x10 bytes; its class getter is fn_710035cb6c).
struct SDeathImpactDisplacement {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
};

// An empty string id built the way the original builds this default: the pointer is set once the
// pool exists, with no check of it in between (CStrId's inline constructor checks it). It is
// released as a CStrId.
struct UnkEmptyStrIdValue {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkEmptyStrIdValue() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrIdValue() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
    const CStrId& Get() const { return *reinterpret_cast<const CStrId*>(this); }
};

void fn_7100408c20(CClass* cls) {
    UnkReflField* field0 = AddReflectedField(cls, CStrId("sDefeatedEnemyClass", true), fn_7100009658(), 0xe0, -1, -1);
    field0->attrs.Add<const char*>(fn_7100075240(), "");
    field0->attrs.Add<const char*>(fn_7100007e80(), "Defeated Enemy class");
    UnkReflField* field1 = AddReflectedField(cls, CStrId("fChaseReachedDistance", true), fn_710000abbc(), 0x124, -1, -1);
    field1->attrs.Add<f32>(fn_7100075240(), 50.0f);
    field1->attrs.Add<const char*>(fn_7100007e80(), "Chase reached distance");
    UnkReflField* field2 = AddReflectedField(cls, CStrId("fChaseMinDistance", true), fn_710000abbc(), 0x118, -1, -1);
    field2->attrs.Add<f32>(fn_7100075240(), -1.0f);
    field2->attrs.Add<const char*>(fn_7100007e80(), "Chase min distance");
    UnkReflField* field3 = AddReflectedField(cls, CStrId("fChaseMaxDistance", true), fn_710000abbc(), 0x11c, -1, -1);
    field3->attrs.Add<f32>(fn_7100075240(), -1.0f);
    field3->attrs.Add<const char*>(fn_7100007e80(), "Chase max distance");
    UnkReflField* field4 = AddReflectedField(cls, CStrId("fChaseCenterDistance", true), fn_710000abbc(), 0x120, -1, -1);
    field4->attrs.Add<f32>(fn_7100075240(), -1.0f);
    field4->attrs.Add<const char*>(fn_7100007e80(), "Chase center distance");
    UnkReflField* field5 = AddReflectedField(cls, CStrId("fChaseMaxLaunchAngleDegs", true), fn_710000abbc(), 0x128, -1, -1);
    field5->attrs.Add<f32>(fn_7100075240(), 70.0f);
    field5->attrs.Add<const char*>(fn_7100007e80(), "Chase max launch angle");
    UnkReflField* field6 = AddReflectedField(cls, CStrId("fChaseProjectileRadius", true), fn_710000abbc(), 0x12c, -1, -1);
    field6->attrs.Add<f32>(fn_7100075240(), 15.0f);
    field6->attrs.Add<const char*>(fn_7100007e80(), "Chase launch projectile radius");
    UnkReflField* field7 = AddReflectedField(cls, CStrId("fChaseDistanceToFreePoint", true), fn_710000abbc(), 0x130, -1, -1);
    field7->attrs.Add<f32>(fn_7100075240(), 50.0f);
    field7->attrs.Add<const char*>(fn_7100007e80(), "Chase separation from free point");
    UnkReflField* field8 = AddReflectedField(cls, CStrId("fMinSeparationFromPreviousRandomChaseCenter", true), fn_710000abbc(), 0x134, -1, -1);
    field8->attrs.Add<f32>(fn_7100075240(), 0.0f);
    field8->attrs.Add<const char*>(fn_7100007e80(), "(When using random chase center) Distance desired from previous chase center");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bUseElectricAreaAttackFX_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_bUseElectricAreaAttackFX_GetterFunction", true);
            fn->return_type = fn_710000d228();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415b48);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bUseElectricAreaAttackFX_SetterFunction", true)))
            fn_7100411160(fn, "_m_bUseElectricAreaAttackFX_SetterFunction");
    }();
    UnkReflField* field9 = AddReflectedField(cls, CStrId("bUseElectricAreaAttackFX", true), fn_710000d228(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field9->attrs.Add<bool>(fn_7100075240(), false);
    field9->attrs.Add<const char*>(fn_7100007e80(), "Use Electric Area Attack FX");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fElectricAreaAttackMaxDist_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_fElectricAreaAttackMaxDist_GetterFunction", true);
            fn->return_type = fn_710000abbc();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415bd8);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fElectricAreaAttackMaxDist_SetterFunction", true)))
            fn_71004112dc(fn, "_m_fElectricAreaAttackMaxDist_SetterFunction");
    }();
    UnkReflField* field10 = AddReflectedField(cls, CStrId("fElectricAreaAttackMaxDist", true), fn_710000abbc(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field10->attrs.Add<f32>(fn_7100075240(), -1.0f);
    field10->attrs.Add<const char*>(fn_7100007e80(), "Electric Area Attack Max dist");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fShortDistance_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_fShortDistance_GetterFunction", true);
            fn->return_type = fn_710000abbc();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415c68);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fShortDistance_SetterFunction", true)))
            fn_7100411458(fn, "_m_fShortDistance_SetterFunction");
    }();
    UnkReflField* field11 = AddReflectedField(cls, CStrId("fShortDistance", true), fn_710000abbc(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field11->attrs.Add<f32>(fn_7100075240(), -1.0f);
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fLongDistance_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_fLongDistance_GetterFunction", true);
            fn->return_type = fn_710000abbc();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415cf8);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fLongDistance_SetterFunction", true)))
            fn_71004115d4(fn, "_m_fLongDistance_SetterFunction");
    }();
    UnkReflField* field12 = AddReflectedField(cls, CStrId("fLongDistance", true), fn_710000abbc(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field12->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field13 = AddReflectedField(cls, CStrId("bUseTargetPos", true), fn_710000d228(), 0x140, -1, -1);
    field13->attrs.Add<bool>(fn_7100075240(), false);
    field13->attrs.Add<const char*>(fn_7100007e80(), "Use the position of the target when deciding the next attack");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bSuspendIfNotInFrustum_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_bSuspendIfNotInFrustum_GetterFunction", true);
            fn->return_type = fn_710000d228();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415d88);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bSuspendIfNotInFrustum_SetterFunction", true)))
            fn_7100411750(fn, "_m_bSuspendIfNotInFrustum_SetterFunction");
    }();
    UnkReflField* field14 = AddReflectedField(cls, CStrId("bSuspendIfNotInFrustum", true), fn_710000d228(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field14->attrs.Add<bool>(fn_7100075240(), true);
    field14->attrs.Add<const char*>(fn_7100007e80(), "Suspend if not in frustum");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_eInFrustumMode_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_eInFrustumMode_GetterFunction", true);
            fn->return_type = &base::reflection::TypeObject<EInFrustumMode>::object;
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415e18);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_eInFrustumMode_SetterFunction", true)))
            fn_71004118cc(fn, "_m_eInFrustumMode_SetterFunction");
    }();
    UnkReflField* field15 = AddReflectedField(cls, CStrId("eInFrustumMode", true), &base::reflection::TypeObject<EInFrustumMode>::object, 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field15->attrs.Add<s32>(fn_7100075240(), &base::reflection::TypeObject<EInFrustumMode>::object, 0);
    field15->attrs.Add<const char*>(fn_7100007e80(), "How to check if entity is in frustum");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bWantsAdjustHeightOnMelee_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_bWantsAdjustHeightOnMelee_GetterFunction", true);
            fn->return_type = fn_710000d228();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415ea4);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bWantsAdjustHeightOnMelee_SetterFunction", true)))
            fn_7100411a40(fn, "_m_bWantsAdjustHeightOnMelee_SetterFunction");
    }();
    UnkReflField* field16 = AddReflectedField(cls, CStrId("bWantsAdjustHeightOnMelee", true), fn_710000d228(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field16->attrs.Add<bool>(fn_7100075240(), false);
    field16->attrs.Add<const char*>(fn_7100007e80(), "Wants Adjust Height On Melee");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bWantsMeleeCamera_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_bWantsMeleeCamera_GetterFunction", true);
            fn->return_type = fn_710000d228();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415f34);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bWantsMeleeCamera_SetterFunction", true)))
            fn_7100411bbc(fn, "_m_bWantsMeleeCamera_SetterFunction");
    }();
    UnkReflField* field17 = AddReflectedField(cls, CStrId("bWantsMeleeCamera", true), fn_710000d228(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field17->attrs.Add<bool>(fn_7100075240(), true);
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fMeleeCameraMinExtraZDist_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_fMeleeCameraMinExtraZDist_GetterFunction", true);
            fn->return_type = fn_710000abbc();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100415fc4);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fMeleeCameraMinExtraZDist_SetterFunction", true)))
            fn_7100411d38(fn, "_m_fMeleeCameraMinExtraZDist_SetterFunction");
    }();
    UnkReflField* field18 = AddReflectedField(cls, CStrId("fMeleeCameraMinExtraZDist", true), fn_710000abbc(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field18->attrs.Add<f32>(fn_7100075240(), 1400.0f);
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fMeleeCameraTime_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_fMeleeCameraTime_GetterFunction", true);
            fn->return_type = fn_710000abbc();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100416054);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fMeleeCameraTime_SetterFunction", true)))
            fn_7100411eb4(fn, "_m_fMeleeCameraTime_SetterFunction");
    }();
    UnkReflField* field19 = AddReflectedField(cls, CStrId("fMeleeCameraTime", true), fn_710000abbc(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field19->attrs.Add<f32>(fn_7100075240(), -1.0f);
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fMeleeCounterCameraMinExtraZDist_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_fMeleeCounterCameraMinExtraZDist_GetterFunction", true);
            fn->return_type = fn_710000abbc();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_71004160e4);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_fMeleeCounterCameraMinExtraZDist_SetterFunction", true)))
            fn_7100412030(fn, "_m_fMeleeCounterCameraMinExtraZDist_SetterFunction");
    }();
    UnkReflField* field20 = AddReflectedField(cls, CStrId("fMeleeCounterCameraMinExtraZDist", true), fn_710000abbc(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field20->attrs.Add<f32>(fn_7100075240(), 1200.0f);
    UnkReflField* field21 = AddReflectedField(cls, CStrId("sAutoLockOnColliderID", true), fn_7100009658(), 0x150, -1, -1);
    field21->attrs.Add<const char*>(fn_7100075240(), "");
    UnkReflField* field22 = AddReflectedField(cls, CStrId("bAutoLockOnColliderMissilesPredictEndPos", true), fn_710000d228(), 0x158, -1, -1);
    field22->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field23 = AddReflectedField(cls, CStrId("bWantsCoordinateAttacks", true), fn_710000d228(), 0x171, -1, -1);
    field23->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field24 = AddReflectedField(cls, CStrId("sCrazyEndAnim", true), fn_7100009658(), 0x168, -1, -1);
    field24->attrs.Add<const char*>(fn_7100075240(), "crazyend");
    UnkReflField* field25 = AddReflectedField(cls, CStrId("sCrazyFXNode", true), fn_7100009658(), 0x178, -1, -1);
    field25->attrs.Add<const char*>(fn_7100075240(), "");
    UnkReflField* field26 = AddReflectedField(cls, CStrId("vCrazyFXPositionOffset", true), fn_71000246e4(), 0x180, -1, -1);
    field26->attrs.Add<base::math::CVector3D>(fn_7100075240(), base::math::CVector3D{0.0f, 0.0f, 0.0f});
    UnkReflField* field27 = AddReflectedField(cls, CStrId("vCrazyFXRotationOffset", true), fn_71000246e4(), 0x18c, -1, -1);
    field27->attrs.Add<base::math::CVector3D>(fn_7100075240(), base::math::CVector3D{0.0f, 0.0f, 0.0f});
    UnkReflField* field28 = AddReflectedField(cls, CStrId("fTargetReachableWidth", true), fn_710000abbc(), 0x1a4, -1, -1);
    field28->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field29 = AddReflectedField(cls, CStrId("fTargetReachableHeight", true), fn_710000abbc(), 0x1a8, -1, -1);
    field29->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field30 = AddReflectedField(cls, CStrId("fTargetReachableRadius", true), fn_710000abbc(), 0x1b0, -1, -1);
    field30->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field31 = AddReflectedField(cls, CStrId("bTargetReachableRadiusIgnoreSegmentNormal", true), fn_710000d228(), 0x1b5, -1, -1);
    field31->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field32 = AddReflectedField(cls, CStrId("bReachableUseRadius", true), fn_710000d228(), 0x1ac, -1, -1);
    field32->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field33 = AddReflectedField(cls, CStrId("bCheckWeaponReachable", true), fn_710000d228(), 0x1b4, -1, -1);
    field33->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field34 = AddReflectedField(cls, CStrId("fReachableMaxSegmentDotToCut", true), fn_710000abbc(), 0x1b8, -1, -1);
    field34->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field35 = AddReflectedField(cls, CStrId("fReachableSegmentCutDistance", true), fn_710000abbc(), 0x1bc, -1, -1);
    field35->attrs.Add<f32>(fn_7100075240(), 0.0f);
    UnkReflField* field36 = AddReflectedField(cls, CStrId("sNavMeshSmartLinksGroup", true), fn_7100009658(), 0x1c8, -1, -1);
    field36->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    UnkReflField* field37 = AddReflectedField(cls, CStrId("sDynamicSmartLinkRulesFile", true), fn_7100009658(), 0x1d8, -1, -1);
    field37->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    UnkReflField* field38 = AddReflectedField(cls, CStrId("sDynamicNavigationRulesFile", true), fn_7100009658(), 0x1e8, -1, -1);
    field38->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sNavMeshNavigationModes_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_sNavMeshNavigationModes_GetterFunction", true);
            fn->return_type = fn_7100009658();
            fn->field_0xb2 = 0x101;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100416174);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sNavMeshNavigationModes_SetterFunction", true)))
            fn_71004121ac(fn, "_sNavMeshNavigationModes_SetterFunction");
    }();
    UnkReflField* field39 = AddReflectedField(cls, CStrId("sNavMeshNavigationModes", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field39->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sNavMeshEnvironmentTypes_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_sNavMeshEnvironmentTypes_GetterFunction", true);
            fn->return_type = fn_7100009658();
            fn->field_0xb2 = 0x101;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_710041621c);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sNavMeshEnvironmentTypes_SetterFunction", true)))
            fn_7100412328(fn, "_sNavMeshEnvironmentTypes_SetterFunction");
    }();
    UnkReflField* field40 = AddReflectedField(cls, CStrId("sNavMeshEnvironmentTypes", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field40->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sForbiddenNavigateOverNavMeshPolygonLogicTypeFlags_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_sForbiddenNavigateOverNavMeshPolygonLogicTypeFlags_GetterFunction", true);
            fn->return_type = fn_7100009658();
            fn->field_0xb2 = 0x101;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_71004162c4);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sForbiddenNavigateOverNavMeshPolygonLogicTypeFlags_SetterFunction", true)))
            fn_71004124a4(fn, "_sForbiddenNavigateOverNavMeshPolygonLogicTypeFlags_SetterFunction");
    }();
    UnkReflField* field41 = AddReflectedField(cls, CStrId("sForbiddenNavigateOverNavMeshPolygonLogicTypeFlags", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field41->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sForbiddenNavigateInsideNavMeshPolygonLogicTypeFlags_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_sForbiddenNavigateInsideNavMeshPolygonLogicTypeFlags_GetterFunction", true);
            fn->return_type = fn_7100009658();
            fn->field_0xb2 = 0x101;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_710041636c);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sForbiddenNavigateInsideNavMeshPolygonLogicTypeFlags_SetterFunction", true)))
            fn_7100412620(fn, "_sForbiddenNavigateInsideNavMeshPolygonLogicTypeFlags_SetterFunction");
    }();
    UnkReflField* field42 = AddReflectedField(cls, CStrId("sForbiddenNavigateInsideNavMeshPolygonLogicTypeFlags", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field42->attrs.Add<CStrId>(fn_7100075240(), CStrId("DynamicItems", true));
    UnkReflField* field43 = AddReflectedField(cls, CStrId("bAwaitOnTargetTunnelEntrances", true), fn_710000d228(), 0x22c, -1, -1);
    field43->attrs.Add<bool>(fn_7100075240(), false);
    field43->attrs.Add<const char*>(fn_7100007e80(), "Calculate paths to target just until tunnel entrances");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sNavMeshGlobalSmartLinkTypes_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_sNavMeshGlobalSmartLinkTypes_GetterFunction", true);
            fn->return_type = fn_7100009658();
            fn->field_0xb2 = 0x101;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100416414);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_sNavMeshGlobalSmartLinkTypes_SetterFunction", true)))
            fn_710041279c(fn, "_sNavMeshGlobalSmartLinkTypes_SetterFunction");
    }();
    UnkReflField* field44 = AddReflectedField(cls, CStrId("sNavMeshGlobalSmartLinkTypes", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field44->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_fVisitedNavMeshEdgesDistance_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_fVisitedNavMeshEdgesDistance_GetterFunction", true);
            fn->return_type = fn_710000abbc();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_71004164bc);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_fVisitedNavMeshEdgesDistance_SetterFunction", true)))
            fn_7100412918(fn, "_fVisitedNavMeshEdgesDistance_SetterFunction");
    }();
    UnkReflField* field45 = AddReflectedField(cls, CStrId("fVisitedNavMeshEdgesDistance", true), fn_710000abbc(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field45->attrs.Add<f32>(fn_7100075240(), 0.0f);
    UnkReflField* field46 = AddReflectedField(cls, CStrId("vNavMeshAStarExtraSize", true), fn_7100024504(), 0x3a4, -1, -1);
    field46->attrs.Add<base::math::CVector2D>(fn_7100075240(), base::math::CVector2D{0.0f, 0.0f});
    UnkReflField* field47 = AddReflectedField(cls, CStrId("eNavigablePathType", true), &base::reflection::TypeObject<ENavigablePathType>::object, 0xec, -1, -1);
    field47->attrs.Add<s32>(fn_7100075240(), &base::reflection::TypeObject<ENavigablePathType>::object, 0);
    UnkReflField* field48 = AddReflectedField(cls, CStrId("fMinYNormalInPath", true), fn_710000abbc(), 0xf0, -1, -1);
    field48->attrs.Add<f32>(fn_7100075240(), 0.7f);
    UnkReflField* field49 = AddReflectedField(cls, CStrId("fNavigablePathCutDistance", true), fn_710000abbc(), 0xf4, -1, -1);
    field49->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field50 = AddReflectedField(cls, CStrId("fNavigablePathCutDistanceInPathLimits", true), fn_710000abbc(), 0x108, -1, -1);
    field50->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field51 = AddReflectedField(cls, CStrId("fNavigablePathCutDistanceWithWalls", true), fn_710000abbc(), 0x10c, -1, -1);
    field51->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field52 = AddReflectedField(cls, CStrId("vNavigablePathCharacterSize", true), fn_71000246e4(), 0xf8, -1, -1);
    field52->attrs.Add<base::math::CVector3D>(fn_7100075240(), base::math::CVector3D{-1.0f, -1.0f, -1.0f});
    UnkReflField* field53 = AddReflectedField(cls, CStrId("bNavigablePathCanStitchEdges", true), fn_710000d228(), 0x104, -1, -1);
    field53->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field54 = AddReflectedField(cls, CStrId("bCanApplyMeleeReactionKinematicMovementModifier", true), fn_710000d228(), 0x240, -1, -1);
    field54->attrs.Add<bool>(fn_7100075240(), true);
    field54->attrs.Add<const char*>(fn_7100007e80(), "Can be pushed reacting to melee attacks");
    UnkReflField* field55 = AddReflectedField(cls, CStrId("bWantsRotateOnMeleeCrazy", true), fn_710000d228(), 0x198, -1, -1);
    field55->attrs.Add<bool>(fn_7100075240(), true);
    field55->attrs.Add<const char*>(fn_7100007e80(), "Wants to rotate on melee crazy");
    UnkReflField* field56 = AddReflectedField(cls, CStrId("bGeneratePathInOpenDoors", true), fn_710000d228(), 0x114, -1, -1);
    field56->attrs.Add<bool>(fn_7100075240(), true);
    field56->attrs.Add<const char*>(fn_7100007e80(), "Generate path inside open doors");
    UnkReflField* field57 = AddReflectedField(cls, CStrId("eCutPathMode", true), &base::reflection::TypeObject<ECutPathMode>::object, 0x110, -1, -1);
    field57->attrs.Add<s32>(fn_7100075240(), &base::reflection::TypeObject<ECutPathMode>::object, 0);
    field57->attrs.Add<const char*>(fn_7100007e80(), "Mode to cut final navigable path");
    UnkReflField* field58 = AddReflectedField(cls, CStrId("tPresets", true), &g_unk_71726e36c8, 0x58, -1, -1);
    field58->attrs_0x70.Add<bool>(fn_7100091d40(), true);
    UnkReflField* field59 = AddReflectedField(cls, CStrId("bAlwaysTargetDetected", true), fn_710000d228(), 0x40b, -1, -1);
    field59->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field60 = AddReflectedField(cls, CStrId("bRequiresActivationPerception", true), fn_710000d228(), 0x36a, -1, -1);
    field60->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field61 = AddReflectedField(cls, CStrId("oActivationPerceptionRuleSet", true), fn_71002d0420(), 0x248, -1, -1);
    UnkReflField* field62 = AddReflectedField(cls, CStrId("oStartDetectionPerceptionRuleSet", true), fn_71002d093c(), 0x2a8, -1, -1);
    UnkReflField* field63 = AddReflectedField(cls, CStrId("oExtendDetectionPerceptionRuleSet", true), fn_71002d0e58(), 0x308, -1, -1);
    UnkReflField* field64 = AddReflectedField(cls, CStrId("bStartDetectionRequiresLOS", true), fn_710000d228(), 0x36b, -1, -1);
    field64->attrs.Add<bool>(fn_7100075240(), false);
    field64->attrs.Add<const char*>(fn_7100007e80(), "Is LOS required to detect the target at start");
    UnkReflField* field65 = AddReflectedField(cls, CStrId("bEndDetectionRequiresLOS", true), fn_710000d228(), 0x36c, -1, -1);
    field65->attrs.Add<bool>(fn_7100075240(), false);
    field65->attrs.Add<const char*>(fn_7100007e80(), "Is LOS required to detect the target at end");
    UnkReflField* field66 = AddReflectedField(cls, CStrId("eDetectionLOSMode", true), &base::reflection::TypeObject<ELOSMode>::object, 0x370, -1, -1);
    field66->attrs.Add<s32>(fn_7100075240(), &base::reflection::TypeObject<ELOSMode>::object, 1);
    field66->attrs.Add<const char*>(fn_7100007e80(), "LOS mode for detection");
    UnkReflField* field67 = AddReflectedField(cls, CStrId("bStartDetectionRequiresTargetInFront", true), fn_710000d228(), 0x374, -1, -1);
    field67->attrs.Add<bool>(fn_7100075240(), false);
    field67->attrs.Add<const char*>(fn_7100007e80(), "Should the target be in front of us to start detecting her");
    UnkReflField* field68 = AddReflectedField(cls, CStrId("bEndDetectionRequiresTargetInFront", true), fn_710000d228(), 0x375, -1, -1);
    field68->attrs.Add<bool>(fn_7100075240(), false);
    field68->attrs.Add<const char*>(fn_7100007e80(), "Should the target be in front of us to stop detecting her");
    UnkReflField* field69 = AddReflectedField(cls, CStrId("fStartMaxDistForTargetDetection", true), fn_710000abbc(), 0x380, -1, -1);
    field69->attrs.Add<f32>(fn_7100075240(), 10000000.0f);
    field69->attrs.Add<const char*>(fn_7100007e80(), "Maximum dist to see the target at start");
    UnkReflField* field70 = AddReflectedField(cls, CStrId("fEndMaxDistForTargetDetection", true), fn_710000abbc(), 0x384, -1, -1);
    field70->attrs.Add<f32>(fn_7100075240(), 10000000.0f);
    field70->attrs.Add<const char*>(fn_7100007e80(), "Maximum dist to see the target at end");
    UnkReflField* field71 = AddReflectedField(cls, CStrId("fTimeForTargetDetection", true), fn_710000abbc(), 0x388, -1, -1);
    field71->attrs.Add<f32>(fn_7100075240(), 0.2f);
    field71->attrs.Add<const char*>(fn_7100007e80(), "Continous time (s) satisfying ALL detection conditions to start detecting the target");
    UnkReflField* field72 = AddReflectedField(cls, CStrId("fTimeForTargetDetectionEnd", true), fn_710000abbc(), 0x38c, -1, -1);
    field72->attrs.Add<f32>(fn_7100075240(), 0.0f);
    field72->attrs.Add<const char*>(fn_7100007e80(), "Continous time (s) not satisfying ANY detection condition to stop detecting the target");
    UnkReflField* field73 = AddReflectedField(cls, CStrId("bStartShouldBeInFrustrumForTargetDetection", true), fn_710000d228(), 0x390, -1, -1);
    field73->attrs.Add<bool>(fn_7100075240(), true);
    field73->attrs.Add<const char*>(fn_7100007e80(), "Should we be on frustrum to detect the Target at start");
    UnkReflField* field74 = AddReflectedField(cls, CStrId("bEndShouldBeInFrustrumForTargetDetection", true), fn_710000d228(), 0x391, -1, -1);
    field74->attrs.Add<bool>(fn_7100075240(), true);
    field74->attrs.Add<const char*>(fn_7100007e80(), "Should we be on frustrum to detect the Target at end");
    UnkReflField* field75 = AddReflectedField(cls, CStrId("bStartShouldCheckIfTargetIsInAreaOfInterest", true), fn_710000d228(), 0x392, -1, -1);
    field75->attrs.Add<bool>(fn_7100075240(), false);
    field75->attrs.Add<const char*>(fn_7100007e80(), "Should the target be within our interest area at start");
    UnkReflField* field76 = AddReflectedField(cls, CStrId("bEndShouldCheckIfTargetIsInAreaOfInterest", true), fn_710000d228(), 0x393, -1, -1);
    field76->attrs.Add<bool>(fn_7100075240(), false);
    field76->attrs.Add<const char*>(fn_7100007e80(), "Should the target be within our interest area at end");
    UnkReflField* field77 = AddReflectedField(cls, CStrId("bUseStartAOIWhenEndAOIIsEmpty", true), fn_710000d228(), 0x394, -1, -1);
    field77->attrs.Add<bool>(fn_7100075240(), true);
    field77->attrs.Add<const char*>(fn_7100007e80(), "Use start AOI if end AOI is empty (e.g. false when AI ONLY needs AOI to awake)");
    UnkReflField* field78 = AddReflectedField(cls, CStrId("bUseAlwaysEndAreaOfInterestAfterFirstDetection", true), fn_710000d228(), 0x395, -1, -1);
    field78->attrs.Add<bool>(fn_7100075240(), false);
    field78->attrs.Add<const char*>(fn_7100007e80(), "Use always end AOI as start AOI if target was already detected (e.g. true if start AOI is only relevant to awake AI)");
    UnkReflField* field79 = AddReflectedField(cls, CStrId("bIsAreaOfInterestOptional", true), fn_710000d228(), 0x396, -1, -1);
    field79->attrs.Add<bool>(fn_7100075240(), false);
    field79->attrs.Add<const char*>(fn_7100007e80(), "True if some of these AIs might have AOI to detect target while others not (e.g. true when an AI can be either spawned 'awaken' or 'in patrol'");
    UnkReflField* field80 = AddReflectedField(cls, CStrId("bStartShouldTargetBeReachableForDetection", true), fn_710000d228(), 0x397, -1, -1);
    field80->attrs.Add<bool>(fn_7100075240(), false);
    field80->attrs.Add<const char*>(fn_7100007e80(), "Should the target be reachable to detect it at start");
    UnkReflField* field81 = AddReflectedField(cls, CStrId("bEndShouldTargetBeReachableForDetection", true), fn_710000d228(), 0x398, -1, -1);
    field81->attrs.Add<bool>(fn_7100075240(), false);
    field81->attrs.Add<const char*>(fn_7100007e80(), "Should the target be reachable to detect it at end");
    UnkReflField* field82 = AddReflectedField(cls, CStrId("bStartCanSeeThroughOpticCamo", true), fn_710000d228(), 0x399, -1, -1);
    field82->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field83 = AddReflectedField(cls, CStrId("bEndCanSeeThroughOpticCamo", true), fn_710000d228(), 0x39a, -1, -1);
    field83->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field84 = AddReflectedField(cls, CStrId("bForceTargetDetectionByOtherSpawnGroupMember", true), fn_710000d228(), 0x368, -1, -1);
    field84->attrs.Add<bool>(fn_7100075240(), false);
    field84->attrs.Add<const char*>(fn_7100007e80(), "Force detection if detected by other unit in spawngroup");
    UnkReflField* field85 = AddReflectedField(cls, CStrId("bResumeOtherSpawnGroupMembersOnTargetDetection", true), fn_710000d228(), 0x369, -1, -1);
    field85->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field86 = AddReflectedField(cls, CStrId("bAllowsTrespass", true), fn_710000d228(), 0x3b0, -1, -1);
    field86->attrs.Add<bool>(fn_7100075240(), true);
    field86->attrs.Add<const char*>(fn_7100007e80(), "Allows Samus to trespass through it");
    UnkReflField* field87 = AddReflectedField(cls, CStrId("bForceOnCollisionStayProcess", true), fn_710000d228(), 0x408, -1, -1);
    field87->attrs.Add<bool>(fn_7100075240(), false);
    field87->attrs.Add<const char*>(fn_7100007e80(), "Force OnCollisionStay process even during invulnerability");
    UnkReflField* field88 = AddReflectedField(cls, CStrId("tTrespassCollidersIds", true), &g_unk_7101d5c998, 0x3b8, -1, -1);
    UnkReflField* field89 = AddReflectedField(cls, CStrId("tTrespassCollidersTags", true), &g_unk_7101d5c998, 0x3e0, -1, -1);
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bMeleeBigAIReaction_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_bMeleeBigAIReaction_GetterFunction", true);
            fn->return_type = fn_710000d228();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100416554);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bMeleeBigAIReaction_SetterFunction", true)))
            fn_7100412a94(fn, "_m_bMeleeBigAIReaction_SetterFunction");
    }();
    UnkReflField* field90 = AddReflectedField(cls, CStrId("bMeleeBigAIReaction", true), fn_710000d228(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field90->attrs.Add<bool>(fn_7100075240(), false);
    field90->attrs.Add<const char*>(fn_7100007e80(), "Determines if Samus reacts as a big enemy or not");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bAllowMultipleMeleeHits_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_bAllowMultipleMeleeHits_GetterFunction", true);
            fn->return_type = fn_710000d228();
            fn->field_0xb2 = 0;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_71004165e4);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_bAllowMultipleMeleeHits_SetterFunction", true)))
            fn_7100412c10(fn, "_m_bAllowMultipleMeleeHits_SetterFunction");
    }();
    UnkReflField* field91 = AddReflectedField(cls, CStrId("bAllowMultipleMeleeHits", true), fn_710000d228(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field91->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field92 = AddReflectedField(cls, CStrId("bUsingDCCenter", true), fn_710000d228(), 0x40a, -1, -1);
    field92->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field93 = AddReflectedField(cls, CStrId("fTargetOnGroundDist", true), fn_710000abbc(), 0x40c, -1, -1);
    field93->attrs.Add<f32>(fn_7100075240(), 420.0f);
    UnkReflField* field94 = AddReflectedField(cls, CStrId("fMinFlyngDistToFloor", true), fn_710000abbc(), 0x410, -1, -1);
    field94->attrs.Add<f32>(fn_7100075240(), 500.0f);
    UnkReflField* field95 = AddReflectedField(cls, CStrId("fMotionSpeed", true), fn_710000abbc(), 0x414, -1, -1);
    field95->attrs.Add<f32>(fn_7100075240(), 0.0f);
    field95->attrs.Add<const char*>(fn_7100007e80(), "Motion speed applied to the speed factor in the kinematic step");
    UnkReflField* field96 = AddReflectedField(cls, CStrId("bIsReskin", true), fn_710000d228(), 0x418, -1, -1);
    field96->attrs.Add<bool>(fn_7100075240(), false);
    field96->attrs.Add<const char*>(fn_7100007e80(), "Is this a reskin of an enemy");
    UnkReflField* field97 = AddReflectedField(cls, CStrId("bUpdatePathToTarget", true), fn_710000d228(), 0x419, -1, -1);
    field97->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field98 = AddReflectedField(cls, CStrId("bTweakWalkCyclesBeforeSmartLink", true), fn_710000d228(), 0x41a, -1, -1);
    field98->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field99 = AddReflectedField(cls, CStrId("fMinNumWalkCyclesToTweak", true), fn_710000abbc(), 0x41c, -1, -1);
    field99->attrs.Add<f32>(fn_7100075240(), 0.1f);
    UnkReflField* field100 = AddReflectedField(cls, CStrId("fMaxNumWalkCyclesToTweak", true), fn_710000abbc(), 0x420, -1, -1);
    field100->attrs.Add<f32>(fn_7100075240(), 9999.0f);
    UnkReflField* field101 = AddReflectedField(cls, CStrId("fMinWalkKinematicDeltaFactor", true), fn_710000abbc(), 0x424, -1, -1);
    field101->attrs.Add<f32>(fn_7100075240(), 0.75f);
    UnkReflField* field102 = AddReflectedField(cls, CStrId("fMaxWalkKinematicDeltaFactor", true), fn_710000abbc(), 0x428, -1, -1);
    field102->attrs.Add<f32>(fn_7100075240(), 1.2f);
    UnkReflField* field103 = AddReflectedField(cls, CStrId("bPlayerCanJump", true), fn_710000d228(), 0x42c, -1, -1);
    field103->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field104 = AddReflectedField(cls, CStrId("bUseNavmeshPath", true), fn_710000d228(), 0xc5, -1, -1);
    field104->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field105 = AddReflectedField(cls, CStrId("bMeleeCameraFinishIfNotForcedCharge", true), fn_710000d228(), 0x42d, -1, -1);
    field105->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field106 = AddReflectedField(cls, CStrId("fBlockableAttackWarningNoticeTime", true), fn_710000abbc(), 0x430, -1, -1);
    field106->attrs.Add<f32>(fn_7100075240(), 0.5f);
    UnkReflField* field107 = AddReflectedField(cls, CStrId("fBlockableAttackWarningRadius", true), fn_710000abbc(), 0x434, -1, -1);
    field107->attrs.Add<f32>(fn_7100075240(), 300.0f);
    UnkReflField* field108 = AddReflectedField(cls, CStrId("fBlockableAttackWarningVolume", true), fn_710000abbc(), 0x438, -1, -1);
    field108->attrs.Add<f32>(fn_7100075240(), 1.2f);
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_sDefaultInvulnerableDamageMask_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_sDefaultInvulnerableDamageMask_GetterFunction", true);
            fn->return_type = fn_7100009658();
            fn->field_0xb2 = 0x101;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_7100416674);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_sDefaultInvulnerableDamageMask_SetterFunction", true)))
            fn_7100412d8c(fn, "_m_sDefaultInvulnerableDamageMask_SetterFunction");
    }();
    UnkReflField* field109 = AddReflectedField(cls, CStrId("sDefaultInvulnerableDamageMask", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field109->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    field109->attrs.Add<const char*>(fn_7100007e80(), "Invulnerability to damage sources by default");
    [&] {
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_sArmoredInvulnerableDamageMask_GetterFunction", true))) {
            fn->name = base::global::UnkStrId("_m_sArmoredInvulnerableDamageMask_GetterFunction", true);
            fn->return_type = fn_7100009658();
            fn->field_0xb2 = 0x101;
            fn->field_0x40 = nullptr;
            fn->invoker = reinterpret_cast<void*>(&fn_71004167c0);
        }
        if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_sArmoredInvulnerableDamageMask_SetterFunction", true)))
            fn_7100412f08(fn, "_m_sArmoredInvulnerableDamageMask_SetterFunction");
    }();
    UnkReflField* field110 = AddReflectedField(cls, CStrId("sArmoredInvulnerableDamageMask", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field110->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    field110->attrs.Add<const char*>(fn_7100007e80(), "Invulnerability to damage sources when armored");
    UnkReflField* field111 = AddReflectedField(cls, CStrId("sMorphingNode", true), fn_7100009658(), 0x458, -1, -1);
    field111->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    UnkReflField* field112 = AddReflectedField(cls, CStrId("v3MorphingOffset", true), fn_71000246e4(), 0x460, -1, -1);
    field112->attrs.Add<base::math::CVector3D>(fn_7100075240(), base::math::CVector3D{0.0f, 0.0f, 0.0f});
    UnkReflField* field113 = AddReflectedField(cls, CStrId("eXpawnAngPolicy", true), &base::reflection::TypeObject<EXpawnAngPolicy>::object, 0x46c, -1, -1);
    field113->attrs.Add<s32>(fn_7100075240(), &base::reflection::TypeObject<EXpawnAngPolicy>::object, 0);
    UnkReflField* field114 = AddReflectedField(cls, CStrId("fXCellMorphingEffectScale", true), fn_710000abbc(), 0x470, -1, -1);
    field114->attrs.Add<f32>(fn_7100075240(), 1.0f);
    UnkReflField* field115 = AddReflectedField(cls, CStrId("sPushAnim", true), fn_7100009658(), 0x478, -1, -1);
    field115->attrs.Add<CStrId>(fn_7100075240(), CStrId("push", true));
    UnkReflField* field116 = AddReflectedField(cls, CStrId("sPushBackAnim", true), fn_7100009658(), 0x480, -1, -1);
    field116->attrs.Add<CStrId>(fn_7100075240(), CStrId(base::global::CStrId::Empty()));
    UnkReflField* field117 = AddReflectedField(cls, CStrId("bIgnoreRotateOnPush", true), fn_710000d228(), 0x488, -1, -1);
    field117->attrs.Add<bool>(fn_7100075240(), false);
    field117->attrs.Add<const char*>(fn_7100007e80(), "Whether to ignore rotating to instigator when entity is pushed");
    UnkReflField* field118 = AddReflectedField(cls, CStrId("bInvertRotateOnPush", true), fn_710000d228(), 0x489, -1, -1);
    field118->attrs.Add<bool>(fn_7100075240(), false);
    field118->attrs.Add<const char*>(fn_7100007e80(), "Whether to invert rotating (i.e. in opposite dir) to instigator when entity is pushed");
    UnkReflField* field119 = AddReflectedField(cls, CStrId("fIgnoreShootsTime", true), fn_710000abbc(), 0x494, -1, -1);
    field119->attrs.Add<f32>(fn_7100075240(), 0.0f);
    UnkReflField* field120 = AddReflectedField(cls, CStrId("fBackToNormalityTime", true), fn_710000abbc(), 0x48c, -1, -1);
    field120->attrs.Add<f32>(fn_7100075240(), 5.0f);
    UnkReflField* field121 = AddReflectedField(cls, CStrId("fBackToNormalityInMoreShootsTime", true), fn_710000abbc(), 0x490, -1, -1);
    field121->attrs.Add<f32>(fn_7100075240(), -1.0f);
    UnkReflField* field122 = AddReflectedField(cls, CStrId("fDistanceToPathLimitInFlee", true), fn_710000abbc(), 0x498, -1, -1);
    field122->attrs.Add<f32>(fn_7100075240(), 200.0f);
    UnkReflField* field123 = AddReflectedField(cls, CStrId("fDashMeleeLimitToAdd", true), fn_710000abbc(), 0x4b0, -1, -1);
    field123->attrs.Add<f32>(fn_7100075240(), 0.0f);
    UnkReflField* field124 = AddReflectedField(cls, CStrId("oDeathImpactDisplacementDefault", true), fn_710035cb6c(), 0x4b4, -1, -1);
    field124->attrs.AddTyped<SDeathImpactDisplacement, &fn_710035cb6c>(fn_7100075240(), SDeathImpactDisplacement{2500.0f, 200.0f, 0.12f, 60.0f});
    UnkReflField* field125 = AddReflectedField(cls, CStrId("oDeathImpactDisplacementStrong", true), fn_710035cb6c(), 0x4c4, -1, -1);
    field125->attrs.AddTyped<SDeathImpactDisplacement, &fn_710035cb6c>(fn_7100075240(), SDeathImpactDisplacement{3500.0f, 400.0f, 0.12f, 100.0f});
    UnkReflField* field126 = AddReflectedField(cls, CStrId("bWantsForceBarelyFrozenDeath", true), fn_710000d228(), 0x4d4, -1, -1);
    field126->attrs.Add<bool>(fn_7100075240(), false);
    field126->attrs.Add<const char*>(fn_7100007e80(), "Forces enemy dying as if it was BarelyFrozen");
    UnkReflField* field127 = AddReflectedField(cls, CStrId("bWantsForceScrewBounceOnDead", true), fn_710000d228(), 0x4d5, -1, -1);
    field127->attrs.Add<bool>(fn_7100075240(), false);
    field127->attrs.Add<const char*>(fn_7100007e80(), "Force bounce instead of trespass after a Screw Attack kill");
    UnkReflField* field128 = AddReflectedField(cls, CStrId("bIgnoreFleeTime", true), fn_710000d228(), 0x49c, -1, -1);
    field128->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field129 = AddReflectedField(cls, CStrId("bSetNewParamsOnFlee", true), fn_710000d228(), 0x49d, -1, -1);
    field129->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field130 = AddReflectedField(cls, CStrId("fFleeSpeed", true), fn_710000abbc(), 0x4a0, -1, -1);
    field130->attrs.Add<f32>(fn_7100075240(), 1.0f);
    UnkReflField* field131 = AddReflectedField(cls, CStrId("fFleeAcceleration", true), fn_710000abbc(), 0x4a4, -1, -1);
    field131->attrs.Add<f32>(fn_7100075240(), 1.0f);
    UnkReflField* field132 = AddReflectedField(cls, CStrId("fPatrolSpeed", true), fn_710000abbc(), 0x4a8, -1, -1);
    field132->attrs.Add<f32>(fn_7100075240(), 1.0f);
    UnkReflField* field133 = AddReflectedField(cls, CStrId("fPatrolAcceleration", true), fn_710000abbc(), 0x4ac, -1, -1);
    field133->attrs.Add<f32>(fn_7100075240(), 1.0f);
    UnkReflField* field134 = AddReflectedField(cls, CStrId("bWantsCauseBumpFXByDefault", true), fn_710000d228(), 0x4d6, -1, -1);
    field134->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field135 = AddReflectedField(cls, CStrId("bCheckSLWhenAttackEnds", true), fn_710000d228(), 0x4d7, -1, -1);
    field135->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field136 = AddReflectedField(cls, CStrId("fDistToDestroyProjectile", true), fn_710000abbc(), 0x4d8, -1, -1);
    field136->attrs.Add<f32>(fn_7100075240(), 5000.0f);
    UnkReflField* field137 = AddReflectedField(cls, CStrId("bAllowBlindAttackOnPush", true), fn_710000d228(), 0x4e4, -1, -1);
    field137->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field138 = AddReflectedField(cls, CStrId("sBlindAttackName", true), fn_7100009658(), 0x4e8, -1, -1);
    field138->attrs.Add<CStrId>(fn_7100075240(), UnkEmptyStrIdValue().Get());
    UnkReflField* field139 = AddReflectedField(cls, CStrId("bComputeSupportOnPath", true), fn_710000d228(), 0x4f1, -1, -1);
    field139->attrs.Add<bool>(fn_7100075240(), true);
    UnkReflField* field140 = AddReflectedField(cls, CStrId("fMinBackSpaceToOverrideMeleeAction", true), fn_710000abbc(), 0x4f4, -1, -1);
    field140->attrs.Add<f32>(fn_7100075240(), -1.0f);
    field140->attrs.Add<const char*>(fn_7100007e80(), "If enemy it's at this distance or less from the back path limit it may override Samus's melee action to separate from her. 0 to disable. -1 to auto-set value");
    UnkReflField* field141 = AddReflectedField(cls, CStrId("fMinDist2TargetToOverrideMeleeAction", true), fn_710000abbc(), 0x4f8, -1, -1);
    field141->attrs.Add<f32>(fn_7100075240(), -1.0f);
    field141->attrs.Add<const char*>(fn_7100007e80(), "If enemy it's at this distance or less from Samus it may override Samus's melee action to separate from her. 0 to disable. -1 to auto-set value");
}
