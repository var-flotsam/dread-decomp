// nn::hid for the port: one handheld controller (id 0x20), always connected, whose state the
// port's window sets with dread_port_set_npad_state; no vibration devices, so the game's vibration
// graph (mixers, targets, connections, players) is built, connected and driven but never reaches a
// motor. Every stub names the boot-path function that depends on it in a TODO.

#include "nn/hid.h"

#include "dread_port.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <mutex>
#include <string>
#include <vector>

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

// The input script ($DREAD_INPUT): button presses at given polls of the handheld pad, for runs with
// no window (boot_distance.py, the frame dump). The game's poll fn_710006deac reads the handheld
// state once a frame, so a poll number is a frame number. Steps, separated by commas or spaces:
//
//   BUTTONS@FRAME[+HOLD][/EVERY]
//
// BUTTONS: names joined by '|' (A B X Y LSTICK RSTICK L R ZL ZR PLUS MINUS LEFT UP RIGHT DOWN, in
// the console's bit order 0-15, as switchbrew's HID services page and libnx's HidNpadButton give
// it) or a number (0x401). Held from poll FRAME for HOLD polls (default 6); with /EVERY, again
// every EVERY polls after that. Example: DREAD_INPUT="A@900+6/120" presses A for 6 frames from
// frame 900, then every 120 frames.
struct ScriptStep {
    u64 buttons = 0;
    s64 start = 0, hold = 6, every = 0;
};

u64 ButtonBit(const std::string& name) {
    static const char* const kNames[] = {"A", "B", "X", "Y", "LSTICK", "RSTICK", "L", "R",
                                         "ZL", "ZR", "PLUS", "MINUS", "LEFT", "UP", "RIGHT", "DOWN"};
    for (u64 i = 0; i < sizeof(kNames) / sizeof(kNames[0]); ++i)
        if (name == kNames[i])
            return u64{1} << i;
    char* end = nullptr;
    const u64 v = std::strtoull(name.c_str(), &end, 0);
    return (end && *end == '\0' && !name.empty()) ? v : 0;
}

// Parses one step; false (and nothing changed) when it is malformed.
bool ParseStep(const std::string& text, ScriptStep* out) {
    const std::size_t at = text.find('@');
    if (at == std::string::npos)
        return false;
    ScriptStep s;
    std::size_t p = 0;
    const std::string names = text.substr(0, at);
    while (p <= names.size()) {
        const std::size_t bar = std::min(names.find('|', p), names.size());
        const u64 bit = ButtonBit(names.substr(p, bar - p));
        if (!bit)
            return false;
        s.buttons |= bit;
        p = bar + 1;
    }
    const char* c = text.c_str() + at + 1;
    char* end = nullptr;
    s.start = std::strtoll(c, &end, 10);
    if (end == c || s.start < 0)
        return false;
    while (*end == '+' || *end == '/') {
        const char op = *end;
        c = end + 1;
        const s64 v = std::strtoll(c, &end, 10);
        if (end == c || v <= 0)
            return false;
        (op == '+' ? s.hold : s.every) = v;
    }
    if (*end != '\0')
        return false;
    *out = s;
    return true;
}

const std::vector<ScriptStep>& Script() {
    static const std::vector<ScriptStep> steps = [] {
        std::vector<ScriptStep> out;
        const char* env = std::getenv("DREAD_INPUT");
        if (!env || !*env)
            return out;
        std::string all = env;
        for (char& ch : all)
            if (ch == ',')
                ch = ' ';
        std::size_t p = 0;
        while (p < all.size()) {
            const std::size_t q = std::min(all.find(' ', p), all.size());
            const std::string token = all.substr(p, q - p);
            ScriptStep s;
            if (!token.empty()) {
                if (ParseStep(token, &s))
                    out.push_back(s);
                else
                    std::fprintf(stderr, "dread: DREAD_INPUT: step \"%s\" is not BUTTONS@FRAME[+HOLD][/EVERY]; left out\n",
                                 token.c_str());
            }
            p = q + 1;
        }
        std::fprintf(stderr, "dread: DREAD_INPUT: %zu steps\n", out.size());
        return out;
    }();
    return steps;
}

// The buttons the script holds at poll `n`.
u64 ScriptedButtons(s64 n) {
    u64 buttons = 0;
    for (const ScriptStep& s : Script()) {
        if (n < s.start)
            continue;
        const s64 since = s.every ? (n - s.start) % s.every : n - s.start;
        if (since < s.hold)
            buttons |= s.buttons;
    }
    return buttons;
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
    const u64 scripted = ScriptedButtons(g_pad.sampling_number);
    static u64 last = 0;
    if (scripted != last && scripted)
        std::fprintf(stderr, "dread: DREAD_INPUT: frame %lld: buttons %#llx\n",
                     static_cast<long long>(g_pad.sampling_number), static_cast<unsigned long long>(scripted));
    last = scripted;
    out->buttons |= scripted;
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
