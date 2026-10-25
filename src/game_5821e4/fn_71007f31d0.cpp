// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CBarelyFrozenIceInfo>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CBarelyFrozenIceInfo>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CBarelyFrozenIceInfo>> reflected types.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_71007f31d0 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_71007f31d0 g_unk_7172720b20 __attribute__((visibility("hidden")));
extern char g_unk_7172720098[];
extern char g_unk_7172720099[];
UnkEnumType* fn_71007ef13c();
extern char g_unk_717272009a[];
UnkEnumType* fn_71007ef218();  // base::reflection::refl_GetType<std::unique_ptr<CBarelyFrozenIceInfo>>
extern char g_unk_717272009b[];
UnkEnumType* fn_71007ef2f4();
extern char g_unk_717272009c[];
UnkEnumType* fn_71007ef3d0();
extern char g_unk_7101d5e2e0[];
void fn_71007ef4ac();
extern UnkEnumType g_unk_71727200a0;
void fn_71007ef830(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71727201d8[];
extern char g_unk_71727201d9[];
UnkEnumType* fn_71007ef9b4();
void fn_71007efa90();
extern UnkEnumType g_unk_71727201e0;
void fn_71007efe14(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7172720318[];
extern char g_unk_7172720319[];
UnkEnumType* fn_71007eff98();
extern UnkEnumType g_unk_7172720320;
void fn_71007f0074(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7172720490[];
extern UnkConversion g_unk_7172720498;
void fn_71007f1e30() __attribute__((visibility("hidden")));
extern char g_unk_71727204b8[];

void fn_71007f31d0() {
    InitHeaderIds(&g_unk_7172720b20.ids);
    fn_71007eea64();
    fn_710009ec00(g_unk_7172720098, g_unk_7172720b20.field_0x28);
    fn_710009ec00(g_unk_7172720099, fn_71007ef13c());
    fn_710009ec00(g_unk_717272009a, fn_71007ef218());
    fn_710009ec00(g_unk_717272009b, fn_71007ef2f4());
    fn_710009ec00(g_unk_717272009c, fn_71007ef3d0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CBarelyFrozenIceInfo>>>::TKeyElementIterator::value_type", true);
        fn_71007ef830(&g_unk_71727200a0, n, g_unk_7101d5e2e0, nullptr, fn_71007ef4ac, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727200a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727201d8, &g_unk_71727200a0);
    fn_710009ec00(g_unk_71727201d9, fn_71007ef9b4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CBarelyFrozenIceInfo>>>::TKeyElementConstIterator::value_type", true);
        fn_71007efe14(&g_unk_71727201e0, n, g_unk_7101d5e2e0, nullptr, fn_71007efa90, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727201e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172720318, &g_unk_71727201e0);
    fn_710009ec00(g_unk_7172720319, fn_71007eff98());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CBarelyFrozenIceInfo>>", true);
        fn_71007f0074(&g_unk_7172720320, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172720320, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172720490, &g_unk_7172720320);
    InitConversion(&g_unk_7172720498, fn_71007ef218, fn_71007ef13c, fn_71007f1e30);
    fn_71007f0258();
    fn_710009ec00(g_unk_71727204b8, g_unk_7172720b20.field_0x160);
}
