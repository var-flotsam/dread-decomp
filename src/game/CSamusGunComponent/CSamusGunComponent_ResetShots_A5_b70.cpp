// CSamusGunComponent unit (1315): when the shots are active (+0x340), clears the flag and +0x344, releases the
// reference at the start of every 0x18-byte entry of the array at +0x348 (count +0x350; count at +0x8, released
// through slot 0x10), empties it and removes the two timeline keys (g_unk_710164ad68, g_unk_710164ad60) from the
// actor at +0x30; then fn_710083acc0. Names and layouts are placeholders.
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkActor;

extern u64 g_unk_710164ad60;
extern u64 g_unk_710164ad68;

struct UnkGunShot_A5_b70 {
    UnkRefObject* ref;
    u8 pad_0x8[0x10];
};

struct UnkSamusGun_A5_b70 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x308];
    bool active;               // 0x340
    u32 field_0x344;           // 0x344
    UnkGunShot_A5_b70* shots;  // 0x348
    u32 count;                 // 0x350
};

void fn_710083acc0(UnkSamusGun_A5_b70* self, void* arg);

void fn_710085baa4(UnkSamusGun_A5_b70* self, void* arg) {
    if (self->active) {
        self->active = false;
        self->field_0x344 = 0;
        for (u64 i = 0; i < self->count; ++i) {
            if (UnkRefObject* ref = self->shots[i].ref) {
                if (--ref->ref_count == 0)
                    ref->vf_0x10();
            }
        }
        self->count = 0;
        if (self->actor) {
            fn_7100d7c94c(self->actor, &g_unk_710164ad68);
            fn_7100d7c94c(self->actor, &g_unk_710164ad60);
        }
    }
    fn_710083acc0(self, arg);
}
