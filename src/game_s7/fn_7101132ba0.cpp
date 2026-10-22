// Static initialiser of the unit of CSpawnerActorBlueprint,
// base::global::CRntVector<CSpawnerActorBlueprint>, CGameLink<CSpawnPointComponent> and others:
// the header ids, then the unit's own statics and registrations (helpers in UnkStaticInitS7.h).
// Layouts are placeholders.
#include "UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7101132ba0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 pad_0x38[0x20];
    u8 field_0x58[8];
};
extern UnkStatics_7101132ba0 g_unk_7172a08680 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_7172a06900;
const void* fn_710000d228();
extern UnkProperty g_unk_7172a06908;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101cb2a58[];
void fn_710112f064(void* p);
void fn_7101120afc(void* p);
extern UnkEnumType g_unk_7172a06930;
void fn_7101120c4c(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
void fn_7101120b74();
extern char g_unk_7172a06a68[];
extern char g_unk_7172a06a69[];
UnkEnumType* fn_7101120dc0();
extern char g_unk_7172a06a6a[];
UnkEnumType* fn_7101120e9c();
extern char g_unk_7172a06a6b[];
UnkEnumType* fn_7101120f78();
extern UnkEnumType g_unk_7172a06a70;
void fn_71011213d8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7101d5e2e0[];
void fn_7101121054();
extern char g_unk_7172a06ba8[];
extern char g_unk_7172a06ba9[];
UnkEnumType* fn_710112155c();
extern UnkEnumType g_unk_7172a06bb0;
void fn_71011219bc(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7101121638();
extern char g_unk_7172a06ce8[];
extern char g_unk_7172a06ce9[];
UnkEnumType* fn_7101121b40();
extern UnkEnumType g_unk_7172a06cf0;
void fn_7101121c1c(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_7172a06e60[];
extern char g_unk_7172a06e61[];
UnkEnumType* fn_7101121e00();
extern char g_unk_7172a06e62[];
UnkEnumType* fn_7101121edc();
extern UnkEnumType g_unk_7172a06e68;
void fn_7101122058(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71727bbe68[];
void fn_7101121fb8();
extern char g_unk_7172a06fa0[];
extern UnkConversion g_unk_7172a06fa8;
void fn_7101130850();
UnkEnumType* fn_7100009658();
extern char g_unk_7172a06fc8[];
UnkEnumType* fn_7101122358();
extern char g_unk_7172a06fc9[];
UnkEnumType* fn_7101122434();
extern UnkEnumType g_unk_7172a06fd0;
void fn_7101122894(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7101122510();
extern char g_unk_7172a07108[];
extern char g_unk_7172a07109[];
UnkEnumType* fn_7101122a18();
extern UnkEnumType g_unk_7172a07110;
void fn_7101122e78(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7101122af4();
extern char g_unk_7172a07248[];
extern char g_unk_7172a07249[];
UnkEnumType* fn_7101122ffc();
extern UnkEnumType g_unk_7172a07250;
void fn_71011230d8(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7172a073c0[];
extern UnkConversion g_unk_7172a074e8;
extern UnkConversion g_unk_7172a07508;
void fn_710112ef68() __attribute__((visibility("hidden")));
void fn_710112ef74() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172a073c8;
extern char g_unk_7172a074e0[];
void fn_7101123478(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71011232cc();
extern UnkConversion g_unk_7172a07648;
extern UnkConversion g_unk_7172a07668;
void fn_710112ef80() __attribute__((visibility("hidden")));
void fn_710112ef8c() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172a07528;
extern char g_unk_7172a07640[];
void fn_7101123744(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71011235f8();
void* fn_7101123918();
extern char g_unk_7172a07688[];
extern char g_unk_7172a07689[];
UnkEnumType* fn_71011201e4();
inline const CStrId& StaticName_71726b2228() {
    static CStrId s("SPAWNPOINT", true);
    return s;
}
struct UnkStruct_7172a0768c {
    u32 field_0x0;
};
extern UnkStruct_7172a0768c g_unk_7172a0768c;
void fn_71011258dc(void* self);  // its destructor, as registered

void fn_7101132ba0() {
    InitHeaderIds(&g_unk_7172a08680.ids);
    InitId(&g_unk_7172a06900, "SPAWNPOINT");
    InitProperty(&g_unk_7172a06908, g_unk_7101cb2a58, g_unk_71726db788, "Enabled", fn_710000d228(), fn_710112f064, fn_7101120afc);
    {
        CStrId n("CSpawnerActorBlueprint", true);
        fn_7101120c4c(&g_unk_7172a06930, n, nullptr, nullptr, fn_7101120b74, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a06930, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a06a68, &g_unk_7172a06930);
    fn_710009ec00(g_unk_7172a06a69, fn_7101120dc0());
    fn_710009ec00(g_unk_7172a06a6a, fn_7101120e9c());
    fn_710009ec00(g_unk_7172a06a6b, fn_7101120f78());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CSpawnerActorBlueprint>>::TKeyElementIterator::value_type", true);
        fn_71011213d8(&g_unk_7172a06a70, n, g_unk_7101d5e2e0, nullptr, fn_7101121054, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a06a70, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a06ba8, &g_unk_7172a06a70);
    fn_710009ec00(g_unk_7172a06ba9, fn_710112155c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CSpawnerActorBlueprint>>::TKeyElementConstIterator::value_type", true);
        fn_71011219bc(&g_unk_7172a06bb0, n, g_unk_7101d5e2e0, nullptr, fn_7101121638, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a06bb0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a06ce8, &g_unk_7172a06bb0);
    fn_710009ec00(g_unk_7172a06ce9, fn_7101121b40());
    {
        CStrId n("base::global::CRntVector<CSpawnerActorBlueprint>", true);
        fn_7101121c1c(&g_unk_7172a06cf0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172a06cf0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a06e60, &g_unk_7172a06cf0);
    fn_710009ec00(g_unk_7172a06e61, fn_7101121e00());
    fn_710009ec00(g_unk_7172a06e62, fn_7101121edc());
    {
        CStrId n("CGameLink<CSpawnPointComponent>", true);
        fn_7101122058(&g_unk_7172a06e68, n, g_unk_71727bbe68, nullptr, fn_7101121fb8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a06e68, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a06fa0, &g_unk_7172a06e68);
    InitConversion(&g_unk_7172a06fa8, fn_7100009658, &g_unk_7172a06e68, fn_7101130850);
    fn_710009ec00(g_unk_7172a06fc8, fn_7101122358());
    fn_710009ec00(g_unk_7172a06fc9, fn_7101122434());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CGameLink<CSpawnPointComponent>>>::TKeyElementIterator::value_type", true);
        fn_7101122894(&g_unk_7172a06fd0, n, g_unk_7101d5e2e0, nullptr, fn_7101122510, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a06fd0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a07108, &g_unk_7172a06fd0);
    fn_710009ec00(g_unk_7172a07109, fn_7101122a18());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CGameLink<CSpawnPointComponent>>>::TKeyElementConstIterator::value_type", true);
        fn_7101122e78(&g_unk_7172a07110, n, g_unk_7101d5e2e0, nullptr, fn_7101122af4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a07110, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a07248, &g_unk_7172a07110);
    fn_710009ec00(g_unk_7172a07249, fn_7101122ffc());
    {
        CStrId n("base::global::CRntVector<CGameLink<CSpawnPointComponent>>", true);
        fn_71011230d8(&g_unk_7172a07250, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172a07250, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a073c0, &g_unk_7172a07250);
    InitEnum(&g_unk_7172a073c8, "CSpawnPointComponent::EDynamicSpawnPositionMode", fn_7101123478, fn_71011232cc, g_unk_7172a074e0, &g_unk_7172a074e8, fn_710112ef68, &g_unk_7172a07508, fn_710112ef74);
    InitEnum(&g_unk_7172a07528, "CSpawnPointComponent::EXCellSpawnPositionMode", fn_7101123744, fn_71011235f8, g_unk_7172a07640, &g_unk_7172a07648, fn_710112ef80, &g_unk_7172a07668, fn_710112ef8c);
    fn_7101123918();
    fn_710009ec00(g_unk_7172a07688, g_unk_7172a08680.field_0x58);
    fn_710009ec00(g_unk_7172a07689, fn_71011201e4());
    InitClassIdFrom(&g_unk_7172a08680.field_0x20, StaticName_71726b2228());
    InitClassId(&g_unk_7172a08680.field_0x28, "SCENARIOSPAWNPOINTS");
    InitId(&g_unk_7172a08680.field_0x30, "xparasite");
    g_unk_7172a0768c.field_0x0 = 0;
    __cxa_atexit(fn_71011258dc, &g_unk_7172a0768c, g_unk_7101a19000);
}
