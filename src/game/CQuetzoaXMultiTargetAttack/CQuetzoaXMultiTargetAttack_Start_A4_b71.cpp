// CQuetzoaXMultiTargetAttack's unit (tools/tu_map.py unit 960): its start. After the base (fn_71003608ac) it
// clears the flag at +0x30 and the counter at +0x34, copies the value at +0xec of +0x28, picks a random
// side (+-1.0) and, when the owner tracks an object (+0x220), hooks the callback fn_710063e878 up to it
// (link at +0x40, as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp).
// Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern "C" int rand();

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_710063e878();

struct UnkQuetzoaMTLink_A4_b71 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkQuetzoaMTSource_A4_b71 {
    u8 pad_0x0[0xec];
    u32 field_0xec;
};

struct UnkQuetzoaMTAttack_A4_b71 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x18];
    UnkQuetzoaMTSource_A4_b71* source;  // +0x28
    bool field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    f32 side;                       // +0x3c
    UnkQuetzoaMTLink_A4_b71 link;   // +0x40
    UnkWeakHandle* handle;          // +0x58
};

void fn_710063d6d4(UnkQuetzoaMTAttack_A4_b71* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkQuetzoaMTSource_A4_b71* source = self->source;
    self->field_0x30 = false;
    self->field_0x34 = 0;
    self->field_0x38 = source->field_0xec;
    self->side = ((f32)(rand() & 1) - 0.5f) * 2.0f;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    if (!track)
        return;
    UnkQuetzoaMTLink_A4_b71* link = &self->link;
    link->context = self;
    link->callback = fn_710063e878;
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
