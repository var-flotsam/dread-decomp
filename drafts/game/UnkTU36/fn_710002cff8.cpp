// Unit 36 of tools/tu_map.py (the NFP worker): the base constructor of the worker object. It sets the
// vtable and flags, counts itself (a second instance aborts), makes the answer event and starts the worker
// thread. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/global/CFilePathStrId.h"
#include "game/UnkUnit36Static.h"
#include "types.h"

namespace nn {
namespace diag::detail {
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line,
                            const Result* result, const char* format, ...);
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line);
}  // namespace diag::detail
}  // namespace nn

extern char g_unk_7101a19f90[];
extern u32 g_unk_7101d55000;
extern u8 g_unk_7101d51000[];
void fn_710002d0d8(void* self);

void fn_710002cff8(UnkUnit36Static* self) {
    self->vtable = g_unk_7101a19f90 + 0x10;
    self->field_0x8 = true;
    self->field_0xc = 0;
    self->field_0x450 = 0;
    self->field_0x460 = 0;
    self->field_0x9 = false;
    if (g_unk_7101d55000++ != 0)
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0);
    nn::os::InitializeEvent(&self->event, false, nn::os::EventClearMode_AutoClear);
    alignas(8) nn::Result result = nn::os::CreateThread(&self->thread, fn_710002d0d8, self, g_unk_7101d51000, 0x4000, 0x10);
    if (result.IsFailure())
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0,
                                    &result, g_unk_710162e2ed);
    nn::os::StartThread(&self->thread);
}
