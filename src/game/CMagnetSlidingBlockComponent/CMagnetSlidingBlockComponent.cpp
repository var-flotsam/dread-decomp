// CMagnetSlidingBlockComponent's static initialiser: the unit header, the component's id, one bool
// property, the class's registration, four names and the looping vibration file.
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

using base::global::CFilePathStrId;

// The unit's data (its statics, merged into one block).
struct UnkMagnetSlidingBlockTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId magnet;               // 0x20
    CStrId logic;                // 0x28
    CStrId logic_2;              // 0x30
    CStrId finished;             // 0x38
    CFilePathStrId loop_vibration;  // 0x40
    UnkReflType type;            // 0x48
};

extern UnkMagnetSlidingBlockTU g_unk_71727375b0 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172737528;
extern UnkProperty g_unk_7172737530;
extern char g_unk_7101a82b50[];
void fn_71009217a4(UnkProperty* p);
void fn_710091d7d0(void* p);
extern char g_unk_7172737558;

void fn_7100921cb0() {
    UnkMagnetSlidingBlockTU& tu = g_unk_71727375b0;
    InitHeader(tu.header);
    new (&g_unk_7172737528) CStrId("MAGNET_SLIDING_BLOCK", true);
    fn_7100000250(fn_710000051c, &g_unk_7172737528, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_7172737530, reinterpret_cast<void**>(g_unk_7101a82b50), g_unk_71726db788,
                            "FinishedMovement", refl_TypeOf<fn_710000d228>(), fn_71009217a4, fn_710091d7d0);
    fn_710091d84c();
    fn_710009ec00(&g_unk_7172737558, &tu.type);
    new (&tu.magnet) CStrId("magnet", true);
    fn_7100000250(fn_710000051c, &tu.magnet, &g_unk_7101a19000);
    new (&tu.logic) CStrId("MagnetSlidingBlockComponent Logic", true);
    fn_7100000250(fn_710000051c, &tu.logic, &g_unk_7101a19000);
    new (&tu.logic_2) CStrId("MagnetSlidingBlockComponent Logic", true);
    fn_7100000250(fn_710000051c, &tu.logic_2, &g_unk_7101a19000);
    new (&tu.finished) CStrId("finished", true);
    fn_7100000250(fn_710000051c, &tu.finished, &g_unk_7101a19000);
    new (&tu.loop_vibration) CFilePathStrId("props/magnetplatform/platformmagnetbig_loop.bnvib", true);
    fn_7100000250(fn_7100000c7c, &tu.loop_vibration, &g_unk_7101a19000);
}
