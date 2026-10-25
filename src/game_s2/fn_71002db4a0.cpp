// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<animtree::CAnimTreeDef::TAnimTreeLinkDef>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<animtree::CAnimTreeDef::TAnimTreeLinkDef>>::TKeyElementConstIterator::value_type, base::global::CRntVector<animtree::CAnimTreeDef::TAnimTreeLinkDef>.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_71002db4a0 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_71002db4a0 g_unk_71726a9d20 __attribute__((visibility("hidden")));
extern char g_unk_71726a93d0[];
extern char g_unk_71726a93d1[];
UnkEnumType* fn_71002d6b20();
extern char g_unk_71726a93d2[];
UnkEnumType* fn_71002d6bfc();
extern UnkEnumType g_unk_71726a93d8;
extern char g_unk_7101d5e2e0[];
void fn_71002d6cd8();
void fn_71002d7068(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71726a9510[];
extern char g_unk_71726a9511[];
UnkEnumType* fn_71002d71ec();
extern UnkEnumType g_unk_71726a9518;
void fn_71002d72c8();
void fn_71002d7658(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726a9650[];
extern char g_unk_71726a9651[];
UnkEnumType* fn_71002d77dc();
extern UnkEnumType g_unk_71726a9658;
void fn_71002d78b8(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726a97c8[];
extern char g_unk_71726a97c9[];

void fn_71002db4a0() {
    InitHeaderIds(&g_unk_71726a9d20.ids);
    fn_71002d6464();
    fn_710009ec00(g_unk_71726a93d0, g_unk_71726a9d20.field_0x28);
    fn_710009ec00(g_unk_71726a93d1, fn_71002d6b20());
    fn_710009ec00(g_unk_71726a93d2, fn_71002d6bfc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<animtree::CAnimTreeDef::TAnimTreeLinkDef>>::TKeyElementIterator::value_type", true);
        fn_71002d7068(&g_unk_71726a93d8, n, g_unk_7101d5e2e0, nullptr, fn_71002d6cd8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726a93d8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a9510, &g_unk_71726a93d8);
    fn_710009ec00(g_unk_71726a9511, fn_71002d71ec());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<animtree::CAnimTreeDef::TAnimTreeLinkDef>>::TKeyElementConstIterator::value_type", true);
        fn_71002d7658(&g_unk_71726a9518, n, g_unk_7101d5e2e0, nullptr, fn_71002d72c8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726a9518, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a9650, &g_unk_71726a9518);
    fn_710009ec00(g_unk_71726a9651, fn_71002d77dc());
    {
        CStrId n("base::global::CRntVector<animtree::CAnimTreeDef::TAnimTreeLinkDef>", true);
        fn_71002d78b8(&g_unk_71726a9658, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726a9658, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a97c8, &g_unk_71726a9658);
    fn_71002d7ab4();
    fn_710009ec00(g_unk_71726a97c9, g_unk_71726a9d20.field_0x160);
}
