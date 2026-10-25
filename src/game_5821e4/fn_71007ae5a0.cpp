// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutsceneComponent::SActorInfo>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutsceneComponent::SActorInfo>>::TKeyElementConstIterator::value_type, base::global::CRntVector<CCutsceneComponent::SActorInfo> reflected types.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_71007ae5a0 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    u8 field_0x30[8];
    u8 pad_0x38[0x130];
    u8 field_0x168[8];
    u8 pad_0x170[0x130];
    u8 field_0x2a0[8];
};
extern UnkStatics_71007ae5a0 g_unk_7172719990 __attribute__((visibility("hidden")));
extern char g_unk_7172718ee8[];
UnkEnumType* fn_71007a11ec();
extern char g_unk_7172718ee9[];
UnkEnumType* fn_71007a12c8();
extern char g_unk_7101d5e2e0[];
void fn_71007a13a4();
extern UnkEnumType g_unk_7172718ef0;
void fn_71007a1734(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_7172719028[];
extern char g_unk_7172719029[];
UnkEnumType* fn_71007a18b8();
void fn_71007a1994();
extern UnkEnumType g_unk_7172719030;
void fn_71007a1d24(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7172719168[];
extern char g_unk_7172719169[];
UnkEnumType* fn_71007a1ea8();
extern UnkEnumType g_unk_7172719170;
void fn_71007a1f84(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727192e0[];
extern char g_unk_71727192e1[];
UnkEnumType* fn_71007a217c();
extern UnkConversion g_unk_71727192e8;
void fn_71007ab7d8() __attribute__((visibility("hidden")));
extern char g_unk_7172719308[];
extern char g_unk_7172719309[];
extern char g_unk_717271930a[];

void fn_71007ae5a0() {
    InitHeaderIds(&g_unk_7172719990.ids);
    fn_710009ec00(g_unk_7172718ee8, fn_71007a11ec());
    fn_710009ec00(g_unk_7172718ee9, fn_71007a12c8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutsceneComponent::SActorInfo>>::TKeyElementIterator::value_type", true);
        fn_71007a1734(&g_unk_7172718ef0, n, g_unk_7101d5e2e0, nullptr, fn_71007a13a4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172718ef0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172719028, &g_unk_7172718ef0);
    fn_710009ec00(g_unk_7172719029, fn_71007a18b8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutsceneComponent::SActorInfo>>::TKeyElementConstIterator::value_type", true);
        fn_71007a1d24(&g_unk_7172719030, n, g_unk_7101d5e2e0, nullptr, fn_71007a1994, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172719030, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172719168, &g_unk_7172719030);
    fn_710009ec00(g_unk_7172719169, fn_71007a1ea8());
    {
        CStrId n("base::global::CRntVector<CCutsceneComponent::SActorInfo>", true);
        fn_71007a1f84(&g_unk_7172719170, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172719170, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727192e0, &g_unk_7172719170);
    fn_710009ec00(g_unk_71727192e1, fn_71007a217c());
    InitConversion(&g_unk_71727192e8, TypeOf<fn_7100005178>, TypeOf<fn_7100009658>, fn_71007ab7d8);
    fn_71007a225c();
    fn_710009ec00(g_unk_7172719308, g_unk_7172719990.field_0x30);
    fn_71007a2c40();
    fn_710009ec00(g_unk_7172719309, g_unk_7172719990.field_0x168);
    fn_71007a2ec0();
    fn_710009ec00(g_unk_717271930a, g_unk_7172719990.field_0x2a0);
}
