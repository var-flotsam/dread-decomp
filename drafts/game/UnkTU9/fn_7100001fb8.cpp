// Unit 9 (tools/tu_map.py): the constructor of the debug UI's context (Dear ImGui's, docs/references.md): its
// input and output block (window size, frame time, file names, key map, allocator hooks), its style
// (fn_71000026cc), its lists and its overlay draw list. Names and layouts are placeholders.
#include <stdlib.h>
#include <string.h>

#include "types.h"

struct UnkTunables_l0026cc;
void fn_71000026cc(UnkTunables_l0026cc* self);
struct UnkPoseBuffer_l002e74;
void fn_7100002e74(UnkPoseBuffer_l002e74* self);
struct UnkBlendState_ovb0003;
void fn_7100003448(UnkBlendState_ovb0003* self);
// The default clipboard and input-method hooks (defined in this unit).
__attribute__((visibility("hidden"))) u64 fn_7100002b10();
__attribute__((visibility("hidden"))) char* fn_7100002b30(char* p0, char* p1);
__attribute__((visibility("hidden"))) void fn_7100002bd0();

// The default font atlas.
__attribute__((visibility("hidden"))) extern u8 g_unk_7101d477a8[];

struct UnkVec2_l001fb8 {
    f32 x;
    f32 y;
    UnkVec2_l001fb8() : x(0.0f), y(0.0f) {}
    UnkVec2_l001fb8(f32 x_, f32 y_) : x(x_), y(y_) {}
};

struct UnkRect_l001fb8 {
    UnkVec2_l001fb8 min;
    UnkVec2_l001fb8 max;
    UnkRect_l001fb8() : min(3.4028235e+38f, 3.4028235e+38f), max(-3.4028235e+38f, -3.4028235e+38f) {}
};

struct UnkVector_l001fb8 {
    s32 size;
    s32 capacity;
    void* data;
    UnkVector_l001fb8() : size(0), capacity(0), data(nullptr) {}
};

// The input and output block.
struct UnkIO_l001fb8 {
    UnkVec2_l001fb8 display_size;     // 0x0
    f32 delta_time;                   // 0x8
    f32 ini_saving_rate;              // 0xc
    const char* ini_filename;         // 0x10
    const char* log_filename;         // 0x18
    f32 double_click_time;            // 0x20
    f32 double_click_max_distance;    // 0x24
    f32 drag_threshold;               // 0x28
    s32 key_map[19];                  // 0x2c
    f32 key_repeat_delay;             // 0x78
    f32 key_repeat_rate;              // 0x7c
    void* user_data;                  // 0x80
    void* fonts;                      // 0x88
    f32 font_scale;                   // 0x90
    void* font_default;               // 0x98
    UnkVec2_l001fb8 framebuffer_scale;  // 0xa0
    u8 pad_0xa8[0x18];
    void* field_0xc0;
    u64 (*get_clipboard)();           // 0xc8
    char* (*set_clipboard)(char*, char*);  // 0xd0
    void* field_0xd8;
    void* (*alloc)(size_t);           // 0xe0
    void (*free)(void*);              // 0xe8
    void (*set_ime_position)();       // 0xf0
    u8 pad_0xf8[0x8];
    UnkVec2_l001fb8 mouse_pos;        // 0x100
    u8 pad_0x108[0x254];
    UnkVec2_l001fb8 mouse_pos_prev;   // 0x35c
    u8 pad_0x364[0x54];
    f32 mouse_down_duration[5];       // 0x3b8
    f32 mouse_down_duration_prev[5];  // 0x3cc
    u8 pad_0x3e0[0x14];
    f32 keys_down_duration[512];      // 0x3f4
    f32 keys_down_duration_prev[512];  // 0xbf4
    u8 pad_0x13f4[0x4];

