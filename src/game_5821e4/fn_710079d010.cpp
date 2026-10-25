// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, game::logic::collision::EColMat>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, game::logic::collision::EColMat>>::TKeyElementConstIterator::value_type, base::global::CRntSmallDictionary<base::global::CStrId, game::logic::collision::EColMat> reflected types.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_710079d010 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_710079d010 g_unk_7172718be0 __attribute__((visibility("hidden")));
extern char g_unk_7172718290[];
void* fn_71007991c0();  // fn_71007991c0
extern char g_unk_7172718291[];
void* fn_710079929c();  // fn_710079929c
extern char g_unk_7101d5e2e0[];
void fn_7100799378();
extern UnkEnumType g_unk_7172718298;
void fn_71007996fc(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71727183d0[];
extern char g_unk_71727183d1[];
void* fn_7100799880();  // fn_7100799880
void fn_710079995c();
extern UnkEnumType g_unk_71727183d8;
void fn_7100799ce0(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7172718510[];
extern char g_unk_7172718511[];
void* fn_7100799e64();  // fn_7100799e64
extern UnkEnumType g_unk_7172718518;
void fn_7100799f40(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7172718688[];
extern char g_unk_7172718689[];
void* fn_710079a138();  // fn_710079a138
extern char g_unk_717271868a[];

void fn_710079d010() {
    InitHeaderIds(&g_unk_7172718be0.ids);
    fn_710009ec00(g_unk_7172718290, fn_71007991c0());
    fn_710009ec00(g_unk_7172718291, fn_710079929c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, game::logic::collision::EColMat>>::TKeyElementIterator::value_type", true);
        fn_71007996fc(&g_unk_7172718298, n, g_unk_7101d5e2e0, nullptr, fn_7100799378, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172718298, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727183d0, &g_unk_7172718298);
    fn_710009ec00(g_unk_71727183d1, fn_7100799880());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, game::logic::collision::EColMat>>::TKeyElementConstIterator::value_type", true);
        fn_7100799ce0(&g_unk_71727183d8, n, g_unk_7101d5e2e0, nullptr, fn_710079995c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727183d8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172718510, &g_unk_71727183d8);
    fn_710009ec00(g_unk_7172718511, fn_7100799e64());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, game::logic::collision::EColMat>", true);
        fn_7100799f40(&g_unk_7172718518, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172718518, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172718688, &g_unk_7172718518);
    fn_710009ec00(g_unk_7172718689, fn_710079a138());
    fn_710079a218();
    fn_710009ec00(g_unk_717271868a, g_unk_7172718be0.field_0x20);
}
