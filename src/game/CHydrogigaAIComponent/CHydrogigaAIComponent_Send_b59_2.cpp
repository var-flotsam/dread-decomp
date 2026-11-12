// The CHydrogigaAIComponent unit. fn_71005c1ec0 sends the owner's (+0x30) sub-object at +0x220 a one
// under id 0xff53965cbab52d86 for four kinds (the empty mask for the first three, the all-flags mask for
// the last), turns off the collider the owner's collision container (+0x2a0) has under g_unk_71726eb450,
// and makes two calls of fn_7100fdcf2c on the music manager (game/sound/CMusicManager.h) with 0.2.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkVector.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

extern base::global::CStrId g_unk_71726eb450 __attribute__((visibility("hidden")));

// The music manager singleton (as src/game/sound/UnkMusicPresets.cpp declares it).
extern UnkMgr_0xf18* g_unk_71726bc560;

static inline void* MusicManager_b59a3() {
    if (!reinterpret_cast<void*&>(g_unk_71726bc560)) {
        void* m = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(m);
        reinterpret_cast<void*&>(g_unk_71726bc560) = m;
    }
    return reinterpret_cast<void*&>(g_unk_71726bc560);
}

struct UnkHydrogigaOwner2_b59a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkHydrogigaAI2_b59a3 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner2_b59a3* owner;
};

void fn_71005c1ec0(UnkHydrogigaAI2_b59a3* self) {
    void* target = self->owner->field_0x220;
    // The goto skips the declarations below, so they get no lifetime markers and keep slots of their own.
    if (!target)
        goto collider;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
    base::reflection::UnkReflTypeIdArg mode3;
    base::reflection::UnkReflTypeIdArg mask3;
    base::reflection::UnkReflTypeIdArg mode4;
    base::reflection::UnkReflTypeIdArg mask4;
    mode1.id = 1;
    mask1.id = UnkNone_SpitSend().value;
    fn_7101176690(target, 0xff53965cbab52d86, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                  &mask1);
    mode2.id = 1;
    mask2.id = UnkNone_SpitSend().value;
    fn_7101176690(target, 0xff53965cbab52d86, 0xbd794f4bf6d1d598, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                  &mask2);
    mode3.id = 1;
    mask3.id = UnkNone_SpitSend().value;
    fn_7101176690(target, 0xff53965cbab52d86, 0x853f6fad912026e1, &mode3, &g_unk_710164b958, &g_unk_7172769740,
                  &mask3);
    mode4.id = 1;
    mask4.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0xff53965cbab52d86, 0x2b145cf4efb3d2c6, &mode4, &g_unk_710164b958, &g_unk_7172769740,
                  &mask4);
collider:
    if (UnkCollisionContainer* container = self->owner->field_0x2a0) {
        if (UnkCollisionObject* object = fn_71008640a4(container, &g_unk_71726eb450))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), false);
    }
    fn_7100fdcf2c(MusicManager_b59a3(), 0, 1, 0.2f);
    fn_7100fdcf2c(MusicManager_b59a3(), 1, 0, 0.2f);
}
