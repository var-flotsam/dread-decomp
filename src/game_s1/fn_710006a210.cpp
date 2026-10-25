// Static initialiser registering 2 enum types: for each, builds the enum info object, registers
// its destructor with atexit, adds it to the registry, then fills two handler records and
// registers both. Enums: EEventProps, TEventPropsFlags.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflConverter.h"

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
extern UnkEnumInfo g_unk_7101d57cf8;
extern UnkEnumInfo g_unk_7101d57e58;
extern UnkConverter g_unk_7101d57e18;
extern UnkConverter g_unk_7101d57e38;
extern UnkConverter g_unk_7101d57f50;
extern UnkConverter g_unk_7101d57f70;
extern u8 g_unk_7101d57e10[];
extern u8 g_unk_7101d57f48[];
void fn_7100069a34();
void fn_7100069e20(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_7100069fa0(UnkEnumInfo* obj, CStrId* name, void (*fn)());
void fn_710006a0f0() HIDDEN;
void fn_710006a0fc() HIDDEN;
void fn_710006a108() HIDDEN;
void fn_710006a114() HIDDEN;
void fn_7100099468(void*);
void fn_710009a368(void*);
void fn_710006a210() {
    {
        CStrId name("EEventProps", true);
        fn_7100069e20(&g_unk_7101d57cf8, &name, fn_7100069a34, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d57cf8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d57e10, &g_unk_7101d57cf8);
    {
        void* a = g_unk_7101d57cf8.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d57e18.from = a;
        g_unk_7101d57e18.to = b;
        g_unk_7101d57e18.convert = fn_710006a0f0;
        g_unk_7101d57e18.next = nullptr;
        fn_71000a16c4(&g_unk_7101d57e18);
        void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        void* d = g_unk_7101d57cf8.desc_0x8->field_0x30;
        g_unk_7101d57e38.from = c;
        g_unk_7101d57e38.to = d;
        g_unk_7101d57e38.convert = fn_710006a0fc;
        g_unk_7101d57e38.next = nullptr;
        fn_71000a16c4(&g_unk_7101d57e38);
    }
    {
        CStrId name("TEventPropsFlags", true);
        fn_7100069fa0(&g_unk_7101d57e58, &name, nullptr);
    }
    fn_7100000250(fn_710009a368, &g_unk_7101d57e58, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d57f48, &g_unk_7101d57e58);
    {
        void* a = g_unk_7101d57e58.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100005178>()->desc_0x8->field_0x30;
        g_unk_7101d57f50.from = a;
        g_unk_7101d57f50.to = b;
        g_unk_7101d57f50.convert = fn_710006a108;
        g_unk_7101d57f50.next = nullptr;
        fn_71000a16c4(&g_unk_7101d57f50);
        void* c = TypeOf<fn_7100005178>()->desc_0x8->field_0x30;
        void* d = g_unk_7101d57e58.desc_0x8->field_0x30;
        g_unk_7101d57f70.from = c;
        g_unk_7101d57f70.to = d;
        g_unk_7101d57f70.convert = fn_710006a114;
        g_unk_7101d57f70.next = nullptr;
        fn_71000a16c4(&g_unk_7101d57f70);
    }
}
