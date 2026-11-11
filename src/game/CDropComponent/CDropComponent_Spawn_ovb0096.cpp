// The CDropComponent unit (tools/tu_map.py unit 1223): the drop component's spawn settings (two
// 0x90-byte records with a string id, copied from the source at +0x170 by fn_71007c1ab0 and passed to
// the spawn fn_71007c21e8 by fn_71007c1fb0, fn_71007c24a4 and fn_71007c26f0, which pick one of six
// directions toward the player when the record's kind is 9), the drop size from the owner's collision
// shape (fn_71007c1edc) and one drop spawn (fn_71007c3874). Names and layouts are placeholders.
#include <math.h>
#include "base/global/CStrId.h"
#include "game/CCharacterMovement.h"
#include "game/CDropComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

// Copied member by member.
struct UnkDropSpawnVec3_ovb0096 {
    f32 x, y, z;
    UnkDropSpawnVec3_ovb0096(const UnkDropSpawnVec3_ovb0096& o) : x(o.x), y(o.y), z(o.z) {}
    UnkDropSpawnVec3_ovb0096& operator=(const UnkDropSpawnVec3_ovb0096& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// The settings' name: copied like CStrId, released in two steps (the original reloads `data` and the
// count after the decrement, then the pool).
struct UnkDropName_ovb0096 : base::global::UnkStrIdBase {
    UnkDropName_ovb0096(const UnkDropName_ovb0096& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkDropName_ovb0096& operator=(const UnkDropName_ovb0096& other) {
        if (data != other.data) {
            base::global::UnkStringData* d = other.data;
            base::global::UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkDropName_ovb0096() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// The spawn settings (0x90 bytes) a drop component keeps twice.
struct UnkDropSpawnParams_ovb0096 {
    u8 field_0x0[0x5c];
    UnkDropName_ovb0096 name;
    UnkDropSpawnVec3_ovb0096 field_0x68;
    UnkDropSpawnVec3_ovb0096 field_0x74;
    u32 kind;  // 9: pick the direction (1..6) from where the player is
    u8 field_0x84[7];
};

struct UnkDropSpawnSource_ovb0096 {
    u8 pad_0x0[0x88];
    UnkDropSpawnParams_ovb0096 normal;
    UnkDropSpawnParams_ovb0096 alternate;
};

struct UnkDropSpawnFlags_ovb0096 {
    u8 pad_0x0[0x248];
    u8 flags;
};

struct UnkDropSpawner2_ovb0096 {
    void** vtable;
};

struct UnkDropSpawnOwner_ovb0096 {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x1d0 - 0x94];
    UnkDropSpawnFlags_ovb0096* flags;
    u8 pad_0x1d8[0x68];
    UnkDropSpawner2_ovb0096* spawner;
};

struct UnkDropSpawnComp_ovb0096 {
    u8 pad_0x0[0x30];
    UnkDropSpawnOwner_ovb0096* owner;
    u8 pad_0x38[0x8];
    UnkDropSpawnParams_ovb0096 normal;
    UnkDropSpawnParams_ovb0096 alternate;
    u8 pad_0x160[4];
    u32 spawn_count;
    u8 pad_0x168[8];
    UnkDropSpawnSource_ovb0096* source;
};

bool fn_71007c1d50(UnkDropSpawnComp_ovb0096* self);

void fn_71007c1ab0(UnkDropSpawnComp_ovb0096* self) {
    self->normal = self->source->normal;
    self->alternate = self->source->alternate;
}

void fn_71007c1fb0(UnkDropSpawnComp_ovb0096* self) {
    if (fn_71007c1d50(self))
        return;
    UnkDropSpawnParams_ovb0096 params = self->normal;
    UnkDropSpawnOwner_ovb0096* owner = self->owner;
    if (owner->flags && (~owner->flags->flags & 3) == 0) {
        params = self->alternate;
        owner = self->owner;
    }
    if (UnkDropSpawner2_ovb0096* spawner = owner->spawner)
        reinterpret_cast<void (*)(UnkDropSpawner2_ovb0096*, UnkDropSpawnParams_ovb0096*)>(spawner->vtable[0x4d8 / 8])(
            spawner, &params);
    fn_71007c21e8(reinterpret_cast<char*>(self), reinterpret_cast<char*>(&params), nullptr);
}

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

// Extra spawn data, passed on only when its count is set.
struct UnkDropSpawnExtra_ovb0096 {
    u8 field_0x0[8];
    s32 count;

    UnkDropSpawnExtra_ovb0096* Get() { return count != 0 ? this : nullptr; }
};

struct UnkDropSpawnRequest_ovb0096 {
    u8 pad_0x0[0xc];
    u8 field_0xc;
    u8 pad_0xd[0x76];
    u8 field_0x83;
    u8 pad_0x84[0xc];
    UnkDropSpawnExtra_ovb0096 extra;
};

UnkDropSpawnParams_ovb0096 fn_7100cb7f90(UnkDropSpawnRequest_ovb0096* request);
UnkDropSpawnParams_ovb0096 fn_7100cb5004(UnkDropSpawnRequest_ovb0096* request);

static inline UnkVec3f Sub_ovb0096(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Which of six directions the player is in, seen from the owner facing along its yaw sign.
__attribute__((always_inline)) static inline void PlayerDirection_ovb0096(UnkDropSpawnComp_ovb0096* self, u32& kind) {
    auto* player = reinterpret_cast<UnkDropSpawnOwner_ovb0096*>(fn_7100dec788(g_unk_7101d47310));
    UnkVec3f d = Sub_ovb0096(player->xform.GetTranslation(), self->owner->xform.GetTranslation());
    f32 dx = d.x;
    f32 dy = d.y;
    f32 dz = d.z;
    f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    dx *= inv;
    dy *= inv;
    dz *= inv;
    f32 side = self->owner->xform.GetAngle().y < 0.0f ? -1.0f : 1.0f;
    f32 forward = dx * side + dy * 0.0f + dz * 0.0f;
    if (forward >= 0.9238795f) {
        kind = 1;
    } else if (forward <= -0.38268343f) {
        kind = 2;
    } else {
        if (!(forward <= 0.38268343f))
            kind = dx * 0.0f + dy + dz * 0.0f > 0.0f ? 5 : 6;
        else
            kind = dx * 0.0f + dy + dz * 0.0f > 0.0f ? 3 : 4;
    }
}

void fn_71007c26f0(UnkDropSpawnComp_ovb0096* self, UnkDropSpawnRequest_ovb0096* request) {
    if (fn_71007c1d50(self))
        return;
    if (request->field_0x83)
        return;
    UnkDropSpawnParams_ovb0096 params = fn_7100cb7f90(request);
    if (params.kind == 9)
        PlayerDirection_ovb0096(self, params.kind);
    fn_71007c21e8(reinterpret_cast<char*>(self), reinterpret_cast<char*>(&params),
                  request->extra.Get());
}

void fn_71007c24a4(UnkDropSpawnComp_ovb0096* self, UnkDropSpawnRequest_ovb0096* request) {
    if (fn_71007c1d50(self))
        return;
    if (__builtin_expect(!request->field_0xc, 1) && (!self->owner->spawner || self->spawn_count != 0))
        return;
    if (request->field_0x83)
        return;
    UnkDropSpawnParams_ovb0096 params = fn_7100cb5004(request);
    if (params.kind == 9)
        PlayerDirection_ovb0096(self, params.kind);
    self->spawn_count++;
    fn_71007c21e8(reinterpret_cast<char*>(self), reinterpret_cast<char*>(&params), nullptr);
}


struct UnkShape;
struct UnkFlagged;

struct UnkDropBounds_ovb0096 {
    f32 min_x, min_y, min_z;
    f32 max_x, max_y, max_z;
};

struct UnkDropCollision_ovb0096 {
    void** vtable;
    u8 field_0x8[0xa0];
    f32 radius;
};

struct UnkDropShape_ovb0096 {
    u8 pad_0x0[0x158];
    UnkDropCollision_ovb0096* collision;
};

struct UnkDropMovement_ovb0096 {
    void** vtable;
};

struct UnkDropOwner_ovb0096 {
    u8 pad_0x0[0x218];
    UnkDropMovement_ovb0096* movement;
};

struct UnkDropComp_ovb0096 {
    u8 pad_0x0[0x30];
    UnkDropOwner_ovb0096* owner;
    u8 pad_0x38[0x128];
    f32 size;
    u8 pad_0x164[4];
    u8 field_0x168;
};

void fn_71007c1edc(UnkDropComp_ovb0096* self) {
    UnkDropMovement_ovb0096* movement = self->owner->movement;
    if (!movement) {
        self->size = 50.0f;
    } else {
        auto* shape = reinterpret_cast<UnkDropShape_ovb0096*>(
            fn_7100976588(reinterpret_cast<UnkFlagged*>(movement)));
        if (!shape) {
            UnkDropMovement_ovb0096* m = self->owner->movement;
            shape = reinterpret_cast<UnkDropShape_ovb0096* (*)(UnkDropMovement_ovb0096*)>(m->vtable[0x2a8 / 8])(m);
            if (!shape) {
                self->field_0x168 = 0;
                return;
            }
        }
        {
            UnkDropCollision_ovb0096* c = shape->collision;
            if (reinterpret_cast<s32 (*)(UnkDropCollision_ovb0096*)>(c->vtable[3])(c) == 2 && c) {
                self->size = c->radius;
            } else {
                UnkDropCollision_ovb0096* col = shape->collision;
                UnkDropBounds_ovb0096 b =
                    reinterpret_cast<UnkDropBounds_ovb0096 (*)(UnkDropCollision_ovb0096*, void*)>(col->vtable[5])(
                        col, col->field_0x8);
                f32 w = b.max_x - b.min_x;
                f32 h = b.max_y - b.min_y;
                self->size = (w < h ? w : h) * 0.5f;
            }
        }
    }
    self->field_0x168 = 0;
}


struct UnkDropVec3_b44a2;
void fn_71007c0284(void* a, void* b, const UnkDropVec3_b44a2* pos, void* d, bool e, bool f, f32 g, f32 h);

struct UnkDropDir_ovb0096 {
    f32 x, y, z;
};

struct UnkDropSpawner_ovb0096 {
    void** vtable;
};

struct UnkDropFlags_ovb0096 {
    u8 pad_0x0[0x248];
    u8 flags;
};

struct UnkDropComp3_ovb0096 {
    u8 pad_0x0[0x1d0];
    UnkDropFlags_ovb0096* flags;
    u8 pad_0x1d8[0x68];
    UnkDropSpawner_ovb0096* spawner;
};

void* fn_71007c4c94(UnkDropComp3_ovb0096* self, void* type, void* params, void* extra);
void fn_71007c4a54(UnkDropComp3_ovb0096* self, void* params, UnkDropDir_ovb0096* dir, f32* speed, u32* slots);
struct UnkDropOwner_b45a2;
f32 fn_71007c7e4c(UnkDropOwner_b45a2* self);

void fn_71007c3874(UnkDropComp3_ovb0096* self, const UnkDropVec3_b44a2* pos, void* type, void* params, u32* slots,
                   void* extra) {
    void* drop;
    if (UnkDropSpawner_ovb0096* spawner = self->spawner)
        drop = reinterpret_cast<void* (*)(UnkDropSpawner_ovb0096*, void*, void*, void*)>(spawner->vtable[0x4e0 / 8])(
            spawner, type, params, extra);
    else
        drop = fn_71007c4c94(self, type, params, extra);
    if (!drop)
        return;
    UnkDropDir_ovb0096 dir;
    dir.x = 0;
    dir.y = 0;
    dir.z = 0;
    f32 speed;
    fn_71007c4a54(self, params, &dir, &speed, slots);
    f32 time = fn_71007c7e4c(static_cast<UnkDropOwner_b45a2*>(params));
    f32 s = speed;
    bool flag = self->flags ? (self->flags->flags >> 2) & 1 : false;
    fn_71007c0284(drop, self, pos, &dir, false, flag, s, time);
}
