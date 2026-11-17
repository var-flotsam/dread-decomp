// Static initialiser of CChangeStateDoorsLogicAction's unit (tools/tu_map.py unit 1547): the header
// statics, the reflected eDoorStateLogicAction enum with its conversions to and from int, DoorStateInfo,
// the CollectionTypeMapper<CRntVector<DoorStateInfo>> iterator value types, the vector type, then the
// logic action's class. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

// The unit's statics (the compiler merges them into one block).
struct UnkStatics_7100a3c7a0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];  // CChangeStateDoorsLogicAction's class
};
extern UnkStatics_7100a3c7a0 g_unk_71727492d8 __attribute__((visibility("hidden")));

extern UnkEnumType g_unk_7172748800;
void fn_7100a37aac(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100a378a0(void* e);
extern char g_unk_7172748918[];
extern UnkConversion g_unk_7172748920;
extern UnkConversion g_unk_7172748940;
void fn_7100a3975c() __attribute__((visibility("hidden")));
void fn_7100a39768() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172748960;
void fn_7100a37c2c(base::reflection::CClass* cls);
void fn_7100a37dcc(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(base::reflection::CClass*), void*);
extern char g_unk_7172748a98[];
extern char g_unk_7172748a99[];
extern char g_unk_7172748a9a[];
UnkEnumType* fn_7100a37f50();  // refl_GetType<CollectionTypeMapper<CRntVector<DoorStateInfo>>::TElementIterator>
UnkEnumType* fn_7100a3802c();  // refl_GetType<CollectionTypeMapper<CRntVector<DoorStateInfo>>::TElementConstIterator>
extern UnkEnumType g_unk_7172748aa0;
void fn_7100a38108(base::reflection::CClass* cls);
void fn_7100a3848c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*,
                   void (*)(base::reflection::CClass*), void*);
extern char g_unk_7172748bd8[];
extern char g_unk_7172748bd9[];
UnkEnumType* fn_7100a38610();
extern UnkEnumType g_unk_7172748be0;
void fn_7100a386ec(base::reflection::CClass* cls);
void fn_7100a38a70(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*,
                   void (*)(base::reflection::CClass*), void*);
extern char g_unk_7172748d18[];
extern char g_unk_7172748d19[];
UnkEnumType* fn_7100a38bf4();
extern UnkEnumType g_unk_7172748d20;
void fn_7100a38cd0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7172748e90[];
extern char g_unk_7172748e91[];

void fn_7100a3c7a0() {
    InitHeaderIds(&g_unk_71727492d8.ids);
    InitEnum(&g_unk_7172748800, "eDoorStateLogicAction", fn_7100a37aac, fn_7100a378a0, g_unk_7172748918,
             &g_unk_7172748920, fn_7100a3975c, &g_unk_7172748940, fn_7100a39768);
    {
        CStrId n("DoorStateInfo", true);
        fn_7100a37dcc(&g_unk_7172748960, n, nullptr, nullptr, fn_7100a37c2c, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172748960, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172748a98, &g_unk_7172748960);
    fn_710009ec00(g_unk_7172748a99, fn_7100a37f50());
    fn_710009ec00(g_unk_7172748a9a, fn_7100a3802c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<DoorStateInfo>>::TKeyElementIterator::"
                 "value_type",
                 true);
        fn_7100a3848c(&g_unk_7172748aa0, n, &g_unk_7101d5e2e0, nullptr, fn_7100a38108, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172748aa0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172748bd8, &g_unk_7172748aa0);
    fn_710009ec00(g_unk_7172748bd9, fn_7100a38610());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<DoorStateInfo>>::"
                 "TKeyElementConstIterator::value_type",
                 true);
        fn_7100a38a70(&g_unk_7172748be0, n, &g_unk_7101d5e2e0, nullptr, fn_7100a386ec, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172748be0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172748d18, &g_unk_7172748be0);
    fn_710009ec00(g_unk_7172748d19, fn_7100a38bf4());
    {
        CStrId n("base::global::CRntVector<DoorStateInfo>", true);
        fn_7100a38cd0(&g_unk_7172748d20, n, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172748d20, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172748e90, &g_unk_7172748d20);
    fn_7100a38ec8();
    fn_710009ec00(g_unk_7172748e91, g_unk_71727492d8.field_0x20);
}
