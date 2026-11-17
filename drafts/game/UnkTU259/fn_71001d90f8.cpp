// Unit 259: sets one of the graphics system's screen effect parameters by its id: five switches
// (a value above one half turns one on) and two ways of starting the timed effect, with a given
// description or the default one. Names and layouts are placeholders.
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

// How the timed effect runs (0x1c bytes).
struct UnkEffectDesc_l1d90f8 {
    f32 duration;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
};

// The two effects' state.
struct UnkEffectTimers_l1d90f8 {
    bool first_active;
    bool second_active;
    u8 pad_0x2[0x6];
    f32 first_time;   // 0x8
    f32 second_time;  // 0xc
    u8 pad_0x10[0x40];
    UnkEffectDesc_l1d90f8 desc;  // 0x50
};
static_assert(__builtin_offsetof(UnkEffectTimers_l1d90f8, desc) == 0x50, "UnkEffectTimers_l1d90f8 layout");

// The effect parameters the renderer reads, after a scale the second effect reads.
struct UnkEffectParams_l1d90f8 {
    u8 pad_0x0[0x28];
    f32 field_0x28;
    f32 field_0x2c;
    u8 pad_0x30[0x8];
    f32 field_0x38;
    u8 pad_0x3c[0x20];
};
struct UnkEffectState_l1d90f8 {
    f32 scale;
    UnkEffectParams_l1d90f8 params;  // 0x4
};

extern HIDDEN UnkEffectTimers_l1d90f8 g_unk_716e595f88;
extern HIDDEN UnkEffectState_l1d90f8 g_unk_7101a1e84c;
extern HIDDEN bool g_unk_7101a1e838;
extern HIDDEN bool g_unk_7101a1e83c;
extern HIDDEN bool g_unk_7101a1e840;
extern HIDDEN bool g_unk_7101a1e844;
extern HIDDEN bool g_unk_7101a1e848;

static inline void StartEffect(const UnkEffectDesc_l1d90f8& desc) {
    UnkEffectTimers_l1d90f8& s = g_unk_716e595f88;
    UnkEffectParams_l1d90f8& p = g_unk_7101a1e84c.params;
    s.desc = desc;
    s.first_time = s.desc.duration;
    p.field_0x28 = s.desc.field_0x8;
    p.field_0x2c = s.desc.field_0xc;
    p.field_0x38 = s.desc.field_0x10;
}

static inline bool IsOn(const void* value) { return *static_cast<const f32*>(value) > 0.5f; }

void fn_71001d90f8(void* self, u64 id, const void* value) {
    switch (id) {
    case 0xede73d1953baf654: {
        const UnkEffectDesc_l1d90f8 desc = {0.3f, 0.1f, 6.9f, 0.875f, 0.14f, 0.015f, 0.0f};
        g_unk_7101a1e84c.scale = *static_cast<const f32*>(value);
        g_unk_716e595f88.second_time = 0.15f;
        StartEffect(desc);
        break;
    }
    case 0xe936254db735c615:
        if (value)
            StartEffect(*static_cast<const UnkEffectDesc_l1d90f8*>(value));
        break;
    case 0x9425d28bc42d531c:
        g_unk_7101a1e83c = IsOn(value);
        break;
    case 0x951be7e886df8ccb:
        g_unk_7101a1e840 = IsOn(value);
        break;
    case 0x9c4f904016c7fa77:
        g_unk_7101a1e844 = IsOn(value);
        break;
    case 0x098464e77967a19e:
        g_unk_7101a1e848 = IsOn(value);
        break;
    case 0x1bfe5fabd24ae2a0:
        g_unk_7101a1e838 = IsOn(value);
        break;
    }
}
