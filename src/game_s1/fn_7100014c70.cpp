// Static initialiser of an enum-type registration: builds the enum info object for
// "base::animation::EBlendFunction", registers its destructor with atexit and adds it to the registry, then
// fills two handler records (value pairs taken from the enum info and the first enum's info)
// and registers both.
#include "base/global/CStrId.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

struct UnkEnumDesc {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};
struct UnkEnumInfo {
    u8 pad_0x0[8];
    UnkEnumDesc* desc_0x8;
};
struct UnkEnumPair {
    u64 first, second;
    void (*fn)();
    u64 zero;
};

extern UnkEnumInfo g_unk_7101d4d5d0;
extern u8 g_unk_7101d4d6e8[];
extern UnkEnumPair g_unk_7101d4d6f0;
extern UnkEnumPair g_unk_7101d4d710;
extern u8 g_unk_7101a19000 HIDDEN;  // __dso_handle

void fn_7100099468(void*);
void fn_710001449c();
void fn_7100014be4() HIDDEN;
void fn_7100014bf0() HIDDEN;
UnkEnumInfo* fn_7100014768();
void fn_71000145e8(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710009ec00(void* registry, void* obj);
void fn_71000a16c4(UnkEnumPair* p);
int fn_7100000250(void (*)(void*), void*, void*);

void fn_7100014c70() {
    {
        CStrId name("base::animation::EBlendFunction", true);
        fn_71000145e8(&g_unk_7101d4d5d0, &name, fn_710001449c, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d4d5d0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d4d6e8, &g_unk_7101d4d5d0);
    u64 a = g_unk_7101d4d5d0.desc_0x8->field_0x30;
    u64 b = fn_7100014768()->desc_0x8->field_0x30;
    g_unk_7101d4d6f0.first = a;
    g_unk_7101d4d6f0.second = b;
    g_unk_7101d4d6f0.fn = fn_7100014be4;
    g_unk_7101d4d6f0.zero = 0;
    fn_71000a16c4(&g_unk_7101d4d6f0);
    u64 c = fn_7100014768()->desc_0x8->field_0x30;
    u64 d = g_unk_7101d4d5d0.desc_0x8->field_0x30;
    g_unk_7101d4d710.first = c;
    g_unk_7101d4d710.second = d;
    g_unk_7101d4d710.fn = fn_7100014bf0;
    g_unk_7101d4d710.zero = 0;
    fn_71000a16c4(&g_unk_7101d4d710);
}
