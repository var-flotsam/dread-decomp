// CSpawnPointComponent's unit (tools/tu_map.py unit 2704): the component's reflected value holders
// and their helpers. A value (UnkReflValue) owns its payload either inline or through a heap block
// that is released through the game manager (fn_7100232124) or deleted; the variants, arrays,
// iterators and vectors of such values below destroy, copy and move them, and the weak references,
// callback iterators, collision tracker teardown and a name test of the spawn point sit beside them.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponentDef.h"
#include "game/CComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

struct UnkBoundObj;
struct UnkAssignedRecord_a1ba4;
void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);

// Releases the payload a value owns (shared by the holders below).
static inline __attribute__((always_inline)) void DestroyPayload(UnkReflValue* v) {
    if (v->type) {
        bool released = fn_7100deabe4(g_unk_7101d47310);
        void* payload = nullptr;
        if (v->type) {
            payload = v;
            if (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
                payload = v->field_0x0;
        }
        if (released) {
            fn_7100232124(static_cast<UnkBoundObj*>(payload));
        } else if (payload) {
            fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
            fn_7100083c00(payload);
        }
    }
}

// ---- Spawn point values

void fn_710112f1f8(void* obj) {
    UnkReflValue* v = static_cast<UnkReflValue*>(obj);
    DestroyPayload(v);
    fn_71000a1c60(v);
}

void* fn_710112f290(void* dst, const void* src) {
    UnkReflValue* self = static_cast<UnkReflValue*>(dst);
    if (dst != src) {
        DestroyPayload(self);
        fn_71000a1ba4(static_cast<UnkAssignedRecord_a1ba4*>(dst), static_cast<const UnkAssignedRecord_a1ba4*>(src));
    }
    return dst;
}

// A value with a state: 1 holds the value, 2 refers to one; resetting destroys it.
struct UnkVariantD304 {
    u8 pad_0x0[0x18];
    UnkReflValue value;  // 0x18
    u32 kind;            // 0x30
};
static_assert(__builtin_offsetof(UnkVariantD304, kind) == 0x30, "layout");

void fn_710112d304(UnkVariantD304* self) {
    if (self->kind == 2) {
        self->kind = 0;
    } else if (self->kind == 1) {
        DestroyPayload(&self->value);
        fn_71000a1c60(&self->value);
        self->kind = 0;
    }
}

void fn_710112d7e8(UnkVariantD304* self) {
    if (self->kind == 2) {
        self->kind = 0;
    } else if (self->kind == 1) {
        DestroyPayload(&self->value);
        fn_71000a1c60(&self->value);
        self->kind = 0;
    }
}

struct UnkVariantF3b4 {
    u8 pad_0x0[8];
    UnkReflValue value;  // 0x8
    u32 kind;            // 0x20
};

void fn_710112f3b4(void* obj) {
    UnkVariantF3b4* self = static_cast<UnkVariantF3b4*>(obj);
    if (self->kind == 2) {
        self->kind = 0;
    } else if (self->kind == 1) {
        DestroyPayload(&self->value);
        fn_71000a1c60(&self->value);
        self->kind = 0;
    }
}

void fn_710112f708(void* obj) {
    UnkVariantF3b4* self = static_cast<UnkVariantF3b4*>(obj);
    if (self->kind == 2) {
        self->kind = 0;
    } else if (self->kind == 1) {
        DestroyPayload(&self->value);
        fn_71000a1c60(&self->value);
        self->kind = 0;
    }
}

// ---- Arrays and iterators of values

// A value as an array element: destroyed through the inline sequence below.
struct UnkReflElem_b0170 {
    UnkReflValue value;
    UnkReflElem_b0170() {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
    }
    UnkReflElem_b0170(UnkReflElem_b0170&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkReflElem_b0170() {
        if (value.type) {
            bool released = fn_7100deabe4(g_unk_7101d47310);
            void* payload = nullptr;
            if (value.type)
                payload = (value.field_0x14 == 1 || *reinterpret_cast<const u64*>(value.type + 0x18) > 8)
                              ? value.field_0x0
                              : &value;
            if (released) {
                fn_7100232124(static_cast<UnkBoundObj*>(payload));
            } else if (payload) {
                fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
                fn_7100083c00(payload);
            }
        }
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkReflElem_b0170) == 0x18, "element size");

struct UnkValueArray_b0170 {
    UnkReflElem_b0170* data;  // 0x0
    u32 count;                // 0x8
};

struct UnkValueIter_b0170;
void fn_710112d438(UnkValueIter_b0170* it);  // re-points `current` at (index, element)

struct UnkValueIter_b0170 {
    u32 index;
    UnkReflElem_b0170* element;
    base::UnkIterValue<UnkReflElem_b0170, u32> current;

    UnkValueIter_b0170(u32 index, UnkReflElem_b0170* element) : index(index), element(element) {
        fn_710112d438(this);
    }
    UnkValueIter_b0170& operator=(const UnkValueIter_b0170& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_710112d438(this);
        }
        return *this;
    }
};

void fn_7101130174(UnkValueArray_b0170* self, UnkValueIter_b0170* out) {
    *out = UnkValueIter_b0170(0, self->data);
}

void fn_7101130270(UnkValueArray_b0170* self, UnkValueIter_b0170* out) {
    *out = UnkValueIter_b0170(self->count, self->data + self->count);
}

void fn_71011266cc(base::UnkVector<UnkReflElem_b0170>* self) {
    self->Free();
}

base::UnkVector<UnkReflElem_b0170>* fn_710112fd34(base::UnkVector<UnkReflElem_b0170>* self,
                                                   base::UnkVector<UnkReflElem_b0170>* other) {
    *self = static_cast<base::UnkVector<UnkReflElem_b0170>&&>(*other);
    return self;
}

struct UnkValueHolder_c3dc {
    u8 pad_0x0[0x10];
    void* field_0x10;
    u8 pad_0x18[0x390 - 0x18];
    UnkReflValue* values;  // 0x390
    u32 count;             // 0x398
};

void fn_710024fb64(UnkValueHolder_c3dc* self);

bool fn_710112c3dc(void* obj) {
    UnkValueHolder_c3dc* self = static_cast<UnkValueHolder_c3dc*>(obj);
    if (self->count == 0)
        return false;
    s32 last = self->count - 1;
    UnkReflValue* values = self->values;
    UnkReflValue* v = &values[last];
    if (v->type) {
        bool released = fn_7100deabe4(g_unk_7101d47310);
        void* payload = nullptr;
        if (v->type)
            payload = (values[last].field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
                          ? v->field_0x0
                          : v;
        if (released) {
            fn_7100232124(static_cast<UnkBoundObj*>(payload));
        } else if (payload) {
            fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
            fn_7100083c00(payload);
        }
    }
    fn_71000a1c60(v);
    self->count = last;
    if (fn_7100deabe4(g_unk_7101d47310)) {
        fn_7100253ad4(self->field_0x10);
        fn_710024fb64(self);
    }
    return true;
}

// ---- Class lookup

extern CStrId g_unk_7172a086a8 __attribute__((visibility("hidden")));                    // the class id
extern base::reflection::CType g_unk_7172a086d8 __attribute__((visibility("hidden")));  // its type

UnkReflObject* fn_710024e2e8(UnkGameManagerIdList* list, void* key);

UnkReflObject* fn_7101126814(const CStrId* id) {
    UnkGameManagerIdList* list = fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172a086a8));
    UnkReflObject* obj = fn_710024e2e8(list, const_cast<CStrId*>(id));
    if (obj == nullptr)
        return nullptr;
    const base::reflection::CType* type = obj->vf_0x0();
    fn_7101123918();
    return fn_710009d50c(type, &g_unk_7172a086d8) ? obj : nullptr;
}

