// Unit 818 (no reliable name): a file-path loader that creates two assets on first use, a watch test comparing a
// progress value with a threshold taken from the owner's class, and an aim update. Names and layouts are placeholders.
#include <new>
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPathLoad_b0258m {
    u8 pad_0x0[0x8];
    void* first;   // 0x8
    void* second;  // 0x10
    u64 field_0x18;
    u8 pad_0x20[0x48];
    CFilePathStrId path_a;  // 0x68
    CFilePathStrId path_b;  // 0x70
};
static_assert(__builtin_offsetof(UnkPathLoad_b0258m, path_b) == 0x70, "layout");

void fn_71001494f8(void* slot, u32 kind, u32 mode, CFilePathStrId* path, u32 flags);

void fn_710057345c(UnkPathLoad_b0258m* self) {
    alignas(8) u8 storage_a[sizeof(CFilePathStrId)];
    alignas(8) u8 storage_b[sizeof(CFilePathStrId)];
    if (!self->first) {
        CFilePathStrId* path = new (storage_a) CFilePathStrId(self->path_a);
        fn_71001494f8(&self->first, 0x2a, 3, path, 4);
        path->~CFilePathStrId();
    }
    if (__builtin_expect(self->second == nullptr, 0)) {
        base::global::UnkStringData* d = self->path_b.data;
        if (d != &g_unk_7101d47200->empty) {
            CFilePathStrId* path = reinterpret_cast<CFilePathStrId*>(storage_b);
            path->data = d;
            ++d->ref_count;
            fn_71001494f8(&self->second, 0x2a, 3, path, 4);
            path->~CFilePathStrId();
        }
    }
    self->field_0x18 = 0;
}

using base::global::CStrId;

bool fn_710054abf4(void* holder);
struct UnkStruct_710117af10;
struct UnkCls_710117af10;
bool fn_710117af10(UnkStruct_710117af10*, UnkCls_710117af10**, bool);

struct UnkWatchHolder_b0258w {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkWatch_b0258w {
    u8 pad_0x0[0x20];
    UnkWatchHolder_b0258w* holder;  // 0x20
    u8 pad_0x28[0x1c];
    f32 field_0x44;
    u8 pad_0x48[0x10];
    bool flag;  // 0x58
    u8 pad_0x59[3];
    f32 progress;  // 0x5c
};
static_assert(__builtin_offsetof(UnkWatch_b0258w, progress) == 0x5c, "layout");

struct UnkWatchArg_b0258w;
bool fn_7100573a60(UnkWatch_b0258w* self, UnkWatchArg_b0258w* arg, f32 a, f32 b);

bool fn_7100573d54(UnkWatch_b0258w* self, UnkWatchArg_b0258w* arg) {
    bool flag = false;
    if (fn_710054abf4(self->holder))
        flag = fn_7100573a60(self, arg, 1.0f, -1.0f);
    self->flag = flag;
    UnkActor* owner = self->holder->actor;
    f32 threshold = self->field_0x44;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    if (animation && fn_710117b348(animation, fn_7100c18138())) {
        threshold = 0.06f;
        if (UnkActor* actor = fn_7100d7c990(self->holder->actor)) {
            static CStrId s_spider("spider", true);
            if (actor->field_0x208 && actor->field_0x220) {
                bool busy = fn_710117af10(static_cast<UnkStruct_710117af10*>(actor->field_0x220),
                                          reinterpret_cast<UnkCls_710117af10**>(&s_spider), false);
                threshold = busy ? 0.0f : 0.06f;
            }
        }
    }
    return self->flag && self->progress >= threshold;
}

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEntity;
bool fn_71011735e8(UnkEntity* entity, const void* key, UnkMatrix44* out, u32 flags);
void fn_71000774fc(UnkXform* x);
struct UnkVec3;
void fn_710007b444(void* dst, const UnkVec3* value);

struct UnkAimHolder_b0258y {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkAim_b0258y {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    u8 pad_0x8[0x18];
    UnkAimHolder_b0258y* holder;  // 0x20
    bool active;                  // 0x28
    u8 pad_0x29[0x1b];
    f32 cap;      // 0x44
    u32 pos_x;    // 0x48
    u32 pos_y;    // 0x4c
    u8 dir[0x8];  // 0x50
    bool pending; // 0x58
    u8 pad_0x59[3];
    f32 timer;    // 0x5c
    u8 pad_0x60[0x18];
    bool seen;    // 0x78
};
static_assert(__builtin_offsetof(UnkAim_b0258y, seen) == 0x78, "layout");

void fn_7100573218(UnkAim_b0258y* self);
bool fn_7100573734(UnkAim_b0258y* self, bool strict);

void fn_71005735b8(UnkAim_b0258y* self, f32 dt) {
    if (!self->active)
        return;
    fn_7100573218(self);
    u64 key = 0xe864f8533b15425aull;
    UnkEntity* world = static_cast<UnkEntity*>(self->holder->actor->field_0x228);
    if (world) {
        UnkMatrix44 m;
        if (fn_71011735e8(world, &key, &m, 0)) {
            UnkVec3f scale = fn_7100076cec(reinterpret_cast<UnkXform*>(&m));
            if (scale.x != 0.0f && scale.y != 0.0f && scale.z != 0.0f)
                fn_71000774fc(reinterpret_cast<UnkXform*>(&m));
            CVector2D flat;
            CVector3D pos;
            __builtin_memcpy(&pos.x, &m.m[3], 4);
            __builtin_memcpy(&pos.y, &m.m[7], 4);
            __builtin_memcpy(&pos.z, &m.m[11], 4);
            fn_710007b430(&flat, &pos);
            __builtin_memcpy(&self->pos_x, &flat.x, 4);
            __builtin_memcpy(&self->pos_y, &flat.y, 4);
            __builtin_memcpy(&pos.x, &m.m[0], 4);
            __builtin_memcpy(&pos.y, &m.m[4], 4);
            __builtin_memcpy(&pos.z, &m.m[8], 4);
            fn_710007b444(self->dir, reinterpret_cast<const UnkVec3*>(&pos));
        }
    }
    // keeps the holder reload after the shape query on both paths
    asm volatile("" ::: "memory");
    bool seen = false;
    if (*(reinterpret_cast<u8*>(self->holder) + 0x14c35))
        seen = fn_7100573734(self, self->seen);
    self->seen = seen;
    self->vf_0x38();
    f32 timer = self->timer;
    if (self->pending) {
        f32 t = timer + dt;
        self->timer = t;
        if (t > self->cap)
            self->timer = self->cap;
        self->pending = false;
    } else if (timer > 0.0f) {
        timer -= dt;
        self->timer = timer;
        if (timer < 0.0f)
            self->timer = 0.0f;
    }
}
