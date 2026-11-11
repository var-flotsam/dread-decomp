// CDoorShieldLifeComponent unit (tools/tu_map.py, the name from reflection): runs the base step
// (fn_710115fe1c), passes the game manager's blackboard (+0x968) to virtual slot 0x140, then, when the
// actor at +0x30 has its flag at +0x1b5 set, sends message 0xa6a4b8f65b760be7 to what it resolves to
// (fn_71002b3b84, fn_71009c0f1c); otherwise it runs fn_71008de628. Names are placeholders.
#include "game/CLifeComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU372.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_71008de628(char* self);

void fn_71008de5a8(char* self) {
    fn_710115fe1c(self);
    void* blackboard = reinterpret_cast<char*>(g_unk_7101d47310) + 0x968;
    using Fn = void (*)(char*, void*);
    (*reinterpret_cast<Fn**>(self))[0x140 / 8](self, blackboard);
    char* actor = *reinterpret_cast<char**>(self + 0x30);
    if (!*reinterpret_cast<bool*>(actor + 0x1b5)) {
        fn_71008de628(self);
        return;
    }
    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(actor)))
        fn_71009c0f1c(target, 0xa6a4b8f65b760be7);
}
