// Static initialiser of the unit of animtree::CAnimTreeRes: the header ids, three function-holding
// statics, the class registration and a resource type registered under "animtree".
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkFnObj {
    u64 field_0x0[3];
    void (*field_0x18)();
    u64 field_0x20[2];
};
void fn_710000cf00();
void fn_710000d470();
void fn_710000d4f4();

struct UnkStatics_71002e0370 {
    HeaderIds ids;
    u8 pad_0x18[8];
    UnkFnObj field_0x20;
    UnkFnObj field_0x50;
    UnkFnObj field_0x80;
    u8 field_0xb0[8];
};
extern UnkStatics_71002e0370 g_unk_71726aadc8 __attribute__((visibility("hidden")));
void* fn_71002dfda8();  // animtree::CAnimTreeRes::refl_GetClass
extern char g_unk_71726aad40[];
extern char g_unk_71726aad48[];

void fn_71002e0370() {
    InitHeaderIds(&g_unk_71726aadc8.ids);
    g_unk_71726aadc8.field_0x20 = {{0, 0, 0}, fn_710000cf00, {0, 0}};
    g_unk_71726aadc8.field_0x50 = {{0, 0, 0}, fn_710000d470, {0, 0}};
    g_unk_71726aadc8.field_0x80 = {{0, 0, 0}, fn_710000d4f4, {0, 0}};
    fn_71002dfda8();
    fn_710009ec00(g_unk_71726aad40, g_unk_71726aadc8.field_0xb0);
    [] {
        UnkResType types[1];
        const char* sources0[1];
        sources0[0] = ".msat";
        fn_7100004578(&types[0], "animtree::CAnimTreeRes", ".bmsat", sources0, 1);
        fn_7100004698(g_unk_71726aad48, "animtree", types, 1);
        fn_7100004a44(&types[0]);
    }();
    __cxa_atexit(fn_7100004b94, g_unk_71726aad48, g_unk_7101a19000);
}
