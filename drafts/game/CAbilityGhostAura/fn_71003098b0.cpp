// CAbilityGhostAura's unit: the per-frame update of the aura. It resyncs the state, arms the aura when the
// owner stands near the ground of its sub-area, follows the owner's position with a segment query against the
// world to track the dash trail it started on, then runs the idle step and counts the retry delay down.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

struct UnkGhostAura_l3098b0;
struct UnkAuraTree;
struct UnkAuraRefCounted;

struct UnkAuraSource_l3098b0 {
    u8 pad_0x0[0x9ec];
    f32 x;  // 0x9ec
    f32 y;
    f32 z;
};

struct UnkAuraActor_l3098b0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x170];
    UnkAuraSource_l3098b0* source;  // 0x1d8
    u8 pad_0x1e0[0x40];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x78];
    void* field_0x2a0;
};

struct UnkAuraLink_l3098b0 {
    u8 pad_0x0[0x30];
    UnkAuraActor_l3098b0* actor;  // 0x30
};

struct UnkAuraHandle_l3098b0 {
    u8 pad_0x0[8];
    s32 count;  // 0x8
    void* target;  // 0x10
};

struct UnkAuraManager_l3098b0 {
    u8 pad_0x0[0x608];
    struct Inner {
        u8 pad_0x0[0x820];
        UnkAuraTree* tree;  // 0x820
    }* field_0x608;
};

struct UnkAuraSubArea_l3098b0 {
    u8 pad_0x0[0x110];
    s32 field_0x110;
};

struct UnkGhostAura_l3098b0 {
    u8 pad_0x0[0xc0];
    UnkAuraLink_l3098b0* link;  // 0xc0
    u8 pad_0xc8[0x334];
    f32 last_x;  // 0x3fc
    f32 last_y;
    f32 last_z;
    u8 pad_0x408[0x24];
    f32 source_x;  // 0x42c
    f32 source_y;
    f32 source_z;
    u8 pad_0x438[8];
    UnkAuraHandle_l3098b0* handle;  // 0x440
    s32 state;  // 0x448
    s32 field_0x44c;
    u8 pad_0x450[0x28];
    f32 field_0x478;
    u8 pad_0x47c[8];
    f32 retry_timer;  // 0x484
    u8 pad_0x488[0x12];
    bool field_0x49a;
    u8 pad_0x49b[6];
    bool tracking;  // 0x4a1
};

void fn_71002f9254(UnkGhostAura_l3098b0* self);
void fn_71002f9460(UnkGhostAura_l3098b0* self, bool flag);
bool fn_7100309204(UnkGhostAura_l3098b0* self);
void fn_710030a8fc(UnkGhostAura_l3098b0* self);
bool fn_710117ad58(UnkAnimationComponent* self);
void fn_7100309fd8(UnkGhostAura_l3098b0* self, u32 a, u32 b);
bool fn_710030b8b4(UnkGhostAura_l3098b0* self, void* a2, void* zero);
void fn_710030b448(UnkGhostAura_l3098b0* self, u32 id, bool on);

static inline UnkAuraSubArea_l3098b0* SubAreaManager() {
    UnkAuraSubArea_l3098b0* manager = reinterpret_cast<UnkAuraSubArea_l3098b0*>(g_unk_7101d47360);
    if (!manager) {
        manager = static_cast<UnkAuraSubArea_l3098b0*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = reinterpret_cast<UnkManager*>(manager);
    }
    return manager;
}

static inline void Release(UnkAuraHandle_l3098b0* handle) {
    if (handle && --handle->count == 0)
        reinterpret_cast<void (*)(UnkAuraHandle_l3098b0*)>((*reinterpret_cast<void***>(handle))[2])(handle);
}

void fn_71003098b0(UnkGhostAura_l3098b0* self, f32 dt) {
    fn_71002f9254(self);
    if (self->state != self->field_0x44c)
        fn_7100309fd8(reinterpret_cast<UnkGhostAura_A2_b70*>(self), self->field_0x44c, 1);
    if (UnkAuraLink_l3098b0* link = self->link) {
        if (UnkAuraActor_l3098b0* actor = link->actor) {
            if (!self->field_0x49a) {
                UnkAuraSubArea_l3098b0* area = SubAreaManager();
                if (area->field_0x110 == 0 && actor->source) {
                    f32 z = actor->source->x;
                    if (z + 0.1f >= 0.0f && z - 0.1f <= 0.0f) {
                        self->field_0x49a = true;
                        fn_71002f9460(self, false);
                        if (self->state == 1)
                            self->field_0x44c = 0;
                        self->retry_timer = 0.0f;
                    }
                }
            }
            f32 x = actor->x;
            f32 y = actor->y;
            f32 z = actor->z;
            bool trail = false;
            if ((self->state | 2) != 2 && self->link && self->link->actor && self->link->actor->animation)
                trail = fn_710117b348(self->link->actor->animation, fn_7100cfde48());
            if (trail || self->tracking) {
                f32 dx = x - self->last_x;
                f32 dy = y - self->last_y;
                f32 dz = z - self->last_z;
                f32 length = sqrtf(dx * dx + dy * dy + dz * dz);
                if (self->tracking) {
                    UnkAuraHandle_l3098b0* handle = self->handle;
                    if (!handle || !handle->target) {
                        self->tracking = false;
                    } else {
                        void* shape = handle->target;
                        (void)shape;
                    }
                } else {
                    (void)length;
                }
            }
            self->last_x = x;
            self->last_y = y;
            self->last_z = z;
        }
    }
    if (fn_7100309204(self)) {
        if (UnkAuraLink_l3098b0* link = self->link) {
            if (UnkAuraActor_l3098b0* actor = link->actor) {
                if (UnkAuraSource_l3098b0* source = actor->source) {
                    self->source_x = source->x;
                    self->source_y = source->y;
                    self->source_z = source->z;
                }
            }
        }
        self->field_0x478 = 0.0f;
        fn_710030a8fc(self);
    }
    if (self->state == 0) {
        if (UnkAuraLink_l3098b0* link = self->link) {
            if (UnkAuraActor_l3098b0* actor = link->actor) {
                if (UnkAnimationComponent* animation = actor->animation) {
                    if (fn_710117b348(animation, fn_7100cfd938()) && fn_710117ad58(animation)) {
                        struct {
                            f32 x, y, z;
                        } at = {actor->x, actor->y, actor->z};
                        bool ok = fn_710030b8b4(reinterpret_cast<UnkSelf*>(self), &at, nullptr);
                        fn_710030b448(self, 6, ok);
                    }
                }
            }
        }
    }
    if (self->retry_timer > 0.0f)
        self->retry_timer = self->retry_timer - dt;
}
