// Static initialiser registering 2 enum types: for each, builds the enum info object, registers
// its destructor with atexit, adds it to the registry, then fills two handler records and
// registers both. Enums: base::math::ETransformSpace, base::math::EMaxAxis.
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

extern u8 g_unk_7101a19000 HIDDEN;  // __dso_handle
void fn_710009ec00(void* registry, void* obj);
void fn_71000a16c4(UnkEnumPair* p);
int fn_7100000250(void (*)(void*), void*, void*);
void fn_71000145e8(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
UnkEnumInfo* fn_7100014768();
extern UnkEnumInfo g_unk_7101d58900;
extern UnkEnumInfo g_unk_7101d58a60;
extern UnkEnumPair g_unk_7101d58a20;
extern UnkEnumPair g_unk_7101d58a40;
extern UnkEnumPair g_unk_7101d58b80;
extern UnkEnumPair g_unk_7101d58ba0;
extern u8 g_unk_7101d58a18[];
extern u8 g_unk_7101d58b78[];
void fn_710007a91c();
void fn_710007aac8(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710007ac48();
void fn_710007af74(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710007b0f4() HIDDEN;
void fn_710007b100() HIDDEN;
void fn_710007b10c() HIDDEN;
void fn_710007b118() HIDDEN;
void fn_7100099468(void*);
void fn_710007b210() {
    {
        CStrId name("base::math::ETransformSpace", true);
        fn_710007aac8(&g_unk_7101d58900, &name, fn_710007a91c, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d58900, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d58a18, &g_unk_7101d58900);
    {
        u64 a = g_unk_7101d58900.desc_0x8->field_0x30;
        u64 b = fn_7100014768()->desc_0x8->field_0x30;
        g_unk_7101d58a20.first = a;
        g_unk_7101d58a20.second = b;
        g_unk_7101d58a20.fn = fn_710007b0f4;
        g_unk_7101d58a20.zero = 0;
        fn_71000a16c4(&g_unk_7101d58a20);
        u64 c = fn_7100014768()->desc_0x8->field_0x30;
        u64 d = g_unk_7101d58900.desc_0x8->field_0x30;
        g_unk_7101d58a40.first = c;
        g_unk_7101d58a40.second = d;
        g_unk_7101d58a40.fn = fn_710007b100;
        g_unk_7101d58a40.zero = 0;
        fn_71000a16c4(&g_unk_7101d58a40);
    }
    {
        CStrId name("base::math::EMaxAxis", true);
        fn_710007af74(&g_unk_7101d58a60, &name, fn_710007ac48, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d58a60, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d58b78, &g_unk_7101d58a60);
    {
        u64 a = g_unk_7101d58a60.desc_0x8->field_0x30;
        u64 b = fn_7100014768()->desc_0x8->field_0x30;
        g_unk_7101d58b80.first = a;
        g_unk_7101d58b80.second = b;
        g_unk_7101d58b80.fn = fn_710007b10c;
        g_unk_7101d58b80.zero = 0;
        fn_71000a16c4(&g_unk_7101d58b80);
        u64 c = fn_7100014768()->desc_0x8->field_0x30;
        u64 d = g_unk_7101d58a60.desc_0x8->field_0x30;
        g_unk_7101d58ba0.first = c;
        g_unk_7101d58ba0.second = d;
        g_unk_7101d58ba0.fn = fn_710007b118;
        g_unk_7101d58ba0.zero = 0;
        fn_71000a16c4(&g_unk_7101d58ba0);
    }
}
