// CGroundShockerAttack's unit (tools/tu_map.py unit 856): its start (fn_71005a0af4). After the base
// (fn_71003608ac) it sets the pair at +0x30 to (0, 1); when the owner tracks an object (+0x220) it
// hooks the callback fn_71005a178c up to it (link at +0x40, as in
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp); then both values at +0x38 are
// -1.0f. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_71005a178c();

struct UnkGroundShockerLink_A3_b66 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkGroundShockerAttack_A3_b66 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    UnkGroundShockerLink_A3_b66 link;  // +0x40
    UnkWeakHandle* handle;             // +0x58
};

void fn_71005a0af4(UnkGroundShockerAttack_A3_b66* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 0;
    self->field_0x34 = 1;
    if (*reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220)) {
        UnkGroundShockerLink_A3_b66* link = &self->link;
        link->context = self;
        link->callback = fn_71005a178c;
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
        if (track) {
            asm("" : "+r"(self));
            fn_7100316018(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
    self->field_0x38 = -1.0f;
    self->field_0x3c = -1.0f;
}
