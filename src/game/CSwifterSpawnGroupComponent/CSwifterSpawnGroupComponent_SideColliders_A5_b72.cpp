// CSwifterSpawnGroupComponent unit (tools/tu_map.py, unit 1524): looks up the owner's collision component
// (fn_710043acb4 on +0x30), finds its collider group named "Left" (side +0x420 == 1) or "Right"
// (fn_710086406c) and clears the flag of every entry in the group's packed list (u32 count at +0x124,
// pointers from +0x128) through fn_7100d35a98. Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CChozoCommanderSentenceSphereAttack.h"
#include "game/CCollisionComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "types.h"

struct UnkPlayerMovementSource;

#pragma pack(push, 4)
struct UnkSwifterGroup_A5_b72 {
    u8 pad_0x0[0x124];
    u32 count;
    UnkEntry* entries[1];
};
#pragma pack(pop)

struct UnkSwifterSpawnGroup_A5_b72 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x420 - 0x38];
    s32 side;
};

void fn_7100a12b24(UnkSwifterSpawnGroup_A5_b72* self) {
    auto* source = static_cast<UnkPlayerMovementSource*>(fn_710043acb4(self->owner));
    UnkSwifterGroup_A5_b72* group;
    {
        // Pinned in each arm: the original branches instead of selecting the string.
        const char* str;
        if (self->side == 1) {
            str = "Left";
            asm volatile("" : "+r"(str));
        } else {
            str = "Right";
            asm volatile("" : "+r"(str));
        }
        base::global::CStrId name(str, true);
        group = static_cast<UnkSwifterGroup_A5_b72*>(fn_710086406c(source, &name));
    }
    u64 bytes = (u64)group->count * 8;
    asm("" : "+r"(bytes));
    for (UnkEntry** it = group->entries; bytes; ++it, bytes -= 8)
        fn_7100d35a98(*it, false);
}
