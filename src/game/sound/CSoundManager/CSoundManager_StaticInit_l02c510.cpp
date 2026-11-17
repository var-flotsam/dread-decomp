// Static initialiser of sound::CSoundManager's unit: the header ids, floats, flag and records, the
// sound events classes, the rule and asset dictionaries' reflected types and the manager's own.
// Names and layouts are placeholders.
#include "../../../game_5821e4/UnkStaticInit.h"
#include "base/UnkAtExit.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "types.h"

using namespace unk_static_init;
using base::reflection::CClass;
using base::reflection::CType;

// The unit's own statics: the header ids, two floats and a flag, three function records, then the
// class descriptors of sound::CSoundEventsDef and its SSoundEventsSelector (built by their getters).
struct UnkStatics_710102c510 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    f32 field_0x30;
    f32 field_0x34;
    bool field_0x38;
    UnkFnRecord fns[3];         // 0x40
    u8 events_def[0x138];       // 0xd0
    u8 events_selector[0x138];  // 0x208
};
static_assert(__builtin_offsetof(UnkStatics_710102c510, fns) == 0x40, "records");
static_assert(__builtin_offsetof(UnkStatics_710102c510, events_def) == 0xd0, "class");
extern UnkStatics_710102c510 g_unk_71729f9b70 __attribute__((visibility("hidden")));
// sound::CSoundManager's class descriptor, built by its getter.
extern u8 g_unk_71729f9eb0[] __attribute__((visibility("hidden")));

typedef void (*UnkClassInit_l02c510)(CClass* cls);

void fn_7101020360(CClass* cls);
void fn_7101020d54(CClass* cls);
void fn_7101021338(CClass* cls);
void fn_7101021ccc(CClass* cls);
void fn_71010224b4(CClass* cls);

const CType* fn_7101020b9c();
const CType* fn_7101020c78();
const CType* fn_710102125c();
const CType* fn_7101021840();
const CType* fn_7101021b14();
const CType* fn_7101021bf0();
const CType* fn_71010223d8();
const CType* fn_71010229bc();
const CType* fn_71010205e0();
const CType* fn_7100ad63b0();
const CType* fn_7101022c78();
const CType* fn_71010221d4();

// Class and vector type constructors: (type, name, base type, -, init, -) and (type, name, -, -).
void fn_710102046c(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l02c510 init, void*);
void fn_71010210d8(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l02c510 init, void*);
void fn_71010216bc(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l02c510 init, void*);
void fn_710102191c(void* type, const CStrId& name, void*, void*);
void fn_7101022050(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l02c510 init, void*);
void fn_7101022838(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l02c510 init, void*);
void fn_7101022a98(void* type, const CStrId& name, void*, void*);

extern char g_unk_71729f8570[];
extern char g_unk_71729f8571[];
extern base::reflection::CType g_unk_71729f8578;
extern char g_unk_71729f86b0[];
extern char g_unk_71729f86b1[];
extern char g_unk_71729f86b2[];
extern char g_unk_71729f86b3[];
extern base::reflection::CType g_unk_71729f86b8;
extern char g_unk_71729f87f0[];
extern char g_unk_71729f87f1[];
extern base::reflection::CType g_unk_71729f87f8;
extern char g_unk_71729f8930[];
extern char g_unk_71729f8931[];
extern base::reflection::CType g_unk_71729f8938;
extern char g_unk_71729f8aa8[];
extern char g_unk_71729f8aa9[];
extern char g_unk_71729f8aaa[];
extern base::reflection::CType g_unk_71729f8ab0;
extern char g_unk_71729f8be8[];
extern char g_unk_71729f8be9[];
extern base::reflection::CType g_unk_71729f8bf0;
extern char g_unk_71729f8d28[];
extern char g_unk_71729f8d29[];
extern base::reflection::CType g_unk_71729f8d30;
extern char g_unk_71729f8ea0[];
extern char g_unk_71729f8ea1[];
extern char g_unk_71729f8ea2[];
extern char g_unk_71729f8ea3[];
extern char g_unk_71729f8ea4[];

