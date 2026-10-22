// Update of a gobbler-style component: runs the base update, nudges the actor's target state
// unless it is already a gobbler-ignore track change, and scales a value by how far the actor
// is from the other (clamped, 1 at the near limit).
#include "types.h"

struct UnkPosHolder {
    u8 pad_0x0[0x44];
    f32 field_0x44;
};

struct UnkActorTarget {
    u8 pad_0x0[0x80];
    u8 field_0x80[8];
};

struct UnkTargetActor {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0x1d8];
    UnkActorTarget* field_0x220;
};

struct UnkRangeSource {
    u8 pad_0x0[0x61c];
    f32 field_0x61c;
    f32 field_0x620;
};

struct UnkGobblerComponent {
    u8 pad_0x0[0x30];
    UnkTargetActor* field_0x30;
    u8 pad_0x38[0x11e6];
    bool field_0x121e;
    u8 pad_0x121f[0x134e1];
    UnkRangeSource* field_0x14700;
};

void fn_7100369c44(UnkGobblerComponent* self);
void* fn_7100c20d58();  // CCharClassGobblerIgnorePrefixChangeTrack::refl_GetClass
bool fn_710117b348(UnkActorTarget* target, void* cls);
void fn_710117662c(UnkActorTarget* target, u32 a, u32 b, u32 c, u32 d, f32 value);
void fn_7101178e08(UnkActorTarget* target, u32 a, u32 b, u32 c, f32 value);
UnkPosHolder* fn_7100d7c990(UnkTargetActor* actor);
extern char g_unk_71726e5148[] __attribute__((visibility("hidden")));
void fn_710033187c(void* dest, void* key, f32 value, f32 one, f32 hi, f32 mid);

void fn_71005820c4(UnkGobblerComponent* self) {
    fn_7100369c44(self);
    UnkActorTarget* target = self->field_0x30->field_0x220;
    UnkTargetActor* actor = self->field_0x30;
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
    UnkPosHolder* pos = fn_7100d7c990(self->field_0x30);
    UnkTargetActor* other = self->field_0x30;
    UnkActorTarget* target2 = other->field_0x220;
    if (!target2)
        return;
    f32 d = __builtin_fabsf(pos->field_0x44 - other->field_0x44);
    UnkRangeSource* range = self->field_0x14700;
    f32 hi = range->field_0x61c;
    f32 lo = range->field_0x620;
    f32 m = d >= hi ? hi : d;
    f32 c = (__builtin_isgreater(d, lo) || __builtin_isunordered(d, lo)) ? m : lo;
    fn_710033187c(target2->field_0x80, g_unk_71726e5148, 1.0f - (c - lo) / (hi - lo), 1.0f, hi, m);
}
