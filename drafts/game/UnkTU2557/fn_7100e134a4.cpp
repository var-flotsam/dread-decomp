// Unit 2557 (loading screen): advances the loading screen's pages by `dt`: fades the current page's
// title and text out as its time runs down and places the text under the title, and with more than one
// page lets the player turn to the next or previous one. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2747.h"
#include "game/nn/time.h"
#include "types.h"

struct UnkSoundGroup;
struct UnkSoundHolder {
    UnkSoundGroup* group;
};

template <class T, u64 Size, void (*Ctor)(T*), u32 Flags = 0x3f>
static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, Flags));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// The controller source the sound holder's first word points at.
struct UnkPadSource_le134a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Update(f32 dt);
    virtual void vf_0x28();
    virtual bool IsHeld(u32 button, u32 pad);
    virtual bool IsPressed(u32 button, u32 pad);
};

// The text view of a label (its line height and line count).
struct UnkLabelFontView_b0035;
f32 CLabel_GetLineHeight(UnkLabelFontView_b0035* self);
u32 fn_71011ef554(UnkLabelFontView_b0035* self);

// Where the page title is placed.
__attribute__((visibility("hidden"))) extern const base::math::CVector2D g_unk_7101aa9b78;

struct UnkLoadingPage_le134a4 {
    UnkGuiDisplayObject* title;  // 0x0
    UnkGuiDisplayObject* text;   // 0x8
    f32 duration;                // 0x10
    u8 pad_0x14[4];
};

struct UnkLoadingPages_le134a4 {
    s32 field_0x0;
    u8 pad_0x4[0x2c];
    UnkLoadingPage_le134a4 pages[16];  // 0x30
    u32 count;                         // 0x1b0
    u32 current;                       // 0x1b4
    f32 page_time;                     // 0x1b8
    f32 fade_time;                     // 0x1bc
    u8 pad_0x1c0[4];
    f32 fade_duration;                 // 0x1c4
    nn::os::MutexType mutex;           // 0x1c8
};
static_assert(__builtin_offsetof(UnkLoadingPages_le134a4, count) == 0x1b0, "count");
static_assert(__builtin_offsetof(UnkLoadingPages_le134a4, mutex) == 0x1c8, "mutex");

static const u32 kButtonNext = 0x100;
static const u32 kButtonPrevious = 0x200;
static const u32 kAnyPad = 0xfd;

// 1 until the last 0.2 seconds of the page, then fading linearly to 0.
static inline f32 PageAlpha(f32 time, f32 duration) {
    f32 alpha;
    if (time >= duration - 0.2f) {
        alpha = (duration - time) / 0.2f;
        if (alpha <= 0.0f)
            alpha = 0.0f;
        else
            alpha = alpha > 1.0f ? 1.0f : alpha;
    } else {
        alpha = 1.0f;
    }
    return alpha;
}

void fn_7100e134a4(UnkLoadingPages_le134a4* self, f32 dt) {
    fn_710006691c(&self->mutex);
    if (self->field_0x0 == 0) {
        f32 fade = self->fade_time - dt;
        u32 count = self->count;
        u32 current = self->current;
        self->fade_time = fade;
        if (fade < 0.0f && count > current) {
            UnkLoadingPage_le134a4& page = self->pages[current];
            f32 alpha = PageAlpha(self->page_time, page.duration);
            fn_71011cfba8(page.title, alpha);
            fn_71011cde64(page.title, &g_unk_7101aa9b78, 1, 1);
            fn_71011cfba8(page.text, alpha);
            f32 lineHeight = CLabel_GetLineHeight(reinterpret_cast<UnkLabelFontView_b0035*>(page.title));
            u32 lines = fn_71011ef554(reinterpret_cast<UnkLabelFontView_b0035*>(page.title));
            base::math::CVector2D position = {0.05f, lineHeight * lines + 0.0f + -0.523f};
            fn_71011cde64(page.text, &position, 0, 0);
            self->page_time -= dt;

            auto* pad = reinterpret_cast<UnkPadSource_le134a4*>(
                Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340)->group);
            pad->Update(dt);
            if (self->count >= 2) {
                bool next = pad->IsPressed(kButtonNext, kAnyPad);
                bool previous = pad->IsPressed(kButtonPrevious, kAnyPad);
                if (next || previous) {
                    fn_71011cdc30(page.title, false);
                    fn_71011cdc30(page.text, false);
                    u32 index = self->current;
                    if (next)
                        index = index + 1 == self->count ? 0 : index + 1;
                    else
                        index = (index == 0 ? self->count : index) - 1;
                    UnkLoadingPage_le134a4& shown = self->pages[index];
                    self->current = index;
                    fn_71011cdc30(shown.title, true);
                    fn_71011cfba8(shown.title, 0.0f);
                    self->fade_time = self->fade_duration;
                    fn_71011cdc30(shown.text, true);
                    fn_71011cfba8(shown.text, 0.0f);
                    self->page_time = shown.duration;
                }
            }
        }
    }
    fn_7100066920(&self->mutex);
}