    UnkIO_l001fb8() {
        memset(static_cast<void*>(this), 0, sizeof(*this));
        display_size = UnkVec2_l001fb8(-1.0f, -1.0f);
        delta_time = 1.0f / 60.0f;
        ini_saving_rate = 5.0f;
        ini_filename = "imgui.ini";
        log_filename = "imgui_log.txt";
        fonts = g_unk_7101d477a8;
        font_scale = 1.0f;
        font_default = nullptr;
        framebuffer_scale = UnkVec2_l001fb8(1.0f, 1.0f);
        mouse_pos = UnkVec2_l001fb8(-1.0f, -1.0f);
        mouse_pos_prev = UnkVec2_l001fb8(-1.0f, -1.0f);
        double_click_time = 0.30f;
        double_click_max_distance = 6.0f;
        drag_threshold = 6.0f;
        for (s32 i = 0; i < 5; i++)
            mouse_down_duration[i] = mouse_down_duration_prev[i] = -1.0f;
        for (s32 i = 0; i < 512; i++)
            keys_down_duration[i] = keys_down_duration_prev[i] = -1.0f;
        for (s32 i = 0; i < 19; i++)
            key_map[i] = -1;
        key_repeat_delay = 0.250f;
        key_repeat_rate = 0.050f;
        user_data = nullptr;
        field_0xc0 = nullptr;
        alloc = malloc;
        free = ::free;
        get_clipboard = fn_7100002b10;
        set_clipboard = fn_7100002b30;
        field_0xd8 = nullptr;
        set_ime_position = fn_7100002bd0;
    }
};
static_assert(sizeof(UnkIO_l001fb8) == 0x13f8, "layout");

struct UnkStyle_l001fb8 {
    u8 data[0x330];
    UnkStyle_l001fb8() { fn_71000026cc(reinterpret_cast<UnkTunables_l0026cc*>(this)); }
};

struct UnkDrawList_l001fb8 {
    UnkVector_l001fb8 commands;  // 0x0
    UnkVector_l001fb8 indices;   // 0x10
    UnkVector_l001fb8 vertices;  // 0x20
    const char* owner_name = nullptr;  // 0x30
    u8 pad_0x38[0x18];
    UnkVector_l001fb8 field_0x50[3];
    u8 pad_0x80[0x8];
    UnkVector_l001fb8 field_0x88;

    UnkDrawList_l001fb8() { fn_7100002e74(reinterpret_cast<UnkPoseBuffer_l002e74*>(this)); }
};
static_assert(sizeof(UnkDrawList_l001fb8) == 0x98, "layout");

struct UnkSlot_l001fb8 {
    u32 field_0x0;
    UnkVec2_l001fb8 values[6];
};

struct UnkSlots_l001fb8 {
    u32 field_0x0;
    UnkSlot_l001fb8 slots[7];
};
static_assert(sizeof(UnkSlots_l001fb8) == 0x170, "layout");

struct UnkBlendState_l001fb8 {
    u8 data[0x70];
    UnkBlendState_l001fb8() { fn_7100003448(reinterpret_cast<UnkBlendState_ovb0003*>(this)); }
};

struct UnkContext_l001fb8 {
    bool initialized;
    UnkIO_l001fb8 io;                      // 0x8
    UnkStyle_l001fb8 style;                // 0x1400
    u64 field_0x1730;
    u64 field_0x1738;
    UnkVec2_l001fb8 field_0x1740;
    u64 field_0x1748;
    u64 field_0x1750;
    UnkVector_l001fb8 field_0x1758;
    UnkVector_l001fb8 field_0x1768;
    u64 field_0x1778;
    UnkVector_l001fb8 field_0x1780;
    u8 field_0x1790[0x1d];
    u8 pad_0x17ad[0x3];
    u8 field_0x17b0[0xf];
    UnkVec2_l001fb8 field_0x17c0;
    u64 field_0x17c8;
    u64 field_0x17d0;
    u32 field_0x17d8;
    UnkVector_l001fb8 field_0x17e0;
    u32 field_0x17f0;
    UnkVector_l001fb8 field_0x17f8[6];
    UnkVec2_l001fb8 field_0x1858;
    bool field_0x1860;
    f32 field_0x1864[4];
    UnkRect_l001fb8 field_0x1874;
    u8 pad_0x1884[0x4];
    u8 field_0x1888[0x13];
    u32 field_0x189c;
    bool field_0x18a0 = false;
    UnkVector_l001fb8 field_0x18a8;
    u32 field_0x18b8 = 0;
    UnkVector_l001fb8 field_0x18c0;
    UnkVector_l001fb8 field_0x18d0;
    UnkVector_l001fb8 field_0x18e0;
    u32 field_0x18f0;
    UnkDrawList_l001fb8 overlay;           // 0x18f8
    UnkSlots_l001fb8 field_0x1990;
    u8 field_0x1b00[0xce8] = {};
    UnkBlendState_l001fb8 field_0x27e8;
    u32 field_0x2858;
    u64 field_0x2860;
    u64 field_0x2868;
    u64 field_0x2870;
    u32 field_0x2878;
    UnkVec2_l001fb8 field_0x287c;
    f32 field_0x2884;
    u8 field_0x2888[0x410];
    UnkVec2_l001fb8 field_0x2c98;
    UnkVec2_l001fb8 field_0x2ca0;
    bool field_0x2ca8;
    u64 field_0x2cb0;
    u64 field_0x2cb8;
    u32 field_0x2cc0;
    u32 field_0x2cc4;
    u8 field_0x2cc8[0x1e8];
    u64 field_0x2eb0;
    u8 field_0x2eb8[0xc01];

