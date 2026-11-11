// CGameManager::CTunableGameManager unit: under the game manager's mutex at +0x7c8, sets the 16-bit
// flag at +0x740, hands the argument to the block at +0x788 (fn_7100fc7210) and stores the value at
// +0x780. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkGameManagerPendingBlock;
void fn_7100fc7210(UnkGameManagerPendingBlock* self, void* arg);

struct UnkGameManagerPendingView {
    u8 pad_0x0[0x740];
    u16 field_0x740;
    u8 pad_0x742[0x780 - 0x742];
    u32 field_0x780;
    u8 pad_0x784[4];
    u8 block_0x788[0x40];
    nn::os::MutexType mutex;  // 0x7c8
};
static_assert(__builtin_offsetof(UnkGameManagerPendingView, mutex) == 0x7c8, "layout");

void fn_7100df7468(void* arg, u32 value) {
    UnkGameManagerPendingView* gm = reinterpret_cast<UnkGameManagerPendingView*>(g_unk_7101d47310);
    fn_710006691c(&gm->mutex);
    gm->field_0x740 = 1;
    fn_7100fc7210(reinterpret_cast<UnkGameManagerPendingBlock*>(gm->block_0x788), arg);
    gm->field_0x780 = value;
    fn_7100066920(&gm->mutex);
}
