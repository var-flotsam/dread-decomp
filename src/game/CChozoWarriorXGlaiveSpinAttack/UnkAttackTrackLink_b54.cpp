// Attack "start" overrides that call their base, then hook a callback up to the object the owner
// tracks: CChozoWarriorXGlaiveSpinAttack fn_71004817f4, CCooldownXBossReaperAttack fn_71004c98dc,
// CKraidFierceSwipeAttack fn_71005f420c. Same shape as fn_7100932b48 (CMenuAnimationChangeComponent).
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CCooldownXBossAttack.h"
#include "game/CKraidAttack.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf;

void fn_7100494c90(char* self);
void fn_7100482254();
void fn_71004ca35c();
void fn_71005f4d14();

namespace {

struct UnkTrackLink {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

template <u32 Off>
struct UnkTrackSelf {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[Off - 0x10];
    UnkTrackLink link;
    UnkWeakHandle* handle;
};

template <u32 LinkOff>
inline void UnkAttachTrackLink(char* raw, void (*callback)()) {
    UnkTrackSelf<LinkOff>* self = reinterpret_cast<UnkTrackSelf<LinkOff>*>(raw);
    UnkTrackLink* link = &self->link;
    link->context = self;
    link->callback = callback;
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

}  // namespace

void fn_71004817f4(char* self) {
    fn_7100494c90(self);
    UnkAttachTrackLink<0x50>(self, fn_7100482254);
}

void fn_71004c98dc(char* self) {
    fn_71004c1944(reinterpret_cast<UnkSelf*>(self));
    UnkAttachTrackLink<0x70>(self, fn_71004ca35c);
}

void fn_71005f420c(char* self) {
    fn_71005f2018(self);
    UnkAttachTrackLink<0x38>(self, fn_71005f4d14);
}
