// Update of a gobbler-style component: runs the base update, nudges the actor's target state
// unless it is already a gobbler-ignore track change, and scales a value by how far the actor
// is from the other (clamped, 1 at the near limit).
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

using UnkActorTarget = UnkAnimationComponent;

struct UnkRangeSource {
    u8 pad_0x0[0x61c];
    f32 field_0x61c;
    f32 field_0x620;
};

struct UnkGobblerComponent {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x11e6];
    bool field_0x121e;
    u8 pad_0x121f[0x134e1];
    UnkRangeSource* field_0x14700;
};

void fn_7100369c44(UnkGobblerComponent* self);
void fn_710117662c(UnkActorTarget* target, u32 a, u32 b, u32 c, u32 d, f32 value);
void fn_7101178e08(UnkActorTarget* target, u32 a, u32 b, u32 c, f32 value);
extern char g_unk_71726e5148[] __attribute__((visibility("hidden")));

void fn_71005820c4(UnkGobblerComponent* self) {
    fn_7100369c44(self);
    UnkActorTarget* target = static_cast<UnkActorTarget*>(self->field_0x30->field_0x220);
    UnkActor* actor = self->field_0x30;
    if (target) {
        if (!fn_710117b348(target, fn_7100c20d58())) {
            if (self->field_0x121e)
                fn_710117662c(target, 0xb, 0, 1, 1, 0.6f);
            else
                fn_7101178e08(target, 0xb, 1, 1, 0.6f);
        }
        actor = self->field_0x30;
    }
    if (!actor || !fn_7100d7c990(actor))
        return;
    UnkActor* pos = fn_7100d7c990(self->field_0x30);
    UnkActor* other = self->field_0x30;
    UnkActorTarget* target2 = static_cast<UnkActorTarget*>(other->field_0x220);
    if (!target2)
        return;
    f32 d = __builtin_fabsf(pos->field_0x44 - other->field_0x44);
    UnkRangeSource* range = self->field_0x14700;
    f32 hi = range->field_0x61c;
    f32 lo = range->field_0x620;
    f32 m = d >= hi ? hi : d;
    f32 c = (__builtin_isgreater(d, lo) || __builtin_isunordered(d, lo)) ? m : lo;
    // fn_710033187c takes one float; the original leaves 1, hi and m in s1-s3 at the call, which only
    // these three unused extra arguments reproduce (the port's callee ignores them).
    using Set = void (*)(unk_prop_bag::UnkPropBag*, const unk_prop_bag::CStrId&, f32, f32, f32, f32);
    reinterpret_cast<Set>(&unk_prop_bag::fn_710033187c)(
        reinterpret_cast<unk_prop_bag::UnkPropBag*>(target2->field_0x80),
        *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_71726e5148), 1.0f - (c - lo) / (hi - lo), 1.0f, hi, m);
}
