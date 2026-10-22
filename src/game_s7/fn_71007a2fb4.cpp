// Reflection fields and functions of CCutsceneComponent. Written from the refl_attrs draft; one
// return-type getter lives in this class's translation unit.
#include "base/reflection/UnkReflAttributes.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflField;
using base::reflection::UnkReflFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
void fn_71007ad4d0();
void fn_71007a515c(UnkReflFunction* fn, const char* name);
const base::reflection::UnkReflAttrKey& fn_7100090a20();
const base::reflection::UnkReflAttrKey& fn_7100075240();
void fn_71007ad5e8();
const base::reflection::UnkReflAttrKey& fn_710000bf80();
extern base::reflection::CType g_unk_7101d679f0;
const base::reflection::UnkReflAttrKey& fn_71000923a0();
extern base::reflection::CType g_unk_7101d55008;
extern base::reflection::CType g_unk_7101d5c078;
void fn_71007ad648();
void fn_71007a52d8(UnkReflFunction* fn, const char* name);
extern base::reflection::CType g_unk_7101d5c998;
extern base::reflection::CType g_unk_7172719170;
const base::reflection::UnkReflAttrKey& fn_710008fd60();
const base::reflection::UnkReflAttrKey& fn_7100091c30();
extern base::reflection::CType g_unk_717274caa0;
extern base::reflection::CType g_unk_7101d5af48;
void fn_71007ad78c();
void fn_71007a544c(UnkReflFunction* fn, const char* name);
void fn_71007ad814();
void fn_71007ad8d4();
void fn_71007ad8e8();
void fn_71007ad91c();
void fn_71007ad950();
void fn_71007ad984();
void fn_71007ad9b8();
void fn_71007ad9ec();
void fn_71007ada28();
void fn_71007adb08();
void fn_71007adb3c();
const base::reflection::CType* fn_71007a225c();
extern const base::reflection::CType g_unk_71727199c0 __attribute__((visibility("hidden")));
void fn_71007adb50();
void fn_71007adba0();
void fn_71007adbe8();
void fn_71007a55c8(UnkReflFunction* fn, const char* name);
void fn_71007ade14();
void fn_71007adc7c(UnkReflFunction* fn);
void fn_71007a5744(UnkReflFunction* fn, const char* name);
const base::reflection::CType* fn_71007adf30();
void fn_71007ae270();
void fn_71007ae2bc();
void fn_7100302b38(UnkReflFunction* fn);
const base::reflection::UnkReflAttrKey& fn_710008f920();
void fn_71007ae4e8();
void fn_71007ae40c(UnkReflFunction* fn);

