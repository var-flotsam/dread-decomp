#include <nn/os.h>

#include "base/global/CFilePathStrId.h"
#include "types.h"

namespace nn::os {
void DestroySystemEvent(SystemEventType* event);
}  // namespace nn::os

namespace nn::audio {
struct AudioRendererHandle {
    void* handle;
    void* context;
};
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

struct UnkAtkCounter_b0147;
struct UnkAtkTaskManager;
void fn_710122fef4(UnkAtkCounter_b0147* self);
void fn_7101237138(void* bank, u32 index);
void fn_71012378bc(void* bank);
void fn_7101236604(void* part, void* other);
u32 fn_7101214040(UnkAtkTaskManager* manager);

struct UnkAtkBank_b0150 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual s32 Count();  // 0x10
};

struct UnkAtkLink_b0150 {
    UnkAtkLink_b0150* link[2];  // prev, next
};

struct UnkAtkRendererParam_b0150 {
    u32 sample_rate;
    u32 mix_buffers;
    u32 sub_mixes;
    u32 field_0xc;
    u32 voices;
    u32 sinks;
    u64 count;
    u8 flag;
    u8 pad_0x21[0x1f];
};

struct UnkAtkSystem_b0150 {
    bool field_0x0;
    u8 pad_0x1[7];
    nn::audio::AudioRendererHandle renderer;  // 0x8
    u8 sub_0x18[0x78];                        // 0x18
    u8 event[0x50];                           // 0x90
    u32 v_e0;
    u32 v_e4;
    u32 v_e8;
    f32 v_ec;
    f32 v_f0;
    u32 v_f4;
    u32 v_f8;
    f32 v_fc;
    f32 v_100;
    u32 v_104;
    u32 v_108;
    u8 pad_0x10c[0x530 - 0x10c];
    u8 counter[0x28];                         // 0x530
    u8 part_558[0x50];                        // 0x558
    UnkAtkBank_b0150 bank_a;                  // 0x5a8
    u8 pad_0x5b0[0xf0];
    UnkAtkBank_b0150 bank_b;                  // 0x6a0
    u8 pad_0x6a8[0xf0];
    u8 part_798[0xf8];
    UnkAtkLink_b0150 list;                    // 0x890
    u8 mutex_8a0[0x20];
    UnkAtkRendererParam_b0150 param;          // 0x8c0
    u8 pad_0x900[0x50];
    u8 b_950;
    u8 b_951;
    u8 b_952;
    u8 b_953;
    u8 b_954;
    u8 b_955;
    u8 b_956;
    u8 b_957;
    u8 pad_0x958[0xb0];
    u32 v_a08;
    u8 pad_0xa0c[0x54];
    u32 v_a60;
    u8 b_a64;
    u8 b_a65;
    u8 b_a66;
};
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, bank_b) == 0x6a0, "layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, part_798) == 0x798, "layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, b_950) == 0x950, "layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, v_a08) == 0xa08, "layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, b_a64) == 0xa64, "layout");

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
    fn_710122fef4((UnkAtkCounter_b0147*)self->counter);
    nn::audio::StopAudioRenderer(self->renderer);
    if (self->b_950) {
        if (self->b_951) {
            DrainBank(&self->bank_a);
            if (self->b_952)
                DrainBank(&self->bank_b);
            if (self->b_953)
                fn_7101237138(self->part_798, 0);
            self->b_950 = 0;
        }
        fn_7101237138(self->part_558, 0);
    }
    fn_71012378bc(&self->bank_a);
    fn_71012378bc(&self->bank_b);
    fn_71012378bc(self->part_798);
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
    fn_7101236604(self->part_558, self->sub_0x18);
    nn::audio::CloseAudioRenderer(self->renderer);
    nn::os::DestroySystemEvent((nn::os::SystemEventType*)self->event);
    if (self->v_a08)
        self->v_a08 = 0;
    self->v_e8 = 2;
    self->v_ec = 1.0f;
    self->v_e0 = 4;
    self->v_e4 = 4;
    self->v_f0 = 1.0f;
    self->v_f4 = 0;
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
