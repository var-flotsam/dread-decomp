// Unit 2702 (hint CAIComponent): asks for a new path: unless path generation is blocked, resets the
// path state at +0x68, fills a path request from the owner, its movement and the tunables at +0x1610,
// runs it (fn_71010fb43c) and, when it succeeds, refreshes the path flags. Names and layouts are
// placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassAIComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/CInputComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::global::CStrId;

struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkEnemyMovementFlags;
bool fn_71009745f0(void*);
void fn_71010c9b08(UnkEnemyMovementFlags* self);

// A 4-byte element whose out-of-line destructor is empty.
struct UnkPathKey_l0fab58 {
    u32 value;
    ~UnkPathKey_l0fab58() { fn_71002874dc(this); }
};

// The path state at +0x68 (the object fn_7100119f98 enables, game/UnkTU108.h).
struct UnkPathState_l0fab58 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u8 pad_0xc[0x1c];
    u32 field_0x28;
    bool enabled;  // 0x2c
    bool field_0x2d;
    u8 pad_0x2e[0x22];
    u32 field_0x50;
    u8 pad_0x54[0x1c];
    UnkPathKey_l0fab58* keys;  // 0x70
    u32 key_count;              // 0x78
    u8 pad_0x7c[0x1c];
    u32 field_0x98;

    void Reset() {
        enabled = false;
        field_0x2d = true;
        field_0x8 = 0;
        field_0x28 = 0;
        field_0x50 = 0;
        for (u64 i = 0; i < key_count; ++i)
            keys[i].~UnkPathKey_l0fab58();
        key_count = 0;
        field_0x98 = 0;
        fn_7100119f98(reinterpret_cast<UnkTileOutline*>(this), false);
    }
};
static_assert(__builtin_offsetof(UnkPathState_l0fab58, field_0x98) == 0x98, "layout");

// The owner's transform block, copied into the request.
struct UnkPathXform_l0fab58 {
    f32 values[23];
};

struct UnkPathMovement_l0fab58 {
    void** vtable;

    void vf_0x288() { reinterpret_cast<void (*)(UnkPathMovement_l0fab58*)>(vtable[0x288 / 8])(this); }
};

struct UnkPathAnimation_l0fab58 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;

    void vf_0x260() { reinterpret_cast<void (*)(UnkPathAnimation_l0fab58*)>(vtable[0x260 / 8])(this); }
};

struct UnkPathNamed_l0fab58 {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkPathGroupRef_l0fab58 {
    u8 pad_0x0[0x30];
    UnkPathNamed_l0fab58* named;
};

struct UnkPathGroupHolder_l0fab58 {
    u8 pad_0x0[0x10];
    UnkPathGroupRef_l0fab58* ref;
};

struct UnkPathLife_l0fab58 {
    u8 pad_0x0[0x48];
    UnkPathGroupHolder_l0fab58* holder;
};

struct UnkPathOwner_l0fab58 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    UnkPathXform_l0fab58 xform;  // 0x38
    u8 pad_0x94[0x124];
    UnkPathLife_l0fab58* life;  // 0x1b8
    u8 pad_0x1c0[0x38];
    UnkPathMovement_l0fab58* movement;  // 0x1f8
    u8 pad_0x200[0x18];
    UnkPathAnimation_l0fab58* animation;  // 0x218
    UnkAnimationComponent* character;  // 0x220
};
static_assert(__builtin_offsetof(UnkPathOwner_l0fab58, character) == 0x220, "layout");

struct UnkPathTunable_l0fab58 {
    void** vtable;
    u8 pad_0x8[0xe8];
    u32 field_0xf0;
    u8 pad_0xf4[0x4];
    f32 reach_x;  // 0xf8
    f32 reach_y;  // 0xfc
    u8 pad_0x100[0x4];
    u8 field_0x104;
    u8 pad_0x105[0x3];
    f32 field_0x108;
    f32 field_0x10c;
    u32 field_0x110;
    u8 field_0x114;
    u8 pad_0x115[0xf3];
    u32 field_0x208;
    u8 pad_0x20c[0xc];
    u32 field_0x218;
    u8 pad_0x21c[0xc];
    u32 field_0x228;
    u8 pad_0x22c[0x174];
    bool use_smart_links;  // 0x3a0

