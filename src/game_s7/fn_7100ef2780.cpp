// Static initialiser of the unit of CSubAreaManager::ETransitionType, CSubAreaManager: the header
// ids, then the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100ef2780 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 pad_0x38[0x10];
    UnkStrIdBase field_0x48;
    u8 pad_0x50[0x18];
    f32 field_0x68;
    f32 field_0x6c;
    bool field_0x70;
    u8 pad_0x71[0x7];
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u32 field_0x90;
    u8 pad_0x94[0x2c];
    u64 field_0xc0;
    u64 field_0xc8;
    u64 field_0xd0;
    void (*field_0xd8)();
    u64 field_0xe0;
    u64 field_0xe8;
    u64 field_0xf0;
    u64 field_0xf8;
    u64 field_0x100;
    void (*field_0x108)();
    u64 field_0x110;
    u64 field_0x118;
    u64 field_0x120;
    u64 field_0x128;
    u64 field_0x130;
    void (*field_0x138)();
    u64 field_0x140;
    u64 field_0x148;
    u8 field_0x150[8];
};
extern UnkStatics_7100ef2780 g_unk_71729daec0 __attribute__((visibility("hidden")));
void fn_710000cf00();
void fn_710000d470();
void fn_710000d4f4();
extern UnkConversion g_unk_71729da830;
extern UnkConversion g_unk_71729da850;
void fn_7100ef0060() __attribute__((visibility("hidden")));
void fn_7100ef006c() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71729da710;
extern char g_unk_71729da828[];
void fn_7100edf860(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100edf654();
extern char g_unk_71729da870[];
extern UnkTunable g_unk_71729da878;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101caa168[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71729da8b0;
void fn_7100ee0df0(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
void fn_7100ee021c();
extern char g_unk_71729da9e8[];
extern char g_unk_71729da9e9[];
UnkEnumType* fn_7100ad61f8();
extern UnkStrIdBase g_unk_71729da9f0[9];
void fn_7100ee1548(void* self);  // the array's destructor, as registered
extern UnkStrIdBase g_unk_71729daa38[9];

void fn_7100ef2780() {
    InitHeaderIds(&g_unk_71729daec0.ids);
    g_unk_71729daec0.field_0x68 = -1.0f;
    g_unk_71729daec0.field_0x6c = -1.0f;
    g_unk_71729daec0.field_0x70 = true;
    const u64 hash0 = fn_7100001570("s010_cave", 9);
    const u64 hash1 = fn_7100001570("collision_camera_000_Init", 25);
    const u64 hash2 = fn_7100001570("PostIntro", 9);
    g_unk_71729daec0.field_0x78 = hash0;
    g_unk_71729daec0.field_0x80 = hash1;
    g_unk_71729daec0.field_0x88 = hash2;
    g_unk_71729daec0.field_0x90 = 2;
    g_unk_71729daec0.field_0xc0 = 0;
    g_unk_71729daec0.field_0xc8 = 0;
    g_unk_71729daec0.field_0xd0 = 0;
    g_unk_71729daec0.field_0xd8 = fn_710000cf00;
    g_unk_71729daec0.field_0xe0 = 0;
    g_unk_71729daec0.field_0xe8 = 0;
    g_unk_71729daec0.field_0xf0 = 0;
    g_unk_71729daec0.field_0xf8 = 0;
    g_unk_71729daec0.field_0x100 = 0;
    g_unk_71729daec0.field_0x108 = fn_710000d470;
    g_unk_71729daec0.field_0x110 = 0;
    g_unk_71729daec0.field_0x118 = 0;
    g_unk_71729daec0.field_0x120 = 0;
    g_unk_71729daec0.field_0x128 = 0;
    g_unk_71729daec0.field_0x130 = 0;
    g_unk_71729daec0.field_0x138 = fn_710000d4f4;
    g_unk_71729daec0.field_0x140 = 0;
    g_unk_71729daec0.field_0x148 = 0;
    InitEnum(&g_unk_71729da710, "CSubAreaManager::ETransitionType", fn_7100edf860, fn_7100edf654, g_unk_71729da828, &g_unk_71729da830, fn_7100ef0060, &g_unk_71729da850, fn_7100ef006c);
    fn_7100edf9e4();
    fn_710009ec00(g_unk_71729da870, g_unk_71729daec0.field_0x150);
    InitTunable(&g_unk_71729da878, fn_7100136ecc, g_unk_7101caa168, fn_7100075104);
    {
        CStrId n("CSubAreaManager", true);
        fn_7100ee0df0(&g_unk_71729da8b0, n, nullptr, nullptr, fn_7100ee021c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729da8b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729da9e8, &g_unk_71729da8b0);
    fn_710009ec00(g_unk_71729da9e9, fn_7100ad61f8());
    InitId(&g_unk_71729daec0.field_0x20, "SubAreaManager");
    InitId(&g_unk_71729daec0.field_0x28, "SubAreaManager");
    InitId(&g_unk_71729daec0.field_0x30, "brsa");
    MakeEmptyId(&g_unk_71729da9f0[0]);
    MakeEmptyId(&g_unk_71729da9f0[1]);
    MakeEmptyId(&g_unk_71729da9f0[2]);
    MakeEmptyId(&g_unk_71729da9f0[3]);
    MakeEmptyId(&g_unk_71729da9f0[4]);
    MakeEmptyId(&g_unk_71729da9f0[5]);
    MakeEmptyId(&g_unk_71729da9f0[6]);
    MakeEmptyId(&g_unk_71729da9f0[7]);
    MakeEmptyId(&g_unk_71729da9f0[8]);
    __cxa_atexit(fn_7100ee1548, g_unk_71729da9f0, g_unk_7101a19000);
    MakeEmptyId(&g_unk_71729daa38[0]);
    MakeEmptyId(&g_unk_71729daa38[1]);
    MakeEmptyId(&g_unk_71729daa38[2]);
    MakeEmptyId(&g_unk_71729daa38[3]);
    MakeEmptyId(&g_unk_71729daa38[4]);
    MakeEmptyId(&g_unk_71729daa38[5]);
    MakeEmptyId(&g_unk_71729daa38[6]);
    MakeEmptyId(&g_unk_71729daa38[7]);
    MakeEmptyId(&g_unk_71729daa38[8]);
    __cxa_atexit(fn_7100ee1548, g_unk_71729daa38, g_unk_7101a19000);
    InitId(&g_unk_71729daec0.field_0x48, "default");
}
