// Unit 36 of tools/tu_map.py: whether the worker object has a request pending (+0x9, see
// UnkUnit36.cpp) while the flag g_unk_7101d50280 is clear. Names and layouts are placeholders.
#include "UnkUnit36_Nfp.h"
#include "types.h"

// The head of the worker object, as UnkUnit36.cpp lays it out.
struct UnkUnit36WorkerHead {
    u8 pad_0x0[0x9];
    bool pending;
};

extern bool g_unk_7101d50280;

bool fn_710002cf80(UnkUnit36Static* self) {
    return reinterpret_cast<UnkUnit36WorkerHead*>(self)->pending && !g_unk_7101d50280;
}
