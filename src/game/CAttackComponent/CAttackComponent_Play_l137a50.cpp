// CAttackComponent's unit (tools/tu_map.py unit 2706): plays the action `id` names on the owner's (+0x30)
// animation component (+0x220) through fn_7101176690, with the attack's speed and duration, once the
// component's vf_0x170 accepts it; while it is sent the flag at +0x4a holds `hold`. A played action sets
// +0x48 and runs vf_0x1c0. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflTypeIdArg;

// A speed with its set flag (two words, passed in one register).
struct UnkPlaySpeed_l137a50 {
    f32 value;
    u32 enabled;
};

// The play request's timing: the start (-1: none) and the speed.
struct UnkPlayTiming_l137a50 {
    s64 start;
    UnkPlaySpeed_l137a50 speed;
    UnkPlayTiming_l137a50(s64 start, UnkPlaySpeed_l137a50 speed) : start(start), speed(speed) {}
};

// The play request's length: no offset and the duration in frames.
struct UnkPlayLength_l137a50 {
    u64 offset;
    f32 frames;
};

struct UnkAttackComponent_l137a50 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual bool vf_0x170(const CStrId* id, bool mode);
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x10];
    bool played;  // 0x48
    u8 pad_0x49;
    bool hold;  // 0x4a
};
static_assert(__builtin_offsetof(UnkAttackComponent_l137a50, hold) == 0x4a, "UnkAttackComponent_l137a50 layout");

bool fn_7101137a50(UnkAttackComponent_l137a50* self, const CStrId* id, bool mode, bool hold, s32 frames,
                   f32 speed) {
    if (!self->owner->field_0x220)
        return false;
    if (!self->vf_0x170(id, mode))
        return false;
    bool saved = self->hold;
    self->hold = hold;
    void* animation = self->owner->field_0x220;
    u64 action = id->data->Key();
    UnkReflTypeIdArg value;
    value.id = mode;
    UnkPlayTiming_l137a50 timing(-1, UnkPlaySpeed_l137a50{speed, 1});
    UnkPlayLength_l137a50 length;
    length.offset = 0;
    length.frames = frames;
    UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    bool played = fn_7101176690(animation, action, 0xf4061ff25a2163d6, &value, &timing, &length, &mask);
    if (played) {
        self->played = true;
        self->vf_0x1c0();
    }
    self->hold = saved;
    return played;
}
