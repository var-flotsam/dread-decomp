#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkInputComponent;
struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};
f32 fn_7100076020(f32, const char*);
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
bool fn_710087238c(UnkInputComponent*);
bool fn_7100eef674(void*);
void fn_7100ee1f34(void*);
bool fn_7101053a0c(void*);
bool fn_7101053a98(void*);
bool fn_7101053a7c(void*);
bool fn_7101053a1c(void*, f32);
base::reflection::UnkReflObject* fn_7100e0398c(UnkGameManager*, void*);
bool fn_7100e039e4(UnkGameManager*, void*);
void fn_7100e038b0(UnkGameManager*, void*, UnkVec*, u64);
void fn_7100e03a78(UnkGameManager*, void*);
extern void* g_unk_7101d47360;
extern char g_unk_717271e920[] __attribute__((visibility("hidden")));
extern char g_unk_717271e930[] __attribute__((visibility("hidden")));

struct UnkCurve_ovb0259 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    union {
        u32 use_count;
    };
};

struct UnkCountDownTunables_ovb0259 {
    u8 pad_0x0[0x38];
    f32 total;  // 0x38
    f32 field_0x3c;
    f32 field_0x40;
    u8 pad_0x44[4];
    UnkCurve_ovb0259* curve_0x48;
};
extern UnkCountDownTunables_ovb0259 g_unk_717271e848;

struct UnkPlayerView_ovb0259 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* field_0x1d8;
};

struct UnkCountDown_ovb0259 {
    u8 pad_0x0[0xc0];
    f32 elapsed;  // 0xc0
};

struct UnkShake_ovb0259 {
    u8 pad_0x0[0x60];
    f32 field_0x60;
};

static inline void* Block() { return reinterpret_cast<u8*>(g_unk_7101d47310) + 0x1040; }

static inline void* Manager() {
    void* instance = g_unk_7101d47360;
    if (!instance) {
        instance = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(instance);
        g_unk_7101d47360 = instance;
    }
    return instance;
}

void fn_71007db2b0(struct UnkCountDown_ovb0259* self);
void fn_71007dacbc(UnkCountDown_ovb0259* self) {
    UnkCountDownTunables_ovb0259* t = &g_unk_717271e848;
    f32 ratio = self->elapsed / t->total;
    ++t->curve_0x48->use_count;
    const char* text = t->curve_0x48->text;
    f32 capped = !(ratio >= 1.0f) ? ratio : 1.0f;
    f32 clamped = !(ratio <= 0.0f) ? capped : 0.0f;
    f32 v = fn_7100076020(clamped, text ? text : g_unk_710162e2ed);
    f32 hi = t->field_0x40;
    f32 value = hi;
    if (!(v >= 1.0f)) {
        f32 lo = t->field_0x3c;
        if (v < 0.0f)
            value = lo;
        else {
            value = (1.0f - v) * lo;
            value += v * hi;
        }
    }
    if (self->elapsed > t->total) {
        UnkPlayerView_ovb0259* player = reinterpret_cast<UnkPlayerView_ovb0259*>(fn_7100dec788(g_unk_7101d47310));
        if (player && player->field_0x1d8 && fn_710087238c(player->field_0x1d8))
            goto done;
        if (fn_7100eef674(Manager()))
            goto done;
        if (fn_7101053a0c(Block()))
            goto done;
        if (fn_7101053a98(Block()))
            goto done;
        if (fn_7101053a7c(Block()))
            goto done;
        if (!fn_7101053a1c(Block(), 0.0f))
            goto next;
    }
done:
    if (base::reflection::UnkReflObject* fx = fn_7100e0398c(g_unk_7101d47310, g_unk_717271e920)) {
        if (fn_710009d50c(fx->vf_0x0(), fn_71010f21a4())) {
            if (!fn_7100e039e4(g_unk_7101d47310, g_unk_717271e920)) {
                UnkVec zero = {0, 0, 0};
                fn_7100e038b0(g_unk_7101d47310, g_unk_717271e920, &zero, 0);
            }
            reinterpret_cast<UnkShake_ovb0259*>(fx)->field_0x60 = value;
        }
    }
    return;
next:
    if (!fn_7100e039e4(g_unk_7101d47310, g_unk_717271e930)) {
        UnkVec zero = {0, 0, 0};
        fn_7100e038b0(g_unk_7101d47310, g_unk_717271e930, &zero, 0);
    }
    fn_7100e03a78(g_unk_7101d47310, g_unk_717271e920);
    fn_71007db2b0(self);
}
