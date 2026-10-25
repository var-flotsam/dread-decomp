// Static initialiser registering 2 enum types: for each, builds the enum info object, registers
// its destructor with atexit, adds it to the registry, then fills two handler records and
// registers both. Enums: base::global::timeline::ELayer, base::global::timeline::TLayers.
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
extern UnkEnumInfo g_unk_7101d57f90;
extern UnkEnumInfo g_unk_7101d580f0;
extern UnkEnumPair g_unk_7101d580b0;
extern UnkEnumPair g_unk_7101d580d0;
extern UnkEnumPair g_unk_7101d581e8;
extern UnkEnumPair g_unk_7101d58208;
extern u8 g_unk_7101d580a8[];
extern u8 g_unk_7101d581e0[];
void fn_710006a428();
void fn_710006a754(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710006a8d4(UnkEnumInfo* obj, CStrId* name, void (*fn)());
void fn_710006aa24() HIDDEN;
void fn_710006aa30() HIDDEN;
void fn_710006aa3c() HIDDEN;
void fn_710006aa48() HIDDEN;
void fn_7100099468(void*);
void fn_710009a368(void*);
void fn_710006ab40() {
    {
        CStrId name("base::global::timeline::ELayer", true);
        fn_710006a754(&g_unk_7101d57f90, &name, fn_710006a428, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d57f90, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d580a8, &g_unk_7101d57f90);
    {
        u64 a = g_unk_7101d57f90.desc_0x8->field_0x30;
        u64 b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d580b0.first = a;
        g_unk_7101d580b0.second = b;
        g_unk_7101d580b0.fn = fn_710006aa24;
        g_unk_7101d580b0.zero = 0;
        fn_71000a16c4(&g_unk_7101d580b0);
        u64 c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        u64 d = g_unk_7101d57f90.desc_0x8->field_0x30;
        g_unk_7101d580d0.first = c;
        g_unk_7101d580d0.second = d;
        g_unk_7101d580d0.fn = fn_710006aa30;
        g_unk_7101d580d0.zero = 0;
        fn_71000a16c4(&g_unk_7101d580d0);
    }
    {
        CStrId name("base::global::timeline::TLayers", true);
        fn_710006a8d4(&g_unk_7101d580f0, &name, nullptr);
    }
    fn_7100000250(fn_710009a368, &g_unk_7101d580f0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d581e0, &g_unk_7101d580f0);
    {
        u64 a = g_unk_7101d580f0.desc_0x8->field_0x30;
        u64 b = TypeOf<fn_7100005178>()->desc_0x8->field_0x30;
        g_unk_7101d581e8.first = a;
        g_unk_7101d581e8.second = b;
        g_unk_7101d581e8.fn = fn_710006aa3c;
        g_unk_7101d581e8.zero = 0;
        fn_71000a16c4(&g_unk_7101d581e8);
        u64 c = TypeOf<fn_7100005178>()->desc_0x8->field_0x30;
        u64 d = g_unk_7101d580f0.desc_0x8->field_0x30;
        g_unk_7101d58208.first = c;
        g_unk_7101d58208.second = d;
        g_unk_7101d58208.fn = fn_710006aa48;
        g_unk_7101d58208.zero = 0;
        fn_71000a16c4(&g_unk_7101d58208);
    }
}
