// Unit 2596 (tools/tu_map.py): the rewards screen. Its text getters (the time and the counters as text
// from the timer singleton g_unk_71726bc558), the "REWARDS" mode object's constructor and its steps (the
// per-frame update, the start, the wait for the music/effects, the timed advance). Names and layouts
// are placeholders.
#include <stdio.h>
#include <string.h>

#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CBossAIComponent.h"
#include "game/CInputComponent.h"
#include "game/CPhotoModeCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2571.h"
#include "game/UnkTU2578.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2725.h"
#include "game/UnkTU2732.h"
#include "game/UnkTU57.h"
#include "game/malloc.h"
#include "game/sound/CMusicManager.h"
#include "game/UnkXformNode.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
#include "base/UnkLoadingMask.h"

using base::global::CRntString;
using base::global::CStrId;

f32 fn_7100ec73b4(u8* self);
u32 fn_7100f14e80();
bool fn_7100f14f80(s32 kind, s32 variant);

// The music manager (as src/base/input/UnkPadVibration.cpp declares it).
struct UnkStruct_0x128;
bool fn_7100f0220c(UnkStruct_0x128*, u32);
void fn_7100132af0(bool);

static inline UnkStruct_0x128* MusicCold() {
    UnkStruct_0x128* m = g_unk_7101d47320;
    if (__builtin_expect(!m, 0)) {
        m = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(m);
        g_unk_7101d47320 = m;
    }
    return m;
}

static inline UnkStruct_0x128* Music() {
    UnkStruct_0x128* m = g_unk_7101d47320;
    if (!m) {
        m = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(m);
        g_unk_7101d47320 = m;
    }
    return m;
}

static inline u8* Timer() {
    u8* t = g_unk_71726bc558;
    if (!t) {
        t = static_cast<u8*>(fn_71000839a0(0x10, 0x3f));
        fn_7100ec65fc(t);
        g_unk_71726bc558 = t;
    }
    return t;
}

struct UnkWideStr_0x20;
u32 fn_7100ec73d4(u8* self);
u32 fn_7100ec74c8(u8* self);
u32 fn_7100ec7768(u8* self);
u32 fn_7100ec6c3c(u8* self, s32 index);
u32 fn_7100ec78a0(u8* self);

struct UnkRewardText_b0242 {
    u8 raw[0x20];
    explicit UnkRewardText_b0242(const char* str) { fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(this), str); }
};

struct UnkRewardsMode_b0242 {
    u8 pad_0x0[0x50];
    s32 field_0x50;
    s32 field_0x54;
    s32 field_0x58;
    u32 state;  // 0x5c
    u32 field_0x60;
    u8 pad_0x64[4];
    const char* field_0x68[3];
    UnkGuiDisplayObject* field_0x80[4];
    u8 pad_0xa0[0x998];
    u32 count;  // 0xa38
    u8 pad_0xa3c[4];
    u32 field_0xa40;
    f32 timer;
    s32 field_0xa48;
    s32 field_0xa4c;
};
static_assert(__builtin_offsetof(UnkRewardsMode_b0242, count) == 0xa38, "layout");
static_assert(__builtin_offsetof(UnkRewardsMode_b0242, field_0xa4c) == 0xa4c, "layout");

void fn_7100f167c0(UnkRewardsMode_b0242* self, f32 dt);
void fn_7100f16a30(UnkRewardsMode_b0242* self, f32 dt);
void fn_7100f16e10(UnkRewardsMode_b0242* self);
void fn_7100f17064(UnkRewardsMode_b0242* self);
void fn_7100f165f4(UnkRewardsMode_b0242* self);
void fn_7100f17218(UnkRewardsMode_b0242* self);

