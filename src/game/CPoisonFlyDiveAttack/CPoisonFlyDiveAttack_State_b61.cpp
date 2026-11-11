// The CPoisonFlyDiveAttack unit: sets the state (+0x54). State 2 sends the owner's (+0x8) character
// class the track of g_unk_71726f98b0 (its +0x30), state 6 track 0x57283d1470fff249, both with mode 0
// (as src/game_s2/UnkSpitSend.h); state 4 hands the owner's +0x298 a kind-5 block (game_s2/UnkParams.h)
// carrying the vector at +0x48; state 5 goes on in fn_7100631c90. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CPoisonFlyDiveAttack.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkPoisonFlyTrack_b61 {
    u8 pad_0x0[0x30];
    u64 hash;
};
extern UnkPoisonFlyTrack_b61* g_unk_71726f98b0 __attribute__((visibility("hidden")));

// The kind-5 block (game_s2/UnkParams.h) with its first fields seen as words.
struct UnkPoisonFlyParams_b61 {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkPoisonFlyDive_b61 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x48 - 0x10];
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 state;
};

void fn_7100632204(UnkPoisonFlyDive_b61* self, u32 state) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mode1;
    UnkPoisonFlyParams_b61 params;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
start:
    self->state = state;
    UnkActor* owner = self->owner;
    void* cls = owner->field_0x220;
    switch (state) {
    case 2: {
        u64 hash = g_unk_71726f98b0->hash;
        mode1.id = 0;
        mask1.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740, &mask1);
        break;
    }
    case 4: {
        void* target = owner->field_0x298;
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        params.field_0x8 = self->field_0x48;
        params.field_0xc = self->field_0x4c;
        params.field_0x10 = self->field_0x50;
        params.field_0x14 = 1;
        fn_71006f4b0c(target, &params, 1);
        break;
    }
    case 5:
        fn_7100631c90(self);
        break;
    case 6: {
        mode2.id = 0;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, 0x57283d1470fff249, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                      &mask2);
        break;
    }
    }
}