    bool vf_0xb8() { return reinterpret_cast<bool (*)(UnkPathTunable_l0fab58*)>(vtable[0xb8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkPathTunable_l0fab58, use_smart_links) == 0x3a0, "layout");

struct UnkSmartLinkKey_l0fab58 {
    u8 pad_0x0[0x30];
    void* key;
};

// The navigation mesh's smart links: values of 0x28 bytes and their keys.
struct UnkSmartLinkMesh_l0fab58 {
    u8 pad_0x0[0x2e8];
    u8 (*values)[0x28];  // 0x2e8
    void** keys;  // 0x2f0
    u32 count;  // 0x2f8

    u8 (*Find(void* key))[0x28] {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }
};

struct UnkPathRequest_l0fab58 {
    CStrId name;
    u32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    bool field_0x14;
    u32 layer;  // 0x18
    UnkPathXform_l0fab58 xform;  // 0x1c
    f32 speed;  // 0x78
    UnkAIReach reach;  // 0x7c
    u32 field_0x84;
    f32 field_0x88;
    f32 field_0x8c;
    f32 field_0x90;
    bool field_0x94;
    bool field_0x95;
    f32 field_0x98;
    u8 field_0x9c;
    void* link;  // 0xa0
    void* group;  // 0xa8
    u8 field_0xb0;
    u32 field_0xb4;
    u64 field_0xb8;
    u64 field_0xc0;
    u32 field_0xc8;
    u32 field_0xcc;

    explicit UnkPathRequest_l0fab58(const CStrId& owner_name) : name(owner_name) {}
};
static_assert(__builtin_offsetof(UnkPathRequest_l0fab58, link) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkPathRequest_l0fab58, field_0xcc) == 0xcc, "layout");

struct UnkPathAI_l0fab58 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPathOwner_l0fab58* owner;  // 0x30
    u8 pad_0x38[0x7];
    u8 field_0x3f;
    u8 pad_0x40[0x8];
    u8 field_0x48;
    u8 pad_0x49[0x3];
    s32 field_0x4c;
    u8 field_0x50[0x18];
    UnkPathState_l0fab58 state;  // 0x68
    u8 pad_0x108[0x1000];
    u64 field_0x1108;
    u8 pad_0x1110[0x28];
    u32 field_0x1138;
    u8 pad_0x113c[0x28c];
    f32 field_0x13c8;
    f32 field_0x13cc;
    bool field_0x13d0;
    bool field_0x13d1;
    u8 pad_0x13d2[0x1be];
    u64 field_0x1590;
    u64 field_0x1598;
    u8 pad_0x15a0[0x70];
    UnkPathTunable_l0fab58* tunable;  // 0x1610
    u8 pad_0x1618[0xcc];
    bool repath;  // 0x16e4
    bool blocked;  // 0x16e5
    u8 pad_0x16e6[0x142];
    u8 list_0x1828[0x80];
    u8 list_0x18a8[0x8];

