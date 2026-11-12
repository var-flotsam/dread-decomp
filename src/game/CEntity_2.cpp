// CEntity: forwarding a call to every component of the entity (the components array at +0xa0,
// its count at +0xb0) that is a CComponent.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/CSoundSystem.h"
#include "game/CActorComponent.h"
#include "game/CAudioComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU50.h"
#include "game/UnkTU604.h"
#include "game/UnkTU63.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkEntityComponent {
    virtual const base::reflection::CType* vf_0x0();
};
struct UnkEntityComponents {
    u8 pad_0x0[0xa0];
    UnkEntityComponent** data;
    u8 pad_0xa8[0x8];
    u32 count;
};

template <u32 Offset, class... A>
static inline __attribute__((always_inline)) void ForEachComponent(UnkEntityComponents* self, A... args) {
    for (UnkEntityComponent** it = self->data; it != self->data + self->count; ++it) {
        UnkEntityComponent* c = *it;
        if (c && fn_710009d50c(c->vf_0x0(), fn_71010c23d0())) {
            using Fn = void (*)(UnkEntityComponent*, A...);
            (*reinterpret_cast<Fn*>(*reinterpret_cast<char**>(c) + Offset))(c, args...);
        }
    }
}

void fn_7100d7afa8(UnkEntityComponents* self, void* a, void* b) { ForEachComponent<0x160>(self, a, b); }
void fn_7100d7b468(UnkEntityComponents* self, void* a) { ForEachComponent<0x130>(self, a); }

struct UnkOwner;
extern base::reflection::UnkTunable g_unk_7172a03d80;
void fn_71010d6878(f32 value);

void fn_7100d7b394(UnkEntityComponents* self, void* a) {
    UnkEntityComponents* player = reinterpret_cast<UnkEntityComponents*>(fn_7100defcd4(g_unk_7101d47310));
    char* tunable = reinterpret_cast<char*>(&g_unk_7172a03d80);
    f32* a0 = reinterpret_cast<f32*>(tunable + 0x24);
    f32* a1 = reinterpret_cast<f32*>(tunable + 0x28);
    asm("" : "+r"(a0));
    fn_71010d6878(*(player == self ? a0 : a1));
    ForEachComponent<0x128>(self, a);
}

// Forwarders to the entity's sound component (+0x2b8) and rumble component (+0x2c0): each returns the
// component's handle, or the empty handle when the entity has no such component.
struct UnkSoundRef;
struct UnkHandleTarget;
struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

struct UnkEntitySoundComponent;
struct CRumbleComponent;
struct UnkRumbleParams;
struct UnkEntityWithSound {
    u8 pad_0x0[0x2b8];
    UnkEntitySoundComponent* sound;
    CRumbleComponent* rumble;
};

UnkSoundHandle fn_710071c080(UnkEntitySoundComponent* self, void* a, void* b, void* c, void* d);
UnkSoundHandle fn_710071a8d0(UnkEntitySoundComponent* self, void* a, void* b, bool c, bool d, bool e);
UnkSoundHandle fn_710071ce1c(UnkEntitySoundComponent* self, void* a, void* b, void* c);
UnkHandle fn_71009db0f4(CRumbleComponent* self, void* a, void* b, bool c, bool d, bool e);
UnkHandle fn_71009db2e0(CRumbleComponent* self, void* a, bool b, bool c, bool d);
UnkHandle fn_71009db4b8(CRumbleComponent* self, void* a, bool b, bool c, bool d);
UnkHandle fn_71009db690(CRumbleComponent* self, void* a, void* b, void* c, void* d);
UnkHandle fn_71009db880(CRumbleComponent* self, void* a, void* b, void* c);
UnkHandle fn_71009dacbc(CRumbleComponent* self, const UnkRumbleParams* params, bool a, bool b);

