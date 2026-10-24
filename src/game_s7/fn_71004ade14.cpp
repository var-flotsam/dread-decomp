// Reflection fields of CCharClassCooldownXBossLavaDropsAttack. Written from the refl_attrs draft;
// one field type getter lives in this class's translation unit.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflField;
using base::reflection::UnkReflFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
const base::reflection::UnkReflAttrKey& fn_7100075240();
extern const base::reflection::CType g_unk_71726d5010 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_71726d3d30;

void fn_71004ade14(CClass* cls) {
    UnkReflField* field0 = fn_7100096524(cls, CStrId("fTimeInit", true), fn_710000abbc(), 0x100, -1, -1);
    field0->attrs.Add<f32>(fn_7100075240(), 0.5f);
    UnkReflField* field1 = fn_7100096524(cls, CStrId("fTimeBetweenShots", true), fn_710000abbc(), 0x104, -1, -1);
    field1->attrs.Add<f32>(fn_7100075240(), 0.3f);
    UnkReflField* field2 = fn_7100096524(cls, CStrId("fTimeLocked", true), fn_710000abbc(), 0x108, -1, -1);
    field2->attrs.Add<f32>(fn_7100075240(), 0.1f);
    UnkReflField* field3 = fn_7100096524(cls, CStrId("fShotPreparationTime", true), fn_710000abbc(), 0x10c, -1, -1);
    field3->attrs.Add<f32>(fn_7100075240(), 1.5f);
    UnkReflField* field4 = fn_7100096524(cls, CStrId("fShotTime", true), fn_710000abbc(), 0x110, -1, -1);
    field4->attrs.Add<f32>(fn_7100075240(), 0.2f);
    UnkReflField* field5 = fn_7100096524(cls, CStrId("fDamageRadius", true), fn_710000abbc(), 0x114, -1, -1);
    field5->attrs.Add<f32>(fn_7100075240(), 100.0f);
    UnkReflField* field6 = fn_7100096524(cls, CStrId("fAimSpeed", true), fn_710000abbc(), 0x118, -1, -1);
    field6->attrs.Add<f32>(fn_7100075240(), 5.0f);
    UnkReflField* field7 = fn_7100096524(cls, CStrId("fAimMinSpeed", true), fn_710000abbc(), 0x11c, -1, -1);
    field7->attrs.Add<f32>(fn_7100075240(), 200.0f);
    UnkReflField* field8 = fn_7100096524(cls, CStrId("fAimMaxSpeed", true), fn_710000abbc(), 0x120, -1, -1);
    field8->attrs.Add<f32>(fn_7100075240(), 500.0f);
    UnkReflField* field9 = fn_7100096524(cls, CStrId("fAimMaxSpeedMaxDistance", true), fn_710000abbc(), 0x124, -1, -1);
    field9->attrs.Add<f32>(fn_7100075240(), 700.0f);
    UnkReflField* field10 = fn_7100096524(cls, CStrId("tInitialState", true), (fn_71004ab5ac(), &g_unk_71726d5010), 0x128, -1, -1);
    UnkReflField* field11 = fn_7100096524(cls, CStrId("tPatterns", true), &g_unk_71726d3d30, 0x168, -1, -1);
}
