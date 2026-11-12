// CPlayerInfo unit (tools/tu_map.py unit 2592): fn_7100f0be58 sets (fn_710117662c) or resets
// (fn_7101178e08) entry 1 on the target at +0x220 of the player (fn_7100defcd4) and of the object at
// +0x10 of the game manager's +0x29a0 entry. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkOwner;
struct UnkActorTarget;
struct UnkPlayerInfo_ovb0071;
void fn_710117662c(UnkActorTarget* target, u32 a, u32 b, u32 c, u32 d, f32 value);
void fn_7101178e08(UnkActorTarget* target, u32 a, u32 b, u32 c, f32 value);

static inline void Apply_ovb0071(UnkActorTarget* target, bool flag) {
    if (flag)
        fn_710117662c(target, 1, 0, 1, 1, -1.0f);
    else
        fn_7101178e08(target, 1, 1, 1, -1.0f);
}

void fn_7100f0be58(UnkPlayerInfo_ovb0071* self, bool flag) {
    if (char* player = reinterpret_cast<char*>(fn_7100defcd4(g_unk_7101d47310)))
        Apply_ovb0071(*reinterpret_cast<UnkActorTarget**>(player + 0x220), flag);
    char* entry = *reinterpret_cast<char**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x29a0);
    if (entry) {
        if (char* owner = *reinterpret_cast<char**>(entry + 0x10))
            Apply_ovb0071(*reinterpret_cast<UnkActorTarget**>(owner + 0x220), flag);
    }
}
