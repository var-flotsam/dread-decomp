// CMorphBallMovement's unit (hint only): resets the movement's transient state and stops its effects and
// material transitions. Names and layouts are placeholders.

#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/UnkSoundHandle.h"

extern const char g_unk_7172739498[] __attribute__((visibility("hidden")));

struct UnkBallWorldOwner_l947dc4 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkBallHash_l947dc4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct __attribute__((packed)) UnkBallMovement_l947dc4 {
    u8 pad_0x0[0x30];
    UnkBallWorldOwner_l947dc4* owner;
    u8 pad_0x38[0x8];
    u64 field_0x40;
    u32 field_0x48;
    u8 pad_0x4c[0x30];
    u32 field_0x7c;
    u8 pad_0x80[0x8c];
    u64 field_0x10c;
    u32 field_0x114;
    u8 pad_0x118[0x5d0];
    u64 field_0x6e8;
    u32 field_0x6f0;
    u8 pad_0x6f4[0x1b94];
    s32 field_0x2288;
    u8 pad_0x228c[0xb4];
    UnkSoundHandle field_0x2340;
    u8 pad_0x2350[0x60];
    u8 field_0x23b0;
    u8 pad_0x23b1[0x7];
    u32 field_0x23b8;
    u8 pad_0x23bc[0x8];
    u64 field_0x23c4;
    u8 pad_0x23cc[0xa];
    u8 field_0x23d6;
    u8 field_0x23d7;
    u8 field_0x23d8;
    u8 pad_0x23d9[0xb0];
    u8 field_0x2489;
    u8 field_0x248a;
    u8 pad_0x248b[0x11];
    u64 field_0x249c[2];
    u8 pad_0x24ac[0x8];
    u32 field_0x24b4;
    u8 pad_0x24b8[0x78];
    u32 field_0x2530;
    u8 pad_0x2534[0x6];
    u8 field_0x253a;
    u8 pad_0x253b[0x4d];
    UnkBallHash_l947dc4* field_0x2588;
    u8 pad_0x2590[0xb4];
    u8 field_0x2644;
    u8 pad_0x2645[0x3];
    f32 field_0x2648;
    u8 pad_0x264c[0xc];
    u32 field_0x2658;
};

void fn_71010da468(UnkBallMovement_l947dc4* self, void* arg);

void fn_7100947dc4(UnkBallMovement_l947dc4* self, void* arg) {
    fn_710114fe8c(self->owner->world, self->field_0x2588->hash, 0, 0);
    fn_7100d7c48c(self->owner, const_cast<char*>(g_unk_7172739498), 0.0f, 0.0f);
    self->field_0x2340 = g_unk_7108b6c460;
    fn_71010da468(self, arg);
    self->field_0x253a = 0;
    if (self->field_0x2644) {
        self->field_0x2644 = 0;
        self->field_0x2530 = 0;
        self->field_0x2648 = -1.0f;
    }
    self->field_0x23b8 = 0;
    self->field_0x2658 = 0;
    self->field_0x23d6 = 1;
    self->field_0x23c4 = 0;
    self->field_0x23d7 = 0;
    self->field_0x23d8 = 0;
    if (self->field_0x2288 != 3)
        self->field_0x7c = 0;
    self->field_0x24b4 = 0;
    self->field_0x6e8 = 0;
    self->field_0x6f0 = 0;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x10c = 0;
    self->field_0x114 = 0;
    self->field_0x23c4 = 0;
    self->field_0x249c[0] = 0;
    self->field_0x249c[1] = 0;
    self->field_0x23b0 = 1;
    self->field_0x2489 = 1;
    self->field_0x248a = 1;
    UnkActor* actor = reinterpret_cast<UnkActor*>(self->owner);
    u64 key = fn_7100001570("stop_fx_notransition", 20);
    fn_7100d7c828(actor, &key, false);
    actor = reinterpret_cast<UnkActor*>(self->owner);
    key = fn_7100001570("stop_materialtransition_notransition", 36);
    fn_7100d7c828(actor, &key, false);
}
