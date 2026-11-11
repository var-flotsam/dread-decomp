#include <nn/os.h>

#include "types.h"

extern u8 g_unk_7108b6b600[] __attribute__((visibility("hidden")));
void* fn_710120806c(void* player, u32 id);
void fn_7101209ce0(void* effect, f32 value);
void fn_7101209ce8(void* effect, s32 index, f32 value);

static inline f32 Unit(f32 value) {
    f32 capped = !(value >= 1.0f) ? value : 1.0f;
    return !(value <= 0.0f) ? capped : 0.0f;
}

void fn_7100100628(void* self, u32 id, u32 kind, f32 a, f32 b) {
    nn::os::GetCurrentThread();
    void* effect = fn_710120806c(g_unk_7108b6b600, id + 0x4000000);
    switch (kind) {
    case 0:
        fn_7101209ce0(effect, 0.0f);
        fn_7101209ce8(effect, -1, 0.0f);
        break;
    case 1:
    case 7: {
        f32 low = b >= 0.0f ? 0.0f : b;
        fn_7101209ce0(effect, !(b <= -1.0f) ? low : -1.0f);
        fn_7101209ce8(effect, -1, 0.0f);
        break;
    }
    case 2:
    case 3:
    case 4:
    case 5:
        fn_7101209ce0(effect, 0.0f);
        fn_7101209ce8(effect, kind - 1, Unit(a));
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        fn_7101209ce0(effect, 0.0f);
        fn_7101209ce8(effect, kind - 7, Unit(a));
        break;
    }
}
