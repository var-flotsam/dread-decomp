#pragma once

// nn::diag: assertions and aborts. The game's checks end in nn::diag::detail::AbortImpl, in two
// forms: four arguments (condition, function, file, line; fn_71012351c0 when GetFileSize fails,
// fn_7100fc3510 when EnsureSaveData does) and with a result and a format (the system start-up
// fn_71001325d0 when MountRom fails, passing the result's address). The release build passes
// empty strings and line 0. On PC: a message on stderr with whatever was passed, and an abort.
//
// `main` imports (data/functions/): detail::AbortImpl (both forms).

#include "nn/nn_common.h"

namespace nn::diag::detail {

[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line);
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line,
                            const Result* result, const char* format, ...);

}  // namespace nn::diag::detail
