// CAIManager's unit (tools/tu_map.py; the name from reflection): the packed list at +8 (u32 count,
// then pointers from +0xc) is scanned for the highest fn_710054ac7c score among entries whose
// object (+0x10) has its byte at +0x14b80 set and whose time stamp at +0x14b94 is no older than the
// frame clock's (+0x2c of the frame source, within FLT_EPSILON). The winner's score goes to
// fn_7100dae6dc twice: with the game manager's fn_7100defcd4 result, then with the target of its
// handle at +0x29a0. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkOwner;
struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;

namespace {

struct UnkScored_A6b73 {
    u8 pad_0x0[0x10];
    char* object;  // 0x10

    char* Get() { return this ? object : nullptr; }
};

}  // namespace

s32 fn_710054ac7c(char* object);
void fn_7100dae6dc(char* self, s32 score, void* target);

void fn_7100dada0c(char* self) {
    s32 best = 0;
    u64 bytes = (u64)*reinterpret_cast<u32*>(self + 8) * 8;
    asm("" : "+r"(bytes));  // the original counts the byte size down
    UnkScored_A6b73** it = reinterpret_cast<UnkScored_A6b73**>(self + 0xc);
    for (; bytes; ++it, bytes -= 8) {
        if (!(*it)->object[0x14b80])
            continue;
        if (fn_710054ac7c((*it)->Get()) <= best)
            continue;
        float now = *reinterpret_cast<float*>(reinterpret_cast<char*>(g_unk_7101d47328) + 0x2c);
        if (now - *reinterpret_cast<float*>((*it)->object + 0x14b94) <= 1.1920929e-7f)
            best = fn_710054ac7c((*it)->Get());
    }
    fn_7100dae6dc(self, best, fn_7100defcd4(g_unk_7101d47310));
    char* handle = *reinterpret_cast<char**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x29a0);
    fn_7100dae6dc(self, best, handle ? *reinterpret_cast<void**>(handle + 0x10) : nullptr);
}