UnkSoundHandle fn_7100d7c42c(UnkEntityWithSound* self, void* a, void* b, void* c, void* d) {
    if (self->sound) return fn_710071c080(self->sound, a, b, c, d);
    return g_unk_7108b6c460;
}
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e) {
    if (self->sound) return fn_710071a8d0(self->sound, a, b, c, d, e);
    return g_unk_7108b6c460;
}
UnkSoundHandle fn_7100d7c45c(UnkEntityWithSound* self, void* a, void* b, void* c) {
    if (self->sound) return fn_710071ce1c(self->sound, a, b, c);
    return g_unk_7108b6c460;
}
UnkSoundHandle fn_7100d7c590(UnkEntityWithSound* self, void* a) {
    if (self->sound) return fn_710071a338(self->sound, a);
    return g_unk_7108b6c460;
}
// fn_71009dacbc is given a third flag it does not read (src/game/CRumbleComponent.cpp).
UnkHandle fn_7100d7c650(UnkEntityWithSound* self, const UnkRumbleParams* params, bool a, bool b, bool c) {
    using Fn = UnkHandle (*)(CRumbleComponent*, const UnkRumbleParams*, bool, bool, bool);
    if (self->rumble) return reinterpret_cast<Fn>(&fn_71009dacbc)(self->rumble, params, a, b, c);
    return g_unk_7101d586c0;
}
UnkHandle fn_7100d7c68c(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e) {
    if (self->rumble) return fn_71009db0f4(self->rumble, a, b, c, d, e);
    return g_unk_7101d586c0;
}
UnkHandle fn_7100d7c6c8(UnkEntityWithSound* self, void* a, bool b, bool c, bool d) {
    if (self->rumble) return fn_71009db2e0(self->rumble, a, b, c, d);
    return g_unk_7101d586c0;
}
UnkHandle fn_7100d7c704(UnkEntityWithSound* self, void* a, bool b, bool c, bool d) {
    if (self->rumble) return fn_71009db4b8(self->rumble, a, b, c, d);
    return g_unk_7101d586c0;
}
UnkHandle fn_7100d7c740(UnkEntityWithSound* self, void* a, void* b, void* c, void* d) {
    if (self->rumble) return fn_71009db690(self->rumble, a, b, c, d);
    return g_unk_7101d586c0;
}
UnkHandle fn_7100d7c770(UnkEntityWithSound* self, void* a, void* b, void* c) {
    if (self->rumble) return fn_71009db880(self->rumble, a, b, c);
    return g_unk_7101d586c0;
}

// The same through the sound component, falling back to the static sound players by kind.
UnkSoundHandle fn_710071bae8(UnkEntitySoundComponent* self, const u32* params, bool a, bool b, bool c);
struct UnkStateSink;
UnkSoundHandle fn_7100fca8cc(void* player, const u32* params);
UnkSoundHandle fn_710101f154(void* player, const u32* params);

UnkSoundHandle fn_710071b550(UnkEntitySoundComponent* self, const u32* params, bool a, bool b, bool c);

UnkSoundHandle fn_7100d7c2f0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c) {
    if (self->sound) return fn_710071b550(self->sound, params, a, b, c);
    if (reinterpret_cast<const u8*>(params)[0x6c]) return g_unk_7108b6c460;
    if (*params == 0) return fn_710101f154(g_unk_71727b85d0, params);
    if (*params == 4) return fn_7100fca8cc(g_unk_717275c380, params);
    return g_unk_7108b6c460;
}

UnkSoundHandle fn_7100d7c3b0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c) {
    if (self->sound) return fn_710071bae8(self->sound, params, a, b, c);
    if (*params == 0) return fn_710101f154(g_unk_71727b85d0, params);
    if (*params == 4) return fn_7100fca8cc(g_unk_717275c380, params);
    return g_unk_7108b6c460;
}

// Entities with neither link at +0x208 nor +0x210 do nothing; the others without a sound component
// pass the call on to the player.
struct UnkPlayer;

struct UnkEntityWithLinks {
    u8 pad_0x0[0x208];
    void* field_0x208;
    void* field_0x210;
    u8 pad_0x218[0x2b8 - 0x218];
    UnkEntitySoundComponent* sound;
};

void fn_7100d7c48c(UnkEntityWithLinks* self, void* a, f32 x, f32 y) {
    UnkEntityWithLinks* e = self;
    while (!e->sound) {
        if (!e->field_0x208 && !e->field_0x210) return;
        e = reinterpret_cast<UnkEntityWithLinks*>(fn_7100dec788(g_unk_7101d47310));
        if (!e) return;
    }
    fn_710071a524(e->sound, a, x, y);
}

