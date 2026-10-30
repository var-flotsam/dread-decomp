// Unit 2768 (tools/tu_map.py: 0x710120320c-0x710120391c, initialiser fn_7101203890): the user data's
// owner (0x2b8 bytes, created on first use by src/game_aca57c/fn_7100dc8828.cpp and
// src/game/CGameManager.cpp; its constructor is fn_710120320c), which starts scenario loads under
// its mutex.
// Holds fn_71012034b0 (start a load). Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

// The list of blackboards a load is given (src/game/CGameManager.cpp defines it).
struct UnkPtrList;

struct UnkUserDataOwner {
    bool field_0x0;
    u8 unk_0x1[7];
    u8 loader[0x40];  // 0x8
    bool field_0x48;
    bool loading;  // 0x49
    u8 unk_0x4a[0x234 - 0x4a];
    u8 field_0x234[0x14];  // cleared by the constructor
    nn::os::LightEventType event;  // 0x248
    u8 unk_0x254[0xc];
    nn::os::MutexType stateMutex;  // 0x260
    nn::os::MutexType mutex;       // 0x280
    u8 unk_0x2a0[0x8];
    bool field_0x2a8;
    u8 unk_0x2a9[7];
    void* field_0x2b0;
};
static_assert(sizeof(UnkUserDataOwner) == 0x2b8, "UnkUserDataOwner layout");

// The loader's constructor (src/game/UnkScenarioLoadRequest.cpp's unit).
void fn_7100fc71ac(void* loader);

// Clears the flags and the fields the owner tests before its first load; the event and the mutexes
// are set up elsewhere.
void fn_710120320c(UnkUserDataOwner* self) {
    self->field_0x0 = false;
    fn_7100fc71ac(self->loader);
    self->field_0x48 = false;
    self->loading = false;
    __builtin_memset(self->field_0x234, 0, sizeof(self->field_0x234));
    self->field_0x2a8 = false;
    self->field_0x2b0 = nullptr;
}

// Holds a mutex for its scope (through the engine's lock and unlock helpers).
struct UnkMutexLock {
    nn::os::MutexType* mutex;
    explicit UnkMutexLock(nn::os::MutexType* m) : mutex(m) { fn_710006691c(m); }
    ~UnkMutexLock() { fn_7100066920(mutex); }
};

// Signals a light event (a call of nn::os::SignalLightEvent).
void fn_7100066698(nn::os::LightEventType* event);

// The load itself, run on the loader at +0x8, with the three strings this builds.
void fn_7100fc7920(void* loader, u32 kind, const CRntString* scenario, const CRntString* name,
                   UnkPtrList* blackboards, bool a, void (*fn)(), const CRntString* c, CRntString* project,
                   CRntString* s2, CRntString* s3);  // src/game/UnkScenarioLoadRequest.cpp
s32 fn_7101203620(UnkUserDataOwner* self);

// Starts a load of `kind` (for kinds 1 and 4 the first string passed on is "Project") and marks the
// owner as loading. Then, unless `b`, an owner with +0x48 set only signals its event and returns 3;
// otherwise returns what fn_7101203620 does.
s32 fn_71012034b0(UnkUserDataOwner* self, u32 kind, const CRntString* scenario, const CRntString* name,
                  UnkPtrList* blackboards, bool a, void (*fn)(), bool b, const CRntString* c) {
    UnkMutexLock lock(&self->mutex);
    CRntString project("");
    CRntString s2("");
    CRntString s3("");
    if (kind == 4 || kind == 1)
        fn_710007e77c(&project, "Project");
    fn_7100fc7920(self->loader, kind, scenario, name, blackboards, a, fn, c, &project, &s2, &s3);
    {
        UnkMutexLock state(&self->stateMutex);
        self->loading = true;
    }
    s32 result;
    if (!b && self->field_0x48) {
        {
            UnkMutexLock again(&self->mutex);
            fn_7100066698(&self->event);
        }
        result = 3;
    } else {
        result = fn_7101203620(self);
    }
    return result;
}
