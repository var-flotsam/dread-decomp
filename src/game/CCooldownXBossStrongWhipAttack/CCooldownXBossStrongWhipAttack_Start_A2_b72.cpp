// CCooldownXBossStrongWhipAttack's unit (tools/tu_map.py): the attack's start, as fn_71004c98dc
// (src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp): after fn_71004c1944 and setting +0x70
// to -1, hooks the callback fn_71004cb0e8 (link at +0x78, handle at +0x90) up to the object the owner (+0x8)
// tracks at +0x220. Names and layouts are placeholders.
#include "game/CCooldownXBossAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSelf;
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_71004cb0e8();

struct UnkWhipTrackLink_A2_b72 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkWhipStart_A2_b72 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x60];
    f32 field_0x70;
    u8 pad_0x74[0x4];
    UnkWhipTrackLink_A2_b72 link;  // 0x78
    UnkWeakHandle* handle;         // 0x90
};

void fn_71004ca700(UnkWhipStart_A2_b72* self) {
    fn_71004c1944(reinterpret_cast<UnkSelf*>(self));
    self->field_0x70 = -1.0f;
    UnkWhipTrackLink_A2_b72* link = &self->link;
    link->context = self;
    link->callback = fn_71004cb0e8;
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
