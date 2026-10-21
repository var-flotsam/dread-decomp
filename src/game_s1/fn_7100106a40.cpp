// Static initialiser of an enum-type registration: builds the enum info object for
// "base::snd::ESoundGroupIds", registers its destructor with atexit and adds it to the registry, then
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

extern UnkEnumInfo g_unk_7108b6c488;
extern u8 g_unk_7108b6c5a0[];
extern UnkEnumPair g_unk_7108b6c5a8;
extern UnkEnumPair g_unk_7108b6c5c8;
extern u8 g_unk_7101a19000 HIDDEN;  // __dso_handle

void fn_7100099468(void*);
void fn_71001061f4();
void fn_71001069b4() HIDDEN;
void fn_71001069c0() HIDDEN;
UnkEnumInfo* fn_7100014768();
void fn_71001066a0(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710009ec00(void* registry, void* obj);
void fn_71000a16c4(UnkEnumPair* p);
int fn_7100000250(void (*)(void*), void*, void*);

void fn_7100106a40() {
    {
        CStrId name("base::snd::ESoundGroupIds", true);
        fn_71001066a0(&g_unk_7108b6c488, &name, fn_71001061f4, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7108b6c488, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6c5a0, &g_unk_7108b6c488);
    u64 a = g_unk_7108b6c488.desc_0x8->field_0x30;
    u64 b = fn_7100014768()->desc_0x8->field_0x30;
    g_unk_7108b6c5a8.first = a;
    g_unk_7108b6c5a8.second = b;
    g_unk_7108b6c5a8.fn = fn_71001069b4;
    g_unk_7108b6c5a8.zero = 0;
    fn_71000a16c4(&g_unk_7108b6c5a8);
    u64 c = fn_7100014768()->desc_0x8->field_0x30;
    u64 d = g_unk_7108b6c488.desc_0x8->field_0x30;
    g_unk_7108b6c5c8.first = c;
    g_unk_7108b6c5c8.second = d;
    g_unk_7108b6c5c8.fn = fn_71001069c0;
    g_unk_7108b6c5c8.zero = 0;
    fn_71000a16c4(&g_unk_7108b6c5c8);
}
