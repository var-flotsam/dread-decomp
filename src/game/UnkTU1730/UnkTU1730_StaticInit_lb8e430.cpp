// Static initialiser of unit 1730 (tools/tu_map.py): the header statics, the reflected action enums
// (EActionProps, EActionPlayMode, EOnActionEnd, EActionSegmentType, EActionInsertFlags) and the
// TActionInsertFlagset flag set with their conversions, then the unit's transition blend settings.
// Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTU1730.h"

using namespace unk_static_init;

extern HeaderIds g_unk_71727697a0 __attribute__((visibility("hidden")));

extern UnkEnumType g_unk_7172768f18;
void fn_7100b8caec(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100b8c7c0(void* e);
extern char g_unk_7172769030[];
extern UnkConversion g_unk_7172769038;
extern UnkConversion g_unk_7172769058;
void fn_7100b8e0dc() __attribute__((visibility("hidden")));
void fn_7100b8e0e8() __attribute__((visibility("hidden")));

extern UnkEnumType g_unk_7172769078;
void fn_7100b8cdb8(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100b8cc6c(void* e);
extern char g_unk_7172769190[];
extern UnkConversion g_unk_7172769198;
extern UnkConversion g_unk_71727691b8;
void fn_7100b8e0f4() __attribute__((visibility("hidden")));
void fn_7100b8e100() __attribute__((visibility("hidden")));

extern UnkEnumType g_unk_71727691d8;
void fn_7100b8d144(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100b8cf38(void* e);
extern char g_unk_71727692f0[];
extern UnkConversion g_unk_71727692f8;
extern UnkConversion g_unk_7172769318;
void fn_7100b8e10c() __attribute__((visibility("hidden")));
void fn_7100b8e118() __attribute__((visibility("hidden")));

extern UnkEnumType g_unk_7172769338;
void fn_7100b8d470(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100b8d2c4(void* e);
extern char g_unk_7172769450[];
extern UnkConversion g_unk_7172769458;
extern UnkConversion g_unk_7172769478;
void fn_7100b8e124() __attribute__((visibility("hidden")));
void fn_7100b8e130() __attribute__((visibility("hidden")));

extern UnkEnumType g_unk_7172769498;
void fn_7100b8dc1c(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100b8d5f0(void* e);
extern char g_unk_71727695b0[];
extern UnkConversion g_unk_71727695b8;
extern UnkConversion g_unk_71727695d8;
void fn_7100b8e13c() __attribute__((visibility("hidden")));
void fn_7100b8e148() __attribute__((visibility("hidden")));

extern UnkEnumType g_unk_71727695f8;
void fn_7100b8dd9c(UnkEnumType* type, const CStrId& name, void* arg);
extern char g_unk_71727696e8[];
extern UnkConversion g_unk_71727696f0;
extern UnkConversion g_unk_7172769710;
void fn_7100b8e154() __attribute__((visibility("hidden")));
void fn_7100b8e160() __attribute__((visibility("hidden")));

// A transition's blend setting: its kind and mode in one word, and a value (unset for some kinds).
struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");
struct __attribute__((packed, aligned(4))) UnkDesc_SpitSend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkDesc_SpitSend) == 0xc, "UnkDesc_SpitSend layout");

// A float with a mode (the float's bits in the low word, the mode in the high).
extern u64 g_unk_7172769730;
extern u64 g_unk_7172769738;
extern UnkTrackBlend g_unk_7172769764;
extern UnkTrackBlend g_unk_717276977c;
extern UnkTrackBlend g_unk_7172769788;

// The kind word of a blend setting: the kind in the low half, the mode in the high.
static inline u64 BlendKind(u32 kind, u32 mode) { return (u64)mode << 32 | kind; }

void InitActionEnumStatics() {
    InitHeaderIds(&g_unk_71727697a0);
    InitEnum(&g_unk_7172768f18, "EActionProps", fn_7100b8caec, fn_7100b8c7c0, g_unk_7172769030, &g_unk_7172769038,
             fn_7100b8e0dc, &g_unk_7172769058, fn_7100b8e0e8);
    InitEnum(&g_unk_7172769078, "EActionPlayMode", fn_7100b8cdb8, fn_7100b8cc6c, g_unk_7172769190, &g_unk_7172769198,
             fn_7100b8e0f4, &g_unk_71727691b8, fn_7100b8e100);
    InitEnum(&g_unk_71727691d8, "EOnActionEnd", fn_7100b8d144, fn_7100b8cf38, g_unk_71727692f0, &g_unk_71727692f8,
             fn_7100b8e10c, &g_unk_7172769318, fn_7100b8e118);
    InitEnum(&g_unk_7172769338, "EActionSegmentType", fn_7100b8d470, fn_7100b8d2c4, g_unk_7172769450,
             &g_unk_7172769458, fn_7100b8e124, &g_unk_7172769478, fn_7100b8e130);
    InitEnum(&g_unk_7172769498, "EActionInsertFlags", fn_7100b8dc1c, fn_7100b8d5f0, g_unk_71727695b0,
             &g_unk_71727695b8, fn_7100b8e13c, &g_unk_71727695d8, fn_7100b8e148);
    InitFlagSet(&g_unk_71727695f8, "TActionInsertFlagset", fn_7100b8dd9c, fn_710009a368, g_unk_71727696e8,
                TypeOf<fn_7100005178>, &g_unk_71727696f0, fn_7100b8e154, &g_unk_7172769710, fn_7100b8e160);

    g_unk_7172769730 = 0;
    g_unk_7172769738 = (u64)2 << 32 | 0x3f800000;  // 1.0f, mode 2
    g_unk_7172769740.kind = BlendKind(0, 0);
    g_unk_7172769740.value = 0.0f;
    g_unk_717276974c.kind = BlendKind(0, 2);
    g_unk_717276974c.value = 1.0f;
    g_unk_7172769758.kind = BlendKind(1, 0);
    g_unk_7172769764.kind = BlendKind(1, 1);
    g_unk_7172769770.kind = BlendKind(1, 2);
    g_unk_717276977c.kind = BlendKind(2, 0);
    g_unk_7172769788.kind = BlendKind(2, 1);
    g_unk_7172769794.kind = BlendKind(2, 2);
}
