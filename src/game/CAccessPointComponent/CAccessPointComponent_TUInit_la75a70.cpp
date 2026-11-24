// CAccessPointComponent unit (tools/tu_map.py, unit 1581): its static initialiser. The unit header, the
// component's id, its "ThermalDoorOpened" property, its class and tunable, two sound paths and the
// reflected types of its dictionary of string-id arrays. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

using base::global::CFilePathStrId;

// The unit's data (its statics, merged into one block).
struct UnkAccessPointTU {
    UnkTUHeader header;
    u8 pad_0x18[0x10];
    CFilePathStrId loop_high_sound;  // 0x28
    CFilePathStrId fake_adam_sound;  // 0x30
    u8 pad_0x38[0x10];
    f32 field_0x48[2];
    bool field_0x50;
    u8 pad_0x51[0x138 - 0x51];
    UnkReflType component_type;  // 0x138
    u8 pad_0x180[0x270 - 0x180];
    UnkReflType tunable_type;  // 0x270
};
static_assert(__builtin_offsetof(UnkAccessPointTU, component_type) == 0x138, "layout");
static_assert(__builtin_offsetof(UnkAccessPointTU, tunable_type) == 0x270, "layout");

extern UnkAccessPointTU g_unk_717274f518 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_717274ec08;
extern UnkProperty g_unk_717274ec10;
extern void* g_unk_7101a901e8[];
void fn_7100a7399c(void* item);  // base::reflection::refl_Register<..>, instantiated in src/reflection_statics
void fn_7100a6b5b4(void* p);
extern char g_unk_717274ec38;
extern char g_unk_717274ec39;
extern UnkTunable g_unk_717274ec40;
extern void* g_unk_7101a90240[];
extern char g_unk_717274ec89;
extern char g_unk_717274ec8a;
UnkReflType* fn_7100a7128c();
UnkReflType* fn_7100a71368();
extern UnkReflType g_unk_717274ec90;
void fn_7100a717c8(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100a71444();
extern char g_unk_717274edc8;
extern char g_unk_717274edc9;
UnkReflType* fn_7100a7194c();
extern UnkReflType g_unk_717274edd0;
void fn_7100a71dac(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100a71a28();
extern char g_unk_717274ef08;
extern char g_unk_717274ef09;
UnkReflType* fn_7100a71f30();
extern UnkReflType g_unk_717274ef10;
void fn_7100a7200c(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
extern char g_unk_717274f080;

static inline UnkReflType* AsUnk(const base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(const_cast<base::reflection::CType*>(t));
}

void CAccessPointComponent_InitStatics() {
    UnkAccessPointTU& tu = g_unk_717274f518;
    InitHeader(tu.header);
    tu.field_0x48[0] = -1.0f;
    tu.field_0x48[1] = -1.0f;
    tu.field_0x50 = true;

    new (&g_unk_717274ec08) CStrId("ACCESSPOINT", true);
    fn_7100000250(CStrId_Destroy, &g_unk_717274ec08, &g_unk_7101a19000);
    InitProperty(&g_unk_717274ec10, g_unk_7101a901e8, g_unk_71726db788, "ThermalDoorOpened",
                 refl_TypeOf<fn_710000d228>(), fn_7100a7399c, fn_7100a6b5b4);

    fn_7100a6b630();
    fn_710009ec00(&g_unk_717274ec38, &tu.component_type);
    fn_7100a6bcfc();
    fn_710009ec00(&g_unk_717274ec39, &tu.tunable_type);
    InitTunable(&g_unk_717274ec40, fn_7100136ecc, g_unk_7101a90240, fn_7100075104);

    new (&tu.loop_high_sound) CFilePathStrId("props/emmydoor/emmydoor_loophigh.wav", true);
    fn_7100000250(fn_7100000c7c, &tu.loop_high_sound, &g_unk_7101a19000);
    new (&tu.fake_adam_sound) CFilePathStrId("props/fakeadam/FakeAdam_Loop.wav", true);
    fn_7100000250(fn_7100000c7c, &tu.fake_adam_sound, &g_unk_7101a19000);

    fn_710009ec00(&g_unk_717274ec89, fn_7100a7128c());
    fn_710009ec00(&g_unk_717274ec8a, fn_7100a71368());
    InitClass(&g_unk_717274ec90,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "base::global::CRntVector<base::global::CStrId>>>::TKeyElementIterator::value_type",
              fn_7100a717c8, fn_7100a71444, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717274edc8, &g_unk_717274ec90);
    fn_710009ec00(&g_unk_717274edc9, fn_7100a7194c());
    InitClass(&g_unk_717274edd0,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "base::global::CRntVector<base::global::CStrId>>>::TKeyElementConstIterator::value_type",
              fn_7100a71dac, fn_7100a71a28, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717274ef08, &g_unk_717274edd0);
    fn_710009ec00(&g_unk_717274ef09, fn_7100a71f30());
    InitVector(&g_unk_717274ef10,
               "base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<base::global::CStrId>>",
               fn_7100a7200c);
    fn_710009ec00(&g_unk_717274f080, &g_unk_717274ef10);
}
