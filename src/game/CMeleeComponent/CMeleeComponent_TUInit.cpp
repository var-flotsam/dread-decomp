// CMeleeComponent's static initialiser: the unit header, animation and trail names, the block
// effect's file, the EMeleeTrailFXType enum and the class's registration.
#include "base/global/CFilePathStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using base::global::CFilePathStrId;

// The unit's data (its statics, merged into one block).
struct UnkMeleeTU {
    UnkTUHeader header;
    CStrId mb2;                    // 0x18
    CStrId climb2;                 // 0x20
    CFilePathStrId block_fx;       // 0x28
    CStrId melee_run;              // 0x30
    CStrId melee_mb_run;           // 0x38
    CStrId melee_trail;            // 0x40
    CStrId melee_trail_dash;       // 0x48
    CStrId melee_trail_end;        // 0x50
    CStrId melee_trail_end_dash;   // 0x58
    u8 field_0x60[0x290 - 0x60];
    UnkReflType type;              // 0x290
};

extern UnkMeleeTU g_unk_7172738588 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_7172738458;  // also named in src/reflection/
void fn_710092872c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71009285e0(void* e);  // src/clones/clones_03.cpp
extern char g_unk_7172738570;
extern char g_unk_7172738571;

// The registration helpers see a type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(t);
}

void fn_7100932100() {
    UnkMeleeTU& tu = g_unk_7172738588;
    InitHeader(tu.header);
    new (&tu.mb2) CStrId("mb2", true);
    fn_7100000250(fn_710000051c, &tu.mb2, &g_unk_7101a19000);
    new (&tu.climb2) CStrId("climb2", true);
    fn_7100000250(fn_710000051c, &tu.climb2, &g_unk_7101a19000);
    new (&tu.block_fx) CFilePathStrId("system/fx/generic/block.bcptl", true);
    fn_7100000250(fn_7100000c7c, &tu.block_fx, &g_unk_7101a19000);
    new (&tu.melee_run) CStrId("meleerun", true);
    fn_7100000250(fn_710000051c, &tu.melee_run, &g_unk_7101a19000);
    new (&tu.melee_mb_run) CStrId("meleembrun", true);
    fn_7100000250(fn_710000051c, &tu.melee_mb_run, &g_unk_7101a19000);
    new (&tu.melee_trail) CStrId("meleetrail", true);
    fn_7100000250(fn_710000051c, &tu.melee_trail, &g_unk_7101a19000);
    new (&tu.melee_trail_dash) CStrId("meleetraildash", true);
    fn_7100000250(fn_710000051c, &tu.melee_trail_dash, &g_unk_7101a19000);
    new (&tu.melee_trail_end) CStrId("meleetrailend", true);
    fn_7100000250(fn_710000051c, &tu.melee_trail_end, &g_unk_7101a19000);
    new (&tu.melee_trail_end_dash) CStrId("meleetrailenddash", true);
    fn_7100000250(fn_710000051c, &tu.melee_trail_end_dash, &g_unk_7101a19000);
    InitEnum(AsUnk(&g_unk_7172738458), "CMeleeComponent::EMeleeTrailFXType", fn_710092872c,
             reinterpret_cast<void (*)()>(fn_71009285e0));
    fn_710009ec00(&g_unk_7172738570, AsUnk(&g_unk_7172738458));
    fn_71009288b0();
    fn_710009ec00(&g_unk_7172738571, &tu.type);
}
