// Static initialiser of navmesh::CDynamicSmartLinkGroup's unit (tools/tu_map.py unit 351): the unit's
// header data, then the reflected types of CRntVector<SDynamicSmartLinkRulesSet> (its two key
// iterator value types and the vector), each noted in the unit's registry. Names and layouts are
// placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "game/navmesh/CDynamicSmartLinkGroup.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

// The unit's data: the header, three function records and the group's class type.
struct UnkStatics_l2693a0 {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
    u8 pad_0x18[0x10];
    UnkFnRecord fns[3];
    u8 field_0xb8[8];
};
static_assert(__builtin_offsetof(UnkStatics_l2693a0, fns) == 0x28, "fns");
static_assert(__builtin_offsetof(UnkStatics_l2693a0, field_0xb8) == 0xb8, "type");

extern UnkStatics_l2693a0 g_unk_71726a2a08 __attribute__((visibility("hidden")));

extern char g_unk_71726a21c8[];
extern char g_unk_71726a21c9[];
extern char g_unk_71726a21ca[];
extern char g_unk_71726a2308[];
extern char g_unk_71726a2309[];
extern char g_unk_71726a2448[];
extern char g_unk_71726a2449[];
extern char g_unk_71726a25c0[];
extern char g_unk_71726a25c1[];

extern CType g_unk_71726a21d0;
extern CType g_unk_71726a2310;
extern CType g_unk_71726a2450;

const CType* fn_7100264b88();
const CType* fn_7100264c64();
const CType* fn_7100265254();
const CType* fn_7100265844();
void fn_7100264d40(CClass* cls);
void fn_7100265330(CClass* cls);

// The class and vector type constructors.
void fn_71002650d0(CType* self, const CStrId& name, CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71002656c0(CType* self, const CStrId& name, CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100265920(CType* self, const CStrId& name, void* a, void* b);

static inline void* AsType(const CType* type) {
    return const_cast<CType*>(type);
}

void fn_71002693a0() {
    InitHeaderFns(g_unk_71726a2a08);

    fn_7100264814();
    fn_710009ec00(g_unk_71726a21c8, g_unk_71726a2a08.field_0xb8);
    fn_710009ec00(g_unk_71726a21c9, AsType(fn_7100264b88()));
    fn_710009ec00(g_unk_71726a21ca, AsType(fn_7100264c64()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<navmesh::"
                    "CDynamicSmartLinkGroup::SDynamicSmartLinkRulesSet>>::TKeyElementIterator::value_type",
                    true);
        fn_71002650d0(&g_unk_71726a21d0, name, &g_unk_7101d5e2e0, nullptr, fn_7100264d40, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71726a21d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a2308, &g_unk_71726a21d0);
    fn_710009ec00(g_unk_71726a2309, AsType(fn_7100265254()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<navmesh::"
                    "CDynamicSmartLinkGroup::SDynamicSmartLinkRulesSet>>::TKeyElementConstIterator::value_type",
                    true);
        fn_71002656c0(&g_unk_71726a2310, name, &g_unk_7101d5e2e0, nullptr, fn_7100265330, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71726a2310, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a2448, &g_unk_71726a2310);
    fn_710009ec00(g_unk_71726a2449, AsType(fn_7100265844()));

    {
        CStrId name("base::global::CRntVector<navmesh::CDynamicSmartLinkGroup::SDynamicSmartLinkRulesSet>", true);
        fn_7100265920(&g_unk_71726a2450, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_71726a2450, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a25c0, &g_unk_71726a2450);

    fn_7100265b1c();
    fn_710009ec00(g_unk_71726a25c1, AsType(&g_unk_71726a2bf8));
}
