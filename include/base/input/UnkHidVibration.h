#pragma once

// The nn::hid vibration classes the game embeds by value. lib/nnsdk has no declarations for them;
// these are opaque stand-ins with the library's members (the imports' mangled names give every
// parameter list), sized from the offsets of the members that follow them
// (src/base/input/CRumbleManager.cpp, src/game_s1/fn_71000707dc.cpp). The SDK's classes are
// polymorphic (a vptr at +0, VibrationNode's): the stand-ins keep that word in their storage and
// declare no virtual function, so the matching build's layouts stay the game's. The port's runtime
// defines the members (port/runtime/include/nn/hid.h completes the small types).

#include "types.h"

namespace nn::util {
template <typename T, typename Traits>
class IntrusiveList;
template <typename T, typename Tag>
class IntrusiveListBaseNodeTraits;
}  // namespace nn::util

namespace nn::hid {

struct VibrationDeviceHandle;
struct VibrationFileInfo;
struct VibrationModulation;
struct VibrationValue;
class VibrationNodeConnection;

// The base of the mixer, the target and the player.
class VibrationNode {
public:
    // Runs every node's sampling once (fn_710006f390, each frame).
    static void Update();
};

class VibrationMixer : public VibrationNode {
public:
    VibrationMixer();
    ~VibrationMixer();
    u8 storage[0x48];
};

class VibrationTarget : public VibrationNode {
public:
    VibrationTarget();
    ~VibrationTarget();
    void SetVibrationDeviceHandle(const VibrationDeviceHandle& handle);
    u8 storage[0x50];
};

class VibrationNodeConnection {
public:
    VibrationNodeConnection();
    ~VibrationNodeConnection();
    void Connect(VibrationNode* source, VibrationNode* destination);
    void SetModulation(const VibrationModulation& modulation);
    u8 storage[0x38];
};

using VibrationNodeConnectionList =
    nn::util::IntrusiveList<VibrationNodeConnection,
                            nn::util::IntrusiveListBaseNodeTraits<VibrationNodeConnection, VibrationNodeConnection>>;

// fn_7100070740 constructs one at +0 of its 0x1c0-byte player, with node connections at +0xf0
// and +0x128, so the player is 0xf0 bytes; the destructors fn_71000707dc and fn_7100070828 put
// the derived class's vtable (unk_7101a1a3a8) back at +0 before calling the base's.
class VibrationPlayer : public VibrationNode {
public:
    VibrationPlayer();
    ~VibrationPlayer();
    // fn_71000708c8 loads a vibration file from memory and sets looping.
    s32 Load(const void* file, u64 size);
    void Unload();
    void Play();
    void Stop();
    void SetLoop(bool loop);
    bool IsLoop() const;
    bool IsPlaying() const;
    s32 GetCurrentPosition() const;
    // Returned through x8 (more than 16 bytes); fn_7100070a28 reads the word at +0xc.
    VibrationFileInfo GetFileInfo() const;
    // The SDK's sampling step, which the game's override (fn_7100070fa0) calls first.
    void OnNextSampleRequired(VibrationValue* value, VibrationNodeConnectionList* connections);

    char* vtable;
    u8 storage_0x8[0xe8];
};

}  // namespace nn::hid
