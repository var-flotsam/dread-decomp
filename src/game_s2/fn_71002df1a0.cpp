// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<animtree::CAnimTreeElementDef>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<animtree::CAnimTreeElementDef>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<animtree::CAnimTreeElementDef>>.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71002df1a0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71002df1a0 g_unk_71726aaa88 __attribute__((visibility("hidden")));
extern char g_unk_71726aa118[];
extern char g_unk_71726aa119[];
UnkEnumType* fn_71002dbffc();  // base::reflection::refl_GetType<std::unique_ptr<animtree::CAnimTreeElementDef>>
extern char g_unk_71726aa11a[];
UnkEnumType* fn_71002d9344();
extern char g_unk_71726aa11b[];
UnkEnumType* fn_71002dc0d8();
extern char g_unk_71726aa11c[];
UnkEnumType* fn_71002dc1b4();
extern UnkEnumType g_unk_71726aa120;
extern char g_unk_7101d5e2e0[];
void fn_71002dc290();
void fn_71002dc614(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71726aa258[];
extern char g_unk_71726aa259[];
UnkEnumType* fn_71002dc798();
extern UnkEnumType g_unk_71726aa260;
void fn_71002dc874();
void fn_71002dcbf8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726aa398[];
extern char g_unk_71726aa399[];
UnkEnumType* fn_71002dcd7c();
extern UnkEnumType g_unk_71726aa3a0;
void fn_71002dce58(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_71726aa510[];
extern UnkConversion g_unk_71726aa518;
UnkEnumType* fn_71002dbffc();
void fn_71002de038() __attribute__((visibility("hidden")));

void fn_71002df1a0() {
    InitHeaderIds(&g_unk_71726aaa88.ids);
    fn_71002dbcf4();
    fn_710009ec00(g_unk_71726aa118, g_unk_71726aaa88.field_0x20);
    fn_710009ec00(g_unk_71726aa119, fn_71002dbffc());
    fn_710009ec00(g_unk_71726aa11a, fn_71002d9344());
    fn_710009ec00(g_unk_71726aa11b, fn_71002dc0d8());
    fn_710009ec00(g_unk_71726aa11c, fn_71002dc1b4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<animtree::CAnimTreeElementDef>>>::TKeyElementIterator::value_type", true);
        fn_71002dc614(&g_unk_71726aa120, n, g_unk_7101d5e2e0, nullptr, fn_71002dc290, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726aa120, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726aa258, &g_unk_71726aa120);
    fn_710009ec00(g_unk_71726aa259, fn_71002dc798());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<animtree::CAnimTreeElementDef>>>::TKeyElementConstIterator::value_type", true);
        fn_71002dcbf8(&g_unk_71726aa260, n, g_unk_7101d5e2e0, nullptr, fn_71002dc874, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726aa260, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726aa398, &g_unk_71726aa260);
    fn_710009ec00(g_unk_71726aa399, fn_71002dcd7c());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<animtree::CAnimTreeElementDef>>", true);
        fn_71002dce58(&g_unk_71726aa3a0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726aa3a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726aa510, &g_unk_71726aa3a0);
    InitConversion(&g_unk_71726aa518, fn_71002dbffc, fn_71002d9344, fn_71002de038);
}
