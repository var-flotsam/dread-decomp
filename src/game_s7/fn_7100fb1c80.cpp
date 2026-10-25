// Static initialiser of the unit of ESubAreaItem, base::global::CArray<base::global::CStrId,
// EnumClass<ESubAreaItem>::Count, ESubAreaItem>: the header ids, then the unit's own statics and
// registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100fb1c80 {
    HeaderIds ids;
};
extern UnkStatics_7100fb1c80 g_unk_71729eee48 __attribute__((visibility("hidden")));
extern char g_unk_71729ee1b8[];
void fn_7100fadb10(void* self, const UnkStrIdBase* ids, u32 count);
void fn_7100ee1548(void* self);  // its destructor, as registered
extern char g_unk_71729ee200[];
extern UnkConversion g_unk_71729ee368;
extern UnkConversion g_unk_71729ee388;
void fn_7100faf7f0() __attribute__((visibility("hidden")));
void fn_7100faf7fc() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71729ee248;
extern char g_unk_71729ee360[];
void fn_7100fae524(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100fae138();
extern UnkConversion g_unk_71729ee3a8;
void fn_7100faf808() __attribute__((visibility("hidden")));
extern UnkConversion g_unk_71729ee3c8;
void fn_7100faf814() __attribute__((visibility("hidden")));
extern char g_unk_71729ee3e8[];
UnkEnumType* fn_7100fae6a4();
extern char g_unk_71729ee3e9[];
UnkEnumType* fn_7100fae780();
extern UnkEnumType g_unk_71729ee3f0;
void fn_7100faebe0(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_7101d5e2e0[];
void fn_7100fae85c();
extern char g_unk_71729ee528[];
extern char g_unk_71729ee529[];
UnkEnumType* fn_7100faed64();
extern UnkEnumType g_unk_71729ee530;
void fn_7100faf1c4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100faee40();
extern char g_unk_71729ee668[];
extern char g_unk_71729ee669[];
UnkEnumType* fn_7100faf348();
extern UnkEnumType g_unk_71729ee670;
void fn_7100faf424(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729ee7e0[];
extern char g_unk_71729ee7e1[];
UnkEnumType* fn_7100faf618();
extern char g_unk_71729ee7e2[];
UnkEnumType* fn_7100faf6f4();

// NON_MATCHING(minor): register allocation only: the original keeps the lists' count (9) in w24
// across the second list where ours rematerialises it, so it saves one more register; the
// instructions are otherwise the same.
void fn_7100fb1c80() {
    InitHeaderIds(&g_unk_71729eee48.ids);
    [] {
        UnkIdElem ids[] = {
            UnkIdElem(UnkEmptyIdTag()),
            UnkIdElem("lg_", true),
            UnkIdElem("ssg_", true),
            UnkIdElem("sg_SubArea_", true),
            UnkIdElem("eg_", true),
            UnkIdElem("bg_SubArea_", true),
            UnkIdElem(UnkEmptyIdTag()),
            UnkIdElem(UnkEmptyIdTag()),
            UnkIdElem(UnkEmptyIdTag()),
        };
        fn_7100fadb10(g_unk_71729ee1b8, (const UnkStrIdBase*)ids, 9);
    }();
    __cxa_atexit(fn_7100ee1548, g_unk_71729ee1b8, g_unk_7101a19000);
    [] {
        UnkIdElem ids[] = {
            UnkIdElem("NO_SUBAREA_RUNTIME", true),
            UnkIdElem("lg_NO_SUBAREA_RUNTIME", true),
            UnkIdElem("ssg_NO_SUBAREA_RUNTIME", true),
            UnkIdElem("sg_SubArea_NO_SUBAREA_RUNTIME", true),
            UnkIdElem("eg_NO_SUBAREA_RUNTIME", true),
            UnkIdElem("bg_SubArea_NO_SUBAREA_RUNTIME", true),
            UnkIdElem("NO_SUBAREA_RUNTIME", true),
            UnkIdElem("NO_SUBAREA_RUNTIME", true),
            UnkIdElem("NO_SUBAREA_RUNTIME", true),
        };
        fn_7100fadb10(g_unk_71729ee200, (const UnkStrIdBase*)ids, 9);
    }();
    __cxa_atexit(fn_7100ee1548, g_unk_71729ee200, g_unk_7101a19000);
    InitEnum(&g_unk_71729ee248, "ESubAreaItem", fn_7100fae524, fn_7100fae138, g_unk_71729ee360, &g_unk_71729ee368, fn_7100faf7f0, &g_unk_71729ee388, fn_7100faf7fc);
    InitConversion(&g_unk_71729ee3a8, &g_unk_71729ee248, TypeOf<fn_7100005178>, fn_7100faf808);
    InitConversion(&g_unk_71729ee3c8, TypeOf<fn_7100005178>, &g_unk_71729ee248, fn_7100faf814);
    fn_710009ec00(g_unk_71729ee3e8, fn_7100fae6a4());
    fn_710009ec00(g_unk_71729ee3e9, fn_7100fae780());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CArray<base::global::CStrId, EnumClass<ESubAreaItem>::Count, ESubAreaItem>>::TKeyElementIterator::value_type", true);
        fn_7100faebe0(&g_unk_71729ee3f0, n, g_unk_7101d5e2e0, nullptr, fn_7100fae85c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729ee3f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ee528, &g_unk_71729ee3f0);
    fn_710009ec00(g_unk_71729ee529, fn_7100faed64());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CArray<base::global::CStrId, EnumClass<ESubAreaItem>::Count, ESubAreaItem>>::TKeyElementConstIterator::value_type", true);
        fn_7100faf1c4(&g_unk_71729ee530, n, g_unk_7101d5e2e0, nullptr, fn_7100faee40, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729ee530, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ee668, &g_unk_71729ee530);
    fn_710009ec00(g_unk_71729ee669, fn_7100faf348());
    {
        CStrId n("base::global::CArray<base::global::CStrId, EnumClass<ESubAreaItem>::Count, ESubAreaItem>", true);
        fn_7100faf424(&g_unk_71729ee670, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729ee670, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ee7e0, &g_unk_71729ee670);
    fn_710009ec00(g_unk_71729ee7e1, fn_7100faf618());
    fn_710009ec00(g_unk_71729ee7e2, fn_7100faf6f4());
}