// The id at +0x30 of the object two links down from +0x398, or the empty id.
struct UnkEntityIdSource {
    u8 pad_0x0[0x30];
    base::global::CStrId* field_0x30;
};
struct UnkEntityIdLink {
    u8 pad_0x0[0x10];
    UnkEntityIdSource* field_0x10;
};
struct UnkEntityWithIdLink {
    u8 pad_0x0[0x398];
    UnkEntityIdLink* field_0x398;
};

const base::global::CStrId& fn_7100d7ba84(const UnkEntityWithIdLink* self) {
    UnkEntityIdSource* src = self->field_0x398 ? self->field_0x398->field_0x10 : nullptr;
    if (!src) return g_unk_7101d470e0;
    base::global::CStrId* p = src->field_0x30;
    base::global::CStrId* name = reinterpret_cast<base::global::CStrId*>(reinterpret_cast<char*>(p) + 0x30);
    const base::global::CStrId* empty = &g_unk_7101d470e0;
    asm("" : "+r"(empty), "+r"(name), "+r"(p));
    return p ? *name : *empty;
}

struct UnkEntityState {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
    bool field_0x1b5;
    u8 pad_0x1b6[0x3ac - 0x1b6];
    f32 field_0x3ac;
};

bool fn_7100d7b1f8(const UnkEntityState* self) {
    if (self->field_0x1b5) return false;
    return (self->field_0x1b4 && (!self->field_0x1b0 || self->field_0x1a8)) || self->field_0x3ac >= 0.0f;
}

// The entity's box: its position (the translation of the transform at +0x38), raised by +0x394,
// widened by the half of the extents at +0x388.
struct UnkEntityBox {
    base::math::CVector3D min;
    base::math::CVector3D max;
};
struct UnkEntityBounds {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x388 - 0x68];
    base::math::CVector3D extents;
    f32 field_0x394;
};

static inline UnkEntityBox EntityBox(const UnkEntityBounds* self) {
    base::math::CVector3D c = {self->x, self->y, self->z};
    c.y += self->field_0x394;
    base::math::CVector3D h = {self->extents.x * 0.5f, self->extents.y * 0.5f, self->extents.z * 0.5f};
    base::math::CVector3D lo = {c.x - h.x, c.y - h.y, c.z - h.z};
    base::math::CVector3D hi = {c.x + h.x, c.y + h.y, c.z + h.z};
    return {lo, hi};
}

UnkEntityBox fn_7100d7bc70(const UnkEntityBounds* self) { return EntityBox(self); }

// The box of the entity's character shape (a 2D one, z = 0) when its movement at +0x1f0 is a
// CCharacterMovement with a shape; else the sphere of the collider behind +0x228; else the box above.
struct UnkFlagged;
struct UnkShape;
struct UnkEntityShape {
    virtual void vf_0x0();
    u8 pad_0x8[0x38];
    f32 min_x, min_y;  // 0x40
    f32 max_x, max_y;  // 0x48
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    void vf_0x98() { (*reinterpret_cast<void (**)(UnkEntityShape*)>(*reinterpret_cast<char**>(this) + 0x98))(this); }
};
struct UnkEntitySphere {
    u8 pad_0x0[0xa8];
    f32 radius;
    u8 pad_0xac[0xc];
    base::math::CVector3D center;  // 0xb8
};
struct UnkEntityCollider {
    u8 pad_0x0[0x50];
    UnkEntitySphere* sphere;
};

UnkEntityBox fn_7100d7bcc8(const UnkEntityBounds* self) {
    const char* raw = reinterpret_cast<const char*>(self);
    UnkEntityComponent* movement = *reinterpret_cast<UnkEntityComponent* const*>(raw + 0x1f0);
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_7100975b3c())) {
        if (auto* shape = reinterpret_cast<UnkEntityShape*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(movement)))) {
            if (shape->dirty) shape->vf_0x98();
            f32 x0 = shape->min_x, y0 = shape->min_y, x1 = shape->max_x, y1 = shape->max_y;
            UnkEntityBox r = {{x0, y0, 0.0f}, {x1, y1, 0.0f}};
            asm volatile("" ::: "memory");
            return r;
        }
    }
    UnkEntityCollider* collider = *reinterpret_cast<UnkEntityCollider* const*>(raw + 0x228);
    if (collider && collider->sphere) {
        const UnkEntitySphere* s = collider->sphere;
        f32 r = s->radius;
        base::math::CVector3D c = s->center;
        base::math::CVector3D lo = {c.x - r, c.y - r, c.z - r};
        base::math::CVector3D hi = {r + c.x, r + c.y, r + c.z};
        return {lo, hi};
    }
    return EntityBox(self);
}

