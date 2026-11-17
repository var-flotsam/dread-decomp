// CCharClassShootActivatorComponent unit (tools/tu_map.py unit 1346): the unit's static initialiser: the header
// ids, a default pair of -1.0 values and a flag, the SHOOT_ACTIVATOR id, the "Used" property, the tunable and
// class registrations, the SActivatabledOnEventInfo types, the piston sound preset's id and path, and the two
// listener sets. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "game/UnkTU25.h"
#include "game/UnkTUProperty.h"
#include "game/CAbilitySonar.h"

using namespace unk_static_init;

struct UnkIdPathPair_7100333544;

// The unit's own statics, merged into one block.
struct UnkShootActivatorStatics_l896410 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    f32 field_0x30;
    f32 field_0x34;
    bool field_0x38;
    u8 pad_0x39[7];
    u8 field_0x40[0x10];  // a sound preset's id and path (fn_7100333544 destroys it)
    u8 field_0x50[8];
    u8 pad_0x58[0x188 - 0x58];
    u8 field_0x188[8];
    u8 pad_0x190[0x2c0 - 0x190];
    u8 field_0x2c0[8];
};
static_assert(__builtin_offsetof(UnkShootActivatorStatics_l896410, field_0x2c0) == 0x2c0, "layout");
extern UnkShootActivatorStatics_l896410 g_unk_717272cf40 __attribute__((visibility("hidden")));

extern UnkStrIdBase g_unk_717272c418;
extern void* g_unk_7101a7c770[];
void fn_7100893ae8(UnkProperty* p);
void fn_710088d2c4(void* p);
extern char g_unk_717272c448[];
extern void* g_unk_7101a7c7c8[];
extern char g_unk_717272c481[];
extern UnkEnumType g_unk_717272c488;
void fn_710088e088(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_710088df40();
extern char g_unk_717272c5c0[];
extern char g_unk_717272c5c1[];
const base::reflection::CType* fn_710088e20c();
extern char g_unk_717272c5c2[];
const base::reflection::CType* fn_710088e2e8();
extern UnkEnumType g_unk_717272c5c8;
void fn_710088e748(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_710088e3c4();
extern char g_unk_717272c700[];
extern char g_unk_717272c701[];
const base::reflection::CType* fn_710088e8cc();
extern UnkEnumType g_unk_717272c708;
void fn_710088ed2c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_710088e9a8();
extern char g_unk_717272c840[];
extern char g_unk_717272c841[];
const base::reflection::CType* fn_710088eeb0();
extern UnkEnumType g_unk_717272c848;
void fn_710088ef8c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717272c9b8[];
extern char g_unk_717272c9b9[];
void fn_710088fae8(void* sets);

// An empty listener set whose arrays use flags 0xf.
static void InitListenerSet(base::UnkDeferredSet<void>* set) {
    new (&set->items) base::global::CRntVector<void*>(0xf);
    new (&set->pending_add) base::global::CRntVector<void*>(0xf);
    new (&set->pending_remove) base::global::CRntVector<void*>(0xf);
    set->locked = false;
}

void fn_7100896410() {
    UnkShootActivatorStatics_l896410& tu = g_unk_717272cf40;
    InitHeaderIds(&tu.ids);
    tu.field_0x30 = -1.0f;
    tu.field_0x34 = -1.0f;
    tu.field_0x38 = true;
    InitId(&g_unk_717272c418, "SHOOT_ACTIVATOR");
    InitPropertyVtableFirst(&g_unk_717272c420, g_unk_7101a7c770, g_unk_71726db788, "Used", TypeOf<fn_710000d228>(),
                            fn_7100893ae8, fn_710088d2c4);
    fn_710088d340();
    fn_710009ec00(g_unk_717272c448, tu.field_0x50);
    InitTunable(&g_unk_717272c450, fn_7100136ecc, g_unk_7101a7c7c8, fn_7100075104);
    fn_710088daec();
    fn_710009ec00(g_unk_717272c481, tu.field_0x188);
    {
        CStrId n("SActivatabledOnEventInfo", true);
        fn_710088e088(&g_unk_717272c488, n, nullptr, nullptr, fn_710088df40, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_717272c488, g_unk_7101a19000);
    fn_710009ec00(g_unk_717272c5c0, &g_unk_717272c488);
    fn_710009ec00(g_unk_717272c5c1, const_cast<base::reflection::CType*>(fn_710088e20c()));
    fn_710009ec00(g_unk_717272c5c2, const_cast<base::reflection::CType*>(fn_710088e2e8()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SActivatabledOnEventInfo>>::"
                 "TKeyElementIterator::value_type",
                 true);
        fn_710088e748(&g_unk_717272c5c8, n, &g_unk_7101d5e2e0, nullptr, fn_710088e3c4, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_717272c5c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717272c700, &g_unk_717272c5c8);
    fn_710009ec00(g_unk_717272c701, const_cast<base::reflection::CType*>(fn_710088e8cc()));
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SActivatabledOnEventInfo>>::"
                 "TKeyElementConstIterator::value_type",
                 true);
        fn_710088ed2c(&g_unk_717272c708, n, &g_unk_7101d5e2e0, nullptr, fn_710088e9a8, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_717272c708, g_unk_7101a19000);
    fn_710009ec00(g_unk_717272c840, &g_unk_717272c708);
    fn_710009ec00(g_unk_717272c841, const_cast<base::reflection::CType*>(fn_710088eeb0()));
    {
        CStrId n("base::global::CRntVector<SActivatabledOnEventInfo>", true);
        fn_710088ef8c(&g_unk_717272c848, n, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_717272c848, g_unk_7101a19000);
    fn_710009ec00(g_unk_717272c9b8, &g_unk_717272c848);
    fn_710088f184();
    fn_710009ec00(g_unk_717272c9b9, tu.field_0x2c0);
    {
        base::UnkNamedEntry path;
        fn_710007e704(&path, "audiopreset:system/snd/presets/pistonmag_machineryloop");
        fn_710001f074(reinterpret_cast<UnkPathIds*>(tu.field_0x40), reinterpret_cast<const UnkInlineString*>(&path));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&path));
    }
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100333544), tu.field_0x40, g_unk_7101a19000);
    base::UnkDeferredSet<void>* sets = reinterpret_cast<base::UnkDeferredSet<void>*>(g_unk_717272c9c0);
    InitListenerSet(&sets[0]);
    InitListenerSet(&sets[1]);
    fn_7100000250(fn_710088fae8, g_unk_717272c9c0, g_unk_7101a19000);
}
