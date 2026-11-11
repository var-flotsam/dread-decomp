// CFXComponent's unit (unit 2712 of tools/tu_map.py): the per-frame update of an effect component
// (fn_710114e8b4), its scene attach step (fn_7101152f88), and two members of the unit's container of
// string ids over an embedded block pool (base/UnkParentPoolList.h): its destructor (fn_7101157530)
// and the removal of one node (fn_7101157324). Names and layouts are placeholders.
#include "base/UnkParentPoolList.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU132.h"
#include "game/UnkTU247.h"
#include "types.h"

// ---- the container of string ids ----

extern char unk_7101cb38e8[];  // the base part's vtable (reached through the GOT)
extern char unk_7101cb38c8[];  // the container's vtable (reached through the GOT)

struct UnkFXIdNode {
    base::global::CStrId id;
    // Union members: Clang gives union accesses no type-based alias info, so the links are
    // reloaded after each store as in the original.
    union {
        UnkFXIdNode* next;
    };
    union {
        UnkFXIdNode* prev;
    };
};

static inline UnkFXIdNode*& Head(UnkParentPoolList* self) { return reinterpret_cast<UnkFXIdNode*&>(self->head); }
static inline UnkFXIdNode*& Tail(UnkParentPoolList* self) { return reinterpret_cast<UnkFXIdNode*&>(self->tail); }

// Releases the node's id and gives the node back to the pool.
static inline void FreeNode(UnkParentPoolList* self, UnkFXIdNode* node) {
    node->id.Reset();
    self->allocator->vf_0x48(node);
}

