// CGunComponent unit (tools/tu_map.py, the name from reflection): picks mode 2 when the active weapon
// slot (+0x120, or +0x118 when the flag at +0x128 is set) holds something and the game manager's gun
// state (fn_7100df5460) is of kind 6 (for kind 7, its object at +0x20 decides), else mode 1; a new
// mode is stored at +0x134 and applied (fn_7100839b00).
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

void* fn_7100df5460(UnkGameManager* manager);
void fn_7100839b00(char* self, bool flag);

struct UnkGunModeSource {
    u8 unk_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkGunModeState {
    u8 unk_0x0[0x20];
    UnkGunModeSource* source;  // 0x20
    u8 unk_0x28[0x8c];
    s32 kind;  // 0xb4
};

static inline s32 GunMode(char* self) {
    u32 off = *reinterpret_cast<bool*>(self + 0x128) ? 0x118 : 0x120;
    if (*reinterpret_cast<void**>(self + off) == nullptr)
        return 1;
    auto* state = static_cast<UnkGunModeState*>(fn_7100df5460(g_unk_7101d47310));
    if (state == nullptr)
        return 1;
    s32 kind = state->kind;
    if (kind == 7) {
        if (state->source == nullptr)
            return 1;
        kind = state->source->state;
    }
    return kind == 6 ? 2 : 1;
}

void fn_7100839cc8(char* self, bool flag) {
    s32 mode = GunMode(self);
    s32* current = reinterpret_cast<s32*>(self + 0x134);
    if (*current == mode)
        return;
    *current = mode;
    fn_7100839b00(self, flag);
}