// ---- Registration under the owner's id

extern u64 g_unk_7172a086a0 __attribute__((visibility("hidden")));

struct UnkSpawnOwner_7e78 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkSpawnPoint_7e78 {
    u8 pad_0x0[0x18];
    UnkSpawnOwner_7e78* owner;  // 0x18
    u8 pad_0x20[0x280 - 0x20];
    u8 sub[1];  // 0x280
};

struct UnkParent;
void fn_7101127f2c(void* sub, const CStrId* name);

void fn_7101127e78(UnkSpawnPoint_7e78* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    fn_7100de9540(g_unk_7101d47310, &g_unk_7172a086a0, reinterpret_cast<UnkWeakTarget*>(self));
    void* sub = self->sub;
    CStrId name(self->owner->name);
    fn_7101127f2c(sub, &name);
}

// ---- Collision tracker teardown

struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);
struct UnkCollisionObject;
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);
void fn_7100d476e4(UnkManager_7100863a8c* self, void* obj);
struct UnkStruct_7100861148_Collider;
void fn_7100d5abfc(void* owner, UnkStruct_7100861148_Collider* collider);

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

struct UnkSpawnOwner_64b4 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkSpawnA_64b4 {
    u8 pad_0x0[0x30];
    UnkSpawnOwner_64b4* owner;  // 0x30
    u8 pad_0x38[0x118 - 0x38];
    void* collider;  // 0x118
    u8 pad_0x120[0x160 - 0x120];
    void* tracked;  // 0x160
};

