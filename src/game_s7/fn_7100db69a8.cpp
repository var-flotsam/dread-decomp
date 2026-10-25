// Reflection fields of CBossRushManager::CTunableBossRushManager (the boss rush survival
// configuration): loop and per-boss debug-time members, then the tunable's registration.
// Written from the refl_attrs draft; the two type getters live in this class's translation unit.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflField;
using base::reflection::UnkReflFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
extern bool g_unk_71727bd260;
const base::reflection::UnkReflAttrKey& fn_7100075240();
// Both getters are defined in this class's translation unit, so the original calls them and then
// passes their known return value.
extern const base::reflection::CType g_unk_71727bdbb8 __attribute__((visibility("hidden")));
extern const base::reflection::CType g_unk_71727bdcf0 __attribute__((visibility("hidden")));
extern UnkTunableRegistry* g_unk_7101d47350;
extern UnkTunable g_unk_71727bd078;
void fn_7100db78b4(UnkTunableRegistry* registry, UnkTunable* obj, const char* name);

void fn_7100db69a8(CClass* cls) {
    if (g_unk_71727bd260)
        return;
    g_unk_71727bd260 = true;
    if (!cls)
        return;
    UnkReflField* field0 = fn_7100096524(cls, CStrId("bBossRushGameOverEnabled", true), fn_710000d228(), 0x21, -1, -1);
    field0->attrs.Add<bool>(fn_7100075240(), false);
    UnkReflField* field1 = fn_7100096524(cls, CStrId("fDeathAddedTime", true), fn_710000abbc(), 0x24, -1, -1);
    field1->attrs.Add<f32>(fn_7100075240(), 15.0f);
    UnkReflField* field2 = fn_7100096524(cls, CStrId("fSurvivalInitialTime", true), fn_710000abbc(), 0x28, -1, -1);
    field2->attrs.Add<f32>(fn_7100075240(), 300.0f);
    UnkReflField* field3 = fn_7100096524(cls, CStrId("fSurvivalInitialLife", true), fn_710000abbc(), 0x2c, -1, -1);
    field3->attrs.Add<f32>(fn_7100075240(), 599.0f);
    UnkReflField* field4 = fn_7100096524(cls, CStrId("fSurvivalInitialMissiles", true), fn_710000abbc(), 0x30, -1, -1);
    field4->attrs.Add<f32>(fn_7100075240(), 90.0f);
    UnkReflField* field5 = fn_7100096524(cls, CStrId("iSurvivalMaxLoops", true), fn_7100014768(), 0x34, -1, -1);
    field5->attrs.Add<s32>(fn_7100075240(), 10);
    UnkReflField* field6 = fn_7100096524(cls, CStrId("oLoop0", true), (fn_7100db5c98(), &g_unk_71727bdbb8), 0x38, -1, -1);
    UnkReflField* field7 = fn_7100096524(cls, CStrId("oLoop1", true), (fn_7100db5c98(), &g_unk_71727bdbb8), 0x58, -1, -1);
    UnkReflField* field8 = fn_7100096524(cls, CStrId("oLoop2", true), (fn_7100db5c98(), &g_unk_71727bdbb8), 0x78, -1, -1);
    UnkReflField* field9 = fn_7100096524(cls, CStrId("oLoop3", true), (fn_7100db5c98(), &g_unk_71727bdbb8), 0x98, -1, -1);
    UnkReflField* field10 = fn_7100096524(cls, CStrId("oCorpiusDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0xb8, -1, -1);
    UnkReflField* field11 = fn_7100096524(cls, CStrId("oKraidDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0xd0, -1, -1);
    UnkReflField* field12 = fn_7100096524(cls, CStrId("oArtariaCUDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0xe8, -1, -1);
    UnkReflField* field13 = fn_7100096524(cls, CStrId("oDrogygaDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x100, -1, -1);
    UnkReflField* field14 = fn_7100096524(cls, CStrId("oStrongRCSDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x118, -1, -1);
    UnkReflField* field15 = fn_7100096524(cls, CStrId("oEscueDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x130, -1, -1);
    UnkReflField* field16 = fn_7100096524(cls, CStrId("oExperimentNo57DebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x148, -1, -1);
    UnkReflField* field17 = fn_7100096524(cls, CStrId("oStrongRCSx2DebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x160, -1, -1);
    UnkReflField* field18 = fn_7100096524(cls, CStrId("oGolzunaDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x178, -1, -1);
    UnkReflField* field19 = fn_7100096524(cls, CStrId("oEliteCSDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x190, -1, -1);
    UnkReflField* field20 = fn_7100096524(cls, CStrId("oFereniaCUDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x1a8, -1, -1);
    UnkReflField* field21 = fn_7100096524(cls, CStrId("oRavenBeakDebugTime", true), (fn_7100db6424(), &g_unk_71727bdcf0), 0x1c0, -1, -1);
    UnkReflField* field22 = fn_7100096524(cls, CStrId("iDebugSurvivalInitialBoss", true), fn_7100014768(), 0x1d8, -1, -1);
    field22->attrs.Add<s32>(fn_7100075240(), 1);
    UnkReflField* field23 = fn_7100096524(cls, CStrId("fDebugSurvivalRemainingTime", true), fn_710000abbc(), 0x1dc, -1, -1);
    field23->attrs.Add<f32>(fn_7100075240(), 0.0f);
    UnkReflField* field24 = fn_7100096524(cls, CStrId("iDebugSurvivalLoop", true), fn_7100014768(), 0x1e0, -1, -1);
    field24->attrs.Add<s32>(fn_7100075240(), 0);
    UnkTunableRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(registry);
        g_unk_7101d47350 = registry;
    }
    fn_7100db78b4(registry, &g_unk_71727bd078, "");
}
