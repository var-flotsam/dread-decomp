// media::CMovieAudioStreamNx (unit 326): resetting the stream position under the lock at 0xcd8 (the
// two words at 0xcf8 and 0xd00 back to -1, the one at 0xd08 to 0). Names are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkMovieAudioStream_7100221b00 {
    u8 pad_0x0[0xcd8];
    nn::os::MutexType field_0xcd8;
    u8 pad_0xcf8_hole[0xcf8 - 0xcd8 - sizeof(nn::os::MutexType)];
    s64 field_0xcf8;
    s64 field_0xd00;
    u64 field_0xd08;
};
static_assert(__builtin_offsetof(UnkMovieAudioStream_7100221b00, field_0xcf8) == 0xcf8, "layout");

void fn_7100221b00(UnkMovieAudioStream_7100221b00* self) {
    fn_710006691c(&self->field_0xcd8);
    self->field_0xd00 = -1;
    self->field_0xcf8 = -1;
    self->field_0xd08 = 0;
    fn_7100066920(&self->field_0xcd8);
}
