// CChozoCommanderZeroLaserGroundedAttack's unit (tools/tu_map.py): the attack's start: after the base
// (fn_710042bb3c) clears +0x58 and +0x28, picks a random 1..3 for +0x60, clears +0x64..+0x83, hooks the
// callback fn_7100448f28 (link at +0xa0, handle at +0xb8) up to the object the owner (+0x8) tracks at +0x220
// (as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp) and sends the request
// 0x8f4e8a524ebacc1b (fn_7100426b54, mode 0). Names and layouts are placeholders.
#include "game_s2/UnkChozoCommanderAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSelf;
void fn_710042bb3c(UnkSelf* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_7100448f28();
int fn_71012395a0();

struct UnkZeroLaserTrackLink_A2_b72 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkZeroLaserAttack_A2_b72 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x18];
    u32 field_0x28;
    u8 pad_0x2c[0x2c];
    u64 field_0x58;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u8 pad_0x74[0xc];
    u32 field_0x80;
    u8 pad_0x84[0x1c];
    UnkZeroLaserTrackLink_A2_b72 link;  // 0xa0
    UnkWeakHandle* handle;              // 0xb8
};

void fn_71004467a4(UnkZeroLaserAttack_A2_b72* self) {
    fn_710042bb3c(reinterpret_cast<UnkSelf*>(self));
    self->field_0x58 = 0;
    self->field_0x28 = 0;
    self->field_0x60 = (fn_71012395a0() & 0x7fff) % 3 + 1;
    self->field_0x80 = 0;
    self->field_0x6c = 0;
    self->field_0x70 = 0;
    self->field_0x64 = 0;
    self->field_0x68 = 0;
    UnkZeroLaserTrackLink_A2_b72* link = &self->link;
    link->context = self;
    link->callback = fn_7100448f28;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    if (track) {
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    fn_7100426b54(reinterpret_cast<UnkChozoCommanderAttack*>(self), 0x8f4e8a524ebacc1bull, false);
}
