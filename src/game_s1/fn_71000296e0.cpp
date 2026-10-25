// Static initialiser registering 3 enum types: for each, builds the enum info object, registers
// its destructor with atexit, adds it to the registry, then fills two handler records and
// registers both. Enums: base::curves::EKeysType, base::curves::ETimeType, base::curves::EInterpMode.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"

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

// The reflected int and unsigned types (base/reflection/UnkReflAttributes.h), seen through the
// placeholder.
template <const base::reflection::CType* (*Getter)()>
static inline UnkEnumInfo* TypeOf() {
    return reinterpret_cast<UnkEnumInfo*>(const_cast<base::reflection::CType*>(Getter()));
}
struct UnkEnumPair {
    u64 first, second;
    void (*fn)();
    u64 zero;
};

void fn_71000a16c4(UnkEnumPair* p);
void fn_71000145e8(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
extern UnkEnumInfo g_unk_7101d4ef10;
extern UnkEnumInfo g_unk_7101d4f070;
extern UnkEnumInfo g_unk_7101d4f1d0;
extern UnkEnumPair g_unk_7101d4f030;
extern UnkEnumPair g_unk_7101d4f050;
extern UnkEnumPair g_unk_7101d4f190;
extern UnkEnumPair g_unk_7101d4f1b0;
extern UnkEnumPair g_unk_7101d4f2f0;
extern UnkEnumPair g_unk_7101d4f310;
extern u8 g_unk_7101d4f028[];
extern u8 g_unk_7101d4f188[];
extern u8 g_unk_7101d4f2e8[];
void fn_7100028aec();
void fn_7100028d58(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_7100028ed8();
void fn_7100029084(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_7100029204();
void fn_71000293b0(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_7100029530() HIDDEN;
void fn_710002953c() HIDDEN;
void fn_7100029548() HIDDEN;
void fn_7100029554() HIDDEN;
void fn_7100029560() HIDDEN;
void fn_710002956c() HIDDEN;
void fn_7100099468(void*);
void fn_71000296e0() {
    {
        CStrId name("base::curves::EKeysType", true);
        fn_7100028d58(&g_unk_7101d4ef10, &name, fn_7100028aec, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d4ef10, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d4f028, &g_unk_7101d4ef10);
    {
        u64 a = g_unk_7101d4ef10.desc_0x8->field_0x30;
        u64 b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d4f030.first = a;
        g_unk_7101d4f030.second = b;
        g_unk_7101d4f030.fn = fn_7100029530;
        g_unk_7101d4f030.zero = 0;
        fn_71000a16c4(&g_unk_7101d4f030);
        u64 c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        u64 d = g_unk_7101d4ef10.desc_0x8->field_0x30;
        g_unk_7101d4f050.first = c;
        g_unk_7101d4f050.second = d;
        g_unk_7101d4f050.fn = fn_710002953c;
        g_unk_7101d4f050.zero = 0;
        fn_71000a16c4(&g_unk_7101d4f050);
    }
    {
        CStrId name("base::curves::ETimeType", true);
        fn_7100029084(&g_unk_7101d4f070, &name, fn_7100028ed8, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d4f070, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d4f188, &g_unk_7101d4f070);
    {
        u64 a = g_unk_7101d4f070.desc_0x8->field_0x30;
        u64 b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d4f190.first = a;
        g_unk_7101d4f190.second = b;
        g_unk_7101d4f190.fn = fn_7100029548;
        g_unk_7101d4f190.zero = 0;
        fn_71000a16c4(&g_unk_7101d4f190);
        u64 c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        u64 d = g_unk_7101d4f070.desc_0x8->field_0x30;
        g_unk_7101d4f1b0.first = c;
        g_unk_7101d4f1b0.second = d;
        g_unk_7101d4f1b0.fn = fn_7100029554;
        g_unk_7101d4f1b0.zero = 0;
        fn_71000a16c4(&g_unk_7101d4f1b0);
    }
    {
        CStrId name("base::curves::EInterpMode", true);
        fn_71000293b0(&g_unk_7101d4f1d0, &name, fn_7100029204, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d4f1d0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d4f2e8, &g_unk_7101d4f1d0);
    {
        u64 a = g_unk_7101d4f1d0.desc_0x8->field_0x30;
        u64 b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d4f2f0.first = a;
        g_unk_7101d4f2f0.second = b;
        g_unk_7101d4f2f0.fn = fn_7100029560;
        g_unk_7101d4f2f0.zero = 0;
        fn_71000a16c4(&g_unk_7101d4f2f0);
        u64 c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        u64 d = g_unk_7101d4f1d0.desc_0x8->field_0x30;
        g_unk_7101d4f310.first = c;
        g_unk_7101d4f310.second = d;
        g_unk_7101d4f310.fn = fn_710002956c;
        g_unk_7101d4f310.zero = 0;
        fn_71000a16c4(&g_unk_7101d4f310);
    }
}
