// GUI::CMainMenu's unit (tools/tu_map.py unit 1687): fn_7100b34488 is the menu's constructor: the display
// object base (fn_71011d8d24), then the members its destructor (fn_7100b34700) tears down, in order.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU57.h"
#include "types.h"

extern char g_unk_7101a9c0f0[];  // the menu's vtable
extern char g_unk_7101a9cec0[];
extern char g_unk_7101a9cfe0[];
extern char g_unk_7101a9d028[];
extern char g_unk_7101a9d148[];
extern char g_unk_7101a9d100[];
extern char g_unk_7101a9d1d8[];
extern char g_unk_7101a9cf98[];
extern char g_unk_7101a9ccd8[];
extern char g_unk_7101a9cd20[];
extern char g_unk_7101a9cf08[];  // the reference pair's vtable
extern char g_unk_7101a9d070[];
extern char g_unk_7101a9ce78[];
extern char g_unk_7101a9cf50[];
extern char g_unk_7101a9d0b8[];
extern char g_unk_7101a9cc90[];

void fn_7100b3686c();
void fn_7100b36874();
void fn_7100b52434(void* self);

// A one-word listener; each member has a vtable of its own.
struct UnkMainMenuListener_lb34488 {
    char* vtable;
    u64 field_0x8;

    void Init(char* vtable_) {
        vtable = vtable_ + 0x10;
        field_0x8 = 0;
    }
};

struct UnkMainMenuCallback_lb34488 {
    void* set;
    void* owner;
    void (*fn)();

    void Init(void* owner_, void (*fn_)()) {
        set = nullptr;
        owner = owner_;
        fn = fn_;
    }
};

struct UnkMainMenuRange_lb34488 {
    u64 field_0x0;
    s64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;

    void Init() {
        field_0x0 = 0;
        field_0x8 = -1;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

struct UnkMainMenuMember3e0_lb34488 {
    UnkMainMenuListener_lb34488 listener;
    u32 field_0x10;
    u32 field_0x14;
    u64 field_0x18[23];
    u32 field_0xd0;
    f32 field_0xd4;
    UnkMainMenuRange_lb34488 field_0xd8;

    void Init() {
        listener.Init(g_unk_7101a9ccd8);
        field_0x10 = 0;
        field_0x14 = 0xd;
        for (u64& value : field_0x18)
            value = 0;
        field_0xd0 = 0;
        field_0xd4 = -1.0f;
        field_0xd8.Init();
    }
};
static_assert(sizeof(UnkMainMenuMember3e0_lb34488) == 0xf8, "layout");

struct UnkMainMenuRefPair_lb34488 {
    UnkMainMenuListener_lb34488 listener;
    void* field_0x10;
    void* field_0x18;

    void Init() {
        listener.Init(g_unk_7101a9cf08);
        field_0x10 = nullptr;
        field_0x18 = nullptr;
    }
};

struct UnkMainMenu_lb34488 {
    char* vtable;
    u8 pad_0x8[0x228];
    UnkMainMenuCallback_lb34488 field_0x230;
    UnkMainMenuCallback_lb34488 field_0x248;
    bool field_0x260;
    void* field_0x268;
    void* field_0x270;
    u8 pad_0x278[0xc];
    f32 field_0x284;
    u8 field_0x288[0x30];  // a base::global::CRntString
    base::global::CRntVector<void*> field_0x2b8;
    UnkMainMenuListener_lb34488 field_0x2e0;
    u32 field_0x2f0;
    u8 field_0x2f8[0x48];  // built by fn_7100b52434
    UnkMainMenuListener_lb34488 field_0x340;
    UnkMainMenuListener_lb34488 field_0x350;
    u8 pad_0x360[0x10];
    UnkMainMenuRange_lb34488 field_0x370;
    u64 field_0x390;
    UnkMainMenuListener_lb34488 field_0x398;
    u8 pad_0x3a8[0x8];
    UnkMainMenuListener_lb34488 field_0x3b0;
    UnkMainMenuListener_lb34488 field_0x3c0;
    UnkMainMenuListener_lb34488 field_0x3d0;
    UnkMainMenuMember3e0_lb34488 field_0x3e0;
    UnkMainMenuListener_lb34488 field_0x4d8;
    UnkMainMenuRefPair_lb34488 field_0x4e8;
    UnkMainMenuListener_lb34488 field_0x508;
    UnkMainMenuListener_lb34488 field_0x518;
    UnkMainMenuListener_lb34488 field_0x528;
    UnkMainMenuListener_lb34488 field_0x538;
    UnkMainMenuListener_lb34488 field_0x548;
};
static_assert(__builtin_offsetof(UnkMainMenu_lb34488, field_0x2b8) == 0x2b8, "layout");
static_assert(__builtin_offsetof(UnkMainMenu_lb34488, field_0x3e0) == 0x3e0, "layout");
static_assert(__builtin_offsetof(UnkMainMenu_lb34488, field_0x548) == 0x548, "layout");

void fn_7100b34488(UnkMainMenu_lb34488* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101a9c0f0 + 0x10;
    self->field_0x230.Init(self, fn_7100b3686c);
    self->field_0x248.Init(self, fn_7100b36874);
    self->field_0x260 = false;
    self->field_0x268 = nullptr;
    self->field_0x270 = nullptr;
    self->field_0x284 = -1.0f;
    fn_710007e6c4(self->field_0x288);
    new (&self->field_0x2b8) base::global::CRntVector<void*>(0xf);
    self->field_0x2e0.Init(g_unk_7101a9cec0);
    self->field_0x2f0 = 0;
    fn_7100b52434(self->field_0x2f8);
    self->field_0x340.Init(g_unk_7101a9cfe0);
    self->field_0x350.Init(g_unk_7101a9d028);
    self->field_0x370.Init();
    self->field_0x390 = 0;
    self->field_0x398.Init(g_unk_7101a9d148);
    self->field_0x3b0.Init(g_unk_7101a9d100);
    self->field_0x3c0.Init(g_unk_7101a9d1d8);
    self->field_0x3d0.Init(g_unk_7101a9cf98);
    self->field_0x3e0.Init();
    self->field_0x4d8.Init(g_unk_7101a9cd20);
    self->field_0x4e8.Init();
    self->field_0x508.Init(g_unk_7101a9d070);
    self->field_0x518.Init(g_unk_7101a9ce78);
    self->field_0x528.Init(g_unk_7101a9cf50);
    self->field_0x538.Init(g_unk_7101a9d0b8);
    self->field_0x548.Init(g_unk_7101a9cc90);
}
