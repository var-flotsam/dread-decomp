// Static initialiser of the unit of CEmmyGrabSamusAttack: the header ids, then the unit's own
// statics (path ids, ids, strings, small-buffer strings, floats) and the class registration.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

// A string with 0x20 bytes of inline storage (0x50 bytes in all).
extern void* g_unk_7101a479c8[];
void fn_7100504fdc(void* self);  // its destructor, as registered

struct UnkInlineStr50Raw {
    u8 str[0x30];
    void** vtable;
    u8 pad_0x38[8];
    u64 field_0x40;
    u32 field_0x48;
    u8 pad_0x4c[4];
};

__attribute__((always_inline)) inline void BuildInlineStr50(UnkInlineStr50Raw* s, const char* str) {
    auto* string = reinterpret_cast<base::global::CRntString*>(s);
    fn_710007e6e4(string, reinterpret_cast<base::UnkAllocator*>(&s->vtable));
    s->vtable = g_unk_7101a479c8 + 2;
    s->field_0x40 = 0;
    s->field_0x48 = 0;
    fn_710007e77c(string, str);
}

__attribute__((always_inline)) inline void InitInlineStr50(UnkInlineStr50Raw* s, const char* str) {
    BuildInlineStr50(s, str);
    __cxa_atexit(fn_7100504fdc, s, g_unk_7101a19000);
}

// A temporary of the same layout.
struct UnkInlineStr50 : UnkInlineStr50Raw {
    UnkInlineStr50(const char* str) { BuildInlineStr50(this, str); }
    ~UnkInlineStr50() {
        fn_710007ed40(reinterpret_cast<base::global::CRntString*>(this));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
    }
};

__attribute__((always_inline)) inline void InitRntString(base::UnkNamedEntry* s, const char* str) {
    fn_710007e704(s, str);
    __cxa_atexit(reinterpret_cast<void (*)(void*)>(fn_710007ecd4), s, g_unk_7101a19000);
}

struct UnkStatics_71005113b0 {
    HeaderIds ids;
    u8 pad_0x18[8];
    UnkFilePathStrIdBase field_0x20;
    UnkFilePathStrIdBase field_0x28;
    UnkFilePathStrIdBase field_0x30;
    UnkFilePathStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    UnkStrIdBase field_0x68;
    UnkStrIdBase field_0x70;
    UnkStrIdBase field_0x78;
    UnkFilePathStrIdBase field_0x80;
    u8 pad_0x88[0x70];
    f32 field_0xf8[2];
    bool field_0x100;
    u8 pad_0x101[3];
    f32 field_0x104[4];
    u8 pad_0x114[4];
    base::UnkNamedEntry field_0x118;
    base::UnkNamedEntry field_0x148;
    base::UnkNamedEntry field_0x178;
    base::UnkNamedEntry field_0x1a8;
    UnkInlineStr50Raw field_0x1d8;
    UnkInlineStr50Raw field_0x228;
    UnkInlineStr50Raw field_0x278;
    UnkInlineStr50Raw field_0x2c8;
    u8 pad_0x318[0xe0];
    u8 field_0x3f8[8];
    u8 pad_0x400[0x130];
    u8 field_0x530[8];
};
extern UnkStatics_71005113b0 g_unk_71726de558 __attribute__((visibility("hidden")));
extern char g_unk_71726de550[];
void fn_7100505000(void* self, const UnkInlineStr50* strs, u32 count);
void fn_710050512c(void* self);  // its destructor, as registered

void fn_71005113b0() {
    UnkStatics_71005113b0& g = g_unk_71726de558;
    InitHeaderIds(&g.ids);
    g.field_0xf8[0] = -1.0f;
    g.field_0xf8[1] = -1.0f;
    g.field_0x100 = true;
    fn_7100504d60();
    fn_710009ec00(g_unk_71726de550, g.field_0x3f8);
    InitPathId(&g.field_0x20, "gui/blockable_emmy.wav");
    InitPathId(&g.field_0x28, "actors/samus/samus_kickattack_05.wav");
    InitPathId(&g.field_0x30, "actors/emmy/melee_success_01.wav");
    InitPathId(&g.field_0x38, "actors/characters/shared/fx/blockingattackend.bcptl");
    InitId(&g.field_0x40, "R01_Finger1_SKN");
    InitId(&g.field_0x48, "C01_Head_SKN");
    InitRntString(&g.field_0x118, "far2");
    InitRntString(&g.field_0x148, "far3");
    InitRntString(&g.field_0x178, "far4");
    InitRntString(&g.field_0x1a8, "headon");
    InitId(&g.field_0x50, "EmmyGrabSamusAttack");
    InitInlineStr50(&g.field_0x1d8, "_samus");
    InitInlineStr50(&g.field_0x228, "_emmy");
    g.field_0x104[0] = 400.0f;
    g.field_0x104[1] = 300.0f;
    g.field_0x104[2] = 200.0f;
    g.field_0x104[3] = 100.0f;
    {
        UnkInlineStr50 strs[4] = {"4m", "3m", "2m", "1m"};
        fn_7100505000(g.field_0x530, strs, 4);
    }
    __cxa_atexit(fn_710050512c, g.field_0x530, g_unk_7101a19000);
    InitInlineStr50(&g.field_0x278, "_tunnel");
    InitInlineStr50(&g.field_0x2c8, "_mb");
    InitId(&g.field_0x58, "Character_Tunnel");
    InitId(&g.field_0x60, "morphball_grab");
    InitId(&g.field_0x68, "TunnelNear");
    InitId(&g.field_0x70, "IgnoreImpactEmmyGrab");
    InitId(&g.field_0x78, "EmmyGrabSamusAttack");
    InitPathId(&g.field_0x80, "actors/characters/emmy/cameras/grabprotoemmytuto.bccam");
}
