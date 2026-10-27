// nn::nfp for the port: no amiibo reader. The library initialises and lists no device; the
// availability event exists and is never signalled; any device call (the game makes none without
// a listed device) fails.

#include "nn/nfp.h"

namespace nn::nfp {

namespace {

bool g_initialized;

// The game only reaches these with a device ListDevices never gave it.
Result NoDevice() {
    return ResultNotPorted();
}

}  // namespace

Result Initialize() {
    g_initialized = true;
    return ResultSuccess();
}

void Finalize() {
    g_initialized = false;
}

State GetState() {
    return g_initialized ? State_Initialized : State_None;
}

Result ListDevices(DeviceHandle*, s32* count, s32) {
    *count = 0;
    return ResultSuccess();
}

Result AttachAvailabilityChangeEvent(os::SystemEventType* event) {
    os::InitializeSystemEventForPort(event, false, os::EventClearMode_ManualClear);
    return ResultSuccess();
}

Result GetNpadId(u32*, const DeviceHandle&) { return NoDevice(); }
Result AttachActivateEvent(os::SystemEventType*, const DeviceHandle&) { return NoDevice(); }
Result AttachDeactivateEvent(os::SystemEventType*, const DeviceHandle&) { return NoDevice(); }
DeviceState GetDeviceState(const DeviceHandle&) { return DeviceState{}; }
Result StartDetection(const DeviceHandle&) { return NoDevice(); }
Result StopDetection(const DeviceHandle&) { return NoDevice(); }
Result Mount(const DeviceHandle&, ModelType, MountTarget) { return NoDevice(); }
Result Unmount(const DeviceHandle&) { return NoDevice(); }
Result Flush(const DeviceHandle&) { return NoDevice(); }
Result Restore(const DeviceHandle&) { return NoDevice(); }
Result CreateApplicationArea(const DeviceHandle&, const ApplicationAreaCreateInfo&) { return NoDevice(); }
Result OpenApplicationArea(const DeviceHandle&, u32) { return NoDevice(); }
Result GetApplicationArea(void*, u64*, const DeviceHandle&, u64) { return NoDevice(); }
Result SetApplicationArea(const DeviceHandle&, const void*, u64) { return NoDevice(); }
Result GetTagInfo(TagInfo*, const DeviceHandle&) { return NoDevice(); }
Result GetRegisterInfo(RegisterInfo*, const DeviceHandle&) { return NoDevice(); }
Result GetCommonInfo(CommonInfo*, const DeviceHandle&) { return NoDevice(); }
Result GetModelInfo(ModelInfo*, const DeviceHandle&) { return NoDevice(); }
Result StartNicknameAndOwnerSettings(DeviceHandle*, bool*, RegisterInfo*, const AmiiboSettingsStartParam&,
                                     const TagInfo&) {
    return NoDevice();
}
Result StartGameDataEraser(DeviceHandle*, const AmiiboSettingsStartParam&, const TagInfo&) { return NoDevice(); }
Result StartRestorer(DeviceHandle*, const AmiiboSettingsStartParam&, const TagInfo&) { return NoDevice(); }

}  // namespace nn::nfp
