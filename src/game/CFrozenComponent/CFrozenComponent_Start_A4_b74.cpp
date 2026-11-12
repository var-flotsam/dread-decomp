// CFrozenComponent unit (tools/tu_map.py, unit 1260): after the base fn_71010c4228, when the owner (+0x30) tracks an
// object at +0x220, hooks the callback fn_71007fb694 (link at +0x48, handle at +0x60) up to it, as the track-link
// family (src/game/CCooldownXBossStrongWhipAttack/CCooldownXBossStrongWhipAttack_Start_A2_b72.cpp). Names and
// layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkParent;
void fn_71007fb694();

struct UnkFrozenTrackLink_A4_b74 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkFrozenStart_A4_b74 {
    u8 pad_0x0[0x30];
    char* owner;
    u8 pad_0x38[0x10];
    UnkFrozenTrackLink_A4_b74 link;  // 0x48
    UnkWeakHandle* handle;           // 0x60
};

void fn_71007f9258(UnkFrozenStart_A4_b74* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    if (!*reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220))
        return;
    UnkFrozenTrackLink_A4_b74* link = &self->link;
    link->context = self;
    link->callback = fn_71007fb694;
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
