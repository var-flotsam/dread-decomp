// Reflection fields of CCharClassCooldownXBossLavaDropsAttack. Written from the refl_attrs draft;
// one field type getter lives in this class's translation unit.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CCooldownXBossLavaDropsMovementDef.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CVariable;
using base::reflection::CFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
extern const base::reflection::CType g_unk_71726d5010 __attribute__((visibility("hidden")));

void fn_71004ade14(CClass* cls) {
    CVariable* field0 = AddReflectedField(cls, CStrId("fTimeInit", true), fn_710000abbc(), 0x100, -1, -1);
    field0->attrs.Add<f32>(fn_7100075240(), 0.5f);
    CVariable* field1 = AddReflectedField(cls, CStrId("fTimeBetweenShots", true), fn_710000abbc(), 0x104, -1, -1);
    field1->attrs.Add<f32>(fn_7100075240(), 0.3f);
    CVariable* field2 = AddReflectedField(cls, CStrId("fTimeLocked", true), fn_710000abbc(), 0x108, -1, -1);
    field2->attrs.Add<f32>(fn_7100075240(), 0.1f);
    CVariable* field3 = AddReflectedField(cls, CStrId("fShotPreparationTime", true), fn_710000abbc(), 0x10c, -1, -1);
    field3->attrs.Add<f32>(fn_7100075240(), 1.5f);
    CVariable* field4 = AddReflectedField(cls, CStrId("fShotTime", true), fn_710000abbc(), 0x110, -1, -1);
    field4->attrs.Add<f32>(fn_7100075240(), 0.2f);
    CVariable* field5 = AddReflectedField(cls, CStrId("fDamageRadius", true), fn_710000abbc(), 0x114, -1, -1);
    field5->attrs.Add<f32>(fn_7100075240(), 100.0f);
    CVariable* field6 = AddReflectedField(cls, CStrId("fAimSpeed", true), fn_710000abbc(), 0x118, -1, -1);
    field6->attrs.Add<f32>(fn_7100075240(), 5.0f);
    CVariable* field7 = AddReflectedField(cls, CStrId("fAimMinSpeed", true), fn_710000abbc(), 0x11c, -1, -1);
    field7->attrs.Add<f32>(fn_7100075240(), 200.0f);
    CVariable* field8 = AddReflectedField(cls, CStrId("fAimMaxSpeed", true), fn_710000abbc(), 0x120, -1, -1);
    field8->attrs.Add<f32>(fn_7100075240(), 500.0f);
    CVariable* field9 = AddReflectedField(cls, CStrId("fAimMaxSpeedMaxDistance", true), fn_710000abbc(), 0x124, -1, -1);
    field9->attrs.Add<f32>(fn_7100075240(), 700.0f);
    CVariable* field10 = AddReflectedField(cls, CStrId("tInitialState", true), (fn_71004ab5ac(), &g_unk_71726d5010), 0x128, -1, -1);
    CVariable* field11 = AddReflectedField(cls, CStrId("tPatterns", true), &g_unk_71726d3d30, 0x168, -1, -1);
}
