// The static initialiser of base::tunable::CTunable's unit (helpers in game/UnkTUInit.h): the "sources"
// and "binaries" ids, the class, and the dictionary of tunables by name with its iterator value types.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTUInit.h"

// The unit's data: the two ids (sources first), then the class type its getter builds.
struct UnkTunableTUData_l1392b0 {
    CStrId sources;
    CStrId binaries;
    u8 unk_0x10[0x8];
    u8 type[0x8];  // 0x18
};
extern UnkTunableTUData_l1392b0 g_unk_710b371b10 __attribute__((visibility("hidden")));

extern char g_unk_710b3710b1;
extern char g_unk_710b3710b2;
extern char g_unk_710b3710b3;
extern char g_unk_710b3710b4;
extern UnkReflType g_unk_710b3710b8;
extern char g_unk_710b3711f0;
extern char g_unk_710b3711f1;
extern UnkReflType g_unk_710b3711f8;
extern char g_unk_710b371330;
extern char g_unk_710b371331;
extern UnkReflType g_unk_710b371338;
extern char g_unk_710b3714a8;
extern char g_unk_710b3714a9;

const base::reflection::CType* fn_7100135da0();
const base::reflection::CType* fn_7100135e7c();
const base::reflection::CType* fn_7100135f58();
const base::reflection::CType* fn_710013653c();
const base::reflection::CType* fn_7100136b20();
void fn_7100136034(base::reflection::CClass* cls);
void fn_7100136618(base::reflection::CClass* cls);
void fn_71001363b8(UnkReflType* self, const CStrId& name, const base::reflection::CType* base, void* a,
                   base::reflection::ClassSetupFn init, void* b);
void fn_710013699c(UnkReflType* self, const CStrId& name, const base::reflection::CType* base, void* a,
                   base::reflection::ClassSetupFn init, void* b);
void fn_7100136bfc(UnkReflType* self, const CStrId& name, void* a, void* b);

static inline void* AsList(const base::reflection::CType* t) {
    return const_cast<void*>(static_cast<const void*>(t));
}

void CTunable_InitStatics() {
    new (&g_unk_710b371b10.sources) CStrId("sources", true);
    fn_7100000250(CStrId_Destroy, &g_unk_710b371b10.sources, &g_unk_7101a19000);
    new (&g_unk_710b371b10.binaries) CStrId("binaries", true);
    fn_7100000250(CStrId_Destroy, &g_unk_710b371b10.binaries, &g_unk_7101a19000);

    fn_7100135a94();
    fn_710009ec00(&g_unk_710b3710b1, g_unk_710b371b10.type);
    fn_710009ec00(&g_unk_710b3710b2, AsList(fn_7100135da0()));
    fn_710009ec00(&g_unk_710b3710b3, AsList(fn_7100135e7c()));
    fn_710009ec00(&g_unk_710b3710b4, AsList(fn_7100135f58()));

    {
        CStrId id("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
                  "CRntString, base::tunable::CTunable*>>::TKeyElementIterator::value_type",
                  true);
        fn_71001363b8(&g_unk_710b3710b8, id, &g_unk_7101d5e2e0, nullptr, fn_7100136034, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_710b3710b8, &g_unk_7101a19000);
    fn_710009ec00(&g_unk_710b3711f0, &g_unk_710b3710b8);
    fn_710009ec00(&g_unk_710b3711f1, AsList(fn_710013653c()));

    {
        CStrId id("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
                  "CRntString, base::tunable::CTunable*>>::TKeyElementConstIterator::value_type",
                  true);
        fn_710013699c(&g_unk_710b3711f8, id, &g_unk_7101d5e2e0, nullptr, fn_7100136618, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_710b3711f8, &g_unk_7101a19000);
    fn_710009ec00(&g_unk_710b371330, &g_unk_710b3711f8);
    fn_710009ec00(&g_unk_710b371331, AsList(fn_7100136b20()));

    {
        CStrId id("base::global::CRntSmallDictionary<base::global::CRntString, base::tunable::CTunable*>", true);
        fn_7100136bfc(&g_unk_710b371338, id, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_710b371338, &g_unk_7101a19000);
    fn_710009ec00(&g_unk_710b3714a8, &g_unk_710b371338);
    fn_710009ec00(&g_unk_710b3714a9, fn_7100136df0());
}
