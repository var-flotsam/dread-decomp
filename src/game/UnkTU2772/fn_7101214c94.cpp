// Unit 2772 (tools/tu_map.py; nn::atk by its library hint): the sound system's shutdown. When it is running:
// stops the audio renderer, drains the two banks and the part at +0x798, empties the list at +0x890, runs
// what is queued (fn_7101214040, aborting on a failing result), closes the renderer and its event, and puts
// the settings and the renderer parameter back to their defaults. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/global/CFilePathStrId.h"
#include "game/UnkAtkSystem.h"
#include "game/UnkTU2772.h"
#include "types.h"

namespace nn::os {
void DestroySystemEvent(SystemEventType* event);
}  // namespace nn::os

namespace nn::audio {
struct AudioRendererParameter;
void StopAudioRenderer(AudioRendererHandle handle);
void CloseAudioRenderer(AudioRendererHandle handle);
void InitializeAudioRendererParameter(AudioRendererParameter* parameter);
bool IsValidAudioRendererParameter(const AudioRendererParameter& parameter);
}  // namespace nn::audio

namespace nn {
class Result;
namespace diag::detail {
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line);
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line,
                            const Result* result, const char* format, ...);
}  // namespace diag::detail
}  // namespace nn

struct UnkAtkTaskManager;
void fn_710122fef4(UnkAtkCounter_b0147* self);
void fn_7101237138(void* bank, u32 index);
void fn_71012378bc(void* bank);
void fn_7101236604(void* part, void* other);

static inline void DrainBank(UnkAtkBank_b0150* bank) {
    if (bank->Count() >= 1) {
        s32 i = 0;
        do {
            fn_7101237138(bank, i);
            i++;
        } while (i < bank->Count());
    }
}

void fn_7101214c94(UnkAtkSystem_b0150* self) {
    if (!self->field_0x0)
        return;
    fn_710122fef4(&self->counter);
    nn::audio::StopAudioRenderer(self->renderer);
    if (self->b_950) {
        if (self->b_951) {
            DrainBank(&self->bank_a);
            if (self->b_952)
                DrainBank(&self->bank_b);
            if (self->b_953)
                fn_7101237138(&self->bank_c, 0);
            self->b_950 = 0;
        }
        fn_7101237138(&self->part_558, 0);
    }
    fn_71012378bc(&self->bank_a);
    fn_71012378bc(&self->bank_b);
    fn_71012378bc(&self->bank_c);
    while (self->list.link[1] != &self->list) {
        UnkAtkLink_b0150* node = self->list.link[1];
        UnkAtkLink_b0150* prev = node->link[0];
        UnkAtkLink_b0150* next = node->link[1];
        UnkAtkLink_b0150* last = next->link[0];
        prev->link[1] = next;
        next->link[0] = node->link[0];
        last->link[1] = node;
        node->link[0] = last;
    }
    alignas(8) u32 result = fn_7101214040((UnkAtkTaskManager*)self);
    if (result)
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0,
                                    (const nn::Result*)&result, g_unk_710162e2ed);
    fn_7101236604(&self->part_558, self->sub_0x18);
    nn::audio::CloseAudioRenderer(self->renderer);
    nn::os::DestroySystemEvent(&self->event.event);
    if (self->v_a08)
        self->v_a08 = 0;
    self->v_e4 = 4;
    self->v_ec = 1.0f;
    self->v_e8 = 2;
    self->v_f4 = 0;
    self->v_f0 = 1.0f;
    self->v_e0 = 4;
    self->v_f8 = 0;
    self->v_fc = 1.0f;
    self->v_100 = 1.0f;
    self->v_104 = 0;
    self->v_108 = 0;
    nn::audio::InitializeAudioRendererParameter((nn::audio::AudioRendererParameter*)&self->param);
    self->param.sample_rate = 48000;
    self->param.mix_buffers = 240;
    self->param.sub_mixes = 30;
    self->param.field_0xc = 1;
    self->param.voices = 0x60;
    self->param.sinks = 1;
    self->param.flag = 0;
    self->param.count = 14;
    if (!nn::audio::IsValidAudioRendererParameter(*(nn::audio::AudioRendererParameter*)&self->param))
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0);
    self->v_a60 = 0;
    self->b_955 = 1;
    self->field_0x0 = false;
    self->b_951 = 1;
    self->b_952 = 0;
    self->b_953 = 0;
    self->b_954 = 0;
    self->b_956 = 0;
    self->b_957 = 0;
    self->b_a64 = 0;
    self->b_a65 = 0;
    self->b_a66 = 0;
}
