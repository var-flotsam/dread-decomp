// nn::prepo for the port: play reports are accepted and dropped. Every stub names the boot-path
// function that depends on it in a TODO.

#include "nn/prepo.h"

namespace nn::prepo {

PlayReport::PlayReport(const char*) {
    // TODO(M1): the report writer fn_7100ec4454 (also its other calls below).
}

Result PlayReport::SetBuffer(void*, u64) {
    return ResultSuccess();
}

Result PlayReport::Add(const char*, const char*) {
    return ResultSuccess();
}

Result PlayReport::Add(const char*, float) {
    return ResultSuccess();
}

Result PlayReport::Add(const char*, s64) {
    return ResultSuccess();
}

Result PlayReport::Save(const account::Uid&) {
    // Nothing is sent: the port has no telemetry.
    return ResultSuccess();
}

}  // namespace nn::prepo
