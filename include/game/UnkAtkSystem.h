#pragma once

// The sound system of unit 2772 (nn::atk by its library hint): a function-local singleton
// (src/game_lo/fn_71001039f8.cpp) built by fn_7101213858 and shut down by fn_7101214c94. One layout for
// every source that reads or writes it. Names and layouts are placeholders.
//
// Its constructor builds the sub-objects in member order, as the original does: the running flag, the
// renderer event's state and the settings first, then the counter at +0x530 (fn_710122fde4), the part at
// +0x558 (its base's constructor fn_710123675c, then its own vtable) and the three banks (fn_71012372c4).
#include <nn/os.h>

#include "types.h"

namespace nn::audio {
struct AudioRendererHandle {
    void* handle;
    void* context;
};
}  // namespace nn::audio

struct UnkAtkCounter_b0147 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    u32 field_0x24;

    UnkAtkCounter_b0147();  // fn_710122fde4
};

// A queue of sounds (0xf8 bytes); fn_7101214c94 counts and drains it through its virtuals.
struct UnkAtkBank_b0150 {
    UnkAtkBank_b0150();  // fn_71012372c4
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual s32 Count();  // 0x10
    u8 unk_0x8[0xf0];
};
static_assert(sizeof(UnkAtkBank_b0150) == 0xf8, "UnkAtkBank_b0150 layout");

// The part at +0x558: built by its base's constructor, then given its own vtable.
struct UnkAtkPartBase_b0150 {
    UnkAtkPartBase_b0150();  // fn_710123675c
    virtual void vf_0x0();
    u8 unk_0x8[0x48];
};

struct UnkAtkPart_b0150 : UnkAtkPartBase_b0150 {  // vtable g_unk_7101cb8da0
    UnkAtkPart_b0150() {}
    void vf_0x0() override;
};
static_assert(sizeof(UnkAtkPart_b0150) == 0x50, "UnkAtkPart_b0150 layout");

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

// The renderer's event: the constructor marks it not initialized (fn_7101214c94 destroys it).
struct UnkAtkSystemEvent_b0150 {
    nn::os::SystemEventType event;

    UnkAtkSystemEvent_b0150() { event.state = nn::os::SystemEventType::State_NotInitialized; }
};

struct UnkAtkSystem_b0150 {
    bool field_0x0 = false;  // running
    u8 pad_0x1[7];
    nn::audio::AudioRendererHandle renderer;  // 0x8
    u8 sub_0x18[0x78];                        // 0x18
    UnkAtkSystemEvent_b0150 event;            // 0x90
    u8 pad_0xc0[0x20];
    u32 v_e0;
    u32 v_e4;
    u32 v_e8;
    f32 v_ec = 0;
    f32 v_f0 = 0;
    u32 v_f4 = 0;
    u32 v_f8 = 0;
    f32 v_fc = 0;
    f32 v_100 = 0;
    u32 v_104 = 0;
    u32 v_108 = 0;
    u8 pad_0x10c[0x530 - 0x10c];
    UnkAtkCounter_b0147 counter;  // 0x530
    UnkAtkPart_b0150 part_558;    // 0x558
    UnkAtkBank_b0150 bank_a;      // 0x5a8
    UnkAtkBank_b0150 bank_b;      // 0x6a0
    UnkAtkBank_b0150 bank_c;      // 0x798
    UnkAtkLink_b0150 list;        // 0x890
    u8 mutex_8a0[0x20];
    UnkAtkRendererParam_b0150 param;  // 0x8c0
    u64 field_0x900[10];
    // Eight flags, set together (the constructor's one 64-bit store: b_951 and b_955).
    u8 b_950;
    u8 b_951;
    u8 b_952;
    u8 b_953;
    u8 b_954;
    u8 b_955;
    u8 b_956;
    u8 b_957;
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
    u8 b_a64;
    u8 b_a65;
    u8 b_a66;

    UnkAtkSystem_b0150();  // fn_7101213858
    ~UnkAtkSystem_b0150();
};
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, event) == 0x90, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, v_e0) == 0xe0, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, counter) == 0x530, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, bank_b) == 0x6a0, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, bank_c) == 0x798, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, b_950) == 0x950, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, v_a08) == 0xa08, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(UnkAtkSystem_b0150, b_a66) == 0xa66, "UnkAtkSystem_b0150 layout");
static_assert(__builtin_offsetof(nn::os::SystemEventType, state) == 0x28, "the event's state is at +0xb8");
