// CEmmyAIComponent unit (tools/tu_map.py): fn_710054abf4 asks whether the player is busy (its part at +0x2a8
// flagged or answering fn_710083eb28, the game manager's fn_7100df59a8, or the part at +0x2e0 answering
// fn_7100300cf4), and otherwise answers whether fn_7100df5740 is false. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;

struct UnkPlayerPart_b62a3 {
    u8 pad_0x0[0x178];
    bool field_0x178;
};
struct UnkPlayerPart2_b62a3;

struct UnkPlayer_b62a3 {
    u8 pad_0x0[0x2a8];
    UnkPlayerPart_b62a3* field_0x2a8;
    u8 pad_0x2b0[0x30];
    UnkPlayerPart2_b62a3* field_0x2e0;
};

bool fn_710083eb28(UnkPlayerPart_b62a3*);
bool fn_7100df59a8(UnkGameManager*);
bool fn_7100300cf4(UnkPlayerPart2_b62a3*);

bool fn_710054abf4(void*) {
    auto* player = reinterpret_cast<UnkPlayer_b62a3*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        UnkPlayerPart_b62a3* part = player->field_0x2a8;
        if (part) {
            if (part->field_0x178 || fn_710083eb28(part) || fn_7100df59a8(g_unk_7101d47310))
                return true;
        }
        if (player->field_0x2e0 && fn_7100300cf4(player->field_0x2e0))
            return true;
    }
    return !fn_7100df5740(g_unk_7101d47310);
}
