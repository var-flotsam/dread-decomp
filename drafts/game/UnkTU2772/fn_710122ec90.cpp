// nn::atk unit 2772: decodes DSP-ADPCM samples (14 samples per 8-byte frame) from a sample position into
// 16-bit output, keeping the frame header and the two history samples in a context. Names and layouts are
// placeholders.
#include "types.h"

struct UnkAdpcmContext_l22ec90 {
    u16 header;  // 0x0
    s16 history1;  // 0x2
    s16 history2;  // 0x4
};

void fn_710122ec90(s64 position, UnkAdpcmContext_l22ec90* context, const s16* coefs, const u8* data, u64 count,
                   s16* out) {
    if (count == 0)
        return;
    s64 frame_index = position / 14;
    s64 n = position % 14;
    const u8* frame = data + frame_index * 8;
    u32 index = context->header >> 4;
    u32 scale = context->header & 0xf;
    for (u32 i = 0; i < count; ++i) {
        if (n == 0) {
            u32 header = frame[0];
            context->header = header;
            index = header >> 4;
            scale = header & 0xf;
        }
        u32 byte = frame[1 + n / 2];
        u32 nibble = (n & 1) == 0 ? byte >> 4 : byte;
        s32 residual = static_cast<s16>(nibble << 12) >> 1;
        s32 sum = context->history1 * coefs[index * 2] + context->history2 * coefs[index * 2 + 1] +
                  ((0x10000 << scale) >> 16) * residual;
        s32 sample;
        if (sum > 0x3fffbff) {
            sample = 0x7fff;
        } else {
            s32 t = (sum >> 10) + 1;
            sample = t < -0x10000 ? -0x8000 : t >> 1;
        }
        context->history2 = context->history1;
        context->history1 = sample;
        out[i] = sample;
        if (++n == 14) {
            n = 0;
            frame += 8;
        }
    }
}