    UnkContext_l001fb8() asm("fn_7100001fb8");
};

UnkContext_l001fb8::UnkContext_l001fb8() {
    field_0x2868 = 0;
    field_0x2860 = 0;
    initialized = false;
    field_0x1730 = 0;
    field_0x1738 = 0;
    field_0x1740 = UnkVec2_l001fb8();
    field_0x1748 = 0;
    field_0x1750 = ~0ull;
    field_0x1778 = 0;
    memset(field_0x1790, 0, sizeof(field_0x1790));
    memset(field_0x17b0, 0, sizeof(field_0x17b0));
    field_0x17c0 = UnkVec2_l001fb8(-1.0f, -1.0f);
    field_0x17f0 = 0;
    field_0x1860 = false;
    field_0x17d8 = 0;
    field_0x17d0 = 0;
    field_0x17c8 = 0;
    field_0x17f8[5] = UnkVector_l001fb8();
    for (s32 i = 0; i < 4; i++)
        field_0x1864[i] = 0.0f;
    field_0x1874 = UnkRect_l001fb8();
    field_0x189c = 0;
    field_0x2858 = 0;
    field_0x2870 = 0;
    field_0x2878 = 0;
    memset(field_0x1888, 0, sizeof(field_0x1888));
    field_0x287c = UnkVec2_l001fb8(0.01f, 0.01f);
    field_0x2884 = 10.0f;
    memset(field_0x2888, 0, sizeof(field_0x2888));
    overlay.owner_name = "##Overlay";
    field_0x2ca0 = UnkVec2_l001fb8(-1.0f, -1.0f);
    field_0x2c98 = UnkVec2_l001fb8(-1.0f, -1.0f);
    field_0x18f0 = 0;
    field_0x2ca8 = false;
    memset(&field_0x1990, 0, sizeof(field_0x1990));
    field_0x2cb0 = 0;
    field_0x2cb8 = 0;
    field_0x2cc0 = 0;
    field_0x2cc4 = 2;
    memset(field_0x2cc8, 0, sizeof(field_0x2cc8));
    field_0x2eb0 = ~0ull;
    memset(field_0x2eb8, 0, sizeof(field_0x2eb8));
}
static_assert(__builtin_offsetof(UnkContext_l001fb8, style) == 0x1400, "layout");
static_assert(__builtin_offsetof(UnkContext_l001fb8, field_0x1790) == 0x1790, "layout");
static_assert(__builtin_offsetof(UnkContext_l001fb8, field_0x1874) == 0x1874, "layout");
static_assert(__builtin_offsetof(UnkContext_l001fb8, overlay) == 0x18f8, "layout");
static_assert(__builtin_offsetof(UnkContext_l001fb8, field_0x27e8) == 0x27e8, "layout");
static_assert(__builtin_offsetof(UnkContext_l001fb8, field_0x2888) == 0x2888, "layout");
static_assert(__builtin_offsetof(UnkContext_l001fb8, field_0x2cc8) == 0x2cc8, "layout");
static_assert(__builtin_offsetof(UnkContext_l001fb8, field_0x2eb8) == 0x2eb8, "layout");
