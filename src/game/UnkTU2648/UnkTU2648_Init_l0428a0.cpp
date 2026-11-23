// The static initialiser of sound::CAudioPresets's unit (2648, helpers in game/UnkTUInit.h): the header with
// its two floats and flag, the class, the preset dictionary's iterator value types and the dictionary itself.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

// The unit's header data: the ids, two floats and a flag, then the three function records.
struct UnkTUHeader_l0428a0 {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
    u8 unk_0x18[0x10];
    f32 field_0x28;
    f32 field_0x2c;
    bool field_0x30;
    UnkFnRecord fns[3];  // 0x38
};
static_assert(__builtin_offsetof(UnkTUHeader_l0428a0, fns) == 0x38, "records");

extern UnkTUHeader_l0428a0 g_unk_71729fc130 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_71729fb488;
extern char g_unk_71729fb5c0;
extern char g_unk_71729fb5c1;
extern char g_unk_71729fb5c2;
extern char g_unk_71729fb5c3;
extern char g_unk_71729fb5c4;
extern UnkReflType g_unk_71729fb5c8;
extern char g_unk_71729fb700;
extern char g_unk_71729fb701;
extern UnkReflType g_unk_71729fb708;
extern char g_unk_71729fb840;
extern char g_unk_71729fb841;
extern base::reflection::CType g_unk_71729fb848;
extern char g_unk_71729fb9b8;
extern char g_unk_71729fb9b9;
extern char g_unk_71729fb9ba;
void fn_710103d788(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710103d05c();
const base::reflection::CType* fn_7101020ac0();  // refl_GetType<sound::CAudioPresets*>
const base::reflection::CType* fn_710103dbdc();  // refl_GetType<sound::CAudioPresets const*>
const base::reflection::CType* fn_710103dcb8();  // refl_GetType<...::TElementIterator>
const base::reflection::CType* fn_710103dd94();  // refl_GetType<...::TElementConstIterator>
void fn_710103e1f4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710103de70();
const base::reflection::CType* fn_710103e378();
void fn_710103e7d8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710103e454();
const base::reflection::CType* fn_710103e95c();
void fn_710103ea38(UnkReflType* self, const CStrId& name, void* a, void* b);
const base::reflection::CType* fn_710103ec08();  // refl_GetType<CRntDictionary<CStrId, CAudioPreset>*>
const base::reflection::CType* fn_710103ece4();  // refl_GetType<CRntDictionary<CStrId, CAudioPreset> const*>

void fn_71010428a0() {
    UnkTUHeader_l0428a0& h = g_unk_71729fc130;
    h.field_0x0 = 6;
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(CStrId_Destroy, &h.binaries, &g_unk_7101a19000);
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(CStrId_Destroy, &h.sources, &g_unk_7101a19000);
    h.field_0x28 = -1.0f;
    h.field_0x2c = -1.0f;
    h.field_0x30 = true;
    InitRecord(h.fns[0], fn_710000cf00);
    InitRecord(h.fns[1], fn_710000d470);
    InitRecord(h.fns[2], fn_710000d4f4);

    InitClass(AsUnk(&g_unk_71729fb488), "sound::CAudioPresets", fn_710103d788, fn_710103d05c);
    fn_710009ec00(&g_unk_71729fb5c0, &g_unk_71729fb488);
    fn_710009ec00(&g_unk_71729fb5c1, refl_TypeOf<fn_7101020ac0>());
    fn_710009ec00(&g_unk_71729fb5c2, refl_TypeOf<fn_710103dbdc>());
    fn_710009ec00(&g_unk_71729fb5c3, refl_TypeOf<fn_710103dcb8>());
    fn_710009ec00(&g_unk_71729fb5c4, refl_TypeOf<fn_710103dd94>());
    InitClass(&g_unk_71729fb5c8,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "sound::CAudioPreset>>::TKeyElementIterator::value_type",
              fn_710103e1f4, fn_710103de70, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729fb700, &g_unk_71729fb5c8);
    fn_710009ec00(&g_unk_71729fb701, refl_TypeOf<fn_710103e378>());
    InitClass(&g_unk_71729fb708,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "sound::CAudioPreset>>::TKeyElementConstIterator::value_type",
              fn_710103e7d8, fn_710103e454, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729fb840, &g_unk_71729fb708);
    fn_710009ec00(&g_unk_71729fb841, refl_TypeOf<fn_710103e95c>());
    InitVector(AsUnk(&g_unk_71729fb848), "base::global::CRntDictionary<base::global::CStrId, sound::CAudioPreset>",
               fn_710103ea38);
    fn_710009ec00(&g_unk_71729fb9b8, &g_unk_71729fb848);
    fn_710009ec00(&g_unk_71729fb9b9, refl_TypeOf<fn_710103ec08>());
    fn_710009ec00(&g_unk_71729fb9ba, refl_TypeOf<fn_710103ece4>());
}