void fn_7100f164ec(UnkRewardsMode_b0242* self, f32 dt) {
    u32 state = self->state;
    if (state & 1)
        return;
    if (state & 2) {
        fn_7100f165f4(self);
        return;
    }
    self->timer += dt;
    if (state & 4) {
        fn_7100f167c0(self, dt);
        state = self->state;
    }
    if (state & 8) {
        fn_7100f16a30(self, dt);
        state = self->state;
    }
    if (state & 0x20) {
        fn_7100f16e10(self);
        state = self->state;
    }
    if (state & 0x10) {
        fn_7100f17064(self);
        state = self->state;
    }
    if (state & 0x40) {
        if (self->timer >= 0.5f) {
            UnkGameManager* manager = g_unk_7101d47310;
            fn_7100deac9c(manager, CRntString("MAINMENU"));
            self->state = 1;
        }
    }
}

void fn_7100f165f4(UnkRewardsMode_b0242* self) {
    if (fn_7100f00a24(Music(), 0x4b)) {
        if (fn_7100f0220c(Music(), 0x4b)) {
            if (fn_7100f00a24(Music(), 0x4b)) {
                fn_7100f011c4(Music(), 0x4b);
                fn_7100f00714(Music());
                fn_7100132af0(false);
                fn_7100132aa0(false);
                fn_7100f003e8(Music(), 0x2710);
                fn_7100f01838(Music(), 0x4e);
            }
            self->state = 4;
            fn_7100f17218(self);
        }
    }
}

struct UnkListedItem;
struct UnkItemListTag;
using UnkItemList = base::UnkSList<UnkItemListTag>;
void fn_710119ba7c(UnkItemList* self, UnkListedItem* item);
extern const char* const g_unk_7101cab038[][3] __attribute__((visibility("hidden")));


void fn_7100f173d0(UnkRewardsMode_b0242* self);

void fn_7100f17218(UnkRewardsMode_b0242* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    void* scene = g_unk_7172729910;
    if (!scene) {
        scene = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(scene);
        g_unk_7172729910 = scene;
    }
    fn_710119ba7c(static_cast<UnkItemList*>(scene), reinterpret_cast<UnkListedItem*>(self->pad_0xa0));
    UnkVec3f a(0.0f, 0.0f, 0.0f);
    UnkVec3f b(0.0f, 0.0f, 0.0f);
    fn_7101053968(&manager->field_0x1040, &a, &b, 0.5f, 1.0f, 0.0f);
    self->field_0x50 = fn_7100f14e80();
    u32 secs = (u32)fn_7100ec73b4(Timer());
    self->field_0x54 = secs < 0x3841 ? 2 : (secs < 0x7081 ? 1 : 0);
    if (self->field_0xa48 != -1 && self->field_0xa4c != -1) {
        s32 kind = self->field_0xa48;
        s32 variant = self->field_0xa4c;
        self->field_0x50 = kind;
        self->field_0x54 = variant;
        self->field_0xa48 = -1;
        self->field_0xa4c = -1;
        extern const u32 g_unk_710164bf4c[][3] __attribute__((visibility("hidden")));
        self->field_0x58 = g_unk_710164bf4c[kind][variant];
    }
    fn_7100f173d0(self);
    if (self->field_0x54 >= 0) {
        const char** dst = self->field_0x68;
        for (s64 i = 0; i <= self->field_0x54; ++i) {
            if (fn_7100f14f80(self->field_0x50, i))
                *dst++ = g_unk_7101cab038[self->field_0x50][i];
        }
    }
}

