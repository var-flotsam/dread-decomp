// base::global::timeline::CEvent's unit (tools/tu_map.py, unit 41): the localized text's constructor from a C string
// (src/base/global/UnkWideTextMethods.cpp has the one from a CRntString: length + 1 UTF-32 characters from the
// default allocator, flags 0x11, started empty, the narrow text converted by fn_710006c000 and terminated), and the
// engine thread wrapper's start (src/base/UnkEngineThread.cpp): from state 0 it records the entry (+0x1d8), the core
// flag (+0x1e0) and the user word (+0x1f0), initializes both light events, creates the thread on a 64 KiB stack
// (priority 27 when the priority argument is negative, else 2) on core 1 or 0, starts it and moves to state 1. The
// name argument is not used. Returns whether it started (the callers declare it void). Names and layouts are
// placeholders.
#include <stddef.h>
#include <atomic>

#include <nn/os.h>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

extern "C" size_t strlen(const char* s);
extern "C" size_t wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

extern char g_unk_7101a1a0d0[];
// The shared empty text (L"").
extern const wchar_t g_unk_7101647684[] __attribute__((visibility("hidden")));

// Converts a narrow string into at most `capacity` UTF-32 characters.
void fn_710006c000(const base::global::CRntString* src, wchar_t* dst, u32 capacity);

struct UnkWideTextStr {
    void* vtable;
    wchar_t* chars;
    u32 count;
    base::UnkAllocator* allocator;

    void Allocate(u64 length) {
        base::UnkAllocator* a = fn_7100083990();
        chars = static_cast<wchar_t*>(a->vf_0x40((u64)(u32)(length + 1) * 4, 8, 0x11));
        wcscpy(chars, g_unk_7101647684);
        count = wcslen(chars);
    }
};
static_assert(sizeof(UnkWideTextStr) == 0x20, "UnkWideTextStr layout");

void fn_7100066b38(UnkWideTextStr* self, const char* str) {
    self->count = 0;
    self->chars = nullptr;
    self->allocator = nullptr;
    self->vtable = g_unk_7101a1a0d0 + 0x10;
    u64 full = strlen(str);
    u32 length = full;
    if (length != 0) {
        self->Allocate(full);
        {
            base::global::CRntString narrow(str);
            fn_710006c000(&narrow, self->chars, length + 1);
        }
        self->chars[length] = 0;
        self->count = length;
    }
}

struct UnkEngineThreadStart {
    nn::os::ThreadType thread;           // +0x0
    nn::os::LightEventType startEvent;   // +0x1c0
    nn::os::LightEventType doneEvent;    // +0x1cc
    void (*entry)(void*);                // +0x1d8
    u32 core;                            // +0x1e0
    std::atomic<s32> state;              // +0x1e4
    void* stack;                         // +0x1e8
    void* user;                          // +0x1f0
};
static_assert(__builtin_offsetof(UnkEngineThreadStart, user) == 0x1f0, "layout");

void fn_71000666b0(void* arg);

bool fn_7100066744(void* thread, const char* name, void (*entry)(void*), u32 core, u32 priority, void* user) {
    UnkEngineThreadStart* self = static_cast<UnkEngineThreadStart*>(thread);
    if (self->state != 0)
        return false;
    self->entry = entry;
    self->core = core;
    self->user = user;
    nn::os::InitializeLightEvent(&self->startEvent, false, nn::os::EventClearMode_AutoClear);
    nn::os::InitializeLightEvent(&self->doneEvent, false, nn::os::EventClearMode_AutoClear);
    s32 prio = (s32)priority < 0 ? 27 : 2;
    self->stack = fn_7100083990()->vf_0x40(0x10000, 0x1000, 3);
    nn::os::CreateThread(&self->thread, fn_71000666b0, self, self->stack, 0x10000, prio);
    s32 c = self->core != 0;
    nn::os::SetThreadCoreMask(&self->thread, c, 1ull << c);
    nn::os::StartThread(&self->thread);
    self->state = 1;
    return true;
}
