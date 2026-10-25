// Static initialiser of an enum-type registration: builds the enum info object for
// "base::serialization::EPersistence", registers its destructor with atexit and adds it to the registry, then
// fills two handler records (value pairs taken from the enum info and the first enum's info)
// and registers both.
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

extern UnkEnumInfo g_unk_7101d679f0;
extern u8 g_unk_7101d67b08[];
extern UnkEnumPair g_unk_7101d67b10;
extern UnkEnumPair g_unk_7101d67b30;

void fn_7100099468(void*);
void fn_71000ecda0();
void fn_71000ed06c() HIDDEN;
void fn_71000ed078() HIDDEN;
void fn_71000eceec(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_71000a16c4(UnkEnumPair* p);

void fn_71000ed100() {
    {
        CStrId name("base::serialization::EPersistence", true);
        fn_71000eceec(&g_unk_7101d679f0, &name, fn_71000ecda0, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d679f0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d67b08, &g_unk_7101d679f0);
    u64 a = g_unk_7101d679f0.desc_0x8->field_0x30;
    u64 b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    g_unk_7101d67b10.first = a;
    g_unk_7101d67b10.second = b;
    g_unk_7101d67b10.fn = fn_71000ed06c;
    g_unk_7101d67b10.zero = 0;
    fn_71000a16c4(&g_unk_7101d67b10);
    u64 c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    u64 d = g_unk_7101d679f0.desc_0x8->field_0x30;
    g_unk_7101d67b30.first = c;
    g_unk_7101d67b30.second = d;
    g_unk_7101d67b30.fn = fn_71000ed078;
    g_unk_7101d67b30.zero = 0;
    fn_71000a16c4(&g_unk_7101d67b30);
}
