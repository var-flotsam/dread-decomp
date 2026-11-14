// Unit 2772 (nn::atk by its library hint): the sound system's constructor. Its members come first, in order
// (include/game/UnkAtkSystem.h): the running flag, the event's state and the settings cleared, then the
// counter, the part at +0x558 and the three banks built. Then it links the empty list, sets up the mutexes,
// clears the queue words and the flags, puts the settings and the renderer parameter at their defaults
// (48 kHz, 240 mix buffers, 30 submixes, 96 voices, one sink) and aborts when the renderer rejects them.
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/global/CFilePathStrId.h"
#include "game/UnkAtkSystem.h"
#include "types.h"

namespace nn::diag::detail {
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line);
}  // namespace nn::diag::detail

namespace nn::audio {
struct AudioRendererParameter;
void InitializeAudioRendererParameter(AudioRendererParameter* parameter);
bool IsValidAudioRendererParameter(const AudioRendererParameter& parameter);
}  // namespace nn::audio

static inline nn::os::MutexType* Mx(u8* p) {
    return reinterpret_cast<nn::os::MutexType*>(p);
}

static inline nn::audio::AudioRendererParameter* Param(UnkAtkRendererParam_b0150* p) {
    return reinterpret_cast<nn::audio::AudioRendererParameter*>(p);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkAtkSystem_b0150::UnkAtkSystem_b0150() {
    list.link[0] = &list;
    list.link[1] = &list;
    nn::os::InitializeMutex(Mx(mutex_8a0), true, 0);
    for (int i = 9; i >= 0; i--)
        field_0x900[i] = 0;
    b_950 = 0;
    b_951 = 1;
    b_952 = 0;
    b_953 = 0;
    b_954 = 0;
    b_955 = 1;
    b_956 = 0;
    b_957 = 0;
    nn::os::InitializeMutex(Mx(mutex_960), true, 0);
    nn::os::InitializeMutex(Mx(mutex_980), true, 0);
    nn::os::InitializeMutex(Mx(mutex_9a0), true, 0);
    nn::os::InitializeMutex(Mx(mutex_9c0), true, 0);
    nn::os::InitializeMutex(Mx(mutex_9e0), true, 0);
    v_e8 = 2;
    v_ec = 1.0f;
    v_e0 = 4;
    v_e4 = 4;
    v_a08 = 0;
    b_a20 = 0;
    v_a38 = 0;
    v_a60 = 0;
    b_a66 = 0;
    v_f0 = 1.0f;
    v_f4 = 0;
    v_f8 = 0;
    v_fc = 1.0f;
    v_100 = 1.0f;
    v_104 = 0;
    v_108 = 0;
    nn::audio::InitializeAudioRendererParameter(Param(&param));
    param.sample_rate = 48000;
    param.mix_buffers = 240;
    param.sub_mixes = 30;
    param.field_0xc = 1;
    param.voices = 0x60;
    param.sinks = 1;
    param.flag = 0;
    param.count = 14;
    if (!nn::audio::IsValidAudioRendererParameter(*Param(&param)))
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0);
}
