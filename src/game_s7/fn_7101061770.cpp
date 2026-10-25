// Static initialiser of the unit of CEditorPathSegment, base::global::CRntVector<SLogicPathNode>,
// base::global::CRntVector<SLogicSubPath> and others: the header ids, then the unit's own statics
// and registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7101061770 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    u8 field_0x30[8];
    u8 pad_0x38[0x130];
    u8 field_0x168[8];
    u8 pad_0x170[0x130];
    u8 field_0x2a0[8];
};
extern UnkStatics_7101061770 g_unk_71729ffb60 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71729fe178;
void fn_710105402c(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
void fn_7101053e24();
extern char g_unk_71729fe2b0[];
extern char g_unk_71729fe2b1[];
extern char g_unk_71729fe2b2[];
extern char g_unk_71729fe2b3[];
extern char g_unk_71729fe2b4[];
UnkEnumType* fn_71010561fc();
extern char g_unk_71729fe2b5[];
UnkEnumType* fn_71010562d8();
extern UnkEnumType g_unk_71729fe2b8;
void fn_7101056744(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7101d5e2e0[];
void fn_71010563b4();
extern char g_unk_71729fe3f0[];
extern char g_unk_71729fe3f1[];
UnkEnumType* fn_71010568c8();
extern UnkEnumType g_unk_71729fe3f8;
void fn_7101056d34(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71010569a4();
extern char g_unk_71729fe530[];
extern char g_unk_71729fe531[];
UnkEnumType* fn_7101056eb8();
extern UnkEnumType g_unk_71729fe538;
void fn_7101056f94(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729fe6a8[];
extern char g_unk_71729fe6a9[];
UnkEnumType* fn_710105718c();
extern char g_unk_71729fe6aa[];
UnkEnumType* fn_7101057268();
extern UnkEnumType g_unk_71729fe6b0;
void fn_71010576d4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7101057344();
extern char g_unk_71729fe7e8[];
extern char g_unk_71729fe7e9[];
UnkEnumType* fn_7101057858();
extern UnkEnumType g_unk_71729fe7f0;
void fn_7101057cc4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7101057934();
extern char g_unk_71729fe928[];
extern char g_unk_71729fe929[];
UnkEnumType* fn_7101057e48();
extern UnkEnumType g_unk_71729fe930;
void fn_7101057f24(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729feaa0[];
extern char g_unk_71729feaa1[];
UnkEnumType* fn_710105811c();
extern char g_unk_71729feaa2[];
UnkEnumType* fn_71010581f8();
extern UnkEnumType g_unk_71729feaa8;
void fn_7101058658(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71010582d4();
extern char g_unk_71729febe0[];
extern char g_unk_71729febe1[];
UnkEnumType* fn_71010587dc();
extern UnkEnumType g_unk_71729febe8;
void fn_7101058c3c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71010588b8();
extern char g_unk_71729fed20[];
extern char g_unk_71729fed21[];
UnkEnumType* fn_7101058dc0();
extern UnkEnumType g_unk_71729fed28;
void fn_7101058e9c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729fee98[];

void fn_7101061770() {
    InitHeaderIds(&g_unk_71729ffb60.ids);
    {
        CStrId n("CEditorPathSegment", true);
        fn_710105402c(&g_unk_71729fe178, n, nullptr, nullptr, fn_7101053e24, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729fe178, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fe2b0, &g_unk_71729fe178);
    fn_7101054330();
    fn_710009ec00(g_unk_71729fe2b1, g_unk_71729ffb60.field_0x30);
    fn_7101054c90();
    fn_710009ec00(g_unk_71729fe2b2, g_unk_71729ffb60.field_0x168);
    fn_7101055ef0();
    fn_710009ec00(g_unk_71729fe2b3, g_unk_71729ffb60.field_0x2a0);
    fn_710009ec00(g_unk_71729fe2b4, fn_71010561fc());
    fn_710009ec00(g_unk_71729fe2b5, fn_71010562d8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SLogicPathNode>>::TKeyElementIterator::value_type", true);
        fn_7101056744(&g_unk_71729fe2b8, n, g_unk_7101d5e2e0, nullptr, fn_71010563b4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729fe2b8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fe3f0, &g_unk_71729fe2b8);
    fn_710009ec00(g_unk_71729fe3f1, fn_71010568c8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SLogicPathNode>>::TKeyElementConstIterator::value_type", true);
        fn_7101056d34(&g_unk_71729fe3f8, n, g_unk_7101d5e2e0, nullptr, fn_71010569a4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729fe3f8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fe530, &g_unk_71729fe3f8);
    fn_710009ec00(g_unk_71729fe531, fn_7101056eb8());
    {
        CStrId n("base::global::CRntVector<SLogicPathNode>", true);
        fn_7101056f94(&g_unk_71729fe538, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729fe538, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fe6a8, &g_unk_71729fe538);
    fn_710009ec00(g_unk_71729fe6a9, fn_710105718c());
    fn_710009ec00(g_unk_71729fe6aa, fn_7101057268());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SLogicSubPath>>::TKeyElementIterator::value_type", true);
        fn_71010576d4(&g_unk_71729fe6b0, n, g_unk_7101d5e2e0, nullptr, fn_7101057344, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729fe6b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fe7e8, &g_unk_71729fe6b0);
    fn_710009ec00(g_unk_71729fe7e9, fn_7101057858());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SLogicSubPath>>::TKeyElementConstIterator::value_type", true);
        fn_7101057cc4(&g_unk_71729fe7f0, n, g_unk_7101d5e2e0, nullptr, fn_7101057934, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729fe7f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fe928, &g_unk_71729fe7f0);
    fn_710009ec00(g_unk_71729fe929, fn_7101057e48());
    {
        CStrId n("base::global::CRntVector<SLogicSubPath>", true);
        fn_7101057f24(&g_unk_71729fe930, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729fe930, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729feaa0, &g_unk_71729fe930);
    fn_710009ec00(g_unk_71729feaa1, fn_710105811c());
    fn_710009ec00(g_unk_71729feaa2, fn_71010581f8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEditorPathSegment>>::TKeyElementIterator::value_type", true);
        fn_7101058658(&g_unk_71729feaa8, n, g_unk_7101d5e2e0, nullptr, fn_71010582d4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729feaa8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729febe0, &g_unk_71729feaa8);
    fn_710009ec00(g_unk_71729febe1, fn_71010587dc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEditorPathSegment>>::TKeyElementConstIterator::value_type", true);
        fn_7101058c3c(&g_unk_71729febe8, n, g_unk_7101d5e2e0, nullptr, fn_71010588b8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729febe8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fed20, &g_unk_71729febe8);
    fn_710009ec00(g_unk_71729fed21, fn_7101058dc0());
    {
        CStrId n("base::global::CRntVector<CEditorPathSegment>", true);
        fn_7101058e9c(&g_unk_71729fed28, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729fed28, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729fee98, &g_unk_71729fed28);
}
