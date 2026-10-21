// Per-frame update of a large actor-driven object: after the base update, it does nothing while
// a check on the owner passes or the owner's track is a CCharClassPushTrack. Under a
// CCharClassCrazyTrack it runs down a timer (only while a hashed condition holds) and fires
// fn_71003697f0 when it expires. Otherwise it runs a one-shot virtual the first frame after the
// flag at +6 was set, then updates an optional helper.
#include "types.h"

namespace base::reflection {
class CClass;
}

const base::reflection::CClass* fn_7100d253f8();  // CCharClassPushTrack::refl_GetClass
const base::reflection::CClass* fn_7100cf3508();  // CCharClassCrazyTrack::refl_GetClass
bool fn_710117b348(void* obj, const base::reflection::CClass* cls);
bool fn_710117adc0(void* obj, u64 hash);
bool fn_71007f9f20(void* obj);

struct UnkHelper_7100369c44;
void fn_71003aa7c4(UnkHelper_7100369c44* helper, f32 dt);

struct UnkOwner_7100369c44 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2c8 - 0x228];
    void* field_0x2c8;
};

struct UnkState_7100369c44 {
    f32 timer;
    bool field_0x4;
    bool field_0x5;
    bool field_0x6;
    u8 pad_0x7[0xd];
    UnkHelper_7100369c44* helper;
} __attribute__((packed));

class UnkSelf_7100369c44 {
public:
    virtual void vf_0x0();
    void vf_0x370_call();

    u8 pad_0x8[0x28];
    UnkOwner_7100369c44* owner;
    u8 pad_0x38[0x146ac - 0x38];
    UnkState_7100369c44 state;
};

struct UnkSelfVtbl_7100369c44 {
    void* slots[0x370 / 8];
    void (*vf_0x370)(UnkSelf_7100369c44* self);
};

void fn_71011000e0(UnkSelf_7100369c44* self, f32 dt);
void fn_71003697f0(UnkSelf_7100369c44* self);

void fn_7100369c44(UnkSelf_7100369c44* self, f32 dt) {
    UnkState_7100369c44* s = &self->state;
    fn_71011000e0(self, dt);
    void* track = self->owner->field_0x220;
    void* other = self->owner->field_0x2c8;
    bool skip = other ? fn_71007f9f20(other) : false;
    if (track) {
        bool push = fn_710117b348(track, fn_7100d253f8());
        if (skip | push)
            return;
        if (bool crazy = fn_710117b348(track, fn_7100cf3508())) {
            if (s->timer >= 0.0f && fn_710117adc0(track, 0xf4061ff25a2163d6)) {
                s->timer -= dt;
                if (s->timer < 0.0f)
                    fn_71003697f0(self);
            }
            s->field_0x6 = crazy;
            return;
        }
    } else if (skip) {
        return;
    }
    if (s->field_0x6 && !s->field_0x5) {
        (*reinterpret_cast<UnkSelfVtbl_7100369c44**>(self))->vf_0x370(self);
        s->field_0x5 = true;
    }
    s->field_0x6 = false;
    if (UnkHelper_7100369c44* h = s->helper)
        fn_71003aa7c4(h, dt);
}
