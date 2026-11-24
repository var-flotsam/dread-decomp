// Reflection fields and functions of CCutsceneComponent. Written from the refl_attrs draft; one
// return-type getter lives in this class's translation unit.
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CCutsceneComponent.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU36.h"
#include "game/UnkTU69.h"
#include "game/UnkTU83.h"
#include "game/UnkTU89.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CVariable;
using base::reflection::CFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
#include "base/reflection/TypeObject.h"
template <> base::reflection::CType base::reflection::TypeObject<void>::object asm("g_unk_7101d5af48");
namespace gameeditor { enum class EEditorModeDataFlow : u32; }
template <> base::reflection::CType base::reflection::TypeObject<gameeditor::EEditorModeDataFlow>::object asm("g_unk_7101d55008");
namespace base::serialization { enum class EPersistence : u32; }
template <> base::reflection::CType base::reflection::TypeObject<base::serialization::EPersistence>::object asm("g_unk_7101d679f0");
base::reflection::UnkReflValue fn_71007ad4d0(void*, char* obj);
void fn_71007a515c(CFunction* fn, const char* name);
void fn_71007ad5e8();
void fn_71007ad648();
void fn_71007a52d8(CFunction* fn, const char* name);
void fn_71007ad78c();
void fn_71007a544c(CFunction* fn, const char* name);
base::reflection::UnkReflValue fn_71007ad814(void*, char* obj);
void fn_71007ad8d4();
void fn_71007ad8e8();
void fn_71007ad91c();
void fn_71007ad950();
void fn_71007ad984();
void fn_71007ad9b8();
void fn_71007ad9ec();
base::reflection::UnkReflValue fn_71007ada28(void*, char* obj);
void fn_71007adb08();
void fn_71007adb3c();
extern const base::reflection::CType g_unk_71727199c0 __attribute__((visibility("hidden")));
void fn_71007adb50();
void fn_71007adba0();
base::reflection::UnkReflValue InvokeCutsceneComponentTryLaunchCutscene(void*, char* obj);
void fn_71007a55c8(CFunction* fn, const char* name);
base::reflection::UnkReflValue fn_71007ade14(void*, char* obj, base::reflection::UnkReflValue* const* args);
void PushSetTakePlayModeParamTypes(CFunction* fn);
void fn_71007a5744(CFunction* fn, const char* name);
void fn_71007ae270();
base::reflection::UnkReflValue fn_71007ae2bc(void*, char* obj, base::reflection::UnkReflValue* const* args);
void fn_7100302b38(CFunction* fn);
base::reflection::UnkReflValue fn_71007ae4e8(void*, char* obj, base::reflection::UnkReflValue* const* args);
void fn_71007ae40c(CFunction* fn);

