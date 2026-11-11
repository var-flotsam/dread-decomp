// A music trigger in unit 2538 of tools/tu_map.py (no class name known): its destructors (its vtable,
// its string (0x68) and file path id (0x58) destroyed, then the music trigger base's destructor
// fn_71011867d4), its path setter, its activation and a flag toggle. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2725.h"
#include "game/UnkTU2730.h"
#include "game/nn/time.h"
#include "types.h"

extern char g_unk_7101aa91c0[];  // the trigger's vtable

void fn_7100da635c(char* self);

struct UnkMusicTrigger2538_b44a3;
template <>
struct base::UnkDtorTraits<UnkMusicTrigger2538_b44a3> {
    static char* vtable() { return g_unk_7101aa91c0; }
    static void base(char* self) { fn_71011867d4(self); }
};

__attribute__((flatten)) void fn_7100da619c(char* self) {
    base::refl_Dtor<UnkMusicTrigger2538_b44a3, base::UnkMemberAt<base::global::CRntString, 0x68>,
                    base::UnkMemberAt<base::UnkStrIdMemberOf<g_unk_7101d47200>, 0x58>>(self);
}

// The deleting form.
__attribute__((flatten)) void fn_7100da621c(char* self) {
    base::refl_DeletingDtor<UnkMusicTrigger2538_b44a3, base::UnkMemberAt<base::global::CRntString, 0x68>,
                            base::UnkMemberAt<base::UnkStrIdMemberOf<g_unk_7101d47200>, 0x58>>(self);
}

// Toggles the flag at 0x50 and returns whether it was clear.
bool fn_7100da6724(char* self) {
    bool old = *reinterpret_cast<bool*>(self + 0x50);
    *reinterpret_cast<bool*>(self + 0x50) = !old;
    return !old;
}

struct UnkMusicTrigger2538Fields_b44a3 {
    u8 pad_0x0[0x58];
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x58
    bool field_0x60;
};

// Sets the path and the flag; when the trigger is the game manager's mode holder, applies it.
void fn_7100da62a4(UnkMusicTrigger2538Fields_b44a3* self, const base::UnkStrIdMemberOf<g_unk_7101d47200>* path,
                   bool flag) {
    self->path = *path;
    self->field_0x60 = flag;
    // Compared as integers, and expected false: as pointers the call takes the loaded copy, and
    // the original lays the call out of line.
    if (__builtin_expect(reinterpret_cast<u64>(g_unk_7101d47310->field_0x498) == reinterpret_cast<u64>(self), 0))
        fn_7100da635c(reinterpret_cast<char*>(self));
}

struct UnkSoundHolder;
extern UnkSoundHolder* g_unk_7101d47340;
void fn_71011b3768(UnkGuiRoot* root);
extern void* g_unk_7101d47330;  // src/engine/HostArguments.cpp calls it the GUI root

// The trigger's activation: the base's step (fn_71011867ec), the sound holder created on first use, an
// "Ingame" string id built and dropped, the two GUI objects told (fn_71011b3768, fn_71011994e0 with
// true), then the trigger applied (fn_7100da635c).
void fn_7100da6524(char* self) {
    fn_71011867ec(self);
    if (!g_unk_7101d47340) {
        UnkSoundHolder* p = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(p);
        g_unk_7101d47340 = p;
    }
    base::global::CStrId("Ingame", true);
    fn_71011b3768(g_unk_717271e8f8);
    fn_71011994e0(g_unk_7101d47330, true);
    fn_7100da635c(self);
}
