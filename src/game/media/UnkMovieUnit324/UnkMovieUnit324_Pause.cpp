// media unit 324 (the media instance of src/game/media/UnkUnit330/UnkUnit330.cpp: state at +0x2c,
// locks at +0x940 and +0x960): moving a playing (6) or finished (9) instance to state 7, keeping
// the state it left at +0x30, and telling its listeners (+0x720, count +0x728: a context and a
// callback each) under the inner lock. Answers 0 when it did, 2 when already in state 7 and 3
// otherwise. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU324.h"
#include "types.h"

struct UnkMedia324;

struct UnkMediaListener324 {
    void* context;
    void (*callback)(void* context, UnkMedia324* media, u32 arg);
};

struct UnkMedia324 {
    u8 pad_0x0[0x2c];
    s32 state;           // 0x2c
    s32 previous_state;  // 0x30
    u8 pad_0x34[0x6ec];
    UnkMediaListener324* listeners;  // 0x720
    u32 listener_count;              // 0x728
    u8 pad_0x72c[0x214];
    nn::os::MutexType listener_lock;  // 0x940
    nn::os::MutexType lock;           // 0x960
};
static_assert(__builtin_offsetof(UnkMedia324, lock) == 0x960, "UnkMedia324 layout");

// As src/engine/render/anim/CSkeletalAnimTree/fn_710014be78.cpp defines it (the tracks' owner).
struct UnkChildOwner60;
void fn_7100224ebc(UnkChildOwner60* self);

u32 fn_710021f950(UnkMedia324* self) {
    fn_710006691c(&self->lock);
    u32 result;
    switch (self->state) {
    case 6:
    case 9:
        fn_7100224ebc(reinterpret_cast<UnkChildOwner60*>(self));
        fn_710006691c(&self->listener_lock);
        self->previous_state = self->state;
        self->state = 7;
        for (UnkMediaListener324* it = self->listeners, *end = it + self->listener_count; it != end; ++it)
            it->callback(it->context, self, 0);
        fn_7100066920(&self->listener_lock);
        result = 0;
        break;
    case 7:
        result = 2;
        break;
    default:
        result = 3;
        break;
    }
    fn_7100066920(&self->lock);
    return result;
}
