// Static initialiser of an enum-type registration: builds the enum info object for
// "base::snd::ESoundGroupIds", registers its destructor with atexit and adds it to the registry, then
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

extern UnkEnumInfo g_unk_7108b6c488;
extern u8 g_unk_7108b6c5a0[];
extern UnkConverter g_unk_7108b6c5a8;
extern UnkConverter g_unk_7108b6c5c8;

void fn_71001061f4();
void fn_71001069b4() HIDDEN;
void fn_71001069c0() HIDDEN;
void fn_71001066a0(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);

void fn_7100106a40() {
    {
        CStrId name("base::snd::ESoundGroupIds", true);
        fn_71001066a0(&g_unk_7108b6c488, &name, fn_71001061f4, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7108b6c488, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6c5a0, &g_unk_7108b6c488);
    void* a = g_unk_7108b6c488.desc_0x8->field_0x30;
    void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    g_unk_7108b6c5a8.from = a;
    g_unk_7108b6c5a8.to = b;
    g_unk_7108b6c5a8.convert = fn_71001069b4;
    g_unk_7108b6c5a8.next = nullptr;
    fn_71000a16c4(&g_unk_7108b6c5a8);
    void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
    void* d = g_unk_7108b6c488.desc_0x8->field_0x30;
    g_unk_7108b6c5c8.from = c;
    g_unk_7108b6c5c8.to = d;
    g_unk_7108b6c5c8.convert = fn_71001069c0;
    g_unk_7108b6c5c8.next = nullptr;
    fn_71000a16c4(&g_unk_7108b6c5c8);
}
