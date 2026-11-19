// The static initialiser of CNavMeshItemComponent's unit (helpers in game/UnkTUInit.h): the unit header, the
// "NAVMESH_OCCLUDERS" property, the component's id, its "ColliderOffset" and "InitialStage" properties, the
// component's class, the enabled occluder colliders map with its iterator value types, then the component's
// registered id and the "_default" id. Names and layouts are placeholders.
#include <new>

#include "../../../src/game_5821e4/UnkStaticInit.h"
#include "base/UnkStrIdRegistry.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTUSymbols.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

// The unit's data (its statics, merged into one block).
struct UnkNavMeshItemTU_l9c4e10 {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId key;         // 0x20, the component's id as registered
    CStrId default_id;  // 0x28
    UnkReflType type;   // 0x30, the component's class
};

extern UnkNavMeshItemTU_l9c4e10 g_unk_717273ede8 __attribute__((visibility("hidden")));
extern UnkPropertyEx<> g_unk_717273e210;  // "NAVMESH_OCCLUDERS"
extern base::global::CStrId g_unk_717273e250;
// The two properties as the unit's readers see them (a named definition); here they are built as
// UnkPropertyEx<0x160>.
struct UnkMapNamedDef_ovb0203 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
struct UnkNavMeshNamedDef_b49 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkNavMeshNamedDef_b49 g_unk_717273e3e0;  // "InitialStage"
void fn_71009c26f4(void* item);  // base::reflection::refl_Register<..>, instantiated in src/reflection_statics
void fn_71009bd8c0(void* p);
extern char g_unk_717273e568;
extern char g_unk_717273e569;
extern char g_unk_717273e56a;
extern char g_unk_717273e6a8;
extern char g_unk_717273e6a9;
extern char g_unk_717273e7e8;
extern char g_unk_717273e7e9;
extern char g_unk_717273e960;
extern base::reflection::CType g_unk_717273e570;
extern base::reflection::CType g_unk_717273e6b0;
using unk_static_init::UnkEnumType;
extern UnkEnumType g_unk_717273e7f0;  // TEnabledOccluderCollidersMap
const base::reflection::CType* fn_71009be878();
const base::reflection::CType* fn_71009be954();
const base::reflection::CType* fn_71009bef38();
const base::reflection::CType* fn_71009bf51c();
void fn_71009bedb4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71009bea30();
void fn_71009bf398(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71009bf014();
void fn_71009bf5f8(UnkReflType* self, const CStrId& name, void* a, void* b);

// The component's id lives in an inline function: its guard and object are reached through the GOT.
inline const CStrId& NavMeshItemId() {
    static CStrId s_id("NAVMESHITEM", true);
    return s_id;
}

void fn_71009c4e10() {
    UnkNavMeshItemTU_l9c4e10& tu = g_unk_717273ede8;
    InitHeader(tu.header);
    InitPropertyEx(&g_unk_717273e210, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                   "NAVMESH_OCCLUDERS", &g_unk_717273e7f0, fn_71004fd2cc, fn_71004fa3b8);
    new (&g_unk_717273e250) CStrId("NAVMESHITEM", true);
    fn_7100000250(fn_710000051c, &g_unk_717273e250, &g_unk_7101a19000);
    InitPropertyExVtableFirst(reinterpret_cast<UnkPropertyEx<0x160>*>(&g_unk_717273e258), g_unk_7101a889c8, g_unk_71726db788, g_unk_7101a88970, fn_7100ae9cc8,
                              "ColliderOffset", refl_TypeOf<fn_7100024504>(), fn_71009c26f4, fn_71009bd8c0);
    InitPropertyExVtableFirst(reinterpret_cast<UnkPropertyEx<0x160>*>(&g_unk_717273e3e0), g_unk_7101a889c8, g_unk_71726db788, g_unk_7101a88970, fn_7100ae9cc8,
                              "InitialStage", refl_TypeOf<fn_7100009658>(), fn_71009c26f4, fn_71009bd8c0);
    fn_71009bd964();
    fn_710009ec00(&g_unk_717273e568, &tu.type);
    fn_710009ec00(&g_unk_717273e569, refl_TypeOf<fn_71009be878>());
    fn_710009ec00(&g_unk_717273e56a, refl_TypeOf<fn_71009be954>());
    InitClass(AsUnk(&g_unk_717273e570),
              "base::reflection::CollectionTypeMapper<TEnabledOccluderCollidersMap>::TKeyElementIterator::value_type",
              fn_71009bedb4, fn_71009bea30, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717273e6a8, &g_unk_717273e570);
    fn_710009ec00(&g_unk_717273e6a9, refl_TypeOf<fn_71009bef38>());
    InitClass(AsUnk(&g_unk_717273e6b0),
              "base::reflection::CollectionTypeMapper<TEnabledOccluderCollidersMap>::TKeyElementConstIterator::"
              "value_type",
              fn_71009bf398, fn_71009bf014, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717273e7e8, &g_unk_717273e6b0);
    fn_710009ec00(&g_unk_717273e7e9, refl_TypeOf<fn_71009bf51c>());
    InitVector(reinterpret_cast<UnkReflType*>(&g_unk_717273e7f0), "TEnabledOccluderCollidersMap", fn_71009bf5f8);
    fn_710009ec00(&g_unk_717273e960, &g_unk_717273e7f0);
    new (&tu.key) CStrId(fn_7100de8f78(NavMeshItemId()));
    fn_7100000250(fn_710000051c, &tu.key, &g_unk_7101a19000);
    new (&tu.default_id) CStrId("_default", true);
    fn_7100000250(fn_710000051c, &tu.default_id, &g_unk_7101a19000);
}
