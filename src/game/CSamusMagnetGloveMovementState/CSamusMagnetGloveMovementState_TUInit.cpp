// The static initialiser of CSamusMagnetGloveMovementState's unit: the unit header, CParams's
// registration, the ESurfaceType enum with its converters to and from int, the tunable's
// registration and the tunable itself, and the state's names and looping vibration file.
#include "base/global/CFilePathStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

using base::global::CFilePathStrId;

// The unit's data (its statics, merged into one block).
struct UnkSamusMagnetGloveTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    CStrId melee_magnet_wall;       // 0x28
    CStrId melee_magnet_ceiling;    // 0x30
    CFilePathStrId loop_vibration;  // 0x38
    CStrId grapple_init_angle;      // 0x40
    UnkReflType params_type;        // 0x48
    u8 field_0x48_rest[0x180 - 0x48 - sizeof(UnkReflType)];
    UnkReflType tunable_type;       // 0x180
};

extern UnkSamusMagnetGloveTU g_unk_717273c6b8 __attribute__((visibility("hidden")));
extern char g_unk_717273c4e0;
extern base::reflection::CType g_unk_717273c4e8;  // also named in src/reflection/
void fn_71009a4b9c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71009a49f0(void* e);  // src/clones/clones_03.cpp
extern char g_unk_717273c600;
extern UnkConverter g_unk_717273c608;
extern UnkConverter g_unk_717273c628;
void fn_71009af32c() __attribute__((visibility("hidden")));
void fn_71009af338() __attribute__((visibility("hidden")));
extern char g_unk_717273c648;
extern UnkTunable g_unk_717273c650;
extern char g_unk_7101a87688[];

// The registration helpers see a type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(t);
}

void fn_71009afa90() {
    UnkSamusMagnetGloveTU& tu = g_unk_717273c6b8;
    InitHeader(tu.header);
    fn_71009a4774();
    fn_710009ec00(&g_unk_717273c4e0, &tu.params_type);
    InitEnum(AsUnk(&g_unk_717273c4e8), "CSamusMagnetGloveMovementState::ESurfaceType", fn_71009a4b9c,
             reinterpret_cast<void (*)()>(fn_71009a49f0));
    fn_710009ec00(&g_unk_717273c600, AsUnk(&g_unk_717273c4e8));
    InitEnumConverters(AsUnk(&g_unk_717273c4e8), g_unk_717273c608, fn_71009af32c, g_unk_717273c628, fn_71009af338);
    fn_71009a4d20();
    fn_710009ec00(&g_unk_717273c648, &tu.tunable_type);
    InitTunable(&g_unk_717273c650, fn_7100136ecc, reinterpret_cast<void**>(g_unk_7101a87688), fn_7100075104);
    new (&tu.melee_magnet_wall) CStrId("meleemagnetwall", true);
    fn_7100000250(fn_710000051c, &tu.melee_magnet_wall, &g_unk_7101a19000);
    new (&tu.melee_magnet_ceiling) CStrId("meleemagnetceiling", true);
    fn_7100000250(fn_710000051c, &tu.melee_magnet_ceiling, &g_unk_7101a19000);
    new (&tu.loop_vibration) CFilePathStrId("props/magnetplatform/platformmagnetbig_loop.bnvib", true);
    fn_7100000250(fn_7100000c7c, &tu.loop_vibration, &g_unk_7101a19000);
    new (&tu.grapple_init_angle) CStrId("GrappleInitAngle", true);
    fn_7100000250(fn_710000051c, &tu.grapple_init_angle, &g_unk_7101a19000);
}
