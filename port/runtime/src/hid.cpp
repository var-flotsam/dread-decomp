// nn::hid for the port: one handheld controller (id 0x20), always connected, whose state the
// port's window sets with dread_port_set_npad_state; no vibration devices, so the game's vibration
// graph (mixers, targets, connections, players) is built, connected and driven but never reaches a
// motor. Every stub names the boot-path function that depends on it in a TODO.

#include "nn/hid.h"

#include "dread_port.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>

namespace nn::hid {

namespace {

std::mutex g_pad_lock;
NpadState g_pad{};

// The SDK's vibration classes are polymorphic; the stand-ins' first word is their vptr. The port's
// nodes never call a virtual function of their own (Update does no sampling), so the vptr points at
// a table of traps that names the class and the slot if anything ever does.
template <int Class, int Slot>
[[noreturn]] void VirtualTrap() {
    static const char* const kClass[] = {"VibrationMixer", "VibrationTarget", "VibrationPlayer"};
    std::fprintf(stderr, "dread: nn::hid::%s virtual slot %d called; the port's vibration nodes have no virtual "
                         "functions yet\n", kClass[Class], Slot);
    std::fflush(stderr);
    std::abort();
}

template <int Class>
struct TrapVtable {
    using Slot = void (*)();
    static constexpr Slot kSlots[] = {VirtualTrap<Class, 0>, VirtualTrap<Class, 1>, VirtualTrap<Class, 2>,
                                      VirtualTrap<Class, 3>, VirtualTrap<Class, 4>, VirtualTrap<Class, 5>,
                                      VirtualTrap<Class, 6>, VirtualTrap<Class, 7>};
};

template <int Class>
void Construct(void* object, u64 size) {
    std::memset(object, 0, size);
    const void* vptr = TrapVtable<Class>::kSlots;
    std::memcpy(object, &vptr, sizeof(vptr));
}

void Sample(NpadState* out, NpadIdType id) {
    std::lock_guard<std::mutex> hold(g_pad_lock);
    if (id != NpadId_Handheld) {
        *out = NpadState{};
        return;
    }
    ++g_pad.sampling_number;
    *out = g_pad;
    out->attributes |= NpadAttribute_IsConnected;
}

}  // namespace

void InitializeNpad() {
    // TODO(M1): the input object's constructor fn_710006daf0.
}

void SetSupportedNpadStyleSet(NpadStyleSet) {
    // TODO(M1): fn_710006daf0 asks for full key and handheld (3); the port has handheld only.
}

void SetSupportedNpadIdType(const NpadIdType*, u64) {
    // TODO(M1): fn_710006daf0, nine ids (0x20, then 0 to 7).
}

void SetNpadJoyHoldType(NpadJoyHoldType) {
    // TODO(M2): the poll fn_710006deac, before the controller applet.
}

NpadStyleSet GetNpadStyleSet(const NpadIdType& id) {
    // TODO(M2): fn_710006deac reads every id each frame and wants exactly one connected.
    return NpadStyleSet{id == NpadId_Handheld ? NpadStyle_Handheld : 0u};
}

void GetNpadState(NpadFullKeyState* out, const NpadIdType& id) {
    // TODO(M2): fn_710006deac on style bit 0 (never, with the port's one handheld controller).
    Sample(out, id);
}

void GetNpadState(NpadHandheldState* out, const NpadIdType& id) {
    // TODO(M2): fn_710006deac on style bit 1, every frame.
    Sample(out, id);
}

void ControllerSupportArg::SetDefault() {
    // TODO(M2): fn_710006deac fills in the fields it cares about after this; the port's applet
    // reads none of them.
}

Result ShowControllerSupport(ControllerSupportResultInfo* out, const ControllerSupportArg&) {
    // TODO(M2): fn_710006deac when the controllers do not suit it. The port's one controller is
    // the answer.
    out->player_count = 1;
    out->selected_id = NpadId_Handheld;
    return ResultSuccess();
}

s32 GetVibrationDeviceHandles(VibrationDeviceHandle*, s32, const NpadIdType&, NpadStyleSet) {
    // TODO(M2): fn_710006ddc8. No vibration devices: the loop that initialises them runs zero times.
    return 0;
}

void InitializeVibrationDevice(const VibrationDeviceHandle&) {
    // Never called: GetVibrationDeviceHandles reports none.
}

void GetVibrationDeviceInfo(VibrationDeviceInfo*, const VibrationDeviceHandle&) {
    // Never called: as InitializeVibrationDevice.
}

// The sampling period the vibration thread (fn_710006f390, src/base/input/UnkVibrationThread.cpp)
// waits between updates: 5 ms, as on the console. A data import, named by its symbol here because
// include/base/input/UnkHidVibration.h's VibrationNode declares only Update.
extern const TimeSpan g_default_vibration_sample_interval asm(
    "_ZN2nn3hid13VibrationNode30DefaultVibrationSampleIntervalE");
const TimeSpan g_default_vibration_sample_interval = TimeSpan::FromNanoSeconds(5'000'000);

void VibrationNode::Update() {
    // TODO(M2): fn_710006f390, each frame. No devices, so nothing to sample.
}

VibrationMixer::VibrationMixer() {
    // TODO(M1): the rumble manager's constructor fn_71000733e4 (two mixers).
    Construct<0>(this, sizeof(*this));
}

VibrationMixer::~VibrationMixer() {
    // The rumble manager's destructor fn_710007054c.
}

VibrationTarget::VibrationTarget() {
    // TODO(M1): fn_71000733e4 (two targets).
    Construct<1>(this, sizeof(*this));
}

VibrationTarget::~VibrationTarget() {
    // fn_710007054c.
}

void VibrationTarget::SetVibrationDeviceHandle(const VibrationDeviceHandle& handle) {
    // fn_710006fb30, when a controller is chosen. Kept, unused.
    std::memcpy(storage + 8, &handle, sizeof(handle));
}

VibrationNodeConnection::VibrationNodeConnection() {
    // TODO(M1): fn_71000733e4 (two) and the player fn_7100070740 (two per player).
    std::memset(storage, 0, sizeof(storage));
}

VibrationNodeConnection::~VibrationNodeConnection() {
    // fn_710007054c, fn_71000707dc, fn_7100070828.
}

void VibrationNodeConnection::Connect(VibrationNode* source, VibrationNode* destination) {
    // fn_710006f4d8 and fn_710007087c wire the graph. Kept, unused.
    std::memcpy(storage, &source, sizeof(source));
    std::memcpy(storage + 8, &destination, sizeof(destination));
}

void VibrationNodeConnection::SetModulation(const VibrationModulation&) {
    // fn_7100070a28, each update of a player. Nothing to modulate.
}

VibrationPlayer::VibrationPlayer() {
    // TODO(M1): fn_7100070740; the game's own constructor writes its vtable over the port's
    // afterwards.
    Construct<2>(this, sizeof(*this));
}

VibrationPlayer::~VibrationPlayer() {
    // fn_71000707dc and fn_7100070828.
}

// The player's own state, in its storage after the vptr: loaded, playing, looping, position.
namespace {
struct PlayerState {
    u8 loaded;
    u8 playing;
    u8 loop;
    u8 reserved;
    s32 position;
};
static_assert(sizeof(PlayerState) <= sizeof(VibrationPlayer::storage_0x8), "fits the player");

PlayerState& State(VibrationPlayer* p) { return *reinterpret_cast<PlayerState*>(p->storage_0x8); }
const PlayerState& State(const VibrationPlayer* p) { return *reinterpret_cast<const PlayerState*>(p->storage_0x8); }
}  // namespace

s32 VibrationPlayer::Load(const void*, u64) {
    // fn_71000708c8. The file is not parsed: nothing plays it.
    State(this) = PlayerState{1, 0, State(this).loop, 0, 0};
    return 0;
}

void VibrationPlayer::Unload() {
    // TODO(M1): fn_7100070dd4, after Stop.
    State(this).loaded = 0;
    State(this).playing = 0;
}

void VibrationPlayer::Play() {
    // fn_7100070a28 and fn_7100070e64. The player stops at once: with no samples the effect has
    // ended, which the game reads through IsPlaying.
    State(this).position = 0;
}

void VibrationPlayer::Stop() {
    // TODO(M1): fn_7100070dd4, fn_7100070a28, fn_7100070e64.
    State(this).playing = 0;
}

void VibrationPlayer::SetLoop(bool loop) {
    // fn_71000708c8.
    State(this).loop = loop;
}

bool VibrationPlayer::IsLoop() const {
    return State(this).loop != 0;
}

bool VibrationPlayer::IsPlaying() const {
    return State(this).playing != 0;
}

s32 VibrationPlayer::GetCurrentPosition() const {
    return State(this).position;
}

VibrationFileInfo VibrationPlayer::GetFileInfo() const {
    // fn_7100070a28 compares the sample length with the position: zero, an empty effect.
    return VibrationFileInfo{};
}

void VibrationPlayer::OnNextSampleRequired(VibrationValue*, VibrationNodeConnectionList*) {
    // The game's override fn_7100070fa0 calls it first; nothing samples it on PC.
}

}  // namespace nn::hid

extern "C" void dread_port_set_npad_state(u64 buttons, s32 left_x, s32 left_y, s32 right_x, s32 right_y) {
    std::lock_guard<std::mutex> hold(nn::hid::g_pad_lock);
    nn::hid::g_pad.buttons = buttons;
    nn::hid::g_pad.analog_stick_l = nn::hid::AnalogStickState{left_x, left_y};
    nn::hid::g_pad.analog_stick_r = nn::hid::AnalogStickState{right_x, right_y};
}
