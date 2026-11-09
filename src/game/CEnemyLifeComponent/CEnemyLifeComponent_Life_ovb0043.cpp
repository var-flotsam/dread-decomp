// CEnemyLifeComponent's unit (tools/tu_map.py): the destructor (fn_710115a848), the reset (fn_710115ab50),
// the AI overrides of the base value and step (fn_710115b0f4, fn_710115bdcc), the frostbite test
// (fn_710115bd60) and the link-list holder's destructor (fn_710115a6e8). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkLinked_ovb0043 {
    void* back;
};

// A holder of three arrays whose first one points at objects that keep a back pointer in their first
// word; the same destructor as fn_71008e03dc's (UnkLinkLists_A4_b75).
struct UnkLinkLists_ovb0043 {
    base::UnkVector<UnkLinked_ovb0043*> linked;  // 0x0
    base::UnkVector<u64> field_0x28;
    base::UnkVector<u64> field_0x50;
};

// Clears every back pointer, empties the three arrays and frees them in reverse order.
void fn_710115a6e8(UnkLinkLists_ovb0043* self) {
    for (UnkLinked_ovb0043 **it = self->linked.data, **end = self->linked.data + self->linked.count; it != end; ++it)
        (*it)->back = nullptr;
    self->linked.count = 0;
    self->field_0x28.count = 0;
    self->field_0x50.count = 0;
    self->field_0x50.Free();
    self->field_0x28.Free();
    self->linked.Free();
}

struct UnkFrostbiteComponent;
struct UnkFrostbiteRequest;
bool fn_71007fb280(UnkFrostbiteComponent* self, UnkFrostbiteRequest* request);

struct UnkEnemyLifeRequest_ovb0043 {
    u8 pad_0x0[0xf8];
    f32 damage;  // 0xf8
};

struct UnkEnemyLifeActor_ovb0043 {
    u8 pad_0x0[0x2c8];
    void* frostbite;  // 0x2c8
};

struct UnkEnemyLifeComponent_ovb0043 {
    u8 pad_0x0[0x30];
    UnkEnemyLifeActor_ovb0043* actor;  // 0x30
};

u32 fn_710116046c(UnkEnemyLifeComponent_ovb0043* self, UnkEnemyLifeRequest_ovb0043* request);

// The base test (fn_710116046c); a request with no damage is also taken when the owner's frostbite
// component (+0x2c8) accepts it (fn_71007fb280).
u32 fn_710115bd60(UnkEnemyLifeComponent_ovb0043* self, UnkEnemyLifeRequest_ovb0043* request) {
    u32 result = fn_710116046c(self, request);
    if (request->damage == 0.0f) {
        if (void* frostbite = self->actor->frostbite) {
            if (fn_71007fb280(static_cast<UnkFrostbiteComponent*>(frostbite),
                              reinterpret_cast<UnkFrostbiteRequest*>(request)))
                result = 1;
        }
    }
    return result;
}


extern void* g_unk_7101cb3b80[];

// A reference-counted object: deleted through its vtable slot 2 when the count reaches zero.
struct UnkRefCounted_ovb0043 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void Delete();
    s32 ref_count;  // 0x8
};

struct UnkLifeRenderObject_ovb0043 {
    void** vtable;
    u8 item[0x8];  // 0x8, the render manager's item
};

struct UnkEnemyLifeDtor_ovb0043 {
    void** vtable;
    u8 pad_0x8[0x250];
    UnkRefCounted_ovb0043* shared;  // 0x258
    u8 pad_0x260[0x10];
    UnkLifeRenderObject_ovb0043* render;  // 0x270
};

void fn_7101158c4c(UnkEnemyLifeDtor_ovb0043* self);

// The destructor: the render object (+0x270) is taken off the render manager (layer 0) and cleared, the
// shared object (+0x258) released, then the base destructor fn_7101158c4c.
void fn_710115a848(UnkEnemyLifeDtor_ovb0043* self) {
    self->vtable = g_unk_7101cb3b80 + 2;
    if (UnkLifeRenderObject_ovb0043* render = self->render) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(render->item), 0);
        self->render = nullptr;
    }
    if (UnkRefCounted_ovb0043* shared = self->shared) {
        if (--shared->ref_count == 0)
            shared->Delete();
    }
    fn_7101158c4c(self);
}

// The AI component's override of the value: vtable slot 73 (+0x248) fills `out` when it handles it.
struct UnkEnemyLifeAI_ovb0043 {
    struct VTable {
        void* slots[73];
        bool (*GetValue)(UnkEnemyLifeAI_ovb0043* self, u32* out, void* a, void* b, void* c);  // 0x248
    };
    VTable* vtable;
};

struct UnkEnemyLifeActorAI_ovb0043 {
    u8 pad_0x0[0x240];
    UnkEnemyLifeAI_ovb0043* ai;  // 0x240
};

struct UnkEnemyLifeComponentAI_ovb0043 {
    u8 pad_0x0[0x30];
    UnkEnemyLifeActorAI_ovb0043* actor;  // 0x30
};

u32 fn_7101158f30(UnkEnemyLifeComponentAI_ovb0043* self, void* a, void* b, void* c);

