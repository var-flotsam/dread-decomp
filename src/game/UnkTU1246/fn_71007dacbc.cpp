#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "types.h"
#include "game/UnkTU53.h"
#include "game/UnkTU2553.h"
#include "game/CInputComponent.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2656.h"

struct UnkInputComponent;
struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    // Zero, each member set in turn (an aggregate's zeroing is stored in another order).
    UnkVec() {
        field_0x0 = 0;
        field_0x4 = 0;
        field_0x8 = 0;
    }
};
struct UnkPlayer;
struct UnkScreenFadeState_b48a3;
bool fn_7101053a1c(UnkScreenFadeState_b48a3* self, f32 t);
base::reflection::UnkReflObject* fn_7100e0398c(UnkGameManager*, void*);
struct UnkMapOwner;
struct UnkKeyObject;
bool fn_7100e039e4(UnkMapOwner* self, UnkKeyObject* const* key);
extern char g_unk_717271e920[] __attribute__((visibility("hidden")));
extern char g_unk_717271e930[] __attribute__((visibility("hidden")));

// Whether the game manager's map holds the key object at `key`.
static inline bool HasKey_l7dacbc(char* key) {
    return fn_7100e039e4(reinterpret_cast<UnkMapOwner*>(g_unk_7101d47310), reinterpret_cast<UnkKeyObject* const*>(key));
}

struct UnkCountDownTunables_ovb0259 {
    u8 pad_0x0[0x38];
    f32 total;  // 0x38
    f32 field_0x3c;
    f32 field_0x40;
    u8 pad_0x44[4];
    base::global::CStrId curve_0x48;
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

static inline f32 LerpClamped(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

static inline void* Block() { return reinterpret_cast<u8*>(g_unk_7101d47310) + 0x1040; }

static inline void* Manager() {
    UnkManager* instance = g_unk_7101d47360;
    if (!instance) {
        instance = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(instance);
        g_unk_7101d47360 = instance;
    }
    return instance;
}

void fn_71007db2b0(struct UnkCountDown_ovb0259* self);
void fn_71007dacbc(UnkCountDown_ovb0259* self) {
    UnkCountDownTunables_ovb0259* t = &g_unk_717271e848;
    f32 ratio = self->elapsed / t->total;
    f32 clamped = ratio <= 0.0f ? 0.0f : (ratio >= 1.0f ? 1.0f : ratio);
    f32 v = fn_7100076020(clamped, t->curve_0x48.Text());
    f32 value = LerpClamped(t->field_0x3c, t->field_0x40, v);
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
        if (!fn_7101053a1c(static_cast<UnkScreenFadeState_b48a3*>(Block()), 0.0f))
            goto next;
    }
done:
    if (base::reflection::UnkReflObject* fx = fn_7100e0398c(g_unk_7101d47310, g_unk_717271e920)) {
        if (fn_710009d50c(fx->vf_0x0(), fn_71010f21a4())) {
            if (!HasKey_l7dacbc(g_unk_717271e920)) {
                UnkVec zero;
                fn_7100e038b0(g_unk_7101d47310, g_unk_717271e920, &zero, 0);
            }
            reinterpret_cast<UnkShake_ovb0259*>(fx)->field_0x60 = value;
        }
    }
    return;
next:
    if (!HasKey_l7dacbc(g_unk_717271e930)) {
        UnkVec zero;
        fn_7100e038b0(g_unk_7101d47310, g_unk_717271e930, &zero, 0);
    }
    fn_7100e03a78(g_unk_7101d47310, g_unk_717271e920);
    fn_71007db2b0(self);
}
