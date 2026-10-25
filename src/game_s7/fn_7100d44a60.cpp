// Static initialiser of the unit of
// base::global::CRntVector<base::global::CWeakPtr<game::logic::collision::CCollider>>,
// base::global::CRntDictionary<base::global::CStrId,
// base::global::CWeakPtr<game::logic::collision::CCollider>>,
// base::global::CRntVector<game::logic::collision::CCollider*> and others: the header ids, then
// the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100d44a60 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_7100d44a60 g_unk_71727b2988 __attribute__((visibility("hidden")));
extern char g_unk_71727b0448[];
extern UnkNamedValue g_unk_71727b0450;
extern char g_unk_71727b0460[];
UnkEnumType* fn_7100d37398();
extern char g_unk_71727b0461[];
UnkEnumType* fn_7100d37474();
extern char g_unk_71727b0462[];
UnkEnumType* fn_7100d37550();
extern char g_unk_71727b0463[];
UnkEnumType* fn_7100d3762c();
extern char g_unk_71727b0464[];
UnkEnumType* fn_7100d37708();
extern UnkEnumType g_unk_71727b0468;
void fn_7100d37b68(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_7101d5e2e0[];
void fn_7100d377e4();
extern char g_unk_71727b05a0[];
extern char g_unk_71727b05a1[];
UnkEnumType* fn_7100d37cec();
extern UnkEnumType g_unk_71727b05a8;
void fn_7100d3814c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d37dc8();
extern char g_unk_71727b06e0[];
extern char g_unk_71727b06e1[];
UnkEnumType* fn_7100d382d0();
extern UnkEnumType g_unk_71727b06e8;
void fn_7100d383ac(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b0858[];
extern char g_unk_71727b0859[];
UnkEnumType* fn_7100d3859c();
extern char g_unk_71727b085a[];
UnkEnumType* fn_7100d38678();
extern UnkEnumType g_unk_71727b0860;
void fn_7100d38ad8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d38754();
extern char g_unk_71727b0998[];
extern char g_unk_71727b0999[];
UnkEnumType* fn_7100d38c5c();
extern UnkEnumType g_unk_71727b09a0;
void fn_7100d390bc(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d38d38();
extern char g_unk_71727b0ad8[];
extern char g_unk_71727b0ad9[];
UnkEnumType* fn_7100d39240();
extern UnkEnumType g_unk_71727b0ae0;
void fn_7100d3931c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b0c50[];
extern char g_unk_71727b0c51[];
UnkEnumType* fn_7100d394ec();
extern char g_unk_71727b0c52[];
UnkEnumType* fn_7100d395c8();
extern char g_unk_71727b0c53[];
UnkEnumType* fn_7100d396a4();
extern UnkEnumType g_unk_71727b0c58;
void fn_7100d39b04(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d39780();
extern char g_unk_71727b0d90[];
extern char g_unk_71727b0d91[];
UnkEnumType* fn_7100d39c88();
extern UnkEnumType g_unk_71727b0d98;
void fn_7100d3a0e8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d39d64();
extern char g_unk_71727b0ed0[];
extern char g_unk_71727b0ed1[];
UnkEnumType* fn_7100d3a26c();
extern UnkEnumType g_unk_71727b0ed8;
void fn_7100d3a348(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b1048[];
extern char g_unk_71727b1049[];
UnkEnumType* fn_7100d3a538();
extern char g_unk_71727b104a[];
UnkEnumType* fn_7100d3a614();
extern UnkEnumType g_unk_71727b1050;
void fn_7100d3aa74(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d3a6f0();
extern char g_unk_71727b1188[];
extern char g_unk_71727b1189[];
UnkEnumType* fn_7100d3abf8();
extern UnkEnumType g_unk_71727b1190;
void fn_7100d3b058(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d3acd4();
extern char g_unk_71727b12c8[];
extern char g_unk_71727b12c9[];
UnkEnumType* fn_7100d3b1dc();
extern UnkEnumType g_unk_71727b12d0;
void fn_7100d3b2b8(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b1440[];

void fn_7100d44a60() {
    InitHeaderIds(&g_unk_71727b2988.ids);
    fn_7100d361c0();
    fn_710009ec00(g_unk_71727b0448, g_unk_71727b2988.field_0x20);
    InitNamedValue(&g_unk_71727b0450, "pShape", TypeOf<fn_7100009658>, fn_710010c424);
    fn_710009ec00(g_unk_71727b0460, fn_7100d37398());
    fn_710009ec00(g_unk_71727b0461, fn_7100d37474());
    fn_710009ec00(g_unk_71727b0462, fn_7100d37550());
    fn_710009ec00(g_unk_71727b0463, fn_7100d3762c());
    fn_710009ec00(g_unk_71727b0464, fn_7100d37708());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CWeakPtr<game::logic::collision::CCollider>>>::TKeyElementIterator::value_type", true);
        fn_7100d37b68(&g_unk_71727b0468, n, g_unk_7101d5e2e0, nullptr, fn_7100d377e4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b0468, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b05a0, &g_unk_71727b0468);
    fn_710009ec00(g_unk_71727b05a1, fn_7100d37cec());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CWeakPtr<game::logic::collision::CCollider>>>::TKeyElementConstIterator::value_type", true);
        fn_7100d3814c(&g_unk_71727b05a8, n, g_unk_7101d5e2e0, nullptr, fn_7100d37dc8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b05a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b06e0, &g_unk_71727b05a8);
    fn_710009ec00(g_unk_71727b06e1, fn_7100d382d0());
    {
        CStrId n("base::global::CRntVector<base::global::CWeakPtr<game::logic::collision::CCollider>>", true);
        fn_7100d383ac(&g_unk_71727b06e8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b06e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b0858, &g_unk_71727b06e8);
    fn_710009ec00(g_unk_71727b0859, fn_7100d3859c());
    fn_710009ec00(g_unk_71727b085a, fn_7100d38678());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CWeakPtr<game::logic::collision::CCollider>>>::TKeyElementIterator::value_type", true);
        fn_7100d38ad8(&g_unk_71727b0860, n, g_unk_7101d5e2e0, nullptr, fn_7100d38754, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b0860, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b0998, &g_unk_71727b0860);
    fn_710009ec00(g_unk_71727b0999, fn_7100d38c5c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CWeakPtr<game::logic::collision::CCollider>>>::TKeyElementConstIterator::value_type", true);
        fn_7100d390bc(&g_unk_71727b09a0, n, g_unk_7101d5e2e0, nullptr, fn_7100d38d38, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b09a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b0ad8, &g_unk_71727b09a0);
    fn_710009ec00(g_unk_71727b0ad9, fn_7100d39240());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, base::global::CWeakPtr<game::logic::collision::CCollider>>", true);
        fn_7100d3931c(&g_unk_71727b0ae0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b0ae0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b0c50, &g_unk_71727b0ae0);
    fn_710009ec00(g_unk_71727b0c51, fn_7100d394ec());
    fn_710009ec00(g_unk_71727b0c52, fn_7100d395c8());
    fn_710009ec00(g_unk_71727b0c53, fn_7100d396a4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<game::logic::collision::CCollider*>>::TKeyElementIterator::value_type", true);
        fn_7100d39b04(&g_unk_71727b0c58, n, g_unk_7101d5e2e0, nullptr, fn_7100d39780, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b0c58, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b0d90, &g_unk_71727b0c58);
    fn_710009ec00(g_unk_71727b0d91, fn_7100d39c88());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<game::logic::collision::CCollider*>>::TKeyElementConstIterator::value_type", true);
        fn_7100d3a0e8(&g_unk_71727b0d98, n, g_unk_7101d5e2e0, nullptr, fn_7100d39d64, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b0d98, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b0ed0, &g_unk_71727b0d98);
    fn_710009ec00(g_unk_71727b0ed1, fn_7100d3a26c());
    {
        CStrId n("base::global::CRntVector<game::logic::collision::CCollider*>", true);
        fn_7100d3a348(&g_unk_71727b0ed8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b0ed8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b1048, &g_unk_71727b0ed8);
    fn_710009ec00(g_unk_71727b1049, fn_7100d3a538());
    fn_710009ec00(g_unk_71727b104a, fn_7100d3a614());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<game::logic::collision::CCollider*>>>::TKeyElementIterator::value_type", true);
        fn_7100d3aa74(&g_unk_71727b1050, n, g_unk_7101d5e2e0, nullptr, fn_7100d3a6f0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b1050, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b1188, &g_unk_71727b1050);
    fn_710009ec00(g_unk_71727b1189, fn_7100d3abf8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<game::logic::collision::CCollider*>>>::TKeyElementConstIterator::value_type", true);
        fn_7100d3b058(&g_unk_71727b1190, n, g_unk_7101d5e2e0, nullptr, fn_7100d3acd4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b1190, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b12c8, &g_unk_71727b1190);
    fn_710009ec00(g_unk_71727b12c9, fn_7100d3b1dc());
    {
        CStrId n("base::global::CRntVector<base::global::CRntVector<game::logic::collision::CCollider*>>", true);
        fn_7100d3b2b8(&g_unk_71727b12d0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b12d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b1440, &g_unk_71727b12d0);
}
