// CAttackComponent (unit 2706): remembers a hit and, unless the target ignores it, sends the target a
// damage request built from the hit and the component's attack settings (virtual 0x190).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern "C" float sqrtf(float);

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

// Where and how a hit landed (0x30 bytes), copied a word at a time.
struct UnkHitInfo_l136314 {
    u32 id;           // 0x0
    UnkVec3f point;   // 0x4
    UnkVec3f normal;  // 0x10
    UnkVec3f back;    // 0x1c
    u32 field_0x28;
    u32 flags;  // 0x2c

    UnkHitInfo_l136314& operator=(const UnkHitInfo_l136314& other) {
        id = other.id;
        point = other.point;
        normal = other.normal;
        back = other.back;
        field_0x28 = other.field_0x28;
        flags = other.flags;
        return *this;
    }
};

struct UnkHitLink_l136314 {
    u8 pad_0x0[0x10];
    void* collider;  // 0x10
};

struct UnkHitMessage_l136314 {
    UnkHitLink_l136314* link;
    u8 pad_0x8[0x8];
    UnkHitInfo_l136314 info;  // 0x10
};

struct UnkHitOther_l136314 {
    u8 pad_0x0[0x20];
    u64 id;  // 0x20
};

struct UnkAttackGate_l136314 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkAttackActor_l136314 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1b8 - 0x94];
    UnkAttackGate_l136314* gate;  // 0x1b8
    u8 pad_0x1c0[0x220 - 0x1c0];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkAttackConfig_l136314 {
    u8 pad_0x0[0x58];
    CStrId damage_id;  // 0x58
    u8 pad_0x60[0x8];
    u32 flags;  // 0x68
    bool rotate_to_instigator;  // 0x6c
    u8 pad_0x6d[0x3];
    u32 field_0x70;
    bool field_0x74;
    bool field_0x75;
    u8 pad_0x76[0x2];
    s32 kind;  // 0x78
    bool field_0x7c;
    u8 pad_0x7d[0xd0 - 0x7d];
    u64* ignored;  // 0xd0
    u32 ignored_count;  // 0xd8
};

// The damage request (fn_71008c0c74 builds it, fn_71008c1558 tears it down).
struct UnkAttackRequest_l136314 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    UnkWeakHandle* target;  // 0x10
    bool field_0x18;
    u8 pad_0x19[0x3];
    s32 mode;  // 0x1c
    u8 pad_0x20[0x1];
    bool field_0x21;
    u8 pad_0x22[0x1];
    bool field_0x23;
    u8 pad_0x24[0x1];
    bool rotate_to_instigator;  // 0x25
    u8 pad_0x26[0x60 - 0x26];
    void* collider;  // 0x60
    UnkHitInfo_l136314 hit;  // 0x68
    UnkHitOther_l136314* other;  // 0x98
    u8 pad_0xa0[0x10];
    u32 field_0xb0;
    u32 flags;  // 0xb4
    u8 id_raw[0x8];  // 0xb8: a CStrId
    u8 pad_0xc0[0x8];
    s32 kind;  // 0xc8
    u8 pad_0xcc[0x8];
    bool field_0xd4;
    u8 pad_0xd5[0x2b];

    CStrId& Id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkAttackRequest_l136314() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkAttackRequest_l136314() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkAttackRequest_l136314) == 0x100, "request layout");

// What fn_71011368a0 finds for a hit: the damage id, its kind and its amount.
struct UnkAttackQuery_l136314 {
    CStrId id;
    s32 kind;
    f32 amount;

    UnkAttackQuery_l136314() : id(g_unk_7101d470e0), kind(0x7fffffff), amount(-1.0f) {}
};

