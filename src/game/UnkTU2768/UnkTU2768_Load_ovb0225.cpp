// Unit 2768 (no reliable name; src/game/UnkUserDataOwner.cpp): the user data owner's other ways of starting a
// load, and the completion check they all end in. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2622.h"
#include "game/UnkTU2768.h"
#include "types.h"

// The loader at +0x8 of the owner: its state word, a completion callback and a ready flag.
struct UnkLoader_b0225 {
    u32 state;  // 0x0
    u8 unk_0x4[4];
    void (*done)(UnkLoader_b0225* self, s32 result);  // 0x8
    u8 unk_0x10[0x28];
    bool ready;  // 0x38
    u8 unk_0x39[7];
};
static_assert(sizeof(UnkLoader_b0225) == 0x40, "loader layout");

struct UnkLoadRunner_b0225 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual s32 vf_0x48(UnkLoader_b0225* loader);
};

struct UnkUserDataOwner {
    bool field_0x0;
    u8 unk_0x1[7];
    UnkLoader_b0225 loader;  // 0x8
    bool field_0x48;
    bool loading;  // 0x49
    u8 unk_0x4a[0x234 - 0x4a];
    u8 field_0x234[0x14];  // cleared by the constructor
    nn::os::LightEventType event;  // 0x248
    u8 unk_0x254[0xc];
    nn::os::MutexType stateMutex;  // 0x260
    nn::os::MutexType mutex;       // 0x280
    UnkLoadRunner_b0225* runner;   // 0x2a0
    bool field_0x2a8;
    u8 unk_0x2a9[7];
    void* field_0x2b0;
};
static_assert(sizeof(UnkUserDataOwner) == 0x2b8, "UnkUserDataOwner layout");

void fn_7100fc73f4(UnkLoader_b0225* loader, u64 arg);
void fn_7100fc7500(UnkLoader_b0225* loader, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6, u64 a7, u64 a8, u64 a9);
void fn_7100fc7b70(UnkLoader_b0225* loader);
void fn_7100066698(nn::os::LightEventType* event);

struct UnkMutexLock {
    nn::os::MutexType* mutex;
    explicit UnkMutexLock(nn::os::MutexType* m) : mutex(m) { fn_710006691c(m); }
    ~UnkMutexLock() { fn_7100066920(mutex); }
};

// Marks the owner as loading, then, unless `b`, an owner with +0x48 set only signals its event and
// returns 3; otherwise returns what fn_7101203620 does.
static inline s32 Finish(UnkUserDataOwner* self, bool b) {
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

s32 fn_710120330c(UnkUserDataOwner* self, u64 arg, bool b) {
    UnkMutexLock lock(&self->mutex);
    fn_7100fc73f4(&self->loader, arg);
    return Finish(self, b);
}

s32 fn_71012033b0(UnkUserDataOwner* self, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6, bool b, u64 a7, u64 a8,
                  u64 a9) {
    UnkMutexLock lock(&self->mutex);
    fn_7100fc7500(&self->loader, a1, a2, a3, a4, a5, a6, a7, a8, a9);
    return Finish(self, b);
}

__attribute__((always_inline)) static inline void Construct(UnkUserDataOwner* self) {
    self->field_0x0 = false;
    fn_7100fc71ac(&self->loader);
    self->field_0x48 = false;
    self->loading = false;
    __builtin_memset(self->field_0x234, 0, sizeof(self->field_0x234));
    self->field_0x2a8 = false;
    self->field_0x2b0 = nullptr;
}

// Runs the pending request of the global owner (created on first use) through the runner at +0x2a0 of this
// one, or just checks it when this one has the flag at +0x2a8; reports the result to the loader's callback and
// signals the event at +0x254. Returns the result (0 while the global loader isn't ready).
s32 fn_7101203620(UnkUserDataOwner* self) {
    UnkMutexLock lock(&self->mutex);
    UnkUserDataOwner* owner = g_unk_7101d47308;
    if (!owner) {
        owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        Construct(owner);
        g_unk_7101d47308 = owner;
    }
    s32 result = 0;
    if (owner->loader.ready) {
        UnkLoader_b0225* loader = &owner->loader;
        if (self->field_0x2a8) {
            u32 state = loader->state;
            switch (state) {
            case 5:
            case 2:
                result = 1;
                break;
            default:
                result = 0;
                break;
            }
        } else {
            result = self->runner->vf_0x48(loader);
        }
        fn_7100fc7b70(loader);
        if (loader->done)
            loader->done(loader, result);
        fn_7100066698(reinterpret_cast<nn::os::LightEventType*>(reinterpret_cast<u8*>(self) + 0x254));
    }
    {
        UnkMutexLock state(&self->stateMutex);
        self->loading = false;
    }
    return result;
}
