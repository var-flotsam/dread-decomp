// CFXComponent's unit (tools/tu_map.py): the component's named effects, kept in parallel arrays
// (+0x50 the 0x20-byte entries, the effect first; +0x58 the names, whose hash is at +0x30; +0x60 the
// count), its named groups of parts (+0x78, keyed by string id), two timed overlay effects (+0x140 and
// +0x158, counted down at +0xcc and +0xd0) and the messages that switch, tint or reset named effects.
// Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CActorComponent.h"
#include "game/CFXComponent.h"
#include "game/CSceneComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU132.h"
#include "game/UnkTU218.h"
#include "game/UnkTU247.h"
#include "game/UnkTU253.h"
#include "game/UnkTU285.h"
#include "types.h"

using base::global::UnkStringData;

extern "C" int rand();
void fn_7100206fac(void* part, f32 t, u32 a, u32 b, f32 from, f32 to);
struct UnkResetOwner;

// Calls the virtual function at byte offset `Off` of `obj`'s vtable.
template <u32 Off, class R = void, class... A>
static inline R VCall(void* obj, A... a) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(obj))[Off / 8])(obj, a...);
}

// fn_710014580c with the scene (or model) it detaches the effect from.
static inline void Detach(void* effect, void* scene) {
    reinterpret_cast<void (*)(void*, void*)>(fn_710014580c)(effect, scene);
}

struct UnkFXName_ovb51 {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline u64 HashOf(UnkStringData* id) { return reinterpret_cast<UnkFXName_ovb51*>(id)->hash; }

// A named part of a model (0x20 bytes); its name sits behind +0x10.
struct UnkFXModelPart_ovb51 {
    u8 pad_0x0[0x10];
    UnkFXName_ovb51** name;
    u8 pad_0x18[0x8];
};

struct UnkFXPartSet_ovb51 {
    u8 pad_0x0[0x34];
    s32 count;  // 0x34
    UnkFXModelPart_ovb51* parts;  // 0x38
};

struct UnkFXModel_ovb51 {
    u8 pad_0x0[0x78];
    UnkFXPartSet_ovb51* parts;  // 0x78
};

struct UnkFXEffect_ovb51 {
    void** vtable;
    union {
        struct {
            u32 field_0x8 : 24;
            u32 kind : 8;  // 0xb, read as a bit-field
        };
        struct {
            u8 pad_0x8[3];
            u8 type;  // 0xb, read as a byte
        };
    };
    u8 pad_0xc[0x14];
    u8 part[0x8];  // 0x20
    u8 pad_0x28[0xf1];
    u8 on;  // 0x119
    u8 pad_0x11a[0x46];
    UnkFXModel_ovb51* model;  // 0x160
};

struct UnkFXEntry_ovb51 {
    UnkFXEffect_ovb51* effect;
    bool saved;  // 0x8
    u8 pad_0x9[0x7];
    void* field_0x10;
    u8 on;  // 0x18
    u8 pad_0x19[0x7];
};

struct UnkFXEffects_ovb51 {
    UnkFXEntry_ovb51* values;
    UnkFXName_ovb51** names;
    u32 count;

