// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): the unit's static initialiser. It builds the
// header data, then registers the unit's reflected types: SDoorInfo, the component's class, the two iterator
// value types of CRntVector<SDoorInfo>, the vector type itself and the enum CThermalDeviceComponent::EPipeGroup
// with its two int converters. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

extern UnkTUHeader g_unk_7172752700 __attribute__((visibility("hidden")));
extern char g_unk_7172752730[] __attribute__((visibility("hidden")));  // the component's class

// SDoorInfo.
extern base::reflection::CType g_unk_7172751c28;
void fn_7100a8fa54(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100a8f90c(CClass* cls);
extern char g_unk_7172751d60[];
extern char g_unk_7172751d61[];
// The iterator types of CRntVector<SDoorInfo>.
const CType* fn_7100a90a80();
extern char g_unk_7172751d62[];
const CType* fn_7100a90b5c();
extern char g_unk_7172751d63[];
// The key iterator's value type.
extern base::reflection::CType g_unk_7172751d68;
void fn_7100a90fbc(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100a90c38(CClass* cls);
extern char g_unk_7172751ea0[];
const CType* fn_7100a91140();
extern char g_unk_7172751ea1[];
// The const key iterator's value type.
extern base::reflection::CType g_unk_7172751ea8;
void fn_7100a915a0(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100a9121c(CClass* cls);
extern char g_unk_7172751fe0[];
const CType* fn_7100a91724();
extern char g_unk_7172751fe1[];
// CRntVector<SDoorInfo>.
extern base::reflection::CType g_unk_7172751fe8;
void fn_7100a91800(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172752158[];
// CThermalDeviceComponent::EPipeGroup and its converters to and from int.
extern base::reflection::CType g_unk_7172752160;
void fn_7100a91d80(CType* self, const CStrId& name, void (*init)(void*), void* extra);
void fn_7100a919f4(void* e);
extern char g_unk_7172752278[];
extern UnkConverter g_unk_7172752280;
extern UnkConverter g_unk_71727522a0;
void fn_7100a9434c() __attribute__((visibility("hidden")));
void fn_7100a94358() __attribute__((visibility("hidden")));

void fn_7100a96560() {
    InitHeader(g_unk_7172752700);

    {
        CStrId name("SDoorInfo", true);
        fn_7100a8fa54(&g_unk_7172751c28, name, nullptr, nullptr, fn_7100a8f90c, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172751c28, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172751d60, &g_unk_7172751c28);
    fn_7100a8fbdc();
    fn_710009ec00(g_unk_7172751d61, g_unk_7172752730);
    fn_710009ec00(g_unk_7172751d62, const_cast<CType*>(fn_7100a90a80()));
    fn_710009ec00(g_unk_7172751d63, const_cast<CType*>(fn_7100a90b5c()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<SDoorInfo>>::"
                    "TKeyElementIterator::value_type",
                    true);
        fn_7100a90fbc(&g_unk_7172751d68, name, &g_unk_7101d5e2e0, nullptr, fn_7100a90c38, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172751d68, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172751ea0, &g_unk_7172751d68);
    fn_710009ec00(g_unk_7172751ea1, const_cast<CType*>(fn_7100a91140()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<SDoorInfo>>::"
                    "TKeyElementConstIterator::value_type",
                    true);
        fn_7100a915a0(&g_unk_7172751ea8, name, &g_unk_7101d5e2e0, nullptr, fn_7100a9121c, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172751ea8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172751fe0, &g_unk_7172751ea8);
    fn_710009ec00(g_unk_7172751fe1, const_cast<CType*>(fn_7100a91724()));

    {
        CStrId name("base::global::CRntVector<SDoorInfo>", true);
        fn_7100a91800(&g_unk_7172751fe8, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172751fe8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172752158, &g_unk_7172751fe8);

    {
        CStrId name("CThermalDeviceComponent::EPipeGroup", true);
        fn_7100a91d80(&g_unk_7172752160, name, fn_7100a919f4, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7172752160, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172752278, &g_unk_7172752160);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&g_unk_7172752160), g_unk_7172752280, fn_7100a9434c,
                       g_unk_71727522a0, fn_7100a94358);
}