struct UnkAttackComponent_l136314 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAttackActor_l136314* owner;  // 0x30
    u8 pad_0x38[0x18];
    UnkHitInfo_l136314 last_hit;  // 0x50
    u8 pad_0x80[0x130 - 0x80];
    UnkAttackConfig_l136314* config;  // 0x130

    bool Send(UnkAttackActor_l136314* target, UnkAttackRequest_l136314* request, void* arg,
              UnkHitOther_l136314* other) {
        using Fn = bool (*)(UnkAttackComponent_l136314*, UnkAttackActor_l136314*, UnkAttackRequest_l136314*, void*,
                            UnkHitOther_l136314*);
        return reinterpret_cast<Fn>(vtable[0x190 / 8])(this, target, request, arg, other);
    }
};

bool fn_71011368a0(UnkAttackComponent_l136314* self, const UnkHitMessage_l136314* hit,
                   UnkAttackQuery_l136314* query);

static inline bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

// Makes `id` refer to the string `other` refers to: the new one is held before the old one is released.
static inline void Replace(CStrId& id, const CStrId& other) {
    base::global::UnkStringData* data = other.data;
    if (data != &g_unk_7101d47108->empty)
        ++data->ref_count;
    id.Reset();
    id.data = data;
}

bool fn_7101136314(UnkAttackComponent_l136314* self, UnkAttackActor_l136314* target, UnkHitMessage_l136314* hit,
                   UnkHitOther_l136314* other, s32 mode) {
    self->last_hit = hit->info;
    if (self->owner == target)
        return false;
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100d19858())) {
        if (target->animation && fn_710117b348(target->animation, fn_7100d0e1d8()))
            return false;
    } else if (!target) {
        return false;
    }
    if (!target->gate || target->gate->state == 2)
        return false;

    UnkAttackRequest_l136314 request;
    UnkAttackQuery_l136314 query;
    bool found = fn_71011368a0(self, hit, &query);
    Replace(request.Id(), (found && !IsEmpty(query.id)) ? query.id : self->config->damage_id);
    if (found && query.amount >= 0.0f)
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), query.amount);
    else if (IsEmpty(request.Id()))
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), 0.0f);
    else
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), fn_7100789f5c(&request.Id()));

    request.kind = found && query.kind != 0x7fffffff ? query.kind : 0;
    if (mode == 1 && (request.kind < 3 || request.kind > 5))
        request.kind = self->config->kind;

    AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
    AssignRef(&request.target, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));

    request.flags = self->config->flags;
    if (self->config->rotate_to_instigator) {
        request.rotate_to_instigator = true;
    } else {
        UnkAnimationComponent* own = self->owner->animation;
        request.rotate_to_instigator = own && fn_710117b348(own, fn_7100c484f8());
    }
    request.field_0xb0 = self->config->field_0x70;
    request.field_0x21 = self->config->field_0x74;
    request.field_0x18 = self->config->field_0x75;
    request.field_0x23 = self->config->field_0x7c;

    if (!(hit->info.flags & 0x20)) {
        UnkXform& from = self->owner->xform;
        f32 dx = target->xform.m[0][3] - from.m[0][3];
        f32 dy = target->xform.m[1][3] - from.m[1][3];
        f32 dz = target->xform.m[2][3] - from.m[2][3];
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        request.hit.id = 0xffffffff;
        request.hit.point = hit->info.point;
        request.hit.normal = UnkVec3f(dx * inv, dy * inv, dz * inv);
        request.hit.back = UnkVec3f(-(dx * inv), -(dy * inv), -(dz * inv));
        request.hit.field_0x28 = 0;
        request.hit.flags = 0x38;
    } else {
        request.hit = hit->info;
    }
    request.collider = hit->link ? hit->link->collider : nullptr;
    request.other = other;

    request.field_0xd4 = true;
    if (other) {
        UnkAttackConfig_l136314* c = self->config;
        for (u32 i = 0; i < c->ignored_count; i++) {
            if (c->ignored[i] == other->id) {
                request.field_0xd4 = false;
                break;
            }
        }
    }
    request.mode = mode;
    return self->Send(target, &request, nullptr, other);
}