// The same forwarding with the flag at +0x3d2 cleared around each call, after the object at +0x270
// is told first.
struct UnkEntityGuarded {
    u8 pad_0x0[0xa0];
    UnkEntityComponent** data;
    u8 pad_0xa8[0x8];
    u32 count;
    u8 pad_0xb4[0x270 - 0xb4];
    void* field_0x270;
    u8 pad_0x278[0x3d2 - 0x278];
    bool field_0x3d2;
};
void fn_7101142340(void* object);

void fn_7100d7b238(UnkEntityGuarded* self, void* a) {
    if (self->field_0x270) {
        self->field_0x3d2 = false;
        fn_7101142340(self->field_0x270);
        self->field_0x3d2 = true;
    }
    for (UnkEntityComponent** it = self->data; it != self->data + self->count; ++it) {
        UnkEntityComponent* c = *it;
        if (c && fn_710009d50c(c->vf_0x0(), fn_71010c23d0())) {
            self->field_0x3d2 = false;
            using Fn = void (*)(UnkEntityComponent*, void*);
            (*reinterpret_cast<Fn*>(*reinterpret_cast<char**>(c) + 0x140))(c, a);
            self->field_0x3d2 = true;
        }
    }
}

// The entity's position and angles: those of the object at +0x238 while fn_710024fbc8 holds for
// it, else those of the entity's transform at +0x38.
struct UnkEntityPosSource {
    u8 pad_0x0[0x40];
    UnkVec3f pos;
    UnkVec3f angle;
};
struct UnkEntityPos {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x238 - 0x94];
    UnkEntityPosSource* field_0x238;
};

UnkVec3f fn_7100d7b7dc(const UnkEntityPos* self) {
    if (self->field_0x238 && fn_710024fbc8(self->field_0x238)) return self->field_0x238->pos;
    return self->xform.GetTranslation();
}

UnkVec3f fn_7100d7b844(UnkEntityPos* self) {
    if (self->field_0x238 && fn_710024fbc8(self->field_0x238)) return self->field_0x238->angle;
    return self->xform.GetAngle();
}

// Moves the components named in `names` from `from` to `to` (each found by name in `from`, kept only
// when it is a CComponent), then lets both rebuild.
struct UnkActor_710023259c;
void fn_710023259c(UnkActor_710023259c* self);
void fn_710023231c(UnkActor_710023259c* self);
bool fn_7100233230(UnkActor_710023259c* self, base::reflection::UnkReflObject* component);
void fn_7100232320(UnkActor_710023259c* self, base::reflection::UnkReflObject* component);
void fn_71002323e0(UnkActor_710023259c* self, bool flag);
struct UnkStrIdArray {
    base::global::CStrId* data;
    u32 count;
};

void fn_7100d7822c(UnkActor_710023259c* from, UnkActor_710023259c* to, const UnkStrIdArray& names) {
    fn_710023231c(from);
    fn_710023231c(to);
    for (const base::global::CStrId *it = names.data, *end = it + names.count; it != end; ++it) {
        base::global::CStrId name = *it;
        base::reflection::UnkReflObject* o = fn_710023341c(from, name);
        if (o) o = fn_710009d50c(o->vf_0x0(), fn_71010c23d0()) ? o : nullptr;
        fn_7100233230(from, o);
        fn_7100232320(to, o);
    }
    fn_71002323e0(to, true);
    fn_71002323e0(from, true);
    fn_710023259c(to);
}

