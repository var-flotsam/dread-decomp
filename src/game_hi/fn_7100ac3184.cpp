// Initialises the component built by fn_7100ac2c5c once it is attached: caches a position
// from the owner's transform, resolves two named targets (by the ids in its definition) into
// weak handles, resets its state, re-registers its id with the owner's table (fn_7100a2cddc)
// and binds its callback to a lazily created global dispatcher.
#include "base/global/CStrId.h"
#include "base/UnkStrIdPathPair.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkVec3 {
    u32 x, y, z;
};

struct UnkTransform {
    u8 unk_0x0[0x44];
    u32 field_0x44;
    u8 unk_0x48[0xc];
    u32 field_0x54;
    u8 unk_0x58[0xc];
    u32 field_0x64;
};

struct UnkTransformHolder {
    u8 unk_0x0[0x10];
    UnkTransform* transform;
};

struct UnkOwner {
    u8 unk_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
    u8 unk_0x2a8[0x40];
    void* field_0x2e8;
};

struct UnkDefinition {
    u8 unk_0x0[0x108];
    CStrId field_0x108;
    CStrId field_0x110;
};

struct UnkCallback;
void fn_71005c4828(void* dispatcher, UnkCallback* callback);  // unregister
void fn_71005c41bc(void* dispatcher, UnkCallback* callback);  // register

struct UnkCallback {
    void* dispatcher;
    void* object;
    void* function;

    // Moves the callback to the dispatcher at +0xa60 of `manager`.
    void Connect(void* manager) {
        if (dispatcher) {
            fn_71005c4828(dispatcher, this);
            dispatcher = nullptr;
        }
        void* to = static_cast<char*>(manager) + 0xa60;
        dispatcher = to;
        fn_71005c41bc(to, this);
    }
};

struct UnkComponent_0x5d0 {
    void** vtable;
    u8 unk_0x8[0x28];
    UnkOwner* owner;
    u8 unk_0x38[0x44];
    f32 field_0x7c;
    u8 unk_0x80[0x88];
    f32 field_0x108;
    u8 unk_0x10c[0x124];
    UnkTransformHolder* field_0x230;
    u8 unk_0x238[0x24c];
    u32 field_0x484;
    u8 unk_0x488[0x20];
    UnkWeakHandle* field_0x4a8;
    UnkWeakHandle* field_0x4b0;
    u8 unk_0x4b8[0xc];
    UnkVec3 field_0x4c4;
    u8 unk_0x4d0[0x1c];
    u32 field_0x4ec[13];
    u32 field_0x520;
    u16 field_0x524;
    u8 unk_0x526[0x2];
    UnkRefObject* field_0x528;
    UnkRefObject* field_0x530;
    u8 unk_0x538[0x2c];
    u32 field_0x564;
    u8 unk_0x568[0x10];
    bool field_0x578;
    bool field_0x579;
    u8 unk_0x57a[0x6];
    u8 field_0x580[0x18];
    CStrId field_0x598;
    UnkCallback field_0x5a0;
    u8 unk_0x5b8[0x10];
    UnkDefinition* field_0x5c8;
};

struct UnkStaticZero_71726b5354 {
    u32 value;
    UnkStaticZero_71726b5354() : value(0) {}
};

inline const UnkStaticZero_71726b5354& UnkGetStaticZero_71726b5354() {
    static UnkStaticZero_71726b5354 s_value;
    return s_value;
}

extern void* g_unk_7101d47360;
extern u8 g_unk_71727556f0 __attribute__((visibility("hidden")));
void fn_7100ac7628(UnkComponent_0x5d0* self);
void fn_7100a2cddc(void* table, void* key, CStrId* id, bool unique, f32 delay);
void fn_7100ee1f34(void* self);

// The global manager, created on first use.
static inline void* UnkGetManager() {
    if (!g_unk_7101d47360) {
        void* manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return g_unk_7101d47360;
}

__attribute__((always_inline)) static inline void ResolveTarget(UnkComponent_0x5d0* self, const CStrId& id, UnkWeakHandle** out) {
    if (id.data == &g_unk_7101d47108->empty)
        return;
    UnkCollisionContainer* registry = self->owner->field_0x2a0;
    if (!registry)
        return;
    UnkWeakHandle* handle = nullptr;
    if (UnkWeakTarget* target = reinterpret_cast<UnkWeakTarget*>(fn_71008640a4(registry, &id)))
        handle = GetWeakHandle(target);
    AssignRef(out, handle);
}

void fn_7100ac3184(UnkComponent_0x5d0* self) {
    fn_7100ac7628(self);
    UnkTransformHolder* holder = self->field_0x230;
    UnkTransform* t = holder ? holder->transform : nullptr;
    UnkVec3 v;
    if (t)
        v = UnkVec3{t->field_0x44, t->field_0x54, t->field_0x64};
    else
        v = UnkVec3{0, 0, 0};
    self->field_0x4c4 = v;
    ResolveTarget(self, self->field_0x5c8->field_0x108, &self->field_0x4a8);
    ResolveTarget(self, self->field_0x5c8->field_0x110, &self->field_0x4b0);
    __builtin_memset(self->field_0x4ec, 0, sizeof(self->field_0x4ec));
    self->field_0x520 = self->field_0x484;
    self->field_0x7c = 1.0f;
    self->field_0x108 = -1.0f;
    self->field_0x524 = 0;
    self->field_0x578 = false;
    self->field_0x598 = g_unk_7101d470e0;
    ResetRef(&self->field_0x528);
    ResetRef(&self->field_0x530);
    self->field_0x564 = UnkGetStaticZero_71726b5354().value;
    if (void* table = self->owner->field_0x2e8) {
        void* key = reinterpret_cast<void* (*)(UnkComponent_0x5d0*)>(self->vtable[0x2e0 / 8])(self);
        fn_7100a2cddc(table, key, &self->field_0x598, false, -1.0f);
    }
    self->field_0x579 = false;
    fn_71000267f8(reinterpret_cast<UnkStrIdPathResource*>(self->field_0x580),
                  reinterpret_cast<const UnkStrIdPathPair&>(g_unk_71727556f0));
    self->field_0x5a0.Connect(UnkGetManager());
}
