// The CShotManager unit (tools/tu_map.py: 0x7100ecfb5c-0x7100edf654, whose reflected classes are
// CShotManager::CTunableShotManager and its siblings): the shot manager's per-frame update. The
// manager keeps its shots (0x290 bytes each) and effects (0xfc bytes each) in vectors, draws up to
// 125 shots of each type and variant through render instances it creates on demand, and hides last
// frame's instances first. The names and layouts are placeholders.
// Holds fn_7100ed6c08 (the update).
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkShot {
    u8 pad_0x0[0x8];
    u32 field_0x8;  // the type (0-4)
    u32 field_0xc;  // the variant (0-1)
    u8 pad_0x10[0xa8];
    f32 field_0xb8;  // the direction
    f32 field_0xbc;
    u8 pad_0xc0[0x10];
    UnkVec3f field_0xd0;  // the position
    u8 pad_0xdc[0x6e];
    bool field_0x14a;  // live
    u8 pad_0x14b[0x25];
    f32 field_0x170;
    u8 pad_0x174[0x4];
    f32 field_0x178;  // the roll the model is drawn with
    u8 pad_0x17c[0x114];
};
static_assert(sizeof(UnkShot) == 0x290, "UnkShot placeholder layout");

struct UnkShotEffect {
    u8 pad_0x0[0x18];
    bool field_0x18;  // live
    u8 pad_0x19[0xf];
    u32 field_0x28;
    u32 field_0x2c;
    u8 pad_0x30[0xcc];
};
static_assert(sizeof(UnkShotEffect) == 0xfc, "UnkShotEffect placeholder layout");

// A weak reference: alive while the target's generation is this one's.
struct UnkShotWeakTarget {
    void* field_0x0;
    s64 generation;
};
struct UnkShotWeakRef {
    UnkShotWeakTarget* target;
    s64 generation;
};

struct UnkShotModel {
    u8 data[0x8];
};

struct UnkShotManager {
    u8 pad_0x0[0x8];
    base::UnkVector<UnkShot> field_0x8;
    base::UnkVector<UnkShotEffect> field_0x30;
    base::UnkVector<UnkShot*> field_0x58;  // the live shots first, then null
    u32 field_0x80;                        // how many shots are live
    u8 pad_0x84[0x4];
    UnkShotModel field_0x88[5][2];
    u32 field_0xd8[5][2];  // instances drawn this frame per type and variant
    u64 field_0x100[5][2];
    u64 field_0x150[5][2];
    u64* field_0x1a0[5][2][2];  // the render instances per type and variant, and of their second part
    void* field_0x240[5][5];
    u8 pad_0x308[0xf00];
    f32 field_0x1208;
    u8 pad_0x120c[0x4];
    base::UnkVector<UnkShotWeakRef> field_0x1210;
    u8 pad_0x1238[0x38];
    bool field_0x1270;  // reset everything on the next update
};
static_assert(__builtin_offsetof(UnkShotManager, field_0x240) == 0x240, "UnkShotManager placeholder layout");
static_assert(__builtin_offsetof(UnkShotManager, field_0x1210) == 0x1210, "UnkShotManager placeholder layout");
static_assert(__builtin_offsetof(UnkShotManager, field_0x1270) == 0x1270, "UnkShotManager placeholder layout");

// The render instances shown last frame, hidden at the start of the next.
extern u32 g_unk_71729d1544 __attribute__((visibility("hidden")));
extern u64 g_unk_71729d1548[] __attribute__((visibility("hidden")));

extern "C" f32 atan2f(f32 y, f32 x);
UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);
void fn_7100219dd0(UnkRenderManager* self, u64 model, const UnkMatrix44* m, u32 a, u32 b, u64* instance, u32 c);
void fn_7100219e58(UnkRenderManager* self, u64 instance, bool visible);
void fn_7100219e64(UnkRenderManager* self, u64 object, const UnkMatrix44* m);
UnkShotWeakRef* fn_7100748064(base::UnkVector<UnkShotWeakRef>* self, u32 i);
void fn_7100ddecac(UnkShotEffect* self, f32 dt);
void fn_7100de13ac(void* self, UnkShotEffect* effect);
void fn_7100ecca00(UnkShot* self, f32 dt);
void fn_7100ecfa10(UnkShotModel* self, const UnkVec3f* pos, UnkVec3f* angle, UnkVec3f* scale, f32 t, bool second);
bool fn_7100ecfa84(UnkShotModel* self);
UnkShot** fn_7100ed4b68(base::UnkVector<UnkShot*>* self, u32 i);
void fn_7100ed7144(UnkShotManager* self);
void fn_7100ed7270(UnkShotManager* self, f32 dt);

