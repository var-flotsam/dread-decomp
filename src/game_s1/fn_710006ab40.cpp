// Static initialiser registering 2 enum types: for each, builds the enum info object, registers
// its destructor with atexit, adds it to the registry, then fills two handler records and
// registers both. Enums: base::global::timeline::ELayer, base::global::timeline::TLayers.
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
extern UnkEnumInfo g_unk_7101d57f90;
extern UnkEnumInfo g_unk_7101d580f0;
extern UnkConverter g_unk_7101d580b0;
extern UnkConverter g_unk_7101d580d0;
extern UnkConverter g_unk_7101d581e8;
extern UnkConverter g_unk_7101d58208;
extern u8 g_unk_7101d580a8[];
extern u8 g_unk_7101d581e0[];
void fn_710006a428();
void fn_710006a754(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710006a8d4(UnkEnumInfo* obj, CStrId* name, void (*fn)());
void fn_710006aa24() HIDDEN;
void fn_710006aa30() HIDDEN;
void fn_710006aa3c() HIDDEN;
void fn_710006aa48() HIDDEN;
void fn_710006ab40() {
    {
        CStrId name("base::global::timeline::ELayer", true);
        fn_710006a754(&g_unk_7101d57f90, &name, fn_710006a428, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d57f90, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d580a8, &g_unk_7101d57f90);
    {
        void* a = g_unk_7101d57f90.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d580b0.from = a;
        g_unk_7101d580b0.to = b;
        g_unk_7101d580b0.convert = fn_710006aa24;
        g_unk_7101d580b0.next = nullptr;
        fn_71000a16c4(&g_unk_7101d580b0);
        void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        void* d = g_unk_7101d57f90.desc_0x8->field_0x30;
        g_unk_7101d580d0.from = c;
        g_unk_7101d580d0.to = d;
        g_unk_7101d580d0.convert = fn_710006aa30;
        g_unk_7101d580d0.next = nullptr;
        fn_71000a16c4(&g_unk_7101d580d0);
    }
    {
        CStrId name("base::global::timeline::TLayers", true);
        fn_710006a8d4(&g_unk_7101d580f0, &name, nullptr);
    }
    fn_7100000250(fn_710009a368, &g_unk_7101d580f0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d581e0, &g_unk_7101d580f0);
    {
        void* a = g_unk_7101d580f0.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100005178>()->desc_0x8->field_0x30;
        g_unk_7101d581e8.from = a;
        g_unk_7101d581e8.to = b;
        g_unk_7101d581e8.convert = fn_710006aa3c;
        g_unk_7101d581e8.next = nullptr;
        fn_71000a16c4(&g_unk_7101d581e8);
        void* c = TypeOf<fn_7100005178>()->desc_0x8->field_0x30;
        void* d = g_unk_7101d580f0.desc_0x8->field_0x30;
        g_unk_7101d58208.from = c;
        g_unk_7101d58208.to = d;
        g_unk_7101d58208.convert = fn_710006aa48;
        g_unk_7101d58208.next = nullptr;
        fn_71000a16c4(&g_unk_7101d58208);
    }
}