    UnkFXEntry_ovb51* End() { return &values[count]; }
    UnkFXEntry_ovb51* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXEntry_ovb51* Get(u64 hash) {
        UnkFXEntry_ovb51* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXGroupItem_ovb51 {
    u64 field_0x0;
    u8 field_0x8[0x8];
};

struct UnkFXGroup_ovb51 {
    UnkFXGroupItem_ovb51* items;
    u32 count;
    u8 pad_0xc[0x1c];
};

struct UnkFXGroups_ovb51 {
    UnkFXGroup_ovb51* values;
    UnkStringData** keys;
    u32 count;

    UnkFXGroup_ovb51* End() { return &values[count]; }
    UnkFXGroup_ovb51* Find(UnkStringData* const& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    UnkFXGroup_ovb51* Get(UnkStringData* const& key) {
        UnkFXGroup_ovb51* p = Find(key);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXScene_ovb51 {
    u8 pad_0x0[0x118];
    bool field_0x118;
};

struct UnkFXSceneHolder_ovb51 {
    u8 pad_0x0[0x50];
    UnkFXScene_ovb51* scene;
};

struct UnkFXOwner_ovb51 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[0x2];
    void* field_0x1b8;
    u8 pad_0x1c0[0x68];
    UnkFXSceneHolder_ovb51* field_0x228;
};

// A string id whose release decrements and tests the count in two steps.
struct UnkFXNodeId_ovb51 {
    UnkStringData* data;

    ~UnkFXNodeId_ovb51() {
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

struct UnkFXIdNode_ovb51 {
    UnkFXNodeId_ovb51 id;
    u64 field_0x8;
    UnkFXIdNode_ovb51* next;
};

// A list of string-id nodes freed through its allocator.
struct UnkFXIdList_ovb51 {
    u32 count;
    UnkFXIdNode_ovb51* head;
    UnkFXIdNode_ovb51* tail;
    base::UnkAllocator* allocator;

    void Clear() {
        for (UnkFXIdNode_ovb51* n = head; n;) {
            UnkFXIdNode_ovb51* next = n->next;
            n->id.~UnkFXNodeId_ovb51();
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkFXComponent_ovb51 {
    u8 pad_0x0[0x30];
    UnkFXOwner_ovb51* owner;  // 0x30
    u8 pad_0x38[0x14];
    f32 scale;  // 0x4c
    UnkFXEffects_ovb51 effects;  // 0x50
    u8 pad_0x68[0x10];
    UnkFXGroups_ovb51 groups;  // 0x78
    u8 pad_0x90[0x3c];
    f32 timer_a;  // 0xcc
    f32 timer_b;  // 0xd0
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;
    UnkFXEffect_ovb51* model_a;  // 0xe0
    UnkFXEffect_ovb51* model_b;  // 0xe8
    u8 pad_0xf0[0x1c];
    s32 field_0x10c;
    bool field_0x110;
    u8 pad_0x111[0x2f];
    void* effect_a;  // 0x140
    u8 pad_0x148[0x10];
    void* effect_b;  // 0x158
    UnkFXScene_ovb51* attached_scene;  // 0x160, the scene the fifth manager model is attached to
    u8 pad_0x168[0x18];
    UnkFXIdList_ovb51 ids;  // 0x180
};
static_assert(__builtin_offsetof(UnkFXComponent_ovb51, ids) == 0x180, "UnkFXComponent_ovb51 layout");

static inline UnkFXEffect_ovb51* EffectNamed(UnkFXComponent_ovb51* self, u64 hash) {
    UnkFXEntry_ovb51* p = self->effects.Get(hash);
    return p ? p->effect : nullptr;
}

// The overlay-model manager, created on first use.
static inline UnkModelManager* Manager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

// ---- the timed overlay effects ----

static inline void StopA(UnkFXComponent_ovb51* self) {
    self->timer_a = 0.0f;
    if (self->effect_a) {
        Detach(Manager()->models[0], self->effect_a);
        self->effect_a = nullptr;
    }
}

static inline void StopB(UnkFXComponent_ovb51* self) {
    self->timer_b = 0.0f;
    if (self->effect_b)
        Detach(Manager()->models[1], self->effect_b);
}

void fn_710114eaa4(UnkFXComponent_ovb51* self, f32 dt) {
    if (self->timer_a > 0.0f) {
        self->timer_a -= dt;
        if (self->timer_a <= 0.0f)
            StopA(self);
    } else if (self->timer_a <= 0.0f) {
        StopA(self);
    }
}

void fn_710114eb28(UnkFXComponent_ovb51* self, f32 dt) {
    if (self->timer_b > 0.0f) {
        self->timer_b -= dt;
        if (self->timer_b <= 0.0f)
            StopB(self);
    } else if (self->timer_b <= 0.0f) {
        StopB(self);
    }
}

// Unless the component is blocked or its first model is already on, stops both overlay effects and
// attaches the two models (+0xe0, +0xe8) to the owner's scene, fading their parts in over `t`.
void fn_7101155124(UnkFXComponent_ovb51* self, f32 t) {
    if (!fn_71009f1988(self) || self->owner->field_0x1b5)
        return;
    if (!self->model_a || self->model_a->on)
        return;
    UnkFXSceneHolder_ovb51* holder = self->owner->field_0x228;
    if (!holder || !holder->scene || !holder->scene->field_0x118)
        return;
    StopA(self);
    StopB(self);
    fn_71001457fc(self->model_a, holder->scene, 0);
    fn_7100206fac(self->model_a->part, t, 1, 0, 0.0f, 1.0f);
    if (self->model_b) {
        fn_71001457fc(self->model_b, holder->scene, 0);
        fn_7100206fac(self->model_b->part, t, 1, 0, 0.0f, 1.0f);
    }
}

// When the flag at +0x110 turns false, forgets the index at +0x10c and detaches the owner's scene
// from the third manager model.
void fn_7101152008(UnkFXComponent_ovb51* self, bool on) {
    if (self->field_0x110 == on)
        return;
    self->field_0x110 = on;
    if (on || self->field_0x10c == -1)
        return;
    self->field_0x10c = -1;
    UnkFXSceneHolder_ovb51* holder = self->owner->field_0x228;
    if (holder && holder->scene)
        Detach(Manager()->models[2], holder->scene);
}

// ---- loops over every effect ----

struct UnkFXVec3_ovb51 {
    f32 x, y, z;
};

void fn_71011576dc(UnkFXComponent_ovb51* self, f32 scale) {
    self->scale = scale;
    for (UnkFXEntry_ovb51* e = self->effects.values; e != self->effects.End(); ++e) {
        if (e->effect) {
            UnkFXVec3_ovb51 v = {scale, scale, scale};
            VCall<0x20>(e->effect, &v);
        }
    }
}

void fn_7101156a28(UnkFXComponent_ovb51* self, bool on, bool restart) {
    for (UnkFXEntry_ovb51* e = self->effects.values; e != self->effects.End(); ++e) {
        VCall<0x40>(e->effect, on);
        if (restart)
            VCall<0x118>(e->effect);
    }
}

void fn_7101150180(UnkFXComponent_ovb51* self);

// Pauses (saving each effect's state) or resumes the effects that can be paused.
void fn_7101150294(UnkFXComponent_ovb51* self, bool on) {
    UnkFXEntry_ovb51* end = self->effects.End();
    for (UnkFXEntry_ovb51* e = self->effects.values; e != end; ++e) {
        if (!VCall<0xb8, bool>(e->effect))
            continue;
        if (on) {
            VCall<0x48>(e->effect, e->saved);
        } else {
            e->saved = VCall<0x60, bool>(e->effect);
            VCall<0x48>(e->effect, false);
        }
    }
    if (!on)
        fn_7101150180(self);
}

void fn_710114f780(UnkFXComponent_ovb51* self);
void fn_71009f19ac(UnkFXComponent_ovb51* self);

void fn_710114f6b8(UnkFXComponent_ovb51* self) {
    fn_710114f780(self);
    self->ids.Clear();
    fn_71009f19ac(self);
}

// Re-applies each effect's switch state (+0x18 of its entry) to the effects that have one.
void fn_710115388c(UnkFXComponent_ovb51* self) {
    fn_710114f780(self);
    for (UnkFXEntry_ovb51* e = self->effects.values; e != self->effects.End(); ++e) {
        if (!e->field_0x10 || !VCall<0x58, bool>(e->effect))
            continue;
        UnkFXEffect_ovb51* effect = e->effect;
        u8 on = e->on;
        if (effect->kind == 4 && effect && (on != 0) != (effect->on != 0)) {
            UnkFXScene_ovb51* scene = self->owner->field_0x228->scene;
            if (on && scene)
                fn_71001457fc(effect, scene, 1);
            else if (scene)
                Detach(effect, scene);
        }
        VCall<0x40>(effect, on != 0);
    }
}

// ---- effects by name ----

struct UnkFXColor_ovb51 {
    f32 r, g, b, a;
};

// A message naming an effect by its string id (+0x10), with the values the different handlers read.
struct UnkFXMessage_ovb51 {
    u8 pad_0x0[0x10];
    UnkStringData* id;
    union {
        struct {
            f32 x, y, z;  // 0x18
            u8 a, b;  // 0x24
        };
        struct {
            bool on;  // 0x18
            u8 pad_0x19;
            bool reset;  // 0x1a
        };
        struct {
            u8 flag;  // 0x18, tested as a byte
            u8 restart;  // 0x19
        };
        struct {
            f32 value;  // 0x18
        };
        struct {
            UnkStringData* param;  // 0x18
            u32 field_0x20;
            u32 field_0x24;
            UnkFXColor_ovb51 color;  // 0x28
            f32 field_0x38;
            f32 field_0x3c;
        };
    };
};
static_assert(__builtin_offsetof(UnkFXMessage_ovb51, field_0x3c) == 0x3c, "UnkFXMessage_ovb51 layout");

void fn_7101156058(UnkFXComponent_ovb51* self, u64 hash, f32 x, f32 y, bool a, bool b, f32 z) {
    if (UnkFXEffect_ovb51* effect = EffectNamed(self, hash))
        VCall<0x88>(effect, a, b, x, z, y);
}

void fn_7101155f94(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg) {
    if (msg->id == &g_unk_7101d47108->empty)
        return;
    fn_7101156058(self, HashOf(msg->id), msg->x, msg->y, msg->a != 0, msg->b != 0, msg->z);
}

void fn_710114ffb8(UnkFXComponent_ovb51* self, u64 hash, bool on, bool start) {
    UnkFXEffect_ovb51* effect = EffectNamed(self, hash);
    if (!effect || effect->kind != 6)
        return;
    UnkSwitchObject* object = reinterpret_cast<UnkSwitchObject*>(effect);
    fn_7100146e60(object, on);
    if (on && start)
        fn_7100208a40(object->field_0x28);
}

// The script binding of fn_710114ffb8: reads the effect's name and the two flags from `stream`.
bool fn_7101156ac4(UnkFXComponent_ovb51* self, void* stream) {
    char name[0x80];
    bool a;
    bool b;
    fn_710007cf30(stream, "sbb", name, &a, &b);
    u64 hash;
    fn_710006663c(&hash, name);
    u32 on = a;
    u32 start = b;
    UnkFXEffect_ovb51* effect = EffectNamed(self, hash);
    if (effect && effect->kind == 6) {
        UnkSwitchObject* object = reinterpret_cast<UnkSwitchObject*>(effect);
        fn_7100146e60(object, on != 0);
        if (on && start)
            fn_7100208a40(object->field_0x28);
    }
    return true;
}

// Drops `effect` from the component's list and hands it back to the manager.
struct UnkRinkaControl_ruc;

static inline void Release(UnkFXComponent_ovb51* self, UnkFXEffect_ovb51* effect) {
    fn_710115596c(reinterpret_cast<UnkRinkaControl_ruc*>(self), effect);
}

void fn_7101155d44(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg) {
    if (msg->id == &g_unk_7101d47108->empty)
        return;
    if (UnkFXEffect_ovb51* effect = EffectNamed(self, HashOf(msg->id)))
        Release(self, effect);
}

void fn_7101155878(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg) {
    if (msg->id == &g_unk_7101d47108->empty)
        return;
    UnkFXEffect_ovb51* effect = EffectNamed(self, HashOf(msg->id));
    if (!effect || effect->kind != 6)
        return;
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(effect), msg->on);
    if (msg->reset)
        Release(self, effect);
}

// Switches a named effect on or off (attaching a scene-bound one to the owner's scene when turning it
// on, detaching it when turning it off), and restarts it when asked.
void fn_7101155280(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg) {
    if (msg->id == &g_unk_7101d47108->empty)
        return;
    UnkFXEffect_ovb51* effect = EffectNamed(self, HashOf(msg->id));
    if (!effect)
        return;
    u8 on = msg->flag;
    u8 restart = msg->restart;
    if (effect->type == 4 && (on != 0) != (effect->on != 0)) {
        UnkFXScene_ovb51* scene = self->owner->field_0x228->scene;
        if (on && scene)
            fn_71001457fc(effect, scene, 1);
        else if (scene)
            Detach(effect, scene);
    }
    VCall<0x40>(effect, on != 0);
    if (restart)
        VCall<0x118>(effect);
}

// Turns off the effect named by the hash 0x444c7220d7fc64c4, detaching it from the owner's scene.
void fn_710115372c(UnkFXComponent_ovb51* self) {
    UnkFXEntry_ovb51* p = self->effects.Get(0x444c7220d7fc64c4);
    if (!p)
        return;
    UnkFXEffect_ovb51* effect = p->effect;
    if (effect->kind == 4 && effect && effect->on) {
        if (UnkFXScene_ovb51* scene = self->owner->field_0x228->scene)
            Detach(effect, scene);
    }
    VCall<0x40>(effect, false);
}

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

void fn_7101156238(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg) {
    if (msg->id == &g_unk_7101d47108->empty)
        return;
    UnkFXEffect_ovb51* effect = EffectNamed(self, HashOf(msg->id));
    if (!effect || effect->kind != 5)
        return;
    f32 r = msg->color.r, g = msg->color.g, b = msg->color.b, a = msg->color.a;
    f32 values[4] = {NonNegative(r), NonNegative(g), NonNegative(b), NonNegative(a)};
    if (msg->param == &g_unk_7101d47108->empty)
        return;
    u64 hash = HashOf(msg->param);
    u32 n = msg->field_0x20;
    fn_71001c3040(effect->model, hash, n, values, 1, msg->field_0x3c);
}

// Shows one part of a named effect's model, picked at random, and hides the others.
void fn_710115635c(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg) {
    if (msg->id == &g_unk_7101d47108->empty)
        return;
    UnkFXEffect_ovb51* effect = EffectNamed(self, HashOf(msg->id));
    if (!effect || effect->type != 5)
        return;
    s32 count = effect->model->parts->count;
    if (!count)
        return;
    s32 pick = (rand() & 0x7fff) % count;
    for (s32 i = 0; i < count; ++i) {
        UnkFXPartSet_ovb51* parts = effect->model->parts;
        u64 hash = (*parts->parts[i].name)->hash;
        fn_7100199420(parts, &hash, i == pick);
    }
}

// ---- groups by name ----

void fn_710115399c(UnkFXComponent_ovb51* self, void* item, f32 value);

void fn_7101153ba0(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg) {
    UnkFXGroup_ovb51* group = self->groups.Get(msg->id);
    if (!group || !group->count)
        return;
    f32 value = msg->value;
    UnkFXGroupItem_ovb51* it = group->items;
    UnkFXGroupItem_ovb51* end = it + group->count;
    do
        fn_710115399c(self, it->field_0x8, value);
    while (++it != end);
}

struct UnkFXState_ovb51 {
    u8 pad_0x0[0x70];
    s32 state;
};

// Attaches the fifth manager model to the owner's scene (at +0x160), after `delay` when one is given
// (+0xd4, +0xd8); unless forced, only while the owner's state object (+0x1b8) allows it. Resets the
// model when more than half of `t` has passed since the last time (+0xdc).
void fn_7101157088(UnkFXComponent_ovb51* self, f32 t, f32 delay, bool force) {
    if (delay > 0.0f) {
        self->field_0xd4 = t;
        self->field_0xd8 = delay;
        return;
    }
    self->field_0xd4 = 0.0f;
    self->field_0xd8 = 0.0f;
    if (self->owner->field_0x1b5)
        return;
    if (self->model_a && self->model_a->on)
        return;
    if (void* state = self->owner->field_0x1b8) {
        if (!force && (!fn_710024fbc8(state) || static_cast<UnkFXState_ovb51*>(state)->state == 2))
            return;
    }
    f32 last = self->field_0xdc;
    self->field_0xdc = t;
    UnkFXSceneHolder_ovb51* holder = self->owner->field_0x228;
    if (!holder || !holder->scene)
        return;
    if (self->attached_scene) {
        Detach(Manager()->models[4], self->attached_scene);
        // The detach may have cleared the scene.
        self->attached_scene = holder->scene;
        if (!self->attached_scene)
            return;
    } else {
        self->attached_scene = holder->scene;
    }
    if (t - last > t * 0.5f)
        fn_71001bc068(*reinterpret_cast<UnkResetOwner**>(reinterpret_cast<char*>(Manager()->models[4]) + 0x150));
    fn_71001457fc(Manager()->models[4], self->attached_scene, 0);
}
