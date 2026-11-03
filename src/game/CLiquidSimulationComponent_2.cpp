// CLiquidSimulationComponent's static initialiser: the unit header, a default pair of -1.0 values
// and a flag in the unit's data, the class's registration and its id.
#include "base/UnkStrIdRegistry.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_71727344b0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71727344d0 __attribute__((visibility("hidden")));
extern char g_unk_71727344a0;

void fn_7100906440() {
    InitHeader(g_unk_71727344b0);
    TUAt<float, 0x38>(g_unk_71727344b0)[0] = -1.0f;
    TUAt<float, 0x38>(g_unk_71727344b0)[1] = -1.0f;
    *TUAt<bool, 0x40>(g_unk_71727344b0) = true;
    fn_71009058b4();
    fn_710009ec00(&g_unk_71727344a0, TUType<0x48>(g_unk_71727344b0));
    new (TUAt<CStrId, 0x20>(g_unk_71727344b0)) CStrId(fn_7100de8f78(CStrId("CLiquidSimulationComponent", true)));
    fn_7100000250(fn_710000051c, &g_unk_71727344d0, &g_unk_7101a19000);
}
