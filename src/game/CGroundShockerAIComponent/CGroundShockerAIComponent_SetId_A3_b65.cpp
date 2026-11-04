// CGroundShockerAIComponent's unit (tools/tu_map.py): when fn_71005a514c accepts the target's +0x98,
// the target's id (+0xb8) is set to g_unk_71726e88f8 (CStrId assignment). Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGroundShockerAI_A3_b65;

struct UnkGroundShockerTarget_A3_b65 {
    u8 unk_0x0[0x98];
    void* field_0x98;
    u8 unk_0xa0[0xb8 - 0xa0];
    base::global::CStrId field_0xb8;
};

extern base::global::CStrId g_unk_71726e88f8 __attribute__((visibility("hidden")));
bool fn_71005a514c(UnkGroundShockerAI_A3_b65* self, void* arg);

void fn_71005a53fc(UnkGroundShockerAI_A3_b65* self, void* unused, UnkGroundShockerTarget_A3_b65* target) {
    if (fn_71005a514c(self, target->field_0x98))
        target->field_0xb8 = g_unk_71726e88f8;
}