// Stops the entity's sounds and releases its list of loaded paths (+0x358, freed through the
// allocator at +0x368) after handing each to the sound manager.
struct UnkPathNode {
    base::global::CFilePathStrId path;
    UnkPathNode* next;
};
struct UnkPathAllocator {
    virtual void vf_0x0();
    void Free(void* p) { (*reinterpret_cast<void (**)(UnkPathAllocator*, void*)>(*reinterpret_cast<char**>(this) + 0x48))(this, p); }
};
struct UnkEntityPaths {
    u8 pad_0x0[0x2b8];
    UnkEntitySoundComponent* sound;
    u8 pad_0x2c0[0x350 - 0x2c0];
    u32 path_count;
    u8 pad_0x354[4];
    UnkPathNode* head;
    UnkPathNode* tail;
    UnkPathAllocator* allocator;
};
struct UnkSoundMgr;
void fn_7100232bf0(UnkEntityPaths* self);

void fn_7100d78108(UnkEntityPaths* self) {
    if (self->sound) fn_7100719064(self->sound, true, true, true, true);
    for (UnkPathNode* n = self->head; n; n = n->next)
        fn_7100108094(g_unk_7101d4d820, n);
    for (UnkPathNode* n = self->head; n;) {
        UnkPathNode* next = n->next;
        base::global::UnkStringData*& data = n->path.data;
        if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        self->allocator->Free(n);
        n = next;
    }
    self->path_count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    fn_7100232bf0(self);
}

// The position of the entity's collision shape (the translation of its matrix) unless the collider
// at +0x218 is of kind 3 or has no shape; else the transform's.
struct UnkEntityCollided {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x218 - 0x94];
    void* collider;
};

UnkVec3f fn_7100d7c7a0(const UnkEntityCollided* self) {
    if (!self->collider || fn_71010cc3a4(self->collider) != 3) {
        if (UnkShape* shape = fn_7100976588(static_cast<UnkFlagged*>(self->collider))) {
            UnkMatrix44 m = fn_7100d36a2c(shape);
            return UnkVec3f(m.m[3], m.m[7], m.m[11]);
        }
    }
    return self->xform.GetTranslation();
}

// The entity's destructor body: releases the counted object at +0x398, empties both node lists
// (+0x348 of paths, +0x318), then runs the base destructor.
extern void* g_unk_7101aa8e80[];
struct UnkCounted {
    virtual void vf_0x0();
    u32 count;
    void Destroy() { (*reinterpret_cast<void (**)(UnkCounted*)>(*reinterpret_cast<char**>(this) + 0x10))(this); }
};
struct UnkNode {
    void* value;
    UnkNode* next;
};
struct UnkNodeList {
    void** vtable;
    u32 count;
    UnkNode* head;
    UnkNode* tail;
    UnkPathAllocator* allocator;
};
struct UnkPathList {
    void** vtable;
    u32 count;
    UnkPathNode* head;
    UnkPathNode* tail;
    UnkPathAllocator* allocator;
};
struct UnkEntityDtor {
    void** vtable;
    u8 pad_0x8[0x8];
    void** vtable_0x10;
    u8 pad_0x18[0x318 - 0x18];
    UnkNodeList nodes;  // 0x318
    u8 pad_0x340[0x8];
    UnkPathList paths;  // 0x348
    u8 pad_0x370[0x398 - 0x370];
    UnkCounted* counted;
};
void fn_7100231e80(UnkEntityDtor* self);

void fn_7100d7cecc(UnkEntityDtor* self) {
    self->vtable = g_unk_7101aa8e80 + 2;
    self->vtable_0x10 = g_unk_7101aa8e80 + 45;
    if (UnkCounted* c = self->counted) {
        if (c->count-- == 1) c->Destroy();
    }
    self->paths.vtable = g_unk_7101a1b030 + 2;
    for (UnkPathNode* n = self->paths.head; n;) {
        UnkPathNode* next = n->next;
        base::global::UnkStringData*& data = n->path.data;
        if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        self->paths.allocator->Free(n);
        n = next;
    }
    self->paths.count = 0;
    self->paths.head = nullptr;
    self->paths.tail = nullptr;
    self->nodes.vtable = g_unk_7101a32680 + 2;
    for (UnkNode* n = self->nodes.head; n;) {
        UnkNode* next = n->next;
        self->nodes.allocator->Free(n);
        n = next;
    }
    self->nodes.count = 0;
    self->nodes.head = nullptr;
    self->nodes.tail = nullptr;
    fn_7100231e80(self);
}
