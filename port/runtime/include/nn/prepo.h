#pragma once

// nn::prepo: play reports (telemetry sent to the console's servers). The report writer
// fn_7100ec4454 builds one on its stack (PlayReport(event id), SetBuffer over 0x20c bytes of its
// frame, Add per field: strings, floats and integers) and submits it with Save for the user; it
// checks every result. On PC the port accepts and drops them; nothing is sent anywhere.
//
// `main` imports (data/functions/; parameter lists from the mangled names): PlayReport::PlayReport,
// PlayReport::SetBuffer, PlayReport::Add (three forms), PlayReport::Save. All declared below.

#include "nn/account.h"
#include "nn/nn_common.h"

namespace nn::prepo {

// The game's object lives in fn_7100ec4454's frame; its size is not known from the game and the
// port keeps no state in it, so the stand-in has no data members.
class PlayReport {
public:
    explicit PlayReport(const char* event_id);
    Result SetBuffer(void* buffer, u64 size);
    Result Add(const char* key, const char* value);
    Result Add(const char* key, float value);
    Result Add(const char* key, s64 value);
    Result Save(const account::Uid& user);
};

}  // namespace nn::prepo
