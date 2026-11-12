// CChozoWarriorXSpitAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac) sets
// +0x48, the pair at +0x4c to (0, 10 degrees in radians), a random 1 or 2 at +0x54, clears +0x68..+0x8b,
// copies the empty sound handle (g_unk_7108b6c460) to +0x98 and hooks the callback fn_7100485918 (link at
// +0xa8, handle at +0xc0) up to the object the owner (+0x8) tracks at +0x220, as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2772.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_7100485918();

struct UnkSoundRef;
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};

struct UnkSpitTrackLink_A2_b72 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkSpitAttack_A2_b72 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x38];
    bool field_0x48;
    u8 pad_0x49[0x3];
    f32 field_0x4c;
    f32 field_0x50;
    u32 field_0x54;
    u8 pad_0x58[0x10];
    u64 field_0x68[2];
    u64 field_0x78[2];
    u32 field_0x88;
    u8 pad_0x8c[0xc];
    UnkSoundHandle sound;           // 0x98
    UnkSpitTrackLink_A2_b72 link;   // 0xa8
    UnkWeakHandle* handle;          // 0xc0
};

void fn_7100483be8(UnkSpitAttack_A2_b72* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x48 = true;
    self->field_0x4c = 0.0f;
    self->field_0x50 = 0.17453292f;
    self->field_0x54 = (fn_71012395a0() & 1) + 1;
    self->field_0x88 = 0;
    self->field_0x78[0] = 0;
    self->field_0x78[1] = 0;
    self->field_0x68[0] = 0;
    self->field_0x68[1] = 0;
    self->sound = g_unk_7108b6c460;
    UnkSpitTrackLink_A2_b72* link = &self->link;
    link->context = self;
    link->callback = fn_7100485918;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (!track)
        return;
    asm("" : "+r"(self));
    fn_7100316018(track, &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
