// GUI::CSamusMenuComposition's unit (1714): closing the menu. Its two display objects are flagged, the
// in-game menu's state set, the music and sound groups restored, and the Lua callback named by the
// menu's string id (which is cleared first) is run. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/GUI/CIngameMenu.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkSoundSystemMasks_ovb0074;
void fn_7100108464(UnkSoundSystemMasks_ovb0074* self, bool on, const u32* mask);
void fn_7100108b34(UnkSoundMgr* self, u32 group, bool on, f32 fade);
bool fn_7100fd63b0(UnkMusicManager* self, u32 index, bool flag);

struct UnkSamusMenuComposition_b6fa64 {
    u8 pad_0x0[0x238];
    CStrId callback;  // 0x238: the Lua function run on close
    u8 pad_0x240[0xd0];
    UnkGuiDisplayObject* field_0x310;
    UnkGuiDisplayObject* field_0x318;
};
static_assert(__builtin_offsetof(UnkSamusMenuComposition_b6fa64, field_0x318) == 0x318, "");

struct UnkSamusMenuCloseArgs_b6fa64 {
    u8 pad_0x0[0x1a];
    bool field_0x1a;
};

static inline UnkMgr_0xf18* MusicManager() {
    UnkMgr_0xf18* manager = g_unk_71726bc560;
    if (!manager) {
        manager = static_cast<UnkMgr_0xf18*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

bool fn_7100b6fa64(UnkSamusMenuComposition_b6fa64* self, UnkSamusMenuCloseArgs_b6fa64* args) {
    fn_71011cdc30(self->field_0x310, true);
    fn_71011cdc30(self->field_0x318, true);
    fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(fn_7100deac84(g_unk_7101d47310)), 1, true, true);
    args->field_0x1a = true;
    fn_7100fd63b0(reinterpret_cast<UnkMusicManager*>(MusicManager()), 0, false);
    fn_7100fd63b0(reinterpret_cast<UnkMusicManager*>(MusicManager()), 1, false);
    fn_7100108b34(g_unk_7101d4d820, 5, false, 0.2f);
    alignas(8) u32 groups = 0x20;
    fn_7100108464(reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(g_unk_7101d4d820), false, &groups);
    if (self->callback.data != &g_unk_7101d47108->empty) {
        CStrId callback = self->callback;
        self->callback = g_unk_7101d470e0;
        if (fn_71010c0bb0(g_unk_7172a01d18, callback.Text())) {
            char result[0x100];
            fn_71010c0d80(g_unk_7172a01d18, 0, result, sizeof(result), nullptr, nullptr);
        }
    }
    return true;
}
