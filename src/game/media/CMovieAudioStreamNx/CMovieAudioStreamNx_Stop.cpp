// media::CMovieAudioStreamNx (unit 326): stopping the stream: fn_7100225e14 first, then under the
// lock at 0xcd8 the position reset as fn_7100221b00 does, fn_7100220710 on the movie's part at
// +0xae0 (with -1), fn_7101239af0 on the stream state's +0xc8 (flagging +0x2b0 when it answers 0),
// the stop flag at 0xc28 and a wake of the thread at 0xa28. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkMovieAudioState_7100221a40 {
    u8 pad_0x0[0xc8];
    u8 player[0x1e8];  // 0xc8
    bool finished;     // 0x2b0
};

struct UnkMovieAudioStream_7100221a40 {
    u8 pad_0x0[0x30];
    char* movie;  // 0x30
    u8 pad_0x38[0x9f0];
    u8 thread[0x1f8];  // 0xa28
    UnkMovieAudioState_7100221a40* state;  // 0xc20
    bool stopping;                         // 0xc28
    u8 pad_0xc29[0xaf];
    nn::os::MutexType lock;  // 0xcd8
    s64 field_0xcf8;
    s64 field_0xd00;
    u64 field_0xd08;
};
static_assert(__builtin_offsetof(UnkMovieAudioStream_7100221a40, field_0xcf8) == 0xcf8, "layout");

void fn_7100225e14(UnkMovieAudioStream_7100221a40* self);
void fn_7100220710(void* part, s64 position);
s32 fn_7101239af0(void* player);
void fn_7100066828(void* thread, u32 a);

void fn_7100221a40(UnkMovieAudioStream_7100221a40* self) {
    fn_7100225e14(self);
    fn_710006691c(&self->lock);
    self->field_0xd00 = -1;
    self->field_0xcf8 = -1;
    self->field_0xd08 = 0;
    fn_7100220710(self->movie + 0xae0, -1);
    UnkMovieAudioState_7100221a40* state = self->state;
    if (fn_7101239af0(state->player) == 0)
        state->finished = true;
    self->stopping = true;
    fn_7100066828(self->thread, 0);
    fn_7100066920(&self->lock);
}
