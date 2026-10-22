// Static initialiser of the unit of ELOSMode, ELOSCheckLevel, CAIComponent::PathLimitSelectionMode
// and others: the header ids, then the unit's own statics and registrations (helpers in
// UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7101116680 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkFilePathStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkFilePathStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkFilePathStrIdBase field_0x48;
    UnkFilePathStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    UnkStrIdBase field_0x68;
    UnkStrIdBase field_0x70;
    UnkStrIdBase field_0x78;
    UnkStrIdBase field_0x80;
    UnkStrIdBase field_0x88;
    UnkFilePathStrIdBase field_0x90;
    UnkFilePathStrIdBase field_0x98;
    UnkStrIdBase field_0xa0;
    u8 pad_0xa8[0x58];
    f32 field_0x100;
    f32 field_0x104;
    bool field_0x108;
    u8 pad_0x109[0x37];
    u8 field_0x140[8];
    u8 pad_0x148[0x98];
    u8 field_0x1e0[8];
};
extern UnkStatics_7101116680 g_unk_7172a062c0 __attribute__((visibility("hidden")));
void fn_71010f41a0(void* self, const UnkIdValue* items, u32 count);
void fn_71010f48b0(void* self);  // its destructor, as registered
extern UnkConversion g_unk_7172a05640;
extern UnkConversion g_unk_7172a05660;
void fn_71011121f0() __attribute__((visibility("hidden")));
void fn_71011121fc() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172a05520;
extern char g_unk_7172a05638[];
void fn_71010f4e10(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71010f4c04();
extern UnkConversion g_unk_7172a057a0;
extern UnkConversion g_unk_7172a057c0;
void fn_7101112208() __attribute__((visibility("hidden")));
void fn_7101112214() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172a05680;
extern char g_unk_7172a05798[];
void fn_71010f531c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71010f4f90();
extern UnkConversion g_unk_7172a05900;
extern UnkConversion g_unk_7172a05920;
void fn_7101112220() __attribute__((visibility("hidden")));
void fn_710111222c() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172a057e0;
extern char g_unk_7172a058f8[];
void fn_71010f57c8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71010f549c();
extern char g_unk_7172a05940[];
UnkEnumType* fn_71010f5948();
extern char g_unk_7172a05941[];
UnkEnumType* fn_71010f5a24();
extern UnkEnumType g_unk_7172a05948;
void fn_71010f5e84(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_7101d5e2e0[];
void fn_71010f5b00();
extern char g_unk_7172a05a80[];
extern char g_unk_7172a05a81[];
UnkEnumType* fn_71010f6008();
extern UnkEnumType g_unk_7172a05a88;
void fn_71010f6468(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71010f60e4();
extern char g_unk_7172a05bc0[];
extern char g_unk_7172a05bc1[];
UnkEnumType* fn_71010f65ec();
extern UnkEnumType g_unk_7172a05bc8;
void fn_71010f66c8(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_7172a05d38[];
extern UnkEnumType g_unk_7172a05d40;
void fn_71010f69c8(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_71010f68bc();
extern char g_unk_7172a05e78[];
void* fn_71010f6b50();
extern char g_unk_7172a05e79[];

void fn_7101116680() {
    InitHeaderIds(&g_unk_7172a062c0.ids);
    g_unk_7172a062c0.field_0x100 = -1.0f;
    g_unk_7172a062c0.field_0x104 = -1.0f;
    g_unk_7172a062c0.field_0x108 = true;
    InitId(&g_unk_7172a062c0.field_0x18, "short");
    InitId(&g_unk_7172a062c0.field_0x20, "crazy");
    InitPathId(&g_unk_7172a062c0.field_0x28, "system/fx/generic/stuncounter.bcptl");
    InitId(&g_unk_7172a062c0.field_0x30, "push");
    InitPathId(&g_unk_7172a062c0.field_0x38, "system/fx/generic/stun.bcptl");
    InitId(&g_unk_7172a062c0.field_0x40, ".OnSpawned");
    InitPathId(&g_unk_7172a062c0.field_0x48, "timelines/x_transformation/fx/splash_impact.bcmdl");
    InitPathId(&g_unk_7172a062c0.field_0x50, "timelines/x_transformation/fx/splash_impact.bcmdl");
    InitId(&g_unk_7172a062c0.field_0x58, "splash_impact1");
    InitId(&g_unk_7172a062c0.field_0x60, "splash_impact2");
    InitId(&g_unk_7172a062c0.field_0x68, "relax");
    InitId(&g_unk_7172a062c0.field_0x70, "Angle");
    InitId(&g_unk_7172a062c0.field_0x78, "fFleeTime");
    InitId(&g_unk_7172a062c0.field_0x80, "Character");
    InitId(&g_unk_7172a062c0.field_0x88, "DoorBlocker");
    [] {
        UnkIdValue items[] = {
            {UnkStrIdTmp("deathparticles", true), 2},
            {UnkStrIdTmp("deathparticles_melee", true), 2},
            {UnkStrIdTmp("deathparticles_barelyfrozen", true), 2},
            {UnkStrIdTmp("deathparticles_speedbooster", true), 1},
            {UnkStrIdTmp("deathparticles_shinespark", true), 1},
            {UnkStrIdTmp("deathparticles_01", true), 0},
            {UnkStrIdTmp("deathparticles_02", true), 0},
            {UnkStrIdTmp("deathparticles_03", true), 0},
            {UnkStrIdTmp("deathparticles_04", true), 0},
            {UnkStrIdTmp("deathparticles_05", true), 0},
        };
        fn_71010f41a0(&g_unk_7172a062c0.field_0x140, items, 10);
    }();
    __cxa_atexit(fn_71010f48b0, &g_unk_7172a062c0.field_0x140, g_unk_7101a19000);
    InitPathId(&g_unk_7172a062c0.field_0x90, "actors/characters/samus/fx/enemyimpact.bcptl");
    InitPathId(&g_unk_7172a062c0.field_0x98, "actors/characters/samus/fx/enemyimpact_big.bcptl");
    InitId(&g_unk_7172a062c0.field_0xa0, "CAIComponent::BlindAttack");
    InitEnum(&g_unk_7172a05520, "ELOSMode", fn_71010f4e10, fn_71010f4c04, g_unk_7172a05638, &g_unk_7172a05640, fn_71011121f0, &g_unk_7172a05660, fn_71011121fc);
    InitEnum(&g_unk_7172a05680, "ELOSCheckLevel", fn_71010f531c, fn_71010f4f90, g_unk_7172a05798, &g_unk_7172a057a0, fn_7101112208, &g_unk_7172a057c0, fn_7101112214);
    InitEnum(&g_unk_7172a057e0, "CAIComponent::PathLimitSelectionMode", fn_71010f57c8, fn_71010f549c, g_unk_7172a058f8, &g_unk_7172a05900, fn_7101112220, &g_unk_7172a05920, fn_710111222c);
    fn_710009ec00(g_unk_7172a05940, fn_71010f5948());
    fn_710009ec00(g_unk_7172a05941, fn_71010f5a24());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SFallBackPath>>::TKeyElementIterator::value_type", true);
        fn_71010f5e84(&g_unk_7172a05948, n, g_unk_7101d5e2e0, nullptr, fn_71010f5b00, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a05948, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a05a80, &g_unk_7172a05948);
    fn_710009ec00(g_unk_7172a05a81, fn_71010f6008());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SFallBackPath>>::TKeyElementConstIterator::value_type", true);
        fn_71010f6468(&g_unk_7172a05a88, n, g_unk_7101d5e2e0, nullptr, fn_71010f60e4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a05a88, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a05bc0, &g_unk_7172a05a88);
    fn_710009ec00(g_unk_7172a05bc1, fn_71010f65ec());
    {
        CStrId n("base::global::CRntVector<SFallBackPath>", true);
        fn_71010f66c8(&g_unk_7172a05bc8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172a05bc8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a05d38, &g_unk_7172a05bc8);
    {
        CStrId n("SFallBackPath", true);
        fn_71010f69c8(&g_unk_7172a05d40, n, nullptr, nullptr, fn_71010f68bc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a05d40, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a05e78, &g_unk_7172a05d40);
    fn_71010f6b50();
    fn_710009ec00(g_unk_7172a05e79, g_unk_7172a062c0.field_0x1e0);
}