// Shows the render instance at `(*instances)[n]` at `m` (creating it from `model` the first time)
// and remembers it for hiding on the next update.
static inline void ShowInstance(u64& instance, u64** instances, u32 n, const u64* model, const UnkMatrix44* m) {
    instance = (*instances)[n];
    if (instance) {
        fn_7100219e64(g_unk_7101d47300, instance, m);
        fn_7100219e58(g_unk_7101d47300, instance, true);
    } else {
        fn_7100219dd0(g_unk_7101d47300, *model, m, 1, 1, &instance, 0);
        (*instances)[n] = instance;
    }
    g_unk_71729d1548[g_unk_71729d1544++] = instance;
}

static inline void ClearDrawCounts(UnkShotManager* self) {
    for (u32 i = 0; i < 5; ++i) {
        self->field_0xd8[i][0] = 0;
        self->field_0xd8[i][1] = 0;
    }
}

// Removes element i, moving the later ones down.
static inline void EraseAt(base::UnkVector<UnkShotWeakRef>* v, u32 i) {
    if (i < v->count) {
        for (u32 j = i; j < v->count - 1; ++j)
            v->data[j] = v->data[j + 1];
        --v->count;
    }
}

// The update: hides last frame's instances, updates the shots (live ones to the front of
// field_0x58) and draws them, updates the effects, drops dead weak references, and on a pending
// reset kills every shot and runs itself again with no time.
// NON_MATCHING(minor): register allocation and scheduling only: the load of field_0x58's count is
// placed after the empty-shots branch instead of before it, two spills are stored in the other
// order, and the reset loop swaps the registers of its pointer and byte count (and the order of
// their two independent updates).
void fn_7100ed6c08(UnkShotManager* self, f32 dt) {
    if (!g_unk_7101d47310->field_0x608)
        return;
    for (u64 i = 0; i < g_unk_71729d1544; ++i) {
        u64 instance = g_unk_71729d1548[i];
        fn_7100219e58(g_unk_7101d47300, instance, false);
    }
    g_unk_71729d1544 = 0;

    UnkShot* shot = self->field_0x8.data;
    UnkShot* end = shot + self->field_0x8.count;
    self->field_0x80 = 0;
    u32 back = self->field_0x58.count - 1;
    u32 front = 0;
    for (; shot != end; ++shot) {
        if (shot->field_0x14a) {
            fn_7100ecca00(shot, dt);
            if (shot->field_0x14a) {
                u32 variant = shot->field_0xc;
                u32 type = shot->field_0x8;
                u32* drawn = &self->field_0xd8[type][variant];
                u32 n = *drawn;
                if (n < 125) {
                    const UnkVec3f& pos = shot->field_0xd0;
                    UnkVec3f angle(shot->field_0x178, 0.0f, atan2f(shot->field_0xbc, shot->field_0xb8));
                    UnkVec3f scale(1.0f, 1.0f, 1.0f);
                    UnkShotModel* model = &self->field_0x88[type][variant];
                    fn_7100ecfa10(model, &pos, &angle, &scale, shot->field_0x170, false);
                    UnkMatrix44 m = fn_71000791a8(pos, angle, scale);
                    if (self->field_0x1a0[type][variant][0]) {
                        u64 instance;
                        ShowInstance(instance, &self->field_0x1a0[type][variant][0], n, &self->field_0x100[type][variant],
                                     &m);
                        if (fn_7100ecfa84(model)) {
                            fn_7100ecfa10(model, &pos, &angle, &scale, shot->field_0x170, true);
                            UnkCopyMatrix(&m, fn_71000791a8(pos, angle, scale));
                            ShowInstance(instance, &self->field_0x1a0[type][variant][1], n,
                                         &self->field_0x150[type][variant], &m);
                        }
                        *drawn = n + 1;
                    }
                }
                *fn_7100ed4b68(&self->field_0x58, front++) = shot;
                self->field_0x80++;
                continue;
            }
        }
        *fn_7100ed4b68(&self->field_0x58, back--) = nullptr;
    }

    ClearDrawCounts(self);
    for (UnkShotEffect *e = self->field_0x30.data, *end = e + self->field_0x30.count; e != end; ++e) {
        if (e->field_0x18) {
            fn_7100de13ac(self->field_0x240[e->field_0x28][e->field_0x2c], e);
            fn_7100ddecac(e, dt);
        }
    }
    fn_7100ed7270(self, dt);

    if (self->field_0x1208 >= 1e-5f)
        self->field_0x1208 -= dt;

    for (s32 i = (s32)self->field_0x1210.count - 1; i >= 0; --i) {
        UnkShotWeakRef* ref = fn_7100748064(&self->field_0x1210, i);
        UnkShotWeakTarget* target = ref->target;
        if (target && target->generation == ref->generation && ref->generation != -1 && target->field_0x0)
            continue;
        EraseAt(&self->field_0x1210, i);
    }

    if (self->field_0x1270) {
        self->field_0x1270 = false;
        for (UnkShot *shot = self->field_0x8.data, *end = shot + self->field_0x8.count; shot != end; ++shot)
            shot->field_0x14a = false;
        ClearDrawCounts(self);
        fn_7100ed6c08(self, 0.0f);
        fn_7100ed7144(self);
    }
}