static void* AsList(const CType* type) {
    return const_cast<CType*>(type);
}

void fn_710102c510() {
    UnkStatics_710102c510& s = g_unk_71729f9b70;
    InitHeaderIds(&s.ids);
    s.field_0x30 = -1.0f;
    s.field_0x34 = -1.0f;
    s.field_0x38 = true;
    InitRecord(s.fns[0], fn_710000cf00);
    InitRecord(s.fns[1], fn_710000d470);
    InitRecord(s.fns[2], fn_710000d4f4);

    fn_710101fb24();  // builds sound::CSoundEventsDef's class in s.events_def
    fn_710009ec00(g_unk_71729f8570, s.events_def);
    fn_7101020058();  // and SSoundEventsSelector's in s.events_selector
    fn_710009ec00(g_unk_71729f8571, s.events_selector);
    {
        CStrId n("sound::CSoundEventsDef::SSoundEventsRule", true);
        fn_710102046c(&g_unk_71729f8578, n, nullptr, nullptr, fn_7101020360, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729f8578, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f86b0, &g_unk_71729f8578);
    fn_71010206c0();  // builds sound::CSoundManager's class
    fn_710009ec00(g_unk_71729f86b1, g_unk_71729f9eb0);
    fn_710009ec00(g_unk_71729f86b2, AsList(fn_7101020b9c()));
    fn_710009ec00(g_unk_71729f86b3, AsList(fn_7101020c78()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
                 "base::core::CAssetLink>>::TKeyElementIterator::value_type",
                 true);
        fn_71010210d8(&g_unk_71729f86b8, n, &g_unk_7101d5e2e0, nullptr, fn_7101020d54, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729f86b8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f87f0, &g_unk_71729f86b8);
    fn_710009ec00(g_unk_71729f87f1, AsList(fn_710102125c()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
                 "base::core::CAssetLink>>::TKeyElementConstIterator::value_type",
                 true);
        fn_71010216bc(&g_unk_71729f87f8, n, &g_unk_7101d5e2e0, nullptr, fn_7101021338, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729f87f8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f8930, &g_unk_71729f87f8);
    fn_710009ec00(g_unk_71729f8931, AsList(fn_7101021840()));
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, base::core::CAssetLink>", true);
        fn_710102191c(&g_unk_71729f8938, n, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_71729f8938, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f8aa8, &g_unk_71729f8938);
    fn_710009ec00(g_unk_71729f8aa9, AsList(fn_7101021b14()));
    fn_710009ec00(g_unk_71729f8aaa, AsList(fn_7101021bf0()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<sound::CSoundEventsDef::TSoundEventRules>::"
                 "TKeyElementIterator::value_type",
                 true);
        fn_7101022050(&g_unk_71729f8ab0, n, &g_unk_7101d5e2e0, nullptr, fn_7101021ccc, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729f8ab0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f8be8, &g_unk_71729f8ab0);
    fn_710009ec00(g_unk_71729f8be9, AsList(fn_71010223d8()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<sound::CSoundEventsDef::TSoundEventRules>::"
                 "TKeyElementConstIterator::value_type",
                 true);
        fn_7101022838(&g_unk_71729f8bf0, n, &g_unk_7101d5e2e0, nullptr, fn_71010224b4, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729f8bf0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f8d28, &g_unk_71729f8bf0);
    fn_710009ec00(g_unk_71729f8d29, AsList(fn_71010229bc()));
    {
        CStrId n("sound::CSoundEventsDef::TSoundEventRules", true);
        fn_7101022a98(&g_unk_71729f8d30, n, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_71729f8d30, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f8ea0, &g_unk_71729f8d30);
    fn_710009ec00(g_unk_71729f8ea1, AsList(fn_71010205e0()));
    fn_710009ec00(g_unk_71729f8ea2, AsList(fn_7100ad63b0()));
    fn_710009ec00(g_unk_71729f8ea3, AsList(fn_7101022c78()));
    fn_710009ec00(g_unk_71729f8ea4, AsList(fn_71010221d4()));
}
