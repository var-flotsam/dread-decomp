// CProjectileMovement unit (tools/tu_map.py unit 1623): goes through the projectile's contacts and
// hands each one to the hit handler its object calls for (a collision component, a breakable tile
// group, or a scenario or liquid pool), keeping the last contact handled. With `facing` set only
// contacts flagged for it that the projectile moves into count. True as soon as a handler reports
// the projectile done. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CLifeComponent.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

struct UnkProjectileMovement5_b46a3;
bool fn_7100ac3ff4(UnkProjectileMovement5_b46a3* self, UnkWeakHandle** hit);
s32 fn_710113912c(void* object);

struct UnkContactActor {
    u8 pad_0x0[0x1b8];
    void* life;  // 0x1b8
    u8 pad_0x1c0[0x70];
    void* field_0x230;
};

// What a contact's object resolves to: a reflected component.
struct UnkContactComponent {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x28];
    UnkContactActor* actor;  // 0x30

    // vtable slot 0x210 (a liquid pool's): whether it takes hits.
    bool vf_0x210() {
        return reinterpret_cast<bool (*)(UnkContactComponent*)>((*reinterpret_cast<void***>(this))[0x210 / 8])(this);
    }
};

struct UnkContactShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();  // the shape's kind
};

struct UnkContactEntity {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();  // flags
    u8 pad_0x8[0x8];
    UnkContactComponent* component;  // 0x10
    u8 pad_0x18[0x140];
    UnkContactShape* shape;  // 0x158
};

struct UnkContactObject : base::UnkRefCounted {
    UnkContactEntity* entity;  // 0x10
};

struct UnkContactTarget : base::UnkRefCounted {
    void* target;  // 0x10
};

