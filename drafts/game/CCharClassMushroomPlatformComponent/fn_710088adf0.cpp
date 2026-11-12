// CCharClassMushroomPlatformComponent's unit: when the platform is hit, switches its state machine to
// the state numbered 4 and asks the owner's target for a result. Names and layouts are placeholders.
#include "types.h"

struct UnkMushState_l88adf0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous_id);
    virtual void vf_0x18(s32 next_id);
    s32 id() const { return id_; }
    u8 pad_0x8[8];
    s32 id_;
};

struct UnkMushMachine_l88adf0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkMushState_l88adf0* previous, UnkMushState_l88adf0* next);
    virtual void vf_0x28(UnkMushState_l88adf0* current, UnkMushState_l88adf0* next);
};

struct UnkMushTarget_l88adf0 {
    void** vtable;
    u8 pad_0x8[0x2d8];
    u8 link_0x2e0[1];
    // Virtual slot 0x168: handles the hit and returns a result.
    s32 Hit(void* arg) { return reinterpret_cast<s32 (*)(UnkMushTarget_l88adf0*, void*)>(vtable[0x168 / 8])(this, arg); }
};

struct UnkMushOwner_l88adf0 {
    u8 pad_0x0[0x2c8];
    UnkMushTarget_l88adf0* target;
};

struct UnkMushLink_l88adf0;
void fn_710088c874(UnkMushLink_l88adf0* list, UnkMushLink_l88adf0** slot);
void fn_710088d004(void* target_link, UnkMushLink_l88adf0** slot);

struct UnkMushHitter_l88adf0 {
    u8 pad_0x0[0xb4];
    u8 flags;
};

struct __attribute__((packed)) UnkMushPlatform_l88adf0 {
    u8 pad_0x0[0x30];
    UnkMushOwner_l88adf0* owner;
    u8 pad_0x38[0x228];
    UnkMushMachine_l88adf0 machine;  // 0x260
    u8 pad_0x268[8];
    UnkMushState_l88adf0* current;   // 0x270
    UnkMushState_l88adf0* previous;  // 0x278
    u32 state_count;                 // 0x280
    UnkMushState_l88adf0* states[1]; // 0x284 (packed)
    u8 pad_0x28c[0x304 - 0x28c];
    s32 hit_timer;                   // 0x304
    u8 keep_state;                   // 0x308
    u8 pad_0x309[0x508 - 0x309];
    UnkMushLink_l88adf0* link;       // 0x508
};

s32 fn_710088adf0(UnkMushPlatform_l88adf0* self, UnkMushHitter_l88adf0* hitter) {
    UnkMushState_l88adf0* current = self->current;
    if (!current || (u32)(current->id() - 1) >= 2 || !(hitter->flags & 2))
        return 0;
    self->hit_timer = 4;
    UnkMushOwner_l88adf0* owner;
    if (self->keep_state) {
        owner = self->owner;
    } else {
        if (current->id() != 4 && self->state_count != 0) {
            UnkMushState_l88adf0* const* it = self->states;
            for (u64 left = (u64)self->state_count << 3; left != 0; left -= 8, ++it) {
                UnkMushState_l88adf0* found = *it;
                if (found->id() != 4)
                    continue;
                if (found) {
                    current->vf_0x18(4);
                    self->machine.vf_0x28(self->current, found);
                    UnkMushState_l88adf0* old = self->current;
                    self->current = found;
                    self->previous = old;
                    found->vf_0x10(old ? old->id() : 0x7fffffff);
                    self->machine.vf_0x20(self->previous, found);
                }
                break;
            }
        }
        self->hit_timer = 0x7fffffff;
        owner = self->owner;
    }
    if (!owner || !owner->target)
        return 0;
    UnkMushTarget_l88adf0* target = owner->target;
    s32 result = target->Hit(hitter);
    UnkMushLink_l88adf0** slot = &self->link;
    if (*slot) {
        fn_710088c874(*slot, slot);
        *slot = nullptr;
    }
    *slot = reinterpret_cast<UnkMushLink_l88adf0*>(target->link_0x2e0);
    fn_710088d004(target->link_0x2e0, slot);
    if (result != 0)
        return result;
    return 0;
}