    f32 vf_0x3c8() { return reinterpret_cast<f32 (*)(UnkPathAI_l0fab58*)>(vtable[0x3c8 / 8])(this); }
    f32 vf_0x3d0() { return reinterpret_cast<f32 (*)(UnkPathAI_l0fab58*)>(vtable[0x3d0 / 8])(this); }
    UnkSmartLinkKey_l0fab58* vf_0x3e0() {
        return reinterpret_cast<UnkSmartLinkKey_l0fab58* (*)(UnkPathAI_l0fab58*)>(vtable[0x3e0 / 8])(this);
    }
    void vf_0x748(u8* a, u8* b, s32* c, u8* d) {
        reinterpret_cast<void (*)(UnkPathAI_l0fab58*, u8*, u8*, s32*, u8*)>(vtable[0x748 / 8])(this, a, b, c, d);
    }
    void vf_0x790() { reinterpret_cast<void (*)(UnkPathAI_l0fab58*)>(vtable[0x790 / 8])(this); }
    void vf_0x798() { reinterpret_cast<void (*)(UnkPathAI_l0fab58*)>(vtable[0x798 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkPathAI_l0fab58, field_0x1108) == 0x1108, "layout");
static_assert(__builtin_offsetof(UnkPathAI_l0fab58, field_0x13d1) == 0x13d1, "layout");
static_assert(__builtin_offsetof(UnkPathAI_l0fab58, field_0x1590) == 0x1590, "layout");
static_assert(__builtin_offsetof(UnkPathAI_l0fab58, tunable) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkPathAI_l0fab58, blocked) == 0x16e5, "layout");
static_assert(__builtin_offsetof(UnkPathAI_l0fab58, list_0x18a8) == 0x18a8, "layout");

bool fn_71010fb43c(UnkPathRequest_l0fab58* request, UnkPathState_l0fab58* state, u32* a, u32* b, bool* flag);

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// Whether path generation is blocked for now.
static inline bool PathBlocked(UnkPathAI_l0fab58* self) {
    if (self->blocked)
        return true;
    if (UnkAnimationComponent* character = self->owner->character) {
        if (fn_710117b348(character, fn_7100d0b278()))
            return true;
    }
    UnkPathAnimation_l0fab58* animation = self->owner->animation;
    if (fn_71010cc3a4(animation) == 4)
        return fn_71009745f0(reinterpret_cast<u8*>(animation) + 0x17a0);
    return false;
}

void fn_71010fab58(UnkPathAI_l0fab58* self) {
    if (!self->owner->movement)
        return;
    if (PathBlocked(self)) {
        self->repath = true;
        return;
    }

    self->state.Reset();
    self->field_0x1108 = 0;
    fn_7100874460(self->list_0x18a8, reinterpret_cast<UnkWeakTarget**>(&self->owner));

    bool flag = self->tunable->vf_0xb8();
    if (!fn_7100976588(reinterpret_cast<UnkFlagged*>(self->owner->movement)))
        self->owner->movement->vf_0x288();
    UnkAIReach reach = self->owner->movement ? fn_7100977b90(self->owner->movement) : UnkAIReach();
    if (!(self->tunable->reach_x < 0.0f))
        reach.radius = self->tunable->reach_x;
    if (!(self->tunable->reach_y < 0.0f))
        reach.field_0x4 = self->tunable->reach_y;
    f32 distance = self->vf_0x3c8();
    UnkPathTunable_l0fab58* tunable = self->tunable;
    if (!(tunable->field_0x108 < 0.0f))
        distance = tunable->field_0x108;
    f32 max_distance = tunable->field_0x10c < 0.0f ? distance : tunable->field_0x10c;
    u32 field_0x110 = tunable->field_0x110;
    if (self->field_0x13c8 < 0.0f) {
        self->field_0x13d0 = true;
        self->field_0x13c8 = distance;
    }
    if (self->field_0x13cc < 0.0f) {
        self->field_0x13d1 = true;
        self->field_0x13cc = distance;
    }
    bool new_b = self->field_0x13d1;
    UnkPathOwner_l0fab58* owner = self->owner;
    u32 field_0xf0 = tunable->field_0xf0;
    bool new_a = self->field_0x13d0;
    u32 layer = fn_7100413fd0(reinterpret_cast<UnkSource_7100286798*>(self->tunable));
    UnkPathOwner_l0fab58* mover = self->owner;
    f32 speed = fn_7100977bec(reinterpret_cast<UnkFlagged*>(mover->movement));
    u32 field_0x208 = self->tunable->field_0x208;
    f32 field_0x13c8 = self->field_0x13c8;
    f32 field_0x13cc = self->field_0x13cc;
    f32 turn = self->vf_0x3d0();
    u8 field_0x104 = self->tunable->field_0x104;

    void* link = nullptr;
    if (self->tunable->use_smart_links) {
        auto* mesh = reinterpret_cast<UnkSmartLinkMesh_l0fab58*>(NavMeshManager()->navmesh);
        if (mesh) {
            if (UnkSmartLinkKey_l0fab58* key = self->vf_0x3e0()) {
                u8(*found)[0x28] = mesh->Find(key->key);
                link = found == &mesh->values[mesh->count] ? nullptr : found;
            }
        }
    }
    void* group = nullptr;
    if (UnkPathLife_l0fab58* life = self->owner->life) {
        if (!life->holder || !life->holder->ref)
            fn_710115fe20(reinterpret_cast<UnkHolder*>(life));
        if (life->holder && life->holder->ref) {
            if (UnkPathNamed_l0fab58* named = life->holder->ref->named)
                group = fn_7100ec1344(NavMeshManager(), &named->name);
        }
    }

    u32 out_a;
    u32 out_b;
    bool changed = false;
    bool found;
    {
        UnkPathRequest_l0fab58 request(owner->name);
        request.field_0x8 = field_0xf0;
        request.field_0xc = -1.0f;
        request.field_0x10 = -1.0f;
        request.field_0x14 = flag;
        request.layer = layer;
        request.xform = mover->xform;
        request.speed = speed;
        request.reach = reach;
        request.field_0x84 = field_0x208;
        request.field_0x88 = field_0x13c8;
        request.field_0x8c = field_0x13cc;
        request.field_0x90 = max_distance;
        request.field_0x94 = new_a;
        request.field_0x95 = new_b;
        request.field_0x98 = turn;
        request.field_0x9c = field_0x104;
        request.link = link;
        request.group = group;
        request.field_0xb0 = self->tunable->field_0x114;
        request.field_0xb4 = field_0x110;
        request.field_0xb8 = self->field_0x1590;
        request.field_0xc0 = self->field_0x1598;
        request.field_0xc8 = self->tunable->field_0x218;
        request.field_0xcc = self->tunable->field_0x228;
        found = fn_71010fb43c(&request, &self->state, &out_a, &out_b, &changed);
        if (found) {
            self->repath = false;
            self->field_0x1138 = 0;
            u8 old_3f = self->field_0x3f;
            u8 old_48 = self->field_0x48;
            self->field_0x4c = -1;
            self->vf_0x748(&self->field_0x3f, &self->field_0x48, &self->field_0x4c, self->field_0x50);
            if (old_3f != self->field_0x3f)
                self->vf_0x790();
            if (old_48 != self->field_0x48)
                self->vf_0x798();
            if (changed) {
                UnkPathAnimation_l0fab58* animation = self->owner->animation;
                if (animation && animation->field_0xf9 != true) {
                    animation->field_0xf9 = true;
                    animation->vf_0x260();
                }
            }
        } else {
            self->repath = true;
        }
    }
    if (!found)
        return;
    if (UnkPathAnimation_l0fab58* animation = self->owner->animation)
        fn_71010c9b08(reinterpret_cast<UnkEnemyMovementFlags*>(animation));
    fn_7100874460(self->list_0x1828, reinterpret_cast<UnkWeakTarget**>(&self->owner));
}