// A counted reference: copies take a reference, assignment is out of line.
struct UnkContactRef : base::UnkRefHandle {
    UnkContactRef(const UnkContactRef& other) {
        ptr = nullptr;
        Set(other.ptr);
    }
    UnkContactRef& operator=(const UnkContactRef& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    ~UnkContactRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

// A contact (0x48 bytes): the object hit and what hit it, where, and the surface normal.
struct UnkContact {
    UnkContactRef object;  // 0x0
    UnkContactRef source;  // 0x8
    s32 field_0x10;
    UnkVec3f position;  // 0x14
    UnkVec3f field_0x20;
    UnkVec3f normal;  // 0x2c
    u32 field_0x38;
    u32 flags;  // 0x3c
    u32 field_0x40;

    UnkContact(const UnkContact& other)
        : object(other.object), source(other.source), field_0x10(other.field_0x10), position(other.position),
          field_0x20(other.field_0x20), normal(other.normal), field_0x38(other.field_0x38), flags(other.flags),
          field_0x40(other.field_0x40) {}
    UnkContact& operator=(const UnkContact& other) {
        if (this != &other) {
            object = other.object;
            source = other.source;
            field_0x10 = other.field_0x10;
            position = other.position;
            field_0x20 = other.field_0x20;
            normal = other.normal;
            field_0x38 = other.field_0x38;
            flags = other.flags;
            field_0x40 = other.field_0x40;
        }
        return *this;
    }

    UnkContactEntity* Entity() const { return static_cast<UnkContactObject*>(object.ptr)->entity; }
    void* SourceTarget() const { return source.ptr ? static_cast<UnkContactTarget*>(source.ptr)->target : nullptr; }
};

static_assert(sizeof(UnkContact) == 0x48, "UnkContact layout");

struct __attribute__((packed)) UnkContactList {
    u8 pad_0x0[0x8];
    u32 count;
    UnkContact items[1];  // 0xc

    UnkContact* begin() { return items; }
    UnkContact* end() { return &items[count]; }
};

struct UnkContactListener {
    virtual void vf_0x0();
    void vf_0x158(u32 id, const UnkVec3f* position, u32 value, void* extra) {
        reinterpret_cast<void (*)(UnkContactListener*, u32, const UnkVec3f*, u32, void*)>(
            (*reinterpret_cast<void***>(this))[0x158 / 8])(this, id, position, value, extra);
    }
};

struct UnkContactListenerSlot {
    u8 pad_0x0[0x10];
    UnkContactListener* listener;  // 0x10
};

struct UnkProjectileOwner {
    u8 pad_0x0[0x64];
    f32 field_0x64;  // translation z
    u8 pad_0x68[0x200];
    void* field_0x268;
};

struct UnkProjectileSettings {
    u8 pad_0x0[0xe0];
    u32 hit_mask;  // 0xe0
    u8 pad_0xe4[0x44];
    bool check_life;  // 0x128
};

struct UnkProjectileTargetInfo {
    u8 pad_0x0[0x1f0];
    void* field_0x1f0;
};

struct UnkProjectileTarget {
    u8 pad_0x0[0x10];
    UnkProjectileTargetInfo* info;  // 0x10
};

// What the hit handlers are passed along with the contact, by value.
struct UnkHitArgs {
    void* field_0x0;
    void* field_0x8;
};

struct UnkProjectileMovement_lac5b8c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkProjectileOwner* owner;  // 0x30
    u8 pad_0x38[0x1f8];
    UnkProjectileTarget* field_0x230;
    u8 pad_0x238[0x1c4];
    u32 field_0x3fc;
    u8 pad_0x400[0xd0];
    UnkContactListenerSlot* listener;  // 0x4d0
    u8 pad_0x4d8[0x50];
    UnkContact last_contact;  // 0x528
    u8 pad_0x570[0x58];
    UnkProjectileSettings* settings;  // 0x5c8

    // vtable slots 0x320, 0x328 and 0x330: the hit handlers.
    bool vf_0x320(UnkContactComponent* tiles, UnkHitArgs args, UnkContact* contact, void* source) {
        return reinterpret_cast<bool (*)(UnkProjectileMovement_lac5b8c*, UnkContactComponent*, UnkHitArgs, UnkContact*,
                                         void*)>(vtable[0x320 / 8])(this, tiles, args, contact, source);
    }
    bool vf_0x328(UnkHitArgs args, UnkContact* contact, void* source) {
        return reinterpret_cast<bool (*)(UnkProjectileMovement_lac5b8c*, UnkHitArgs, UnkContact*, void*)>(
            vtable[0x328 / 8])(this, args, contact, source);
    }
    bool vf_0x330(UnkContactActor* actor, UnkHitArgs args, UnkContact* contact, void* source) {
        return reinterpret_cast<bool (*)(UnkProjectileMovement_lac5b8c*, UnkContactActor*, UnkHitArgs, UnkContact*,
                                         void*)>(vtable[0x330 / 8])(this, actor, args, contact, source);
    }

    void Notify(UnkContact* contact, u32 value) {
        if (listener && listener->listener)
            listener->listener->vf_0x158(field_0x3fc, &contact->position, value, nullptr);
    }
};
static_assert(__builtin_offsetof(UnkProjectileMovement_lac5b8c, last_contact) == 0x528, "layout");
static_assert(__builtin_offsetof(UnkProjectileMovement_lac5b8c, settings) == 0x5c8, "layout");

template <const CClass* (*GetClass)()>
static inline bool IsA(UnkContactComponent* component) {
    return fn_710009d50c(component->vf_0x0(), GetClass());
}

template <const CClass* (*GetClass)()>
static inline UnkContactComponent* CastTo(UnkContactComponent* component) {
    return IsA<GetClass>(component) ? component : nullptr;
}

static inline bool IsNearZero(f32 v, f32 epsilon) {
    return v - epsilon <= 0.0f && v + epsilon >= 0.0f;
}

static inline f32 Dot(const UnkVec3f& a, const UnkVec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool fn_7100ac5b8c(UnkProjectileMovement_lac5b8c* self, UnkContactList* contacts, bool facing, const UnkVec3f* dir,
                   UnkHitArgs args) {
    bool done = false;
    for (UnkContact* it = contacts->begin(); it != contacts->end(); ++it) {
        UnkContact contact = *it;
        UnkContactComponent* component = nullptr;
        bool hit_component = false;
        bool solid = false;
        if (UnkContactEntity* entity = it->Entity()) {
            component = entity->component;
            if (component) {
                solid = IsA<fn_710085fc44>(component) && component->actor->field_0x230 != nullptr;
                hit_component = true;
            }
        }
        if (it->Entity()->shape->vf_0x18() == 0x10)
            contact.position.z = self->owner->field_0x64;

        if (facing) {
            if (!(it->flags & 0x10))
                continue;
            if (!(IsNearZero(dir->x, 1e-5f) && IsNearZero(dir->y, 1e-5f) && IsNearZero(dir->z, 1e-5f)) && !solid) {
                f32 dot = Dot(*dir, it->normal);
                if (dot > 0.0f || IsNearZero(dot, 0.001f))
                    continue;
            }
        }

        if (hit_component) {
            self->last_contact = *it;
            if (UnkContactComponent* collision = CastTo<fn_710085fc44>(component)) {
                UnkProjectileSettings* settings = self->settings;
                if (!settings->check_life || !collision->actor->life || !fn_7101161088(collision->actor->life)) {
                    u32 mask = self->settings->hit_mask;
                    if (void* kind_source = self->owner->field_0x268) {
                        switch (fn_710113912c(kind_source)) {
                        case 1:
                            mask |= 0x81480;
                            break;
                        case 2:
                            mask |= 0x10a0;
                            break;
                        case 4:
                            mask |= 0x80420;
                            break;
                        }
                    }
                    if (mask & 0x4000000) {
                        if (!(it->Entity()->vf_0x58() & 0x800000) && !(it->Entity()->vf_0x58() & 0x40) &&
                            self->vf_0x330(collision->actor, args, &contact, it->SourceTarget()))
                            return true;
                    } else if (fn_7100ac3ff4(reinterpret_cast<UnkProjectileMovement5_b46a3*>(self),
                                             reinterpret_cast<UnkWeakHandle**>(&it->object)) ||
                               self->vf_0x330(collision->actor, args, &contact, it->SourceTarget())) {
                        return true;
                    }
                }
            } else if (UnkContactComponent* tiles = CastTo<fn_7100ca0484>(component)) {
                done = self->vf_0x320(tiles, args, &contact, it->SourceTarget());
                self->Notify(&contact, 1);
            } else {
                bool scenario = IsA<fn_710119ea84>(component);
                UnkContactComponent* pool = CastTo<fn_71008fe62c>(component);
                bool liquid = pool ? pool->vf_0x210() : false;
                if (scenario || liquid) {
                    UnkProjectileTarget* target = self->field_0x230;
                    if (!target || !target->info || !target->info->field_0x1f0 || Dot(*dir, contact.normal) <= 0.9f) {
                        done = self->vf_0x328(args, &contact, it->SourceTarget());
                        self->Notify(&contact, (contact.flags & 0x40) ? contact.field_0x40 : 0);
                    }
                }
            }
        }
        if (done)
            return true;
    }
    return done;
}
