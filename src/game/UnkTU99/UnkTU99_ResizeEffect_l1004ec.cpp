// Unit 99 (the sound system's): if the effect of the given kind has not changed, resizes its buffer from
// the block kept at the matching member of the system. Names and layouts are placeholders.
#include <nn/os.h>
#include <nn/util.h>

#include "base/global/CFilePathStrId.h"
#include "game/UnkTU2772.h"
#include "game/UnkTU99.h"

struct UnkSystem_l1004ec {
    u8 pad_0x0[0x80];
    void* field_0x80;
    void* field_0x88;
    void* field_0x90;
    void* field_0x98;
};

void fn_710120b230(u32 zero, void* effect, void* block, u64 size);

void fn_71001004ec(UnkSystem_l1004ec* self, u32 kind) {
    nn::os::GetCurrentThread();
    switch (kind) {
    case 0:
        return;
    case 1:
        if (fn_7101234dc4(g_unk_7108b6af50) & 1)
            return;
        fn_7101234dcc(g_unk_7108b6af50, 1);
        fn_710120b230(0, g_unk_7108b6af50, self->field_0x90, fn_71012346b8(g_unk_7108b6af50));
        return;
    case 2:
        if (fn_7101234580(g_unk_7108b6afe8) & 1)
            return;
        fn_7101234588(g_unk_7108b6afe8, 1);
        fn_710120b230(0, g_unk_7108b6afe8, self->field_0x98, fn_7101234068(g_unk_7108b6afe8));
        return;
    case 3:
        if (fn_7101233f24(g_unk_7108b6adb0) & 1)
            return;
        fn_7101233f2c(g_unk_7108b6adb0, 1);
        fn_710120b230(0, g_unk_7108b6adb0, self->field_0x80, fn_7101233880(g_unk_7108b6adb0));
        return;
    case 4:
        if (fn_7101233f24(g_unk_7108b6ae38) & 1)
            return;
        fn_7101233f2c(g_unk_7108b6ae38, 1);
        fn_710120b230(0, g_unk_7108b6ae38, self->field_0x88, fn_7101233880(g_unk_7108b6ae38));
        return;
    default:
        NN_UNEXPECTED_DEFAULT;
    }
}