void fn_71011264b4(UnkSpawnA_64b4* self, void*) {
    if (self->tracked) {
        if (self->collider) {
            fn_7100d5abfc(self->tracked, static_cast<UnkStruct_7100861148_Collider*>(self->collider));
            fn_7100d46c60(Manager(), static_cast<UnkCollisionObject*>(self->collider));
            self->collider = nullptr;
        }
        {
            UnkManager_7100863a8c* manager = Manager();
            CStrId name(self->owner->name);
            fn_7100d476e4(manager, &name);
        }
        self->tracked = nullptr;
    }
}

struct UnkSpawnPoint_l129064 {
    u8 pad_0x0[0x30];
    UnkSpawnOwner_64b4* owner;  // 0x30
    u8 pad_0x38[0x168 - 0x38];
    void* collider;  // 0x168
    u8 pad_0x170[0x1b0 - 0x170];
    void* tracked;  // 0x1b0
};

void fn_71011265c0(UnkSpawnPoint_l129064* self, void* shape) {
    if (self->tracked) {
        if (self->collider) {
            fn_7100d5abfc(self->tracked, static_cast<UnkStruct_7100861148_Collider*>(self->collider));
            fn_7100d46c60(Manager(), static_cast<UnkCollisionObject*>(self->collider));
            self->collider = nullptr;
        }
        {
            UnkManager_7100863a8c* manager = Manager();
            CStrId name(self->owner->name);
            fn_7100d476e4(manager, &name);
        }
        self->tracked = nullptr;
    }
}

// ---- String ids

struct UnkIdQuad_b684 {
    CStrId id0;  // 0x0
    CStrId id1;  // 0x8
    CStrId id2;  // 0x10
    CStrId id3;  // 0x18
};

void fn_710112b684(UnkIdQuad_b684* self) {
    self->id3.Reset();
    self->id2.Reset();
    self->id1.Reset();
    self->id0.Reset();
}

struct UnkNamedActor_baa0 {
    u8 pad_0x0[0x48];
    CStrId name;  // 0x48
};

bool fn_710112baa0(void* actor) {
    static CStrId s_rinka("rinka", true);
    static CStrId s_cucannon("cucannon", true);
    UnkNamedActor_baa0* a = static_cast<UnkNamedActor_baa0*>(actor);
    return (a->name.data == s_rinka.data) | (a->name.data == s_cucannon.data);
}

// ---- Callback iterator and weak reference

struct UnkClone_fn_710112e7e0;
extern void* g_unk_7101cb2880[];
template <>
struct base::UnkCallbackTraits<UnkClone_fn_710112e7e0> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101cb2880); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

using UnkSpawnCallbackList_248c = base::UnkCallbackListShort<UnkClone_fn_710112e7e0>;
static_assert(sizeof(UnkSpawnCallbackList_248c) == 0x40, "layout");

struct UnkSpawnCallbackIter_248c {
    u32 index;
    UnkSpawnCallbackList_248c* element;
    base::UnkIterValue<UnkSpawnCallbackList_248c, u32> current;

    UnkSpawnCallbackIter_248c(u32 index, UnkSpawnCallbackList_248c* element) : index(index), element(element) {
        fn_710112e7e0(this);
    }
    UnkSpawnCallbackIter_248c& operator=(const UnkSpawnCallbackIter_248c& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_710112e7e0(this);
        }
        return *this;
    }
};

void fn_710113248c(const base::UnkVector<UnkSpawnCallbackList_248c>* v, UnkSpawnCallbackIter_248c* out) {
    *out = UnkSpawnCallbackIter_248c{v->count, v->data + v->count};
}

struct UnkWeakRefSlot_06a4 {
    u8 pad_0x0[0x30];
    s32 kind;  // 0x30: 0 none, 2 resolved
    u8 pad_0x34[4];
    UnkWeakHandle* handle;  // 0x38
};

void fn_71011306a4(UnkWeakRefSlot_06a4* self, UnkWeakTarget* target) {
    if (target) {
        UnkWeakHandle* handle = GetWeakHandle(target);
        AssignRef(&self->handle, handle);
        self->kind = 2;
    } else {
        ResetRef(&self->handle);
        self->kind = 0;
    }
}
