// Unit 2702 (hint CAIComponent): the AI starts playing: joins the team manager, runs the owner's
// "<name>.BeginPlay" script, resets its state, hooks the owner's +0x2c8 object, sets up the sight
// collider and registers with the sub-area manager. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU55.h"
#include "game/UnkTU57.h"
#include "game/UnkTU883.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/lua_lib.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector3D;

void fn_71010fa6d4(char* self);
void fn_710088c874(void* set, void** slot);
void fn_710088d004(void* set, void** slot);
void fn_7101115fc4(void* self, void* arg);
void fn_7101115fd0(void* self, void* arg);
void fn_7101115fdc(void* self, void* arg);
UnkCollisionObject* fn_7100863814(UnkCollisionContainer* self, const CStrId* id, const u32* extra);

extern CStrId g_unk_7172a06340 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a06348 __attribute__((visibility("hidden")));

using UnkCallback_l0fa290 = void (*)(void* self, void* arg);

// A shape as the sight collider holds it: slot 0x18 gives its kind (2: a circle).
struct UnkShape_l0fa290 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};

struct UnkCollider_l0fa290 {
    u8 pad_0x0[0x158];
    UnkShape_l0fa290* shape;  // 0x158
};

// The owner's collision container, through its vtable.
struct UnkCollision_l0fa290 {
    void** vtable;

    UnkCollider_l0fa290* vf_0x168(const CStrId* name, const CStrId* a, const CStrId* b, const CStrId* c, u32 flags,
                                  bool f1, bool f2, void* p1, u32 s1, void* p2, u32 s2, void* p3, u32 s3, bool f3,
                                  bool f4, bool f5) {
        using Fn = UnkCollider_l0fa290* (*)(UnkCollision_l0fa290*, const CStrId*, const CStrId*, const CStrId*,
                                            const CStrId*, u32, bool, bool, void*, u32, void*, u32, void*, u32, bool,
                                            bool, bool);
        return reinterpret_cast<Fn>(vtable[0x168 / 8])(this, name, a, b, c, flags, f1, f2, p1, s1, p2, s2, p3, s3,
                                                       f3, f4, f5);
    }
};

struct UnkNamedObject_l0fa290 {
    u8 pad_0x0[0x98];
    CRntString name;  // 0x98
};

// The owner's +0x2c8 object: a callback set and two callbacks.
struct UnkHooks_l0fa290 {
    u8 pad_0x0[0x2e0];
    u8 set[0x98];  // 0x2e0
    void* first_object;  // 0x378
    UnkCallback_l0fa290 first;
    void* second_object;  // 0x388
    UnkCallback_l0fa290 second;
};
static_assert(__builtin_offsetof(UnkHooks_l0fa290, second) == 0x390, "layout");

struct UnkOwner_l0fa290 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x178];
    UnkNamedObject_l0fa290* named;  // 0x1e0
    u8 pad_0x1e8[0xb8];
    UnkCollision_l0fa290* collision;  // 0x2a0
    u8 pad_0x2a8[0x20];
    UnkHooks_l0fa290* hooks;  // 0x2c8
};
static_assert(__builtin_offsetof(UnkOwner_l0fa290, hooks) == 0x2c8, "layout");

struct UnkTunable_l0fa290 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    u8 pad_0xc5[0x2f];
    f32 sight_radius;  // 0xf4
    u8 pad_0xf8[0x272];
    bool field_0x36a;
};
static_assert(__builtin_offsetof(UnkTunable_l0fa290, field_0x36a) == 0x36a, "layout");

struct UnkSubAreaManager_l0fa290 {
    u8 pad_0x0[0xa60];
    u8 callbacks[0x88];  // 0xa60
};

struct UnkTarget_l0fa290 {
    s32 id;
    u32 field_0x4;
    u64 field_0x8;
    bool field_0x10;
    bool field_0x11;
    bool field_0x12;
    u8 pad_0x13;
    CVector3D field_0x14;
    u16 field_0x20;
};
static_assert(__builtin_offsetof(UnkTarget_l0fa290, field_0x20) == 0x20, "layout");

