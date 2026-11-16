// The static initialiser of CBeamBoxComponent's unit (helpers in game/UnkTUInit.h): the unit header, the
// component's id, its "Pos" property, the SBeamBoxActivatable class and the vector of them with its iterator
// value types, the component's class, then two more ids. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTUSymbols.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

// The unit's data (its statics, merged into one block).
struct UnkBeamBoxTU_l8ac5f0 {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId light;       // 0x20
    CStrId collision;   // 0x28
    UnkReflType type;   // 0x30, the component's class
};

extern UnkBeamBoxTU_l8ac5f0 g_unk_717272eb48 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_717272e160;
extern UnkProperty g_unk_717272e168;
extern void* g_unk_7101a7d6e0[];  // the property's vtable
void fn_71008a9ab0(UnkProperty* p);
void fn_71008a47e0(void* p);
extern base::reflection::CType g_unk_717272e190;
extern base::reflection::CType g_unk_717272e2d0;
extern base::reflection::CType g_unk_717272e410;
extern base::reflection::CType g_unk_717272e550;
extern char g_unk_717272e2c8;
extern char g_unk_717272e2c9;
extern char g_unk_717272e2ca;
extern char g_unk_717272e408;
extern char g_unk_717272e409;
extern char g_unk_717272e548;
extern char g_unk_717272e549;
extern char g_unk_717272e6c0;
extern char g_unk_717272e6c1;
void fn_71008a49a0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71008a4858();
const base::reflection::CType* fn_71008a4b24();
const base::reflection::CType* fn_71008a4c00();
void fn_71008a5060(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71008a4cdc();
const base::reflection::CType* fn_71008a51e4();
void fn_71008a5644(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71008a52c0();
const base::reflection::CType* fn_71008a57c8();
void fn_71008a58a4(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_71008ac5f0() {
    UnkBeamBoxTU_l8ac5f0& tu = g_unk_717272eb48;
    InitHeader(tu.header);
    new (&g_unk_717272e160) CStrId("BEAMBOX", true);
    fn_7100000250(fn_710000051c, &g_unk_717272e160, &g_unk_7101a19000);
    InitProperty(&g_unk_717272e168, g_unk_7101a7d6e0, g_unk_71726db788, "Pos", refl_TypeOf<fn_7100024504>(),
                 fn_71008a9ab0, fn_71008a47e0);
    InitClass(AsUnk(&g_unk_717272e190), "SBeamBoxActivatable", fn_71008a49a0, fn_71008a4858);
    fn_710009ec00(&g_unk_717272e2c8, &g_unk_717272e190);
    fn_710009ec00(&g_unk_717272e2c9, refl_TypeOf<fn_71008a4b24>());
    fn_710009ec00(&g_unk_717272e2ca, refl_TypeOf<fn_71008a4c00>());
    InitClass(AsUnk(&g_unk_717272e2d0),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<SBeamBoxActivatable>>::"
              "TKeyElementIterator::value_type",
              fn_71008a5060,
              fn_71008a4cdc,
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717272e408, &g_unk_717272e2d0);
    fn_710009ec00(&g_unk_717272e409, refl_TypeOf<fn_71008a51e4>());
    InitClass(AsUnk(&g_unk_717272e410),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<SBeamBoxActivatable>>::"
              "TKeyElementConstIterator::value_type",
              fn_71008a5644,
              fn_71008a52c0,
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717272e548, &g_unk_717272e410);
    fn_710009ec00(&g_unk_717272e549, refl_TypeOf<fn_71008a57c8>());
    InitVector(AsUnk(&g_unk_717272e550), "base::global::CRntVector<SBeamBoxActivatable>", fn_71008a58a4);
    fn_710009ec00(&g_unk_717272e6c0, &g_unk_717272e550);
    fn_71008a5a9c();
    fn_710009ec00(&g_unk_717272e6c1, &tu.type);
    new (&tu.light) CStrId("light", true);
    fn_7100000250(fn_710000051c, &tu.light, &g_unk_7101a19000);
    new (&tu.collision) CStrId("collision", true);
    fn_7100000250(fn_710000051c, &tu.collision, &g_unk_7101a19000);
}