void fn_71007a2fb4(CClass* cls) {
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("_m_sCutsceneName_GetterFunction", true))) {
        fn->name = base::global::UnkStrId("_m_sCutsceneName_GetterFunction", true);
        fn->return_type = fn_7100009658();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad4d0);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("_m_sCutsceneName_SetterFunction", true)))
        fn_71007a515c(fn, "_m_sCutsceneName_SetterFunction");
    UnkReflField* field0 = fn_7100096524(cls, CStrId("sCutsceneName", true), fn_7100009658(), 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field0->attrs.Add<bool>(fn_7100090a20(), true);
    UnkReflField* field1 = fn_7100096524(cls, CStrId("bDisableScenarioEntitiesOnPlay", true), fn_710000d228(), 0x20e, -1, -1);
    field1->attrs.Add<bool>(fn_7100075240(), true);
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("_m_vOriginalPos_GetterFunction", true))) {
        fn->name = base::global::UnkStrId("_m_vOriginalPos_GetterFunction", true);
        fn->return_type = fn_71000246e4();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad5e8);
    }
    UnkReflField* field2 = fn_7100096524(cls, CStrId("vOriginalPos", true), fn_71000246e4(), 0xffffffff, cls->m_array_0x110.size() - 1, -1);
    field2->attrs.Add<s32>(fn_710000bf80(), &g_unk_7101d679f0, 0);
    field2->attrs.Add<s32>(fn_71000923a0(), &g_unk_7101d55008, 1);
    field2->attrs.Add<bool>(fn_7100090a20(), false);
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("_m_vctCutscenesOffsets_GetterFunction", true))) {
        fn->name = base::global::UnkStrId("_m_vctCutscenesOffsets_GetterFunction", true);
        fn->return_type = &g_unk_7101d5c078;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad648);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("_m_vctCutscenesOffsets_SetterFunction", true)))
        fn_71007a52d8(fn, "_m_vctCutscenesOffsets_SetterFunction");
    UnkReflField* field3 = fn_7100096524(cls, CStrId("vctCutscenesOffsets", true), &g_unk_7101d5c078, 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1);
    field3->attrs.Add<bool>(fn_7100090a20(), false);
    UnkReflField* field4 = fn_7100096524(cls, CStrId("vctExtraInvolvedSubareas", true), &g_unk_7101d5c998, 0x80, -1, -1);
    UnkReflField* field5 = fn_7100096524(cls, CStrId("vctExtraInvolvedActors", true), &g_unk_7172719170, 0xd0, -1, -1);
    field5->attrs.Add<CStrId>(fn_710008fd60(), CStrId("CreateExtraActor", true));
    field5->attrs_0x70.Add<CStrId>(fn_7100091c30(), CStrId("Extra Actor", true));
    UnkReflField* field6 = fn_7100096524(cls, CStrId("vctOnBeforeCutsceneStartsLA", true), &g_unk_717274caa0, 0xf8, -1, -1);
    UnkReflField* field7 = fn_7100096524(cls, CStrId("vctOnAfterCutsceneEndsLA", true), &g_unk_717274caa0, 0x120, -1, -1);
    UnkReflField* field8 = fn_7100096524(cls, CStrId("bHasSamusAsExtraActor", true), fn_710000d228(), 0x210, -1, -1);
    field8->attrs.Add<bool>(fn_7100090a20(), false);
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SetupCutsceneForEditor", true))) {
        fn->name = base::global::UnkStrId("SetupCutsceneForEditor", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad78c);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("ChangeToTake", true)))
        fn_71007a544c(fn, "ChangeToTake");
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SetupCurrentTakePositions", true))) {
        fn->name = base::global::UnkStrId("SetupCurrentTakePositions", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad814);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SetGlobalfreeCamera", true))) {
        fn->name = base::global::UnkStrId("SetGlobalfreeCamera", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad8d4);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("LoadExtraInvolvedSubareas", true))) {
        fn->name = base::global::UnkStrId("LoadExtraInvolvedSubareas", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad8e8);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("ShowAllExtraActors", true))) {
        fn->name = base::global::UnkStrId("ShowAllExtraActors", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad91c);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("HideAllExtraActors", true))) {
        fn->name = base::global::UnkStrId("HideAllExtraActors", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad950);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("RetrieveExtraActorsVisibleStatus", true))) {
        fn->name = base::global::UnkStrId("RetrieveExtraActorsVisibleStatus", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad984);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("LaunchCutscene", true))) {
        fn->name = base::global::UnkStrId("LaunchCutscene", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad9b8);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("LaunchCutsceneImmediate", true))) {
        fn->name = base::global::UnkStrId("LaunchCutsceneImmediate", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad9ec);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("HasCutsceneBeenPlayed", true))) {
        fn->name = base::global::UnkStrId("HasCutsceneBeenPlayed", true);
        fn->return_type = fn_710000d228();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ada28);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("OnTakeChanged", true))) {
        fn->name = base::global::UnkStrId("OnTakeChanged", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adb08);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("ResetOffsets", true))) {
        fn->name = base::global::UnkStrId("ResetOffsets", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adb3c);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("CreateExtraActor", true))) {
        fn->name = base::global::UnkStrId("CreateExtraActor", true);
        fn->return_type = (fn_71007a225c(), &g_unk_71727199c0);
        fn->field_0xb2 = 0x1;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adb50);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("ContinueCutsceneIfTakeLooped", true))) {
        fn->name = base::global::UnkStrId("ContinueCutsceneIfTakeLooped", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adba0);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("TryLaunchCutscene", true))) {
        fn->name = base::global::UnkStrId("TryLaunchCutscene", true);
        fn->return_type = fn_710000d228();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adbe8);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("QueueCutscenePlayer", true)))
        fn_71007a55c8(fn, "QueueCutscenePlayer");
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SetTakePlayMode", true))) {
        fn->name = base::global::UnkStrId("SetTakePlayMode", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ade14);
        fn_71007adc7c(fn);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SkipCutscene", true)))
        fn_71007a5744(fn, "SkipCutscene");
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("GetCutscene", true))) {
        fn->name = base::global::UnkStrId("GetCutscene", true);
        fn->return_type = fn_71007adf30();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ae270);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SetExtraActorStatus", true))) {
        fn->name = base::global::UnkStrId("SetExtraActorStatus", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ae2bc);
        fn_7100302b38(fn);
        fn_710009ae5c(fn, fn->params.count);
        fn->ParamAttrs(0)->Add<CStrId>(fn_710008f920(), CStrId("_sTargetActorId", true));
        fn->ParamAttrs(1)->Add<CStrId>(fn_710008f920(), CStrId("_bEnabled", true));
        fn->ParamAttrs(2)->Add<CStrId>(fn_710008f920(), CStrId("_bRestoreVisibility", true));
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("OnExtraActorVisibilityPerTakeChanged", true))) {
        fn->name = base::global::UnkStrId("OnExtraActorVisibilityPerTakeChanged", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ae4e8);
        fn_71007ae40c(fn);
        fn_710009ae5c(fn, fn->params.count);
        fn->ParamAttrs(0)->Add<CStrId>(fn_710008f920(), CStrId("_uExtraActorIndex", true));
        fn->ParamAttrs(1)->Add<CStrId>(fn_710008f920(), CStrId("_uTakeIndex", true));
        fn->ParamAttrs(2)->Add<CStrId>(fn_710008f920(), CStrId("_bState", true));
    }
}
