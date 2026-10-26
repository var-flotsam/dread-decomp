// nn::diag for the port: an abort prints what the game passed (the release build passes empty
// strings and line 0, so mostly the result) and stops. Every stub names the boot-path function
// that depends on it in a TODO.

#include "nn/diag.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace nn::diag::detail {

namespace {

void PrintWhere(const char* condition, const char* function, const char* file, s32 line) {
    std::fprintf(stderr, "dread: the game aborted (nn::diag::detail::AbortImpl): %s %s %s:%d", condition ? condition : "",
                 function ? function : "", file ? file : "", line);
}

}  // namespace

void AbortImpl(const char* condition, const char* function, const char* file, s32 line) {
    // TODO(M1): fn_71012351c0 (GetFileSize failed), fn_7100fc3510 (EnsureSaveData failed).
    PrintWhere(condition, function, file, line);
    std::fprintf(stderr, "\n");
    std::fflush(stderr);
    std::abort();
}

void AbortImpl(const char* condition, const char* function, const char* file, s32 line, const Result* result,
               const char* format, ...) {
    // TODO(M1): the system start-up fn_71001325d0 when MountRom fails; fn_7100fc3510 when
    // MountSaveData does.
    PrintWhere(condition, function, file, line);
    if (result)
        std::fprintf(stderr, " result 0x%x", result->value);
    if (format && *format) {
        std::fprintf(stderr, ": ");
        va_list args;
        va_start(args, format);
        std::vfprintf(stderr, format, args);
        va_end(args);
    }
    std::fprintf(stderr, "\n");
    std::fflush(stderr);
    std::abort();
}

}  // namespace nn::diag::detail
