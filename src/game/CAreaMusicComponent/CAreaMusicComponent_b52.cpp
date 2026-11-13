// Unit 1142 (tools/tu_map.py; hint CAreaMusicComponent): after the base step, unless the owner's flag at
// +0x1b5 is set, asks the music manager (g_unk_71726bc560, created on first use, game/sound/CMusicManager.h)
// for fn_7100fd7b44 with the values 10 and 8 and the component's fade time. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CBossAIComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
#include "base/UnkLoadingMask.h"

// The channel mask as src/game/UnkLoadingScreen.cpp defines it (passed through a copy).
void fn_7100718388(void* self, void* arg);

struct UnkAreaMusicOwner {
    u8 unk_0x0[0x1b5];
    u8 field_0x1b5;
};
struct UnkAreaMusicComponent {
    u8 unk_0x0[0x30];
    UnkAreaMusicOwner* field_0x30;
    u8 unk_0x38[0xad8 - 0x38];
    f32 field_0xad8;
};

void fn_7100715fa8(UnkAreaMusicComponent* self, void* arg) {
    fn_7100718388(self, arg);
    if (self->field_0x30->field_0x1b5) return;
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    f32 time = self->field_0xad8;
    fn_7100fd7b44(instance, UnkLoadingMask(10), UnkLoadingMask(8), time);
}
