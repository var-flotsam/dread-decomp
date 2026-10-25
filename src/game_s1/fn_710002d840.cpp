// Static initialiser of an enum-type registration: builds the enum info object for
// "gameeditor::EEditorModeDataFlow", registers its destructor with atexit and adds it to the registry, then
// fills two handler records (value pairs taken from the enum info and the first enum's info)
// and registers both.
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

extern UnkEnumInfo g_unk_7101d55008;
extern u8 g_unk_7101d55120[];
extern UnkConverter g_unk_7101d55128;
extern UnkConverter g_unk_7101d55148;

void fn_710002d420();
void fn_710002d7ac() HIDDEN;
void fn_710002d7b8() HIDDEN;
void fn_710002d62c(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);

void fn_710002d840() {
    {
        CStrId name("gameeditor::EEditorModeDataFlow", true);
        fn_710002d62c(&g_unk_7101d55008, &name, fn_710002d420, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7101d55008, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d55120, &g_unk_7101d55008);
    void* a = g_unk_7101d55008.desc_0x8->field_0x30;
    void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    g_unk_7101d55128.from = a;
    g_unk_7101d55128.to = b;
    g_unk_7101d55128.convert = fn_710002d7ac;
    g_unk_7101d55128.next = nullptr;
    fn_71000a16c4(&g_unk_7101d55128);
    void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    void* d = g_unk_7101d55008.desc_0x8->field_0x30;
    g_unk_7101d55148.from = c;
    g_unk_7101d55148.to = d;
    g_unk_7101d55148.convert = fn_710002d7b8;
    g_unk_7101d55148.next = nullptr;
    fn_71000a16c4(&g_unk_7101d55148);
}