static inline void Copy(CVector3D& to, const CVector3D& from) {
    to.x = from.x;
    to.y = from.y;
    to.z = from.z;
}

struct UnkPos_l0fa290 {
    f32 x, y, z;
};

struct UnkAI_l0fa290 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l0fa290* owner;  // 0x30
    u8 pad_0x38[0x5];
    bool field_0x3d;
    bool field_0x3e;
    u8 pad_0x3f[0x11a1];
    bool field_0x11e0;
    u8 pad_0x11e1[0x3];
    u32 field_0x11e4;
    u32 field_0x11e8;
    bool field_0x11ec;
    bool field_0x11ed;
    u8 pad_0x11ee[0x2];
    f32 field_0x11f0;
    u32 field_0x11f4;
    u32 field_0x11f8;
    u32 field_0x11fc;
    u8 pad_0x1200[0x4];
    CVector3D field_0x1204;
    CVector3D field_0x1210;
    bool field_0x121c;
    bool field_0x121d;
    bool field_0x121e;
    bool field_0x121f;
    bool field_0x1220;
    u8 pad_0x1221[0x1b1];
    bool field_0x13d2;
    u8 pad_0x13d3[0x129];
    f32 field_0x14fc;
    u8 pad_0x1500[0xe8];
    void* area_slot;  // 0x15e8
    u8 pad_0x15f0[0x10];
    u16 field_0x1600;
    u8 field_0x1602;
    u8 pad_0x1603;
    u8 field_0x1604[0xc];
    UnkTunable_l0fa290* tunable;  // 0x1610
    u8 pad_0x1618[0x186];
    bool field_0x179e;
    u8 pad_0x179f[0x209];
    void* hook_slot;  // 0x19a8
    void* hook_object;  // 0x19b0
    UnkCallback_l0fa290 hook;  // 0x19b8
    u8 pad_0x19c0[0x12cac];
    CVector3D field_0x1466c;
    UnkTarget_l0fa290 target;  // 0x14678

    bool vf_0x198() { return reinterpret_cast<bool (*)(UnkAI_l0fa290*)>(vtable[0x198 / 8])(this); }
    void vf_0x718() { reinterpret_cast<void (*)(UnkAI_l0fa290*)>(vtable[0x718 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkAI_l0fa290, field_0x11e0) == 0x11e0, "layout");
static_assert(__builtin_offsetof(UnkAI_l0fa290, field_0x13d2) == 0x13d2, "layout");
static_assert(__builtin_offsetof(UnkAI_l0fa290, field_0x14fc) == 0x14fc, "layout");
static_assert(__builtin_offsetof(UnkAI_l0fa290, tunable) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkAI_l0fa290, field_0x179e) == 0x179e, "layout");
static_assert(__builtin_offsetof(UnkAI_l0fa290, hook_slot) == 0x19a8, "layout");
static_assert(__builtin_offsetof(UnkAI_l0fa290, field_0x1466c) == 0x1466c, "layout");
static_assert(__builtin_offsetof(UnkAI_l0fa290, target) == 0x14678, "layout");

static inline char* TeamManager() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return instance;
}

