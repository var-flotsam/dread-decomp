// Static initialiser registering 3 enum types: for each, builds the enum info object, registers
// its destructor with atexit, adds it to the registry, then fills two handler records and
// registers both. Enums: base::curves::EKeysType, base::curves::ETimeType, base::curves::EInterpMode.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "game/UnkTUInit.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

struct UnkEnumDesc {
    u8 pad_0x0[0x30];
    void* field_0x30;
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

void fn_71000145e8(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
extern UnkEnumInfo g_unk_7101d4ef10;
extern UnkEnumInfo g_unk_7101d4f070;
extern UnkEnumInfo g_unk_7101d4f1d0;
extern UnkConverter g_unk_7101d4f030;
extern UnkConverter g_unk_7101d4f050;
extern UnkConverter g_unk_7101d4f190;
extern UnkConverter g_unk_7101d4f1b0;
extern UnkConverter g_unk_7101d4f2f0;
extern UnkConverter g_unk_7101d4f310;
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
void fn_71000296e0() {
    {
        CStrId name("base::curves::EKeysType", true);
        fn_7100028d58(&g_unk_7101d4ef10, &name, fn_7100028aec, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d4ef10, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d4f028, &g_unk_7101d4ef10);
    {
        void* a = g_unk_7101d4ef10.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d4f030.from = a;
        g_unk_7101d4f030.to = b;
        g_unk_7101d4f030.convert = fn_7100029530;
        g_unk_7101d4f030.next = nullptr;
        fn_71000a16c4(&g_unk_7101d4f030);
        void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        void* d = g_unk_7101d4ef10.desc_0x8->field_0x30;
        g_unk_7101d4f050.from = c;
        g_unk_7101d4f050.to = d;
        g_unk_7101d4f050.convert = fn_710002953c;
        g_unk_7101d4f050.next = nullptr;
        fn_71000a16c4(&g_unk_7101d4f050);
    }
    {
        CStrId name("base::curves::ETimeType", true);
        fn_7100029084(&g_unk_7101d4f070, &name, fn_7100028ed8, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d4f070, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d4f188, &g_unk_7101d4f070);
    {
        void* a = g_unk_7101d4f070.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d4f190.from = a;
        g_unk_7101d4f190.to = b;
        g_unk_7101d4f190.convert = fn_7100029548;
        g_unk_7101d4f190.next = nullptr;
        fn_71000a16c4(&g_unk_7101d4f190);
        void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        void* d = g_unk_7101d4f070.desc_0x8->field_0x30;
        g_unk_7101d4f1b0.from = c;
        g_unk_7101d4f1b0.to = d;
        g_unk_7101d4f1b0.convert = fn_7100029554;
        g_unk_7101d4f1b0.next = nullptr;
        fn_71000a16c4(&g_unk_7101d4f1b0);
    }
    {
        CStrId name("base::curves::EInterpMode", true);
        fn_71000293b0(&g_unk_7101d4f1d0, &name, fn_7100029204, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d4f1d0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d4f2e8, &g_unk_7101d4f1d0);
    {
        void* a = g_unk_7101d4f1d0.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d4f2f0.from = a;
        g_unk_7101d4f2f0.to = b;
        g_unk_7101d4f2f0.convert = fn_7100029560;
        g_unk_7101d4f2f0.next = nullptr;
        fn_71000a16c4(&g_unk_7101d4f2f0);
        void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        void* d = g_unk_7101d4f1d0.desc_0x8->field_0x30;
        g_unk_7101d4f310.from = c;
        g_unk_7101d4f310.to = d;
        g_unk_7101d4f310.convert = fn_710002956c;
        g_unk_7101d4f310.next = nullptr;
        fn_71000a16c4(&g_unk_7101d4f310);
    }
}