template <class T>
struct UnkLoadingMaskOf {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag;

static inline void* MusicManagerCold() {
    void* manager = g_unk_71726bc560;
    if (__builtin_expect(!manager, 0)) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

struct UnkVec;
struct UnkRewardsPosition_b0242 {
    u64 xy;
    u32 z;
};

void fn_7100f17064(UnkRewardsMode_b0242* self) {
    if (!(self->timer >= 3.0f)) {
        self->count = 0;
        return;
    }
    u32 n = self->count;
    if (n == 0)
        return;
    s32 i = 0;
    do {
        if (*reinterpret_cast<s32*>(reinterpret_cast<u8*>(self) + (s64)i * 20 + 0xdc) == 1)
            goto found;
        ++i;
        // keeps the entry address computed from the index each pass, as the original does
        asm("" : "+r"(i));
    } while (n != (u32)i);
    return;
found:
    UnkGameManager* const* slot = &g_unk_7101d47310;
    self->state = 0x20;
    UnkRewardsPosition_b0242 position;
    fn_7101053850(&(*slot)->field_0x1040, reinterpret_cast<UnkVec*>(&(position = {0, 0})), 1, 1.0f, 1.0f);
    self->timer = 0.0f;
    if (self->field_0x60 <= 2 && self->field_0x68[self->field_0x60])
        return;
    void* music = MusicManager();
    fn_7100fd7b44(music, UnkLoadingMask(0xc), UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), 1.0f);
}

struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};
extern CRntString g_unk_71729dc3d8 __attribute__((visibility("hidden")));

void fn_7100f178b4(UnkRewardsMode_b0242* self) {
    fn_71011867ec(reinterpret_cast<char*>(self));
    self->field_0x50 = -1;
    self->field_0x54 = -1;
    self->field_0x58 = 0;
    __builtin_memset(self->field_0x68, 0, sizeof(self->field_0x68));
    self->field_0xa40 = 0;
    self->timer = 0.0f;
    self->state = 2;
    self->field_0x60 = 0;
    fn_7100f003e8(Music(), -10000);
    if (__builtin_expect(!fn_7100f00a24(MusicCold(), 0x4b), 0))
        fn_7100f01838(Music(), 0x4b);
    void* music = MusicManagerCold();
    UnkMusicMode mode;
    mode.kind = 0x4e;
    fn_7100fd90d8(0.0f, 0.0f, music, &g_unk_71729dc3d8, 7, &mode, 0, 0, nullptr);
    UnkGameManager* manager = g_unk_7101d47310;
    UnkVec3f a(0.0f, 0.0f, 0.0f);
    UnkVec3f b(0.0f, 0.0f, 0.0f);
    fn_7101053968(&manager->field_0x1040, &a, &b, 0.2f, 1.0f, 0.0f);
}

// A pooled string whose hash is at +0x30.
struct UnkKeyData_b0242 {
    u8 pad_0x0[0x30];
    u64 hash;
};
extern UnkKeyData_b0242* g_unk_71729dc388 __attribute__((visibility("hidden")));
extern UnkKeyData_b0242* g_unk_71729dc390 __attribute__((visibility("hidden")));
extern UnkKeyData_b0242* g_unk_71729dc398 __attribute__((visibility("hidden")));
extern UnkKeyData_b0242* g_unk_71729dc3a0 __attribute__((visibility("hidden")));
extern UnkKeyData_b0242* g_unk_71729dc3a8 __attribute__((visibility("hidden")));
extern UnkKeyData_b0242* g_unk_71729dc3b0 __attribute__((visibility("hidden")));
extern UnkKeyData_b0242* g_unk_71729dc3b8 __attribute__((visibility("hidden")));

UnkRewardText_b0242 fn_7100f159e0(const char* name) {
    u64 key = fn_7100001570(name, strlen(name));
    u32 value = 0;
    if (g_unk_71729dc388->hash == key)
        value = fn_7100ec73d4(Timer());
    if (g_unk_71729dc390->hash == key)
        value = fn_7100ec74c8(Timer());
    if (g_unk_71729dc398->hash == key)
        value = fn_7100ec7768(Timer());
    char buf[0x80];
    sprintf(buf, "%u", value);
    return UnkRewardText_b0242(buf);
}

