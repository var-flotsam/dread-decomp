// Static initialiser of the CCutscene::CTunableCutscene unit (2661): the header ids and records, then the
// CCutscene, CCutscene::CActor and vector-of-them reflected types, the tunable and the "Cutscene" id.
// Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkAtExit.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "types.h"

using namespace unk_static_init;
using base::reflection::CClass;
using base::reflection::CType;

// The unit's own statics: the header ids, the "Cutscene" id, two floats and a flag, three function
// records, then the CTunableCutscene class type (built by its getter).
struct UnkStatics_7101073440 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    f32 field_0x28;
    f32 field_0x2c;
    bool field_0x30;
    UnkFnRecord fns[3];  // 0x38
    u8 type[0x10];       // 0xc8
};
static_assert(__builtin_offsetof(UnkStatics_7101073440, fns) == 0x38, "records");
static_assert(__builtin_offsetof(UnkStatics_7101073440, type) == 0xc8, "class type");
extern UnkStatics_7101073440 g_unk_7172a01490 __attribute__((visibility("hidden")));

typedef void (*UnkClassInit_l073440)(CClass* cls);

const base::reflection::CType* fn_71007adf30();  // refl_GetType<CCutscene*>
const CType* fn_71010626f0();
const CType* fn_71010627cc();
const CType* fn_7101062db0();
const CType* fn_7101063394();
const CType* fn_7101063664();
const CType* fn_7101063740();
const CType* fn_7101063d24();
const CType* fn_7101064308();

void fn_71010628a8(CClass* cls);
void fn_7101062e8c(CClass* cls);
void fn_710106381c(CClass* cls);
void fn_7101063e00(CClass* cls);
void fn_7101064c68(CClass* cls);
void fn_7101064df0(CClass* cls);

// Class and vector type constructors: (type, name, base type, -, init, -) and (type, name, -, -).
void fn_7101062c2c(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l073440 init, void*);
void fn_7101063210(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l073440 init, void*);
void fn_7101063470(void* type, const CStrId& name, void*, void*);
void fn_7101063ba0(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l073440 init, void*);
void fn_7101064184(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l073440 init, void*);
void fn_71010643e4(void* type, const CStrId& name, void*, void*);
void fn_7101064c6c(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l073440 init, void*);
void fn_7101065160(void* type, const CStrId& name, const CType* base, void*, UnkClassInit_l073440 init, void*);

extern char g_unk_7172a00160[];
extern char g_unk_7172a00161[];
extern char g_unk_7172a00162[];
extern base::reflection::CType g_unk_7172a00168;
extern char g_unk_7172a002a0[];
extern char g_unk_7172a002a1[];
extern base::reflection::CType g_unk_7172a002a8;
extern char g_unk_7172a003e0[];
extern char g_unk_7172a003e1[];
extern base::reflection::CType g_unk_7172a003e8;
extern char g_unk_7172a00558[];
extern char g_unk_7172a00559[];
extern char g_unk_7172a0055a[];
extern base::reflection::CType g_unk_7172a00560;
extern char g_unk_7172a00698[];
extern char g_unk_7172a00699[];
extern base::reflection::CType g_unk_7172a006a0;
extern char g_unk_7172a007d8[];
extern char g_unk_7172a007d9[];
extern base::reflection::CType g_unk_7172a007e0;
extern char g_unk_7172a00950[];
extern char g_unk_7172a00951[];
extern UnkTunable g_unk_7172a00958;
extern char g_unk_7172a00990[];
extern char g_unk_7172a00ac8[];
extern base::reflection::CType g_unk_7172a00ad0;
extern char g_unk_7172a00c08[];
extern void* g_unk_7101cac9d8[];

static void* AsList(const CType* type) {
    return const_cast<CType*>(type);
}

void fn_7101073440() {
    UnkStatics_7101073440& s = g_unk_7172a01490;
    InitHeaderIds(&s.ids);
    InitRecord(s.fns[0], fn_710000cf00);
    InitRecord(s.fns[1], fn_710000d470);
    InitRecord(s.fns[2], fn_710000d4f4);
    s.field_0x28 = -1.0f;
    s.field_0x2c = -1.0f;
    s.field_0x30 = true;

    fn_710009ec00(g_unk_7172a00160, AsList(fn_71007adf30()));
    fn_710009ec00(g_unk_7172a00161, AsList(fn_71010626f0()));
    fn_710009ec00(g_unk_7172a00162, AsList(fn_71010627cc()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutscene::CActor>>::"
                 "TKeyElementIterator::value_type",
                 true);
        fn_7101062c2c(&g_unk_7172a00168, n, &g_unk_7101d5e2e0, nullptr, fn_71010628a8, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a00168, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a002a0, &g_unk_7172a00168);
    fn_710009ec00(g_unk_7172a002a1, AsList(fn_7101062db0()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutscene::CActor>>::"
                 "TKeyElementConstIterator::value_type",
                 true);
        fn_7101063210(&g_unk_7172a002a8, n, &g_unk_7101d5e2e0, nullptr, fn_7101062e8c, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a002a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a003e0, &g_unk_7172a002a8);
    fn_710009ec00(g_unk_7172a003e1, AsList(fn_7101063394()));
    {
        CStrId n("base::global::CRntVector<CCutscene::CActor>", true);
        fn_7101063470(&g_unk_7172a003e8, n, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172a003e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a00558, &g_unk_7172a003e8);
    fn_710009ec00(g_unk_7172a00559, AsList(fn_7101063664()));
    fn_710009ec00(g_unk_7172a0055a, AsList(fn_7101063740()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutscene*>>::"
                 "TKeyElementIterator::value_type",
                 true);
        fn_7101063ba0(&g_unk_7172a00560, n, &g_unk_7101d5e2e0, nullptr, fn_710106381c, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a00560, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a00698, &g_unk_7172a00560);
    fn_710009ec00(g_unk_7172a00699, AsList(fn_7101063d24()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCutscene*>>::"
                 "TKeyElementConstIterator::value_type",
                 true);
        fn_7101064184(&g_unk_7172a006a0, n, &g_unk_7101d5e2e0, nullptr, fn_7101063e00, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a006a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a007d8, &g_unk_7172a006a0);
    fn_710009ec00(g_unk_7172a007d9, AsList(fn_7101064308()));
    {
        CStrId n("base::global::CRntVector<CCutscene*>", true);
        fn_71010643e4(&g_unk_7172a007e0, n, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172a007e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a00950, &g_unk_7172a007e0);
    fn_71010645d8();  // builds the class type in s.type
    fn_710009ec00(g_unk_7172a00951, s.type);
    InitTunable(&g_unk_7172a00958, fn_7100136ecc, g_unk_7101cac9d8, fn_7100075104);
    {
        CStrId n("CCutscene::CActor", true);
        fn_7101064c6c(g_unk_7172a00990, n, nullptr, nullptr, fn_7101064c68, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7172a00990, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a00ac8, g_unk_7172a00990);
    {
        CStrId n("CCutscene", true);
        fn_7101065160(&g_unk_7172a00ad0, n, nullptr, nullptr, fn_7101064df0, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a00ad0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a00c08, &g_unk_7172a00ad0);
    InitId(&s.field_0x20, "Cutscene");
}