void fn_71007a2fb4(CClass* cls) {
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_sCutsceneName_GetterFunction", true))) {
        fn->name = base::global::UnkStrId("_m_sCutsceneName_GetterFunction", true);
        fn->return_type = fn_7100009658();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad4d0);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_sCutsceneName_SetterFunction", true)))
        fn_71007a515c(fn, "_m_sCutsceneName_SetterFunction");
    CVariable* field0 = AddReflectedField(cls, CStrId("sCutsceneName", true), fn_7100009658(), 0xffffffff, cls->m_functions.size() - 2, cls->m_functions.size() - 1);
    field0->attrs.Add<bool>(base::reflection::attr::EditorVisible(), true);
    CVariable* field1 = AddReflectedField(cls, CStrId("bDisableScenarioEntitiesOnPlay", true), fn_710000d228(), 0x20e, -1, -1);
    field1->attrs.Add<bool>(base::reflection::attr::DefaultValue(), true);
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_vOriginalPos_GetterFunction", true))) {
        fn->name = base::global::UnkStrId("_m_vOriginalPos_GetterFunction", true);
        fn->return_type = fn_71000246e4();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad5e8);
    }
    CVariable* field2 = AddReflectedField(cls, CStrId("vOriginalPos", true), fn_71000246e4(), 0xffffffff, cls->m_functions.size() - 1, -1);
    field2->attrs.Add<s32>(base::reflection::attr::PersistenceKey(), &base::reflection::TypeObject<base::serialization::EPersistence>::object, 0);
    field2->attrs.Add<s32>(base::reflection::attr::EditorModeDataFlowKey(), &base::reflection::TypeObject<gameeditor::EEditorModeDataFlow>::object, 1);
    field2->attrs.Add<bool>(base::reflection::attr::EditorVisible(), false);
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_vctCutscenesOffsets_GetterFunction", true))) {
        fn->name = base::global::UnkStrId("_m_vctCutscenesOffsets_GetterFunction", true);
        fn->return_type = &g_unk_7101d5c078;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad648);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("_m_vctCutscenesOffsets_SetterFunction", true)))
        fn_71007a52d8(fn, "_m_vctCutscenesOffsets_SetterFunction");
    CVariable* field3 = AddReflectedField(cls, CStrId("vctCutscenesOffsets", true), &g_unk_7101d5c078, 0xffffffff, cls->m_functions.size() - 2, cls->m_functions.size() - 1);
    field3->attrs.Add<bool>(base::reflection::attr::EditorVisible(), false);
    CVariable* field4 = AddReflectedField(cls, CStrId("vctExtraInvolvedSubareas", true), &g_unk_7101d5c998, 0x80, -1, -1);
    CVariable* field5 = AddReflectedField(cls, CStrId("vctExtraInvolvedActors", true), &g_unk_7172719170, 0xd0, -1, -1);
    field5->attrs.Add<CStrId>(base::reflection::attr::CreateElement(), CStrId("CreateExtraActor", true));
    field5->attrs_0x70.Add<CStrId>(base::reflection::attr::DisplayName(), CStrId("Extra Actor", true));
    CVariable* field6 = AddReflectedField(cls, CStrId("vctOnBeforeCutsceneStartsLA", true), &g_unk_717274caa0, 0xf8, -1, -1);
    CVariable* field7 = AddReflectedField(cls, CStrId("vctOnAfterCutsceneEndsLA", true), &g_unk_717274caa0, 0x120, -1, -1);
    CVariable* field8 = AddReflectedField(cls, CStrId("bHasSamusAsExtraActor", true), fn_710000d228(), 0x210, -1, -1);
    field8->attrs.Add<bool>(base::reflection::attr::EditorVisible(), false);
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SetupCutsceneForEditor", true))) {
        fn->name = base::global::UnkStrId("SetupCutsceneForEditor", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad78c);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("ChangeToTake", true)))
        fn_71007a544c(fn, "ChangeToTake");
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SetupCurrentTakePositions", true))) {
        fn->name = base::global::UnkStrId("SetupCurrentTakePositions", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad814);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SetGlobalfreeCamera", true))) {
        fn->name = base::global::UnkStrId("SetGlobalfreeCamera", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad8d4);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("LoadExtraInvolvedSubareas", true))) {
        fn->name = base::global::UnkStrId("LoadExtraInvolvedSubareas", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad8e8);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("ShowAllExtraActors", true))) {
        fn->name = base::global::UnkStrId("ShowAllExtraActors", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad91c);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("HideAllExtraActors", true))) {
        fn->name = base::global::UnkStrId("HideAllExtraActors", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad950);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("RetrieveExtraActorsVisibleStatus", true))) {
        fn->name = base::global::UnkStrId("RetrieveExtraActorsVisibleStatus", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad984);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("LaunchCutscene", true))) {
        fn->name = base::global::UnkStrId("LaunchCutscene", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad9b8);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("LaunchCutsceneImmediate", true))) {
        fn->name = base::global::UnkStrId("LaunchCutsceneImmediate", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ad9ec);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("HasCutsceneBeenPlayed", true))) {
        fn->name = base::global::UnkStrId("HasCutsceneBeenPlayed", true);
        fn->return_type = fn_710000d228();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ada28);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("OnTakeChanged", true))) {
        fn->name = base::global::UnkStrId("OnTakeChanged", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adb08);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("ResetOffsets", true))) {
        fn->name = base::global::UnkStrId("ResetOffsets", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adb3c);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("CreateExtraActor", true))) {
        fn->name = base::global::UnkStrId("CreateExtraActor", true);
        fn->return_type = (fn_71007a225c(), &g_unk_71727199c0);
        fn->field_0xb2 = 0x1;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adb50);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("ContinueCutsceneIfTakeLooped", true))) {
        fn->name = base::global::UnkStrId("ContinueCutsceneIfTakeLooped", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007adba0);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("TryLaunchCutscene", true))) {
        fn->name = base::global::UnkStrId("TryLaunchCutscene", true);
        fn->return_type = fn_710000d228();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&InvokeCutsceneComponentTryLaunchCutscene);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("QueueCutscenePlayer", true)))
        fn_71007a55c8(fn, "QueueCutscenePlayer");
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SetTakePlayMode", true))) {
        fn->name = base::global::UnkStrId("SetTakePlayMode", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ade14);
        PushSetTakePlayModeParamTypes(fn);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SkipCutscene", true)))
        fn_71007a5744(fn, "SkipCutscene");
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("GetCutscene", true))) {
        fn->name = base::global::UnkStrId("GetCutscene", true);
        fn->return_type = fn_71007adf30();
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ae270);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SetExtraActorStatus", true))) {
        fn->name = base::global::UnkStrId("SetExtraActorStatus", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ae2bc);
        fn_7100302b38(fn);
        ResizeParameterAttributes(fn, fn->params.count);
        fn->ParamAttrs(0)->Add<CStrId>(base::reflection::attr::Name(), CStrId("_sTargetActorId", true));
        fn->ParamAttrs(1)->Add<CStrId>(base::reflection::attr::Name(), CStrId("_bEnabled", true));
        fn->ParamAttrs(2)->Add<CStrId>(base::reflection::attr::Name(), CStrId("_bRestoreVisibility", true));
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("OnExtraActorVisibilityPerTakeChanged", true))) {
        fn->name = base::global::UnkStrId("OnExtraActorVisibilityPerTakeChanged", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_71007ae4e8);
        fn_71007ae40c(fn);
        ResizeParameterAttributes(fn, fn->params.count);
        fn->ParamAttrs(0)->Add<CStrId>(base::reflection::attr::Name(), CStrId("_uExtraActorIndex", true));
        fn->ParamAttrs(1)->Add<CStrId>(base::reflection::attr::Name(), CStrId("_uTakeIndex", true));
        fn->ParamAttrs(2)->Add<CStrId>(base::reflection::attr::Name(), CStrId("_bState", true));
    }
}
