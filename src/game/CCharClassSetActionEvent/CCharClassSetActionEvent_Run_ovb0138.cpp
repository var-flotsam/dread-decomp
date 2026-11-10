// CCharClassSetActionEvent's unit (tools/tu_map.py): running the event on an actor. With a named
// action (+0x18) and, below certainty, a random draw under the chance (+0x24), the action goes to the
// actor's timeline (+0x258, fn_7101137a38) when the event asks for it directly (+0x20), else as a
// request to its animation (+0x220) with the flag (+0xc) and the value (+0x10). Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkActionTimeline_ovb0138;
void fn_7101137a38(UnkActionTimeline_ovb0138* timeline, const CStrId* action, bool flag, u32 value, f32 time);

struct UnkActionActor_ovb0138 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x30];
    UnkActionTimeline_ovb0138* timeline;  // 0x258
};

// The second descriptor of the request: a word and a value.
struct UnkActionValue_ovb0138 {
    u64 field_0x0;
    f32 value;
};

struct UnkSetActionEvent_ovb0138 {
    u8 pad_0x0[0xc];
    bool flag;     // 0xc
    s32 value;     // 0x10
    u8 pad_0x14[4];
    CStrId action;  // 0x18
    bool direct;    // 0x20
    f32 chance;     // 0x24
};

static inline f32 Random(f32 lo, f32 hi) {
    return lo + (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * (hi - lo);
}

void fn_7100cd0278(UnkSetActionEvent_ovb0138* self, UnkActionActor_ovb0138** target) {
    if (!*target)
        return;
    if (self->action.data == &g_unk_7101d47108->empty)
        return;
    if (self->chance < 1.0f && !(Random(0.0f, 1.0f) <= self->chance))
        return;
    UnkActionActor_ovb0138* actor = *target;
    if (actor->timeline && self->direct) {
        fn_7101137a38(actor->timeline, &self->action, self->flag, self->value, -1.0f);
        return;
    }
    if (void* animation = actor->animation) {
        u32 flag = self->flag;
        u64 hash = reinterpret_cast<base::global::UnkStringEntry*>(self->action.data)->buffer.key;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = flag;
        f32 v = (f32)self->value;
        UnkActionValue_ovb0138 value;
        value.field_0x0 = 0;
        value.value = v;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &value, &mask);
    }
}