UnkRewardText_b0242 fn_7100f15b10(const char* name) {
    char buf[0x80];
    if (Timer()[8]) {
        u64 key = fn_7100001570(name, strlen(name));
        u32 value = 0;
        if (g_unk_71729dc3a0->hash == key)
            value = fn_7100ec6c3c(Timer(), 0);
        if (g_unk_71729dc3b8->hash == key)
            value = fn_7100ec6c3c(Timer(), 2);
        if (g_unk_71729dc3a8->hash == key)
            value = fn_7100ec6c3c(Timer(), 1) / 1000;
        if (g_unk_71729dc3b0->hash == key)
            value = fn_7100ec78a0(Timer());
        sprintf(buf, "%u", value);
    } else {
        buf[0] = '-';
        buf[1] = 0;
    }
    return UnkRewardText_b0242(buf);
}

bool fn_7100f15054(CStrId* id);
extern const char* const g_unk_7101cab158[] __attribute__((visibility("hidden")));
extern const char* const g_unk_7101cab178[] __attribute__((visibility("hidden")));

struct UnkKeyText_b0242 {
    u8 raw[0x30];
    UnkKeyText_b0242() { fn_710007e6c4(raw); }
    ~UnkKeyText_b0242() { fn_710007ecd4(reinterpret_cast<CRntString*>(raw)); }
};

union UnkIdSlot_b0242 {
    CStrId id;
    UnkIdSlot_b0242() {}
    ~UnkIdSlot_b0242() { id.~CStrId(); }
};

bool fn_7100f14f80(s32 kind, s32 variant) {
    UnkKeyText_b0242 text;
    if ((u32)kind <= 3)
        fn_710007f564(text.raw, g_unk_7101cab158[kind]);
    if ((u32)variant <= 2)
        fn_710007f564(text.raw, g_unk_7101cab178[variant]);
    UnkIdSlot_b0242 slot;
    fn_7100000850(&slot.id, *reinterpret_cast<CRntString*>(text.raw), true);
    return fn_7100f15054(&slot.id);
}

struct UnkMusicTrigger;

extern void* g_unk_7101caaf80[];
extern void* g_unk_71729dc270[];
extern void* g_unk_71729dc2a8[];

// The constructor's view of the same object: its vtable and the name at +0x8.
struct UnkRewardsCtorView_b0242 {
    void** vtable;
    CRntString name;  // 0x8
    u8 pad_0x38[0x18];
    s32 field_0x50;
    s32 field_0x54;
    s32 field_0x58;
    u32 state;  // 0x5c
    u32 field_0x60;
    u8 pad_0x64[4];
    void* field_0x68[3];
    void* field_0x80[4];
    u8 pad_0xa0[0x9a0];
    u32 field_0xa40;
    f32 timer;
    s32 field_0xa48;
    s32 field_0xa4c;
};
static_assert(__builtin_offsetof(UnkRewardsCtorView_b0242, field_0xa40) == 0xa40, "layout");

void fn_7100f1641c(UnkRewardsMode_b0242* object) {
    UnkRewardsCtorView_b0242* self = reinterpret_cast<UnkRewardsCtorView_b0242*>(object);
    fn_7101186780(reinterpret_cast<UnkMusicTrigger*>(self));
    self->vtable = &g_unk_7101caaf80[2];
    self->field_0x50 = -1;
    self->field_0x54 = -1;
    self->field_0x58 = 0;
    self->state = 1;
    self->field_0x68[0] = nullptr;
    self->field_0x68[1] = nullptr;
    self->field_0x68[2] = nullptr;
    self->field_0x60 = 0;
    self->field_0x80[0] = nullptr;
    self->field_0x80[1] = nullptr;
    self->field_0x80[2] = nullptr;
    self->field_0x80[3] = nullptr;
    fn_71009ca9fc(self->pad_0xa0, "RewardsMode", 9);
    self->field_0xa40 = 0;
    self->timer = 0;
    self->field_0xa48 = -1;
    self->field_0xa4c = -1;
    fn_710007e77c(&self->name, "REWARDS");
    __builtin_memset(g_unk_71729dc270, 0, 0x38);
    __builtin_memset(g_unk_71729dc2a8, 0, 0xc8);
}
