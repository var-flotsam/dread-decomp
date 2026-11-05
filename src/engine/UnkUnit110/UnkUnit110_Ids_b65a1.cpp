// Unit 110 of tools/tu_map.py: the user id and the session id kept by src/engine/SystemStartup.cpp
// (fn_7100132a00 is used by src/game/UnkPlayReport.cpp). Names are placeholders.
#include "types.h"

namespace nn::util {
struct Uuid {
    u8 data[16];
};
}  // namespace nn::util

extern u64 g_unk_710b371030[2];  // the user id
extern nn::util::Uuid g_unk_710b371058;  // the session id

struct UnkUserId_b65a1 {
    u64 words[2];
};

UnkUserId_b65a1 fn_71001329f0() {
    UnkUserId_b65a1 id = {{g_unk_710b371030[0], g_unk_710b371030[1]}};
    return id;
}

nn::util::Uuid fn_7100132a00() { return g_unk_710b371058; }
