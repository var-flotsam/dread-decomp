// Static initialiser of CXParasiteAIComponent's unit (with the CXParasiteAIComponent::EXParasiteBehaviorType enum).
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006e5b10 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    UnkStrIdBase field_0x68;
    UnkStrIdBase field_0x70;
    UnkStrIdBase field_0x78;
    u8 pad_0x80[0x8];
    u8 field_0x88[8];
};
extern UnkStatics_71006e5b10 g_unk_717270b898 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270b710;
void fn_71006e590c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006e5520();
extern char g_unk_717270b828[];
extern UnkConversion g_unk_717270b830;
extern UnkConversion g_unk_717270b850;
void fn_71006ebac8() __attribute__((visibility("hidden")));
void fn_71006ebad4() __attribute__((visibility("hidden")));
extern char g_unk_717270b880[];

void fn_71006e5b10() {
    InitHeaderIds(&g_unk_717270b898.ids);
    InitEnum(&g_unk_717270b710, "CXParasiteAIComponent::EXParasiteBehaviorType", fn_71006e590c, fn_71006e5520,
             g_unk_717270b828, &g_unk_717270b830, fn_71006ebac8, &g_unk_717270b850, fn_71006ebad4);
    InitClassId(&g_unk_717270b898.field_0x18, "CXParasiteAIComponent");
    InitId(&g_unk_717270b898.field_0x20, "v3StayPos");
    InitId(&g_unk_717270b898.field_0x28, "v3PowerBombPos");
    InitId(&g_unk_717270b898.field_0x30, "fInitTime");
    InitId(&g_unk_717270b898.field_0x38, "fWanderTime");
    InitId(&g_unk_717270b898.field_0x40, "bCanBeAbsorbed");
    InitId(&g_unk_717270b898.field_0x48, "bIsBeingAttracted");
    InitId(&g_unk_717270b898.field_0x50, "sEntityName");
    InitId(&g_unk_717270b898.field_0x58, "bResetWanderCenter");
    InitId(&g_unk_717270b898.field_0x60, "Speed");
    InitId(&g_unk_717270b898.field_0x68, "Angle");
    InitId(&g_unk_717270b898.field_0x70, "AbsorptionAngle");
    InitId(&g_unk_717270b898.field_0x78, "traileffect");
    fn_71006e5f28();
    fn_710009ec00(g_unk_717270b880, g_unk_717270b898.field_0x88);
}