static inline void ClearIds(UnkParentPoolList* self) {
    UnkFXIdNode* node = Head(self);
    while (node) {
        UnkFXIdNode* next = node->next;
        FreeNode(self, node);
        node = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

// The destructor: the class's part, then the pool's, then the base's.
void fn_7101157530(UnkParentPoolList* self) {
    self->vtable = unk_7101cb38c8 + 0x10;
    self->pool.field_0x3c = true;
    ClearIds(self);
    self->allocator = nullptr;
    self->pool.~UnkBlockPool();
    self->vtable = unk_7101cb38e8 + 0x10;
    ClearIds(self);
}

// Removes `node` and returns the node after it; after the first or the last node, the new first or
// last one. A node that isn't in the list is left alone (null).
UnkFXIdNode* fn_7101157324(UnkParentPoolList* self, UnkFXIdNode* node) {
    if (Head(self) == node) {
        if (!node)
            return nullptr;
        Head(self) = node->next;
        if (Head(self))
            Head(self)->prev = nullptr;
        FreeNode(self, node);
        if (--self->count == 0)
            Tail(self) = Head(self);
        return Head(self);
    }
    if (Tail(self) == node) {
        if (!node)
            return nullptr;
        Tail(self) = node->prev;
        if (Tail(self))
            Tail(self)->next = nullptr;
        FreeNode(self, node);
        if (--self->count == 0)
            Tail(self) = Head(self);
        return Tail(self);
    }
    for (UnkFXIdNode* it = Head(self); it;) {
        UnkFXIdNode* next = it->next;
        if (it == node) {
            node->prev->next = next;
            if (node->next)
                node->next->prev = node->prev;
            --self->count;
            UnkFXIdNode* after = node->next;
            FreeNode(self, node);
            return after;
        }
        it = next;
    }
    return nullptr;
}

// ---- the component ----

struct UnkParent;
struct UnkPlayer;
struct UnkObject;
struct UnkResetOwner;
struct UnkBinderNames_71001bbfa8;
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_710114fe8c(UnkObject*, u64, u32, u32);
void fn_71001bbfa8(UnkBinderNames_71001bbfa8* self, const base::global::CStrId* name);

struct UnkFXSceneHolder_b0052 {
    u8 unk_0x0[0x50];
    void* scene;  // 0x50
};

struct UnkFXRider_b0052 {
    u8 unk_0x0[0x70];
    s32 state;  // 0x70
};

// The owning actor, and the player as the distance test reads it: the translation column of the
// world transform at +0x44/+0x54/+0x64.
struct UnkFXActor_b0052 {
    u8 unk_0x0[0x44];
    f32 x;  // 0x44
    u8 unk_0x48[0xc];
    f32 y;  // 0x54
    u8 unk_0x58[0xc];
    f32 z;  // 0x64
    u8 unk_0x68[0x14d];
    bool field_0x1b5;
    u8 unk_0x1b6[2];
    UnkFXRider_b0052* rider;  // 0x1b8
    u8 unk_0x1c0[0x68];
    UnkFXSceneHolder_b0052* scene_holder;  // 0x228
};
static_assert(__builtin_offsetof(UnkFXActor_b0052, scene_holder) == 0x228, "UnkFXActor_b0052 layout");

struct UnkFXSub_b0052 {
    u8 unk_0x0[0x119];
    bool field_0x119;
};

struct UnkFXModelFlags_b0052 {
    u8 unk_0x0[0x11d];
    bool field_0x11d;
};

struct UnkFXComponent_b0052 {
    u8 unk_0x0[0x30];
    UnkFXActor_b0052* actor;  // 0x30
    u8 unk_0x38[0x78];
    f32 field_0xb0;
    u8 unk_0xb4[0x18];
    f32 field_0xcc;
    u8 unk_0xd0[4];
    f32 field_0xd4;
    u8 unk_0xd8[8];
    UnkFXSub_b0052* field_0xe0;
    u8 unk_0xe8[0x20];
    f32 field_0x108;
    u8 unk_0x10c[4];
    bool field_0x110;
    bool field_0x111;
    u8 unk_0x112[2];
    f32 far_radius;   // 0x114
    f32 near_radius;  // 0x118
    u8 unk_0x11c[8];
    f32 field_0x124;
    u8 field_0x128[0x18];
    void* scene;                  // 0x140
    base::global::CStrId* names;  // 0x148
    u32 name_count;               // 0x150
    u8 unk_0x154[0x1c];
    void* field_0x170;
};
static_assert(__builtin_offsetof(UnkFXComponent_b0052, field_0xe0) == 0xe0, "UnkFXComponent_b0052 layout");
static_assert(__builtin_offsetof(UnkFXComponent_b0052, scene) == 0x140, "UnkFXComponent_b0052 layout");
static_assert(__builtin_offsetof(UnkFXComponent_b0052, field_0x170) == 0x170, "UnkFXComponent_b0052 layout");

void fn_710114eaa4(UnkFXComponent_b0052* self, f32 dt);
void fn_710114eb28(UnkFXComponent_b0052* self, f32 dt);
void fn_710114ebb8(UnkFXComponent_b0052* self, f32 dt);
void fn_7101150088(UnkFXComponent_b0052* self, s32 band);
void fn_71011531cc(UnkFXComponent_b0052* self, u32 flag, void* arg, f32 value);
void fn_710115351c(UnkFXComponent_b0052* self);
void fn_710114ec8c(UnkFXComponent_b0052* self, f32 dt);
void fn_710114eeac(UnkFXComponent_b0052* self);

static inline UnkModelManager* GetManager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

static inline char* FirstModel() { return reinterpret_cast<char*>(GetManager()->models[0]); }

// Attaches the effect to the owner's scene (detaching it from the previous one first), with the
// names at +0x148 bound to the first overlay model, unless the owner or the effect is off.
void fn_7101152f88(UnkFXComponent_b0052* self, f32 value) {
    UnkFXActor_b0052* actor = self->actor;
    if (actor->field_0x1b5)
        return;
    if (self->field_0xe0 && self->field_0xe0->field_0x119)
        return;
    if (UnkFXRider_b0052* rider = actor->rider) {
        if (!fn_710024fbc8(rider) || rider->state == 2)
            return;
    }
    self->field_0xcc = value;
    UnkFXSceneHolder_b0052* holder = self->actor->scene_holder;
    if (!holder || !holder->scene)
        return;
    if (self->scene) {
        // The detach takes the scene too.
        reinterpret_cast<void (*)(void*, void*)>(fn_710014580c)(FirstModel(), self->scene);
        self->scene = holder->scene;
        if (!self->scene)
            return;
    } else {
        self->scene = holder->scene;
    }
    fn_71001bc068(*reinterpret_cast<UnkResetOwner**>(FirstModel() + 0x150));
    if (!self->name_count || !self->names) {
        fn_71001457fc(FirstModel(), self->scene, 0);
        return;
    }
    // The manager is fetched before each bound check; the last fetch serves the attach.
    for (u64 i = 0;; ++i) {
        u64 count = self->name_count;
        char* model = FirstModel();
        if (i >= count) {
            fn_71001457fc(model, self->scene, 1);
            return;
        }
        fn_71001bbfa8(*reinterpret_cast<UnkBinderNames_71001bbfa8**>(model + 0x150), &self->names[i]);
    }
}

struct UnkFXPos_b0052 {
    f32 x, y, z;
};

static inline UnkFXPos_b0052 Position(const UnkFXActor_b0052* e) { return {e->x, e->y, e->z}; }

// 0 within the near radius of the player, 1 within the far one, -1 beyond. (Written as a switch:
// the original branches where a chain of ifs becomes a select.)
static inline s32 Band(UnkFXComponent_b0052* self, UnkFXActor_b0052* player) {
    UnkFXPos_b0052 p = Position(player);
    UnkFXPos_b0052 a = Position(self->actor);
    f32 dx = p.x - a.x;
    f32 dy = p.y - a.y;
    f32 dz = p.z - a.z;
    f32 d2 = dx * dx + dy * dy + dz * dz;
    switch (d2 < self->near_radius * self->near_radius ? 0 : d2 < self->far_radius * self->far_radius ? 1 : 2) {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return -1;
    }
}

// The per-frame update: the base step, the three sub-updates, the distance band to the player, two
// countdown timers and the model check, then the final steps.
void fn_710114e8b4(UnkFXComponent_b0052* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    self->field_0xb0 -= dt;
    if (!self->actor->field_0x1b5) {
        fn_710114eaa4(self, dt);
        fn_710114eb28(self, dt);
        fn_710114ebb8(self, dt);
        if (self->field_0x110 && !self->field_0x111) {
            if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
                fn_7101150088(self, Band(self, reinterpret_cast<UnkFXActor_b0052*>(player)));
        }
        if (self->field_0x124 > 0.0f) {
            self->field_0x124 -= dt;
            if (self->field_0x124 <= 0.0f) {
                self->field_0x124 = 0.0f;
                fn_71011531cc(self, 1, self->field_0x128, self->field_0xd4);
            }
        }
        if (self->field_0x170 && !reinterpret_cast<UnkFXModelFlags_b0052*>(GetManager()->models[5])->field_0x11d)
            fn_710115351c(self);
        if (self->field_0x108 > 0.0f) {
            self->field_0x108 -= dt;
            if (self->field_0x108 <= 0.0f) {
                self->field_0x108 = 0.0f;
                fn_710114fe8c(reinterpret_cast<UnkObject*>(self), 0x444c7220d7fc64c4, 1, 1);
            }
        }
        fn_710114ec8c(self, dt);
    }
    fn_710114eeac(self);
}