// The AI component's value when it gives one, else the base's (fn_7101158f30).
u32 fn_710115b0f4(UnkEnemyLifeComponentAI_ovb0043* self, void* a, void* b, void* c) {
    u32 value = 0;
    UnkEnemyLifeAI_ovb0043* ai = self->actor->ai;
    if (ai && ai->vtable->GetValue(ai, &value, a, b, c))
        return value;
    value = fn_7101158f30(self, a, b, c);
    return value;
}

void fn_71011a664c(char* instance);  // constructs the instance
void fn_71011a710c(char* instance);
extern char* g_unk_71726b2398;       // the instance, reached through the GOT

struct UnkEnemyLifeAIState_ovb0043 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
};

struct UnkEnemyLifeAI2_ovb0043 {
    u8 pad_0x0[0x1610];
    UnkEnemyLifeAIState_ovb0043* state;  // 0x1610
};

struct UnkEnemyLifeActor2_ovb0043 {
    u8 pad_0x0[0x240];
    UnkEnemyLifeAI2_ovb0043* ai;  // 0x240
};

struct UnkEnemyLifeComponent2_ovb0043 {
    u8 pad_0x0[0x30];
    UnkEnemyLifeActor2_ovb0043* actor;  // 0x30
};

bool fn_7101158d78(UnkEnemyLifeComponent2_ovb0043* self, bool flag);
void fn_71011a726c(char* instance, UnkEnemyLifeActor2_ovb0043* actor);

// The global instance (0x108 bytes), created on first use.
static inline char* Instance_ovb0043() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return instance;
}

// The base step (fn_7101158d78); when it succeeds and the AI's state (+0x1610) does not hold back
// (+0xc4 clear), the owner is passed to the global instance (fn_71011a726c, then fn_71011a710c).
bool fn_710115bdcc(UnkEnemyLifeComponent2_ovb0043* self, bool flag) {
    bool result = fn_7101158d78(self, flag);
    if (result) {
        UnkEnemyLifeActor2_ovb0043* actor = self->actor;
        UnkEnemyLifeAI2_ovb0043* ai = actor->ai;
        if (!ai || !ai->state || ai->state->field_0xc4) {
            fn_71011a726c(Instance_ovb0043(), self->actor);
            fn_71011a710c(Instance_ovb0043());
        }
    }
    return result;
}

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern base::global::CStrId g_unk_71726b7cb8;

// The id table (fn_7100031894 builds it); its +0xf80 is a blackboard property name.
struct UnkIdTable_ovb0043 {
    UnkIdTable_ovb0043();  // fn_7100031894
    ~UnkIdTable_ovb0043();
    u8 pad_0x0[0xf80];
    UnkPropertyName field_0xf80;
};

inline UnkIdTable_ovb0043& IdTable_ovb0043() {
    static UnkIdTable_ovb0043 s_table;
    return s_table;
}

struct UnkEnemyLifeAI3_ovb0043 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
};

struct UnkEnemyLifeActor3_ovb0043 {
    u8 pad_0x0[0x240];
    UnkEnemyLifeAI3_ovb0043* ai;  // 0x240
    u8 pad_0x248[0x38];
    void* field_0x280;
};

struct UnkEnemyLifeComponent3_ovb0043 {
    u8 pad_0x0[0x30];
    UnkEnemyLifeActor3_ovb0043* actor;  // 0x30
    u8 pad_0x38[0x18];
    f32 field_0x50;
    f32 field_0x54;
    u8 pad_0x58[0x18];
    s32 field_0x70;
    u8 pad_0x74[0x1d4];
    bool field_0x248;
    u8 pad_0x249[3];
    f32 field_0x24c;
    u32 field_0x250;
    u8 pad_0x254[0xc];
    void* field_0x260;
    bool field_0x268;
};

void fn_710115fc5c(char*);
f32 fn_71008bce1c(void* obj);
void fn_7101156fe8(void* obj);
void fn_7100da9bd4(UnkManager_71726af468* self, UnkEnemyLifeActor3_ovb0043* actor);

// The reset: the life component's (fn_710115fc5c), both values at +0x50/+0x54 from fn_71008bce1c, the
// AI blackboard's property (id table +0xf80) zeroed, fn_7101156fe8 on the actor's +0x280 and the actor
// handed to the global manager (fn_7100da9bd4).
void fn_710115ab50(UnkEnemyLifeComponent3_ovb0043* self) {
    fn_710115fc5c(reinterpret_cast<char*>(self));
    f32 value = fn_71008bce1c(self->field_0x260);
    self->field_0x70 = 1;
    self->field_0x50 = value;
    self->field_0x54 = value;
    self->field_0x248 = false;
    self->field_0x24c = -1.0f;
    self->field_0x250 = 0;
    self->field_0x268 = false;
    if (UnkEnemyLifeAI3_ovb0043* ai = self->actor->ai) {
        CBlackboard* blackboard = &ai->blackboard;
        UnkPropertyName* name = &IdTable_ovb0043().field_0xf80;
        UnkTypedValue v;
        f32 zero = 0.0f;
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
            if (!blackboard->field_0xe8) {
                v.type = fn_710000abbc();
                v.value = &zero;
                fn_7100332c80(s, name, &v);
            }
        }
    }
    if (void* obj = self->actor->field_0x280)
        fn_7101156fe8(obj);
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_7100da9bd4(manager, self->actor);
}
