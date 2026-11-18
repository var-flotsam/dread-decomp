// CRotationalPlatformComponent's unit (tools/tu_map.py unit 1498): the component's set-up once its entity is
// loaded. Fills the registry map at +0x98 under the entity's name, reads the "Used" flag from the current
// scenario's blackboard section, switches the player detector of the rotation's direction and the closed
// and opened collisions to match, tells the owner's object the initial or finished id, turns a used platform
// to its end angle, resolves the linked object once and registers in its list, and enables the collision.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CRotationalPlatformComponent.h"
#include "game/CScenario.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU372.h"
#include "game/UnkTU437.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::global::CStrId;

// The unit's data (its statics, merged into one block).
struct UnkRotationalPlatformTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId player_detector_right;  // 0x20
    CStrId player_detector_left;   // 0x28
    CStrId collision_opened;       // 0x30
    CStrId collision_closed;       // 0x38
    CStrId initial;                // 0x40
    CStrId finished;               // 0x48
    UnkReflType type;              // 0x50
};

struct UnkRefl_fn_71009d8744;
extern template bool base::reflection::refl_RegistryMapInit<UnkRefl_fn_71009d8744>(
    base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);

struct UnkRefl_fn_710031cd8c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(void*);

struct UnkWeakTarget;

struct UnkRotationalPlatformEntity_l9d83b8 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

// The object the link resolves to: the list it keeps its registrations in.
struct UnkRotationalPlatformLinked_l9d83b8 {
    u8 pad_0x0[0x198];
    u8 list[0x10];  // 0x198
};

struct UnkRotationalPlatformOwner_l9d83b8 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x20c];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[0x118];
    u8 dirty[4];  // 0x3c0: byte flags; [0] is set when the transform changes (a byte array: stores reload)
};
static_assert(__builtin_offsetof(UnkRotationalPlatformOwner_l9d83b8, collision) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkRotationalPlatformOwner_l9d83b8, dirty) == 0x3c0, "layout");

struct UnkRotationalPlatform_l9d83b8 {
    u8 pad_0x0[0x18];
    UnkRotationalPlatformEntity_l9d83b8* entity;  // 0x18
    u8 pad_0x20[0x10];
    UnkRotationalPlatformOwner_l9d83b8* owner;  // 0x30
    u8 pad_0x38[0x8];
    u8 collision_arg[0x10];  // 0x40
    u8 link[0x28];  // 0x50
    u32 link_state;  // 0x78
    u8 pad_0x7c[0x4];
    base::UnkRefHandle linked;  // 0x80
    bool field_0x88;
    bool field_0x89;
    u8 pad_0x8a[0x2];
    f32 field_0x8c;
    f32 angle;  // 0x90, in degrees
    u8 pad_0x94[0x4];
    base::reflection::UnkReflRegistryMap properties;  // 0x98
    u8 pad_0xc8[0x8];
    UnkLinkList* registration;  // 0xd0
    u8 pad_0xd8[0x10];
    s32 direction;  // 0xe8
};
static_assert(__builtin_offsetof(UnkRotationalPlatform_l9d83b8, properties) == 0x98, "layout");
static_assert(__builtin_offsetof(UnkRotationalPlatform_l9d83b8, registration) == 0xd0, "layout");
static_assert(__builtin_offsetof(UnkRotationalPlatform_l9d83b8, direction) == 0xe8, "layout");

// The weak handle the link refers to; the object it stands for is at +0x10.
struct UnkRotationalPlatformTarget_l9d83b8 : base::UnkRefCounted {
    void* object;
};

// The object the link stands for, or null while the link is unresolved.
static inline void* LinkedObject(const base::UnkRefHandle* handle) {
    auto* target = static_cast<UnkRotationalPlatformTarget_l9d83b8*>(handle->ptr);
    return target ? target->object : nullptr;
}

static const f32 kDegToRad = 0.017453292f;

static inline void SetSwitchedOn(UnkCollisionObject* object, bool on) {
    if (object)
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), on);
}

// Fills the component's registry map under `name` (passed by value, as the original's caller copies it).
static inline void InitProperties(base::reflection::UnkReflRegistryMap* map, CStrId name) {
    base::reflection::refl_RegistryMapInit<UnkRefl_fn_71009d8744>(map, name);
}

void fn_71009d83b8(UnkRotationalPlatform_l9d83b8* self) {
    UnkRotationalPlatformTU& tu = g_unk_7172740970;
    InitProperties(&self->properties, self->entity->name);

    UnkGameManager* manager = g_unk_7101d47310;
    bool used = false;
    if (manager->field_0x608) {
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 count = self->properties.count;
        if (count) {
            CStrId* keys = self->properties.keys;
            for (; index != count; ++index) {
                if (keys[index].data == g_unk_71727408f0.name.data)
                    break;
            }
        }
        base::reflection::UnkReflRegistryMap::Value* values = self->properties.values;
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
            fn_710006d11c(section, reinterpret_cast<const CStrId*>(&values[index]), &used);
    }

    UnkCollisionContainer* collision = self->owner->collision;
    if (self->direction == 1) {
        SetSwitchedOn(fn_71008640a4(collision, &tu.player_detector_left), !used);
        self->angle = 90.0f;
    } else {
        SetSwitchedOn(fn_71008640a4(collision, &tu.player_detector_right), !used);
    }
    SetSwitchedOn(fn_71008640a4(collision, &tu.collision_closed), !used);
    SetSwitchedOn(fn_71008640a4(collision, &tu.collision_opened), used);

    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner))) {
        fn_71009c0974(target, true);
        const CStrId& state = used ? tu.finished : tu.initial;
        fn_71009c0f1c(target, state.data->Key());
    }

    if (used) {
        UnkRotationalPlatformOwner_l9d83b8* owner = self->owner;
        owner->xform.SetAngle(UnkVec3f(0.0f, 0.0f, self->angle * kDegToRad));
        owner->dirty[0] = 1;
    }

    if (self->link_state == 1) {
        {
            base::UnkRefPtrMember reference = fn_7100353be8(self->link);
            fn_710031d208(&self->linked, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->link_state = 2;
    }
    if (LinkedObject(&self->linked)) {
        auto* linked = reinterpret_cast<UnkRotationalPlatformLinked_l9d83b8*>(
            base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(LinkedObject(&self->linked)));
        if (linked) {
            UnkLinkList** slot = &self->registration;
            if (*slot) {
                fn_71002aeb9c(*slot, slot);
                *slot = nullptr;
            }
            UnkLinkList* list = reinterpret_cast<UnkLinkList*>(linked->list);
            *slot = list;
            fn_71002af070(list, slot);
        }
    }

    if (UnkCollisionContainer* container = self->owner->collision)
        fn_71008656d8(container, 0x10000, 0x10, self->collision_arg);
}
