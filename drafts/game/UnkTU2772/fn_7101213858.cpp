#include <nn/os.h>

#include "base/global/CFilePathStrId.h"
#include "types.h"

namespace nn::diag::detail {
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line);
}  // namespace nn::diag::detail

namespace nn::audio {
struct AudioRendererParameter;
void InitializeAudioRendererParameter(AudioRendererParameter* parameter);
bool IsValidAudioRendererParameter(const AudioRendererParameter& parameter);
}  // namespace nn::audio

void fn_710122fde4(void* part);
void fn_710123675c(void* part);
void fn_71012372c4(void* part);
extern void* g_unk_7101cb8da0[];

struct UnkAtkRendererParam_b0150 {
    u32 sample_rate;   // 0x0
    u32 mix_buffers;   // 0x4
    u32 sub_mixes;     // 0x8
    u32 field_0xc;
    u32 voices;        // 0x10
    u32 sinks;         // 0x14
    u64 count;         // 0x18
    u8 flag;           // 0x20
    u8 pad_0x21[0x1f];
};

struct UnkAtkSystem_b0150 {
    bool field_0x0;
    u8 pad_0x1[0xb7];
    bool field_0xb8;
    u8 pad_0xb9[0x27];
    u32 v_e0;   // 0xe0
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
    u8 part_530[0x28];
    void** vtable_558;   // 0x558
    u8 pad_0x560[0x5a8 - 0x560];
    u8 part_5a8[0xf8];
    u8 part_6a0[0xf8];
    u8 part_798[0xf8];
    void* list_prev;     // 0x890
    void* list_next;     // 0x898
    u8 mutex_8a0[0x20];
    UnkAtkRendererParam_b0150 param;  // 0x8c0
    u64 zero_900[10];    // 0x900
    u32 f_950;
    u32 f_954;
    u8 pad_0x958[8];
    u8 mutex_960[0x20];
    u8 mutex_980[0x20];
    u8 mutex_9a0[0x20];
    u8 mutex_9c0[0x20];
    u8 mutex_9e0[0x20];
    u8 pad_0xa00[8];
    u32 v_a08;
    u8 pad_0xa0c[0x14];
    u8 b_a20;
    u8 pad_0xa21[0x17];
    u64 v_a38;
    u8 pad_0xa40[0x20];
    u32 v_a60;
    u8 pad_0xa64[2];
    u8 b_a66;
};
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, b_a66) == 0xa66, "layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, mutex_960) == 0x960, "layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, zero_900) == 0x900, "layout");

static inline nn::os::MutexType* Mx(u8* p) {
    return reinterpret_cast<nn::os::MutexType*>(p);
}

void fn_7101213858(UnkAtkSystem_b0150* self) {
    self->field_0x0 = false;
    self->field_0xb8 = false;
    self->v_ec = 0;
    self->v_f0 = 0;
    self->v_f4 = 0;
    self->v_f8 = 0;
    self->v_fc = 0;
    self->v_100 = 0;
    self->v_104 = 0;
    self->v_108 = 0;
    fn_710122fde4(self->part_530);
    fn_710123675c(&self->vtable_558);
    self->vtable_558 = g_unk_7101cb8da0 + 2;
    fn_71012372c4(self->part_5a8);
    fn_71012372c4(self->part_6a0);
    fn_71012372c4(self->part_798);
    self->list_prev = &self->list_prev;
    self->list_next = &self->list_prev;
    nn::os::InitializeMutex(Mx(self->mutex_8a0), true, 0);
    for (int i = 9; i >= 0; i--)
        self->zero_900[i] = 0;
    self->f_950 = 0x100;
    self->f_954 = 0x100;
    nn::os::InitializeMutex(Mx(self->mutex_960), true, 0);
    nn::os::InitializeMutex(Mx(self->mutex_980), true, 0);
    nn::os::InitializeMutex(Mx(self->mutex_9a0), true, 0);
    nn::os::InitializeMutex(Mx(self->mutex_9c0), true, 0);
    nn::os::InitializeMutex(Mx(self->mutex_9e0), true, 0);
    self->v_e8 = 2;
    self->v_ec = 1.0f;
    self->v_e0 = 4;
    self->v_e4 = 4;
    self->v_a08 = 0;
    self->b_a20 = 0;
    self->v_a38 = 0;
    self->v_a60 = 0;
    self->b_a66 = 0;
    self->v_f0 = 1.0f;
    self->v_f4 = 0;
    self->v_f8 = 0;
    self->v_fc = 1.0f;
    self->v_100 = 1.0f;
    self->v_104 = 0;
    self->v_108 = 0;
    nn::audio::InitializeAudioRendererParameter(reinterpret_cast<nn::audio::AudioRendererParameter*>(&self->param));
    self->param.sample_rate = 48000;
    self->param.mix_buffers = 240;
    self->param.sub_mixes = 30;
    self->param.field_0xc = 1;
    self->param.voices = 0x60;
    self->param.sinks = 1;
    self->param.flag = 0;
    self->param.count = 14;
    if (!nn::audio::IsValidAudioRendererParameter(*reinterpret_cast<nn::audio::AudioRendererParameter*>(&self->param)))
        nn::diag::detail::AbortImpl(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0);
}
