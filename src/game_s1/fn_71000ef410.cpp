// Static initialiser of an enum-type registration: builds the enum info object for
// "base::serialization::EDeserializeCollectionElementMode", registers its destructor with atexit and adds it to the registry, then
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

extern UnkEnumInfo g_unk_7101d67c10;
extern u8 g_unk_7101d67d28[];
extern UnkEnumPair g_unk_7101d67d30;
extern UnkEnumPair g_unk_7101d67d50;
extern u8 g_unk_7101a19000 HIDDEN;  // __dso_handle

void fn_7100099468(void*);
void fn_71000ef058();
void fn_71000ef384() HIDDEN;
void fn_71000ef390() HIDDEN;
UnkEnumInfo* fn_7100014768();
void fn_71000ef204(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710009ec00(void* registry, void* obj);
void fn_71000a16c4(UnkEnumPair* p);
int fn_7100000250(void (*)(void*), void*, void*);

void fn_71000ef410() {
    {
        CStrId name("base::serialization::EDeserializeCollectionElementMode", true);
        fn_71000ef204(&g_unk_7101d67c10, &name, fn_71000ef058, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d67c10, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d67d28, &g_unk_7101d67c10);
    u64 a = g_unk_7101d67c10.desc_0x8->field_0x30;
    u64 b = fn_7100014768()->desc_0x8->field_0x30;
    g_unk_7101d67d30.first = a;
    g_unk_7101d67d30.second = b;
    g_unk_7101d67d30.fn = fn_71000ef384;
    g_unk_7101d67d30.zero = 0;
    fn_71000a16c4(&g_unk_7101d67d30);
    u64 c = fn_7100014768()->desc_0x8->field_0x30;
    u64 d = g_unk_7101d67c10.desc_0x8->field_0x30;
    g_unk_7101d67d50.first = c;
    g_unk_7101d67d50.second = d;
    g_unk_7101d67d50.fn = fn_71000ef390;
    g_unk_7101d67d50.zero = 0;
    fn_71000a16c4(&g_unk_7101d67d50);
}
