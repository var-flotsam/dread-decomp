// Static initialiser of an enum-type registration: builds the enum info object for
// "base::serialization::EDeserializeCollectionElementMode", registers its destructor with atexit and adds it to the registry, then
// fills two handler records (value pairs taken from the enum info and the first enum's info)
// and registers both.
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

extern UnkEnumInfo g_unk_7101d67c10;
extern u8 g_unk_7101d67d28[];
extern UnkConverter g_unk_7101d67d30;
extern UnkConverter g_unk_7101d67d50;

void fn_7100099468(void*);
void fn_71000ef058();
void fn_71000ef384() HIDDEN;
void fn_71000ef390() HIDDEN;
void fn_71000ef204(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);

void fn_71000ef410() {
    {
        CStrId name("base::serialization::EDeserializeCollectionElementMode", true);
        fn_71000ef204(&g_unk_7101d67c10, &name, fn_71000ef058, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d67c10, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d67d28, &g_unk_7101d67c10);
    void* a = g_unk_7101d67c10.desc_0x8->field_0x30;
    void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    g_unk_7101d67d30.from = a;
    g_unk_7101d67d30.to = b;
    g_unk_7101d67d30.convert = fn_71000ef384;
    g_unk_7101d67d30.next = nullptr;
    fn_71000a16c4(&g_unk_7101d67d30);
    void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    void* d = g_unk_7101d67c10.desc_0x8->field_0x30;
    g_unk_7101d67d50.from = c;
    g_unk_7101d67d50.to = d;
    g_unk_7101d67d50.convert = fn_71000ef390;
    g_unk_7101d67d50.next = nullptr;
    fn_71000a16c4(&g_unk_7101d67d50);
}
