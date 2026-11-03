// CEmmyAIComponent unit (tools/tu_map.py, the name from reflection): whether `part` is named
// "collision_head" (its string id at +0x20 against a function-local static id). Same shape as
// fn_71008ca968. Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkEmmyCollisionPart {
    u8 unk_0x0[0x20];
    UnkStringData* name;  // 0x20
};

bool fn_7100550e88(void*, const UnkEmmyCollisionPart* part) {
    static CStrId s_head("collision_head", true);
    return part && part->name == s_head.data;
}
