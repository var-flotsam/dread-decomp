// CKraidAIComponent's unit (tools/tu_map.py): an update (fn_710060086c) that, after the base
// (fn_71003ad414), works on the block at +0x149c0. While it is active and its time (+0x10) has reached
// the tunables' threshold (+0x70, +0x544), a CCharClassKraidCanUncoverBellyTrack on the owner's
// (+0x30) character class runs fn_71005fe518 and latches +0x54/+0x14 from the target actor. Then,
// once the countdown at +0x4 has run out (and +0x478 is set when +0x1140 is), it draws a new one between
// the tunables at +0x53c and +0x540 and, unless a CCharClassAttackFailTrack is playing, alternates
// between two ids (+0x8 the previous pick, +0xc the new one, the first pick fixed) and sends the
// chosen id's hash with mode 0 (UnkSpitSend.h). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkHelper;

struct UnkKraidUpdateAI_b62a4;
void fn_71003ad414(UnkKraidUpdateAI_b62a4* self);
void fn_71005fe518(UnkKraidUpdateAI_b62a4* self);

struct UnkKraidUpdateId_b62a4 {
    u64 hash;
};
extern UnkKraidUpdateId_b62a4 g_unk_710164a280;  // reached through the GOT
extern UnkKraidUpdateId_b62a4 g_unk_710164a218;  // reached through the GOT

struct UnkKraidUpdateTunables_b62a4 {
    u8 pad_0x0[0x53c];
    f32 min;
    f32 max;
    f32 threshold;
};

struct UnkKraidUpdateTarget_b62a4 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkKraidUpdateBlock_b62a4 {
    bool active;
    f32 countdown;
    u32 previous;
    u32 current;
    f32 time;
    bool field_0x14;
    u8 pad_0x15[0x54 - 0x15];
    bool field_0x54;
    u8 pad_0x55[0x70 - 0x55];
    UnkKraidUpdateTunables_b62a4* tunables;
    u8 pad_0x78[0x478 - 0x78];
    void* field_0x478;
};

struct UnkKraidUpdateAI_b62a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
    u8 pad_0x1144[0x149c0 - 0x1144];
    UnkKraidUpdateBlock_b62a4 block;
};

void fn_710060086c(UnkKraidUpdateAI_b62a4* self) {
    UnkKraidUpdateBlock_b62a4* b = &self->block;
    fn_71003ad414(self);
    if (b->active && b->time >= b->tunables->threshold) {
        UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
        if (fn_710117b348(cls, fn_7100c39668())) {
            fn_71005fe518(self);
            UnkActor* target = fn_7100d7c990(self->owner);
            if (!target)
                goto clear;
            if (target->field_0x208 && fn_71009e6434(static_cast<UnkHelper*>(target->field_0x208))) {
                b->field_0x54 = true;
            } else {
                if (!b->field_0x54)
                    goto clear;
                bool v = (*reinterpret_cast<UnkKraidUpdateTarget_b62a4**>(reinterpret_cast<char*>(target) + 0x1f0))
                             ->field_0xf9;
                b->field_0x54 = v;
                if (!v)
                    goto done;
            }
            b->field_0x14 = true;
            goto done;
        clear:
            b->field_0x54 = false;
        }
    }
done:
    if (self->field_0x1140 != 0 && !b->field_0x478)
        return;
    if (!(b->countdown <= 0.0f))
        return;
    f32 min = b->tunables->min;
    f32 max = b->tunables->max;
    f32 r = (f32)((f64)(fn_71012395a0() & 0x7fff) * (1.0 / 32767.0));
    b->countdown = min + (max - min) * r;
    UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (fn_710117b348(cls, fn_7100c0d1d8()))
        return;
    const UnkKraidUpdateId_b62a4* id;
    if (b->previous == b->current && b->current == 0) {
        b->previous = 0;
        b->current = 1;
        id = &g_unk_710164a280;
    } else {
        u32 pick = fn_71012395a0() & 1;
        b->previous = b->current;
        b->current = pick;
        id = pick != 0 ? &g_unk_710164a280 : &g_unk_710164a218;
    }
    u64 hash = id->hash;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
