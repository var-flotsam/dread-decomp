// CScourgeTongueSlashAttack unit (tools/tu_map.py unit 1031): the update (fn_710069b338). After the base
// part (fn_71003609a8) the requested state (+0x34) becomes the current one (+0x30); leaving state 5 first
// tells the owner's collision container (+0x2a0) the id at g_unk_7172704968. Then fn_710069afac, and in
// states 0 and 1 a switch to state 2 with the tuning's time (+0xec) as the timer (+0x60); in state 3 the
// timer runs down, and fn_710069b410 runs once it is spent. Names and layouts are placeholders.
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

extern const base::global::CStrId g_unk_7172704968 __attribute__((visibility("hidden")));

struct UnkTuning_st {
    u8 pad_0x0[0xec];
    f32 time;
};

struct UnkOwner_st {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkScourgeTongueSlashAttack {
    u8 pad_0x0[0x8];
    UnkOwner_st* owner;
    u8 pad_0x10[0x18];
    UnkTuning_st* tuning;  // 0x28
    u32 state;  // 0x30
    u32 next_state;  // 0x34
    u8 pad_0x38[0x28];
    f32 timer;  // 0x60
};

void fn_710069afac(UnkScourgeTongueSlashAttack* self, u32 unused);
void fn_710069b410(UnkScourgeTongueSlashAttack* self);

void fn_710069b338(UnkScourgeTongueSlashAttack* self, f32 delta) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), delta);
    u32 state = self->state;
    u32 next = self->next_state;
    if (state != next) {
        if (state == 5 && self->owner->collision) {
            fn_7100863a8c(self->owner->collision, &g_unk_7172704968);
            next = self->next_state;
        }
        self->state = next;
    }
    fn_710069afac(self, 0);
    switch (self->state) {
    case 0:
    case 1:
        self->next_state = 2;
        self->timer = self->tuning->time;
        break;
    case 3:
        if (self->timer > 0.001f)
            self->timer -= delta;
        else
            fn_710069b410(self);
        break;
    }
}
