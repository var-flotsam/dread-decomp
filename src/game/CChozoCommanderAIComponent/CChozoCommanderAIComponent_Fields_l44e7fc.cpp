// CChozoCommanderAIComponent's unit (tools/tu_map.py unit 665): fn_710044e7fc registers the component's
// reflected fields: the ultimate grab test flag and the landmark and cutscene player links (each a link
// target of kind 6 or 1). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU2536.h"
#include "types.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflField;

struct UnkEnumType;
extern UnkEnumType g_unk_717269bb48;  // the landmark link's type
extern UnkEnumType g_unk_71727b6270;  // the cutscene player link's type

static inline const CType* TypeOf(UnkReflType& t) { return reinterpret_cast<const CType*>(&t); }
static inline const CType* TypeOf(UnkEnumType& t) { return reinterpret_cast<const CType*>(&t); }

void fn_710044e7fc(CClass* cls) {
    UnkReflField* testMode = AddReflectedField(cls, CStrId("bUltimateGrabTestMode", true), fn_710000d228(), 0x15d70, -1, -1);
    testMode->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* landmark = AddReflectedField(cls, CStrId("wpUltimateGrabLandmark", true), TypeOf(g_unk_717269bb48), 0x15d30, -1, -1);
    landmark->attrs.Add<u32>(fn_710025daa0(), TypeOf(base::reflection::TypeObject<SLinkType::ELinkType, UnkReflType>::object), 6);
    UnkReflField* dead = AddReflectedField(cls, CStrId("wpDeadCutScenePlayer", true), TypeOf(g_unk_71727b6270), 0x15d78, -1, -1);
    dead->attrs.Add<u32>(fn_710025daa0(), TypeOf(base::reflection::TypeObject<SLinkType::ELinkType, UnkReflType>::object), 1);
    UnkReflField* phase2 = AddReflectedField(cls, CStrId("wpPhase2CutscenePlayer", true), TypeOf(g_unk_71727b6270), 0x15db8, -1, -1);
    phase2->attrs.Add<u32>(fn_710025daa0(), TypeOf(base::reflection::TypeObject<SLinkType::ELinkType, UnkReflType>::object), 1);
    UnkReflField* phase3 = AddReflectedField(cls, CStrId("wpPhase3CutscenePlayer", true), TypeOf(g_unk_71727b6270), 0x15df8, -1, -1);
    phase3->attrs.Add<u32>(fn_710025daa0(), TypeOf(base::reflection::TypeObject<SLinkType::ELinkType, UnkReflType>::object), 1);
    UnkReflField* endLeft = AddReflectedField(cls, CStrId("wpPhase3EndLeftCutscenePlayer", true), TypeOf(g_unk_71727b6270), 0x15e38, -1, -1);
    endLeft->attrs.Add<u32>(fn_710025daa0(), TypeOf(base::reflection::TypeObject<SLinkType::ELinkType, UnkReflType>::object), 1);
    UnkReflField* endRight = AddReflectedField(cls, CStrId("wpPhase3EndRightCutscenePlayer", true), TypeOf(g_unk_71727b6270), 0x15e78, -1, -1);
    endRight->attrs.Add<u32>(fn_710025daa0(), TypeOf(base::reflection::TypeObject<SLinkType::ELinkType, UnkReflType>::object), 1);
}