static inline UnkSubAreaManager_l0fa290* SubAreaManager() {
    UnkManager* manager = g_unk_7101d47360;
    if (!manager) {
        manager = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return reinterpret_cast<UnkSubAreaManager_l0fa290*>(manager);
}

void fn_71010fa290(char* object) {
    UnkAI_l0fa290* self = reinterpret_cast<UnkAI_l0fa290*>(object);
    fn_71010fa6d4(object);

    if (!self->tunable || self->tunable->field_0xc4)
        fn_71011a710c(TeamManager());

    if (UnkNamedObject_l0fa290* named = self->owner->named) {
        CRntString script = CRntString_ConcatCString(&named->name, ".BeginPlay");
        if (fn_71010c0bb0(g_unk_7172a01d18, script.data ? script.data : g_unk_710162e2ed)) {
            char out[0x100];
            fn_7100fbc4e0(g_unk_7172a01d18, self->owner);
            fn_71010c0d80(g_unk_7172a01d18, 1, out, 0x100, nullptr, nullptr);
        }
    }

    if (self->tunable)
        self->field_0x11e0 = self->vf_0x198();

    self->field_0x121e = false;
    self->field_0x121f = false;
    self->field_0x1220 = false;
    self->field_0x14fc = 9999.9f;
    self->field_0x11e4 = 1;
    self->field_0x11e8 = 15;
    self->field_0x11f0 = -1.0f;
    self->field_0x11f4 = 0;
    self->field_0x11f8 = 0;
    Copy(self->field_0x1204, g_unk_710164779c);
    Copy(self->field_0x1210, g_unk_710164779c);
    self->field_0x11ec = false;
    self->field_0x11ed = false;
    self->field_0x11fc = 0;
    self->field_0x121d = !self->field_0x121c && !self->tunable->field_0x36a;
    self->field_0x3d = false;
    self->field_0x3e = false;
    UnkTarget_l0fa290* target = &self->target;
    target->field_0x10 = false;
    target->field_0x11 = false;
    target->field_0x20 = 0;
    self->field_0x179e = false;
    self->field_0x13d2 = false;
    target->field_0x12 = false;
    self->field_0x1466c = CVector3D();
    target->field_0x14.x = -1.0f;
    target->field_0x14.y = -1.0f;
    target->field_0x14.z = -1.0f;
    target->id = -1;
    target->field_0x4 = 0;
    target->field_0x8 = 0;
    fn_71010fa828(reinterpret_cast<UnkAIComponent*>(self));

    if (UnkHooks_l0fa290* hooks = self->owner->hooks) {
        self->hook_object = self;
        self->hook = fn_7101115fc4;
        if (self->hook_slot) {
            fn_710088c874(self->hook_slot, &self->hook_slot);
            self->hook_slot = nullptr;
        }
        self->hook_slot = hooks->set;
        fn_710088d004(hooks->set, &self->hook_slot);
        hooks->first_object = self;
        hooks->first = fn_7101115fd0;
        hooks->second_object = self;
        hooks->second = fn_7101115fdc;
    }

    if (UnkCollision_l0fa290* collision = self->owner->collision) {
        if (!(self->tunable->sight_radius >= 0.0f)) {
            u32 flags = 0x10000000;
            fn_7100863814(reinterpret_cast<UnkCollisionContainer*>(collision), &g_unk_7172a06340, &flags);
        } else if (!fn_71008640a4(reinterpret_cast<UnkCollisionContainer*>(collision), &g_unk_7172a06348)) {
            const CStrId& group = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x13e];
            UnkCollider_l0fa290* collider =
                collision->vf_0x168(&g_unk_7172a06348, &g_unk_7172a06348, &g_unk_7101d470e0, &group, 0x10002000, true,
                                    false, nullptr, 0xffffffff, nullptr, 0xffffffff, nullptr, 0xffffffff, false, false,
                                    true);
            if (collider) {
                UnkShape_l0fa290* shape = collider->shape;
                fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(shape->vf_0x18() == 2 ? shape : nullptr),
                              self->tunable->sight_radius);
                fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), true);
            }
        }
    }

    UnkPos_l0fa290 pos = {self->owner->x, self->owner->y, self->owner->z};
    fn_710007b444(self->field_0x1604, reinterpret_cast<const UnkVec3*>(&pos));
    self->field_0x1602 = 0xff;
    self->field_0x1600 = 0xffff;
    self->vf_0x718();

    UnkSubAreaManager_l0fa290* manager = SubAreaManager();
    void** slot = &self->area_slot;
    if (*slot) {
        fn_71005c4828(*slot, slot);
        *slot = nullptr;
    }
    *slot = manager->callbacks;
    fn_71005c41bc(manager->callbacks, reinterpret_cast<UnkCallback*>(slot));
}
