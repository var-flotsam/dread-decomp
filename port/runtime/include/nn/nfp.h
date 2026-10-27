#pragma once

// nn::nfp: amiibo (NFC figures). The game initialises the library at boot (Initialize,
// ListDevices, the Attach*Event calls) and polls tag state from a menu, each call through a small
// engine wrapper (fn_710002c420 Initialize, fn_710002c440 Finalize, fn_710002c460 ListDevices, then
// the device calls up to fn_710002cbd0 AttachAvailabilityChangeEvent; src/lifted/lifted_002.cpp)
// that returns the result. On PC there is no reader: the library initialises, lists no device, and
// every device call fails, which the game already handles as "no amiibo".
//
// `main` imports these 26 (data/functions/; parameter lists from the mangled names), all declared
// below. The structures are the game's; the port never reads or writes them, so they are
// declared only.

#include "nn/nn_common.h"
#include "nn/os.h"

namespace nn::nfp {

struct DeviceHandle;
struct TagInfo;
struct CommonInfo;
struct ModelInfo;
struct RegisterInfo;
struct AmiiboSettingsStartParam;
struct ApplicationAreaCreateInfo;
enum ModelType : s32 {};
enum MountTarget : s32 {};
// What GetState and GetDeviceState return (the values the port reports are named below).
enum State : s32 { State_None = 0, State_Initialized = 1 };
enum DeviceState : s32 {};

Result Initialize();
void Finalize();
State GetState();
// No reader: always none listed.
Result ListDevices(DeviceHandle* out, s32* count, s32 max);
Result AttachAvailabilityChangeEvent(os::SystemEventType* event);

Result GetNpadId(u32* out, const DeviceHandle& device);
Result AttachActivateEvent(os::SystemEventType* event, const DeviceHandle& device);
Result AttachDeactivateEvent(os::SystemEventType* event, const DeviceHandle& device);
DeviceState GetDeviceState(const DeviceHandle& device);
Result StartDetection(const DeviceHandle& device);
Result StopDetection(const DeviceHandle& device);
Result Mount(const DeviceHandle& device, ModelType model, MountTarget target);
Result Unmount(const DeviceHandle& device);
Result Flush(const DeviceHandle& device);
Result Restore(const DeviceHandle& device);
Result CreateApplicationArea(const DeviceHandle& device, const ApplicationAreaCreateInfo& info);
Result OpenApplicationArea(const DeviceHandle& device, u32 access_id);
Result GetApplicationArea(void* buffer, u64* size, const DeviceHandle& device, u64 buffer_size);
Result SetApplicationArea(const DeviceHandle& device, const void* data, u64 size);
Result GetTagInfo(TagInfo* out, const DeviceHandle& device);
Result GetRegisterInfo(RegisterInfo* out, const DeviceHandle& device);
Result GetCommonInfo(CommonInfo* out, const DeviceHandle& device);
Result GetModelInfo(ModelInfo* out, const DeviceHandle& device);
Result StartNicknameAndOwnerSettings(DeviceHandle* device, bool* changed, RegisterInfo* info,
                                     const AmiiboSettingsStartParam& param, const TagInfo& tag);
Result StartGameDataEraser(DeviceHandle* device, const AmiiboSettingsStartParam& param, const TagInfo& tag);
Result StartRestorer(DeviceHandle* device, const AmiiboSettingsStartParam& param, const TagInfo& tag);

}  // namespace nn::nfp
