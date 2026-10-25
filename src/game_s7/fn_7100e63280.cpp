// Static initialiser of the unit of SGeoData, CMinimapData::SEntityData, CMinimapData::SDoorData
// and others: the header ids, then the unit's own statics and registrations (helpers in
// UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100e63280 {
    HeaderIds2 ids;
    u8 pad_0x20[0x8];
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
    UnkStrIdBase field_0x80;
    UnkStrIdBase field_0x88;
    UnkStrIdBase field_0x90;
    UnkStrIdBase field_0x98;
    UnkStrIdBase field_0xa0;
    UnkStrIdBase field_0xa8;
    UnkStrIdBase field_0xb0;
    UnkStrIdBase field_0xb8;
    u8 pad_0xc0[0x30];
    f32 field_0xf0;
    f32 field_0xf4;
    bool field_0xf8;
    u8 pad_0xf9[0x7];
    u64 field_0x100;
    u64 field_0x108;
    u64 field_0x110;
    void (*field_0x118)();
    u64 field_0x120;
    u64 field_0x128;
    u64 field_0x130;
    u64 field_0x138;
    u64 field_0x140;
    void (*field_0x148)();
    u64 field_0x150;
    u64 field_0x158;
    u64 field_0x160;
    u64 field_0x168;
    u64 field_0x170;
    void (*field_0x178)();
    u64 field_0x180;
    u64 field_0x188;
    u8 field_0x190[8];
};
extern UnkStatics_7100e63280 g_unk_71727c7170 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71727c15f0;
void fn_7100e2ffd8(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
void fn_7100e2fd9c();
extern char g_unk_71727c1728[];
extern UnkEnumType g_unk_71727c1730;
void fn_7100e303b8(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e3015c();
extern char g_unk_71727c1868[];
extern UnkEnumType g_unk_71727c1870;
void fn_7100e30920(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e3053c();
extern char g_unk_71727c19a8[];
extern UnkEnumType g_unk_71727c19b0;
void fn_7100e30bb0(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e30aa4();
extern char g_unk_71727c1ae8[];
extern UnkEnumType g_unk_71727c1af0;
void fn_7100e30f00(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e30d34();
extern char g_unk_71727c1c28[];
extern UnkEnumType g_unk_71727c1c30;
void fn_7100e315c4(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e31084();
extern char g_unk_71727c1d68[];
extern UnkEnumType g_unk_71727c1d70;
void fn_7100e31a94(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100e31748();
extern char g_unk_71727c1ea8[];
extern char g_unk_71727c1ea9[];
extern char g_unk_71727c1eaa[];
UnkEnumType* fn_7100e32bec();
extern char g_unk_71727c1eab[];
UnkEnumType* fn_7100e32cc8();
extern UnkEnumType g_unk_71727c1eb0;
void fn_7100e33128(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7101d5e2e0[];
void fn_7100e32da4();
extern char g_unk_71727c1fe8[];
extern char g_unk_71727c1fe9[];
UnkEnumType* fn_7100e332ac();
extern UnkEnumType g_unk_71727c1ff0;
void fn_7100e3370c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e33388();
extern char g_unk_71727c2128[];
extern char g_unk_71727c2129[];
UnkEnumType* fn_7100e33890();
extern UnkEnumType g_unk_71727c2130;
void fn_7100e3396c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c22a0[];
extern char g_unk_71727c22a1[];
UnkEnumType* fn_7100e33b60();
extern char g_unk_71727c22a2[];
UnkEnumType* fn_7100e33c3c();
extern UnkEnumType g_unk_71727c22a8;
void fn_7100e3409c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e33d18();
extern char g_unk_71727c23e0[];
extern char g_unk_71727c23e1[];
UnkEnumType* fn_7100e34220();
extern UnkEnumType g_unk_71727c23e8;
void fn_7100e34680(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e342fc();
extern char g_unk_71727c2520[];
extern char g_unk_71727c2521[];
UnkEnumType* fn_7100e34804();
extern UnkEnumType g_unk_71727c2528;
void fn_7100e348e0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c2698[];
extern char g_unk_71727c2699[];
UnkEnumType* fn_7100e34ad8();
extern char g_unk_71727c269a[];
UnkEnumType* fn_7100e34bb4();
extern UnkEnumType g_unk_71727c26a0;
void fn_7100e35014(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e34c90();
extern char g_unk_71727c27d8[];
extern char g_unk_71727c27d9[];
UnkEnumType* fn_7100e35198();
extern UnkEnumType g_unk_71727c27e0;
void fn_7100e355f8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e35274();
extern char g_unk_71727c2918[];
extern char g_unk_71727c2919[];
UnkEnumType* fn_7100e3577c();
extern UnkEnumType g_unk_71727c2920;
void fn_7100e35858(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c2a90[];
extern char g_unk_71727c2a91[];
UnkEnumType* fn_7100e35a50();
extern char g_unk_71727c2a92[];
UnkEnumType* fn_7100e35b2c();
extern UnkEnumType g_unk_71727c2a98;
void fn_7100e35f8c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e35c08();
extern char g_unk_71727c2bd0[];
extern char g_unk_71727c2bd1[];
UnkEnumType* fn_7100e36110();
extern UnkEnumType g_unk_71727c2bd8;
void fn_7100e36570(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e361ec();
extern char g_unk_71727c2d10[];
extern char g_unk_71727c2d11[];
UnkEnumType* fn_7100e366f4();
extern UnkEnumType g_unk_71727c2d18;
void fn_7100e367d0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c2e88[];
extern char g_unk_71727c2e89[];
UnkEnumType* fn_7100e369c8();
extern char g_unk_71727c2e8a[];
UnkEnumType* fn_7100e36aa4();
extern UnkEnumType g_unk_71727c2e90;
void fn_7100e36f04(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e36b80();
extern char g_unk_71727c2fc8[];
extern char g_unk_71727c2fc9[];
UnkEnumType* fn_7100e37088();
extern UnkEnumType g_unk_71727c2fd0;
void fn_7100e374e8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e37164();
extern char g_unk_71727c3108[];
extern char g_unk_71727c3109[];
UnkEnumType* fn_7100e3766c();
extern UnkEnumType g_unk_71727c3110;
void fn_7100e37748(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c3280[];
extern char g_unk_71727c3281[];
UnkEnumType* fn_7100e37940();
extern char g_unk_71727c3282[];
UnkEnumType* fn_7100e37a1c();
extern UnkEnumType g_unk_71727c3288;
void fn_7100e37e7c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e37af8();
extern char g_unk_71727c33c0[];
extern char g_unk_71727c33c1[];
UnkEnumType* fn_7100e38000();
extern UnkEnumType g_unk_71727c33c8;
void fn_7100e38460(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e380dc();
extern char g_unk_71727c3500[];
extern char g_unk_71727c3501[];
UnkEnumType* fn_7100e385e4();
extern UnkEnumType g_unk_71727c3508;
void fn_7100e386c0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c3678[];
extern char g_unk_71727c3679[];
UnkEnumType* fn_7100e388b8();
extern char g_unk_71727c367a[];
UnkEnumType* fn_7100e38994();
extern UnkEnumType g_unk_71727c3680;
void fn_7100e38df4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e38a70();
extern char g_unk_71727c37b8[];
extern char g_unk_71727c37b9[];
UnkEnumType* fn_7100e38f78();
extern UnkEnumType g_unk_71727c37c0;
void fn_7100e393d8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e39054();
extern char g_unk_71727c38f8[];
extern char g_unk_71727c38f9[];
UnkEnumType* fn_7100e3955c();
extern UnkEnumType g_unk_71727c3900;
void fn_7100e39638(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c3a70[];
extern char g_unk_71727c3a71[];
UnkEnumType* fn_7100e39830();
extern char g_unk_71727c3a72[];
UnkEnumType* fn_7100e3990c();
extern UnkEnumType g_unk_71727c3a78;
void fn_7100e39d6c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e399e8();
extern char g_unk_71727c3bb0[];
extern char g_unk_71727c3bb1[];
UnkEnumType* fn_7100e39ef0();
extern UnkEnumType g_unk_71727c3bb8;
void fn_7100e3a350(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e39fcc();
extern char g_unk_71727c3cf0[];
extern char g_unk_71727c3cf1[];
UnkEnumType* fn_7100e3a4d4();
extern UnkEnumType g_unk_71727c3cf8;
void fn_7100e3a5b0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c3e68[];
extern char g_unk_71727c3e69[];
UnkEnumType* fn_7100e3a7a8();
extern char g_unk_71727c3e6a[];
UnkEnumType* fn_7100e3a884();
extern UnkEnumType g_unk_71727c3e70;
void fn_7100e3ace4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e3a960();
extern char g_unk_71727c3fa8[];
extern char g_unk_71727c3fa9[];
UnkEnumType* fn_7100e3ae68();
extern UnkEnumType g_unk_71727c3fb0;
void fn_7100e3b2c8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e3af44();
extern char g_unk_71727c40e8[];
extern char g_unk_71727c40e9[];
UnkEnumType* fn_7100e3b44c();
extern UnkEnumType g_unk_71727c40f0;
void fn_7100e3b528(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c4260[];
extern char g_unk_71727c4261[];
UnkEnumType* fn_7100e3b720();
extern char g_unk_71727c4262[];
UnkEnumType* fn_7100e3b7fc();
extern UnkEnumType g_unk_71727c4268;
void fn_7100e3bc5c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e3b8d8();
extern char g_unk_71727c43a0[];
extern char g_unk_71727c43a1[];
UnkEnumType* fn_7100e3bde0();
extern UnkEnumType g_unk_71727c43a8;
void fn_7100e3c240(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100e3bebc();
extern char g_unk_71727c44e0[];
extern char g_unk_71727c44e1[];
UnkEnumType* fn_7100e3c3c4();
extern UnkEnumType g_unk_71727c44e8;
void fn_7100e3c4a0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727c4658[];
extern UnkStrIdBase g_unk_71727c4660;
extern UnkStrIdBase g_unk_71727c4668;
extern char g_unk_71727c4670[];

void fn_7100e63280() {
    InitHeaderIds(&g_unk_71727c7170.ids);
    g_unk_71727c7170.field_0xf0 = -1.0f;
    g_unk_71727c7170.field_0xf4 = -1.0f;
    g_unk_71727c7170.field_0xf8 = true;
    g_unk_71727c7170.field_0x100 = 0;
    g_unk_71727c7170.field_0x108 = 0;
    g_unk_71727c7170.field_0x110 = 0;
    g_unk_71727c7170.field_0x118 = fn_710000cf00;
    g_unk_71727c7170.field_0x120 = 0;
    g_unk_71727c7170.field_0x128 = 0;
    g_unk_71727c7170.field_0x130 = 0;
    g_unk_71727c7170.field_0x138 = 0;
    g_unk_71727c7170.field_0x140 = 0;
    g_unk_71727c7170.field_0x148 = fn_710000d470;
    g_unk_71727c7170.field_0x150 = 0;
    g_unk_71727c7170.field_0x158 = 0;
    g_unk_71727c7170.field_0x160 = 0;
    g_unk_71727c7170.field_0x168 = 0;
    g_unk_71727c7170.field_0x170 = 0;
    g_unk_71727c7170.field_0x178 = fn_710000d4f4;
    g_unk_71727c7170.field_0x180 = 0;
    g_unk_71727c7170.field_0x188 = 0;
    {
        CStrId n("SGeoData", true);
        fn_7100e2ffd8(&g_unk_71727c15f0, n, nullptr, nullptr, fn_7100e2fd9c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c15f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c1728, &g_unk_71727c15f0);
    {
        CStrId n("CMinimapData::SEntityData", true);
        fn_7100e303b8(&g_unk_71727c1730, n, nullptr, nullptr, fn_7100e3015c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c1730, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c1868, &g_unk_71727c1730);
    {
        CStrId n("CMinimapData::SDoorData", true);
        fn_7100e30920(&g_unk_71727c1870, n, nullptr, nullptr, fn_7100e3053c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c1870, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c19a8, &g_unk_71727c1870);
    {
        CStrId n("CMinimapData::SGeneratorData", true);
        fn_7100e30bb0(&g_unk_71727c19b0, n, nullptr, nullptr, fn_7100e30aa4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c19b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c1ae8, &g_unk_71727c19b0);
    {
        CStrId n("CMinimapData::SMagnetSurfaceData", true);
        fn_7100e30f00(&g_unk_71727c1af0, n, nullptr, nullptr, fn_7100e30d34, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c1af0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c1c28, &g_unk_71727c1af0);
    {
        CStrId n("CMinimapData::SMovingMagnetSurfaceData", true);
        fn_7100e315c4(&g_unk_71727c1c30, n, nullptr, nullptr, fn_7100e31084, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c1c30, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c1d68, &g_unk_71727c1c30);
    {
        CStrId n("CMinimapData::STransportSignData", true);
        fn_7100e31a94(&g_unk_71727c1d70, n, nullptr, nullptr, fn_7100e31748, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c1d70, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c1ea8, &g_unk_71727c1d70);
    fn_7100e31c70();
    fn_710009ec00(g_unk_71727c1ea9, g_unk_71727c7170.field_0x190);
    fn_710009ec00(g_unk_71727c1eaa, fn_7100e32bec());
    fn_710009ec00(g_unk_71727c1eab, fn_7100e32cc8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SGeoData>>::TKeyElementIterator::value_type", true);
        fn_7100e33128(&g_unk_71727c1eb0, n, g_unk_7101d5e2e0, nullptr, fn_7100e32da4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c1eb0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c1fe8, &g_unk_71727c1eb0);
    fn_710009ec00(g_unk_71727c1fe9, fn_7100e332ac());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SGeoData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e3370c(&g_unk_71727c1ff0, n, g_unk_7101d5e2e0, nullptr, fn_7100e33388, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c1ff0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2128, &g_unk_71727c1ff0);
    fn_710009ec00(g_unk_71727c2129, fn_7100e33890());
    {
        CStrId n("base::global::CRntVector<SGeoData>", true);
        fn_7100e3396c(&g_unk_71727c2130, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c2130, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c22a0, &g_unk_71727c2130);
    fn_710009ec00(g_unk_71727c22a1, fn_7100e33b60());
    fn_710009ec00(g_unk_71727c22a2, fn_7100e33c3c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<uint64, SGeoData>>::TKeyElementIterator::value_type", true);
        fn_7100e3409c(&g_unk_71727c22a8, n, g_unk_7101d5e2e0, nullptr, fn_7100e33d18, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c22a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c23e0, &g_unk_71727c22a8);
    fn_710009ec00(g_unk_71727c23e1, fn_7100e34220());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<uint64, SGeoData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e34680(&g_unk_71727c23e8, n, g_unk_7101d5e2e0, nullptr, fn_7100e342fc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c23e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2520, &g_unk_71727c23e8);
    fn_710009ec00(g_unk_71727c2521, fn_7100e34804());
    {
        CStrId n("base::global::CRntSmallDictionary<uint64, SGeoData>", true);
        fn_7100e348e0(&g_unk_71727c2528, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c2528, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2698, &g_unk_71727c2528);
    fn_710009ec00(g_unk_71727c2699, fn_7100e34ad8());
    fn_710009ec00(g_unk_71727c269a, fn_7100e34bb4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, SGeoData>>::TKeyElementIterator::value_type", true);
        fn_7100e35014(&g_unk_71727c26a0, n, g_unk_7101d5e2e0, nullptr, fn_7100e34c90, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c26a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c27d8, &g_unk_71727c26a0);
    fn_710009ec00(g_unk_71727c27d9, fn_7100e35198());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, SGeoData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e355f8(&g_unk_71727c27e0, n, g_unk_7101d5e2e0, nullptr, fn_7100e35274, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c27e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2918, &g_unk_71727c27e0);
    fn_710009ec00(g_unk_71727c2919, fn_7100e3577c());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, SGeoData>", true);
        fn_7100e35858(&g_unk_71727c2920, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c2920, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2a90, &g_unk_71727c2920);
    fn_710009ec00(g_unk_71727c2a91, fn_7100e35a50());
    fn_710009ec00(g_unk_71727c2a92, fn_7100e35b2c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::TColliderGeoDatasMap>>::TKeyElementIterator::value_type", true);
        fn_7100e35f8c(&g_unk_71727c2a98, n, g_unk_7101d5e2e0, nullptr, fn_7100e35c08, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c2a98, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2bd0, &g_unk_71727c2a98);
    fn_710009ec00(g_unk_71727c2bd1, fn_7100e36110());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::TColliderGeoDatasMap>>::TKeyElementConstIterator::value_type", true);
        fn_7100e36570(&g_unk_71727c2bd8, n, g_unk_7101d5e2e0, nullptr, fn_7100e361ec, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c2bd8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2d10, &g_unk_71727c2bd8);
    fn_710009ec00(g_unk_71727c2d11, fn_7100e366f4());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::TColliderGeoDatasMap>", true);
        fn_7100e367d0(&g_unk_71727c2d18, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c2d18, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2e88, &g_unk_71727c2d18);
    fn_710009ec00(g_unk_71727c2e89, fn_7100e369c8());
    fn_710009ec00(g_unk_71727c2e8a, fn_7100e36aa4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SDoorData>>::TKeyElementIterator::value_type", true);
        fn_7100e36f04(&g_unk_71727c2e90, n, g_unk_7101d5e2e0, nullptr, fn_7100e36b80, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c2e90, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c2fc8, &g_unk_71727c2e90);
    fn_710009ec00(g_unk_71727c2fc9, fn_7100e37088());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SDoorData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e374e8(&g_unk_71727c2fd0, n, g_unk_7101d5e2e0, nullptr, fn_7100e37164, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c2fd0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3108, &g_unk_71727c2fd0);
    fn_710009ec00(g_unk_71727c3109, fn_7100e3766c());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SDoorData>", true);
        fn_7100e37748(&g_unk_71727c3110, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c3110, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3280, &g_unk_71727c3110);
    fn_710009ec00(g_unk_71727c3281, fn_7100e37940());
    fn_710009ec00(g_unk_71727c3282, fn_7100e37a1c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SEntityData>>::TKeyElementIterator::value_type", true);
        fn_7100e37e7c(&g_unk_71727c3288, n, g_unk_7101d5e2e0, nullptr, fn_7100e37af8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c3288, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c33c0, &g_unk_71727c3288);
    fn_710009ec00(g_unk_71727c33c1, fn_7100e38000());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SEntityData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e38460(&g_unk_71727c33c8, n, g_unk_7101d5e2e0, nullptr, fn_7100e380dc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c33c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3500, &g_unk_71727c33c8);
    fn_710009ec00(g_unk_71727c3501, fn_7100e385e4());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SEntityData>", true);
        fn_7100e386c0(&g_unk_71727c3508, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c3508, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3678, &g_unk_71727c3508);
    fn_710009ec00(g_unk_71727c3679, fn_7100e388b8());
    fn_710009ec00(g_unk_71727c367a, fn_7100e38994());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::TOccludedIcons>>::TKeyElementIterator::value_type", true);
        fn_7100e38df4(&g_unk_71727c3680, n, g_unk_7101d5e2e0, nullptr, fn_7100e38a70, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c3680, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c37b8, &g_unk_71727c3680);
    fn_710009ec00(g_unk_71727c37b9, fn_7100e38f78());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::TOccludedIcons>>::TKeyElementConstIterator::value_type", true);
        fn_7100e393d8(&g_unk_71727c37c0, n, g_unk_7101d5e2e0, nullptr, fn_7100e39054, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c37c0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c38f8, &g_unk_71727c37c0);
    fn_710009ec00(g_unk_71727c38f9, fn_7100e3955c());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::TOccludedIcons>", true);
        fn_7100e39638(&g_unk_71727c3900, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c3900, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3a70, &g_unk_71727c3900);
    fn_710009ec00(g_unk_71727c3a71, fn_7100e39830());
    fn_710009ec00(g_unk_71727c3a72, fn_7100e3990c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SMagnetSurfaceData>>::TKeyElementIterator::value_type", true);
        fn_7100e39d6c(&g_unk_71727c3a78, n, g_unk_7101d5e2e0, nullptr, fn_7100e399e8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c3a78, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3bb0, &g_unk_71727c3a78);
    fn_710009ec00(g_unk_71727c3bb1, fn_7100e39ef0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SMagnetSurfaceData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e3a350(&g_unk_71727c3bb8, n, g_unk_7101d5e2e0, nullptr, fn_7100e39fcc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c3bb8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3cf0, &g_unk_71727c3bb8);
    fn_710009ec00(g_unk_71727c3cf1, fn_7100e3a4d4());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::SMagnetSurfaceData>", true);
        fn_7100e3a5b0(&g_unk_71727c3cf8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c3cf8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3e68, &g_unk_71727c3cf8);
    fn_710009ec00(g_unk_71727c3e69, fn_7100e3a7a8());
    fn_710009ec00(g_unk_71727c3e6a, fn_7100e3a884());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::STransportSignData>>::TKeyElementIterator::value_type", true);
        fn_7100e3ace4(&g_unk_71727c3e70, n, g_unk_7101d5e2e0, nullptr, fn_7100e3a960, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c3e70, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c3fa8, &g_unk_71727c3e70);
    fn_710009ec00(g_unk_71727c3fa9, fn_7100e3ae68());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::STransportSignData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e3b2c8(&g_unk_71727c3fb0, n, g_unk_7101d5e2e0, nullptr, fn_7100e3af44, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c3fb0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c40e8, &g_unk_71727c3fb0);
    fn_710009ec00(g_unk_71727c40e9, fn_7100e3b44c());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, CMinimapData::STransportSignData>", true);
        fn_7100e3b528(&g_unk_71727c40f0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c40f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c4260, &g_unk_71727c40f0);
    fn_710009ec00(g_unk_71727c4261, fn_7100e3b720());
    fn_710009ec00(g_unk_71727c4262, fn_7100e3b7fc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CMinimapData::SMovingMagnetSurfaceData>>::TKeyElementIterator::value_type", true);
        fn_7100e3bc5c(&g_unk_71727c4268, n, g_unk_7101d5e2e0, nullptr, fn_7100e3b8d8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c4268, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c43a0, &g_unk_71727c4268);
    fn_710009ec00(g_unk_71727c43a1, fn_7100e3bde0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CMinimapData::SMovingMagnetSurfaceData>>::TKeyElementConstIterator::value_type", true);
        fn_7100e3c240(&g_unk_71727c43a8, n, g_unk_7101d5e2e0, nullptr, fn_7100e3bebc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727c43a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c44e0, &g_unk_71727c43a8);
    fn_710009ec00(g_unk_71727c44e1, fn_7100e3c3c4());
    {
        CStrId n("base::global::CRntVector<CMinimapData::SMovingMagnetSurfaceData>", true);
        fn_7100e3c4a0(&g_unk_71727c44e8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727c44e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727c4658, &g_unk_71727c44e8);
    InitId(&g_unk_71727c7170.field_0x28, "doorcentralunit");
    InitId(&g_unk_71727c7170.field_0x30, "doorchargecharge");
    InitId(&g_unk_71727c7170.field_0x38, "doorchargeclosed");
    InitId(&g_unk_71727c7170.field_0x40, "doorclosedcharge");
    InitId(&g_unk_71727c7170.field_0x48, "doorpowerpower");
    InitId(&g_unk_71727c7170.field_0x50, "doorclosedpower");
    InitId(&g_unk_71727c7170.field_0x58, "doorpowerclosed");
    InitId(&g_unk_71727c7170.field_0x60, "doorpresencepresence");
    InitId(&g_unk_71727c7170.field_0x68, "doorpresenceframe");
    InitId(&g_unk_71727c7170.field_0x70, "doorframepresence");
    InitId(&g_unk_71727c7170.field_0x78, "doorgrapplegrapple");
    InitId(&g_unk_71727c7170.field_0x80, "doorgrappleclosed");
    InitId(&g_unk_71727c7170.field_0x88, "doorclosedgrapple");
    InitId(&g_unk_71727c7170.field_0x90, "doorframe");
    InitId(&g_unk_71727c7170.field_0x98, "dooremmy");
    InitId(&g_unk_71727c7170.field_0xa0, "doorheat");
    InitId(&g_unk_71727c7170.field_0xa8, "doorshutter");
    InitId(&g_unk_71727c7170.field_0xb0, "trap_thermal_horizontal");
    InitId(&g_unk_71727c7170.field_0xb8, "LM_Samus_Ship");
    InitId(&g_unk_71727c4660, "SG_Scorpius");
    InitId(&g_unk_71727c4668, "SG_Scorpius_ItemCube");
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".mmap";
        fn_7100004578(&types[0], "CMinimapData", ".bmmap", sources0, 1);
        fn_7100004698(g_unk_71727c4670, "minimap", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71727c4670, g_unk_7101a19000);
}
