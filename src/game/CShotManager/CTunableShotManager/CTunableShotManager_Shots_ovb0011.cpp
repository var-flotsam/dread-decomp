// CShotManager::CTunableShotManager's unit (tools/tu_map.py): more of the shot manager (the shot and
// effect vectors, the per-type shot sounds, the shot lights and the explosion models; the layout
// src/game/CShotManager.cpp spells out) and the containers it is built from: a key and value that
// are held or pointed at, a callback bound to an object, and growable arrays of reference-counted
// records. Names and layouts are placeholders.
#include <new>

#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "engine/scene/UnkToggle.h"
#include "game/CLightManager.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU290.h"
#include "game/UnkTU315.h"
#include "game/UnkTU437.h"
#include "game/sound/CSoundManager.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

// ---- The shot manager ----

struct UnkShotEffectState_b0011 {
    bool live;  // 0x18
    u8 pad_0x19[0x7];
    f32 field_0x20;
    f32 field_0x24;
    u32 type;     // 0x28
    u32 variant;  // 0x2c
    u8 pad_0x30[0xcc];
};

// An explosion effect slot (0xfc bytes). Its copy takes the first six words one by one and the
// rest as a block.
struct UnkShotEffect_b0011 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    UnkShotEffectState_b0011 state;  // 0x18

    UnkShotEffect_b0011(const UnkShotEffect_b0011& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          state(other.state) {}
};
static_assert(sizeof(UnkShotEffect_b0011) == 0xfc, "UnkShotEffect_b0011 layout");

// The explosion a shot type and variant makes; three models at +0xa0.
struct UnkShotGfx_b0011 {
    u8 pad_0x0[0xa0];
    u64 field_0xa0;
    u64 field_0xa8;
    u64 field_0xb0;
};

// A shot sound: a sound id and its file path.
struct UnkShotSound_b0011 {
    UnkStrIdPathPair ids;
    u64 field_0x10;
};

struct UnkListedObj;
struct UnkPtrList;

struct UnkShotManager_b0011 {
    u8 pad_0x0[0x30];
    base::UnkVector<UnkShotEffect_b0011> field_0x30;
    u8 pad_0x58[0x148];
    void** field_0x1a0[5][2][2];  // the render instances per type and variant, and of their second part
    UnkShotGfx_b0011* field_0x240[5][5];
    u8 pad_0x308[0x3c0];
    UnkShotSound_b0011 field_0x6c8[5][8];
    u8 pad_0xa88[0x7b0];
    base::UnkVector<UnkListedObj*> field_0x1238;  // the shot lights
    u32 field_0x1260;

    bool HasPath(u32 type, u32 variant) { return field_0x6c8[type][variant].ids.path.data != &g_unk_7101d47200->empty; }
};
static_assert(__builtin_offsetof(UnkShotManager_b0011, field_0x1a0) == 0x1a0, "UnkShotManager_b0011 layout");
static_assert(__builtin_offsetof(UnkShotManager_b0011, field_0x240) == 0x240, "UnkShotManager_b0011 layout");
static_assert(__builtin_offsetof(UnkShotManager_b0011, field_0x6c8) == 0x6c8, "UnkShotManager_b0011 layout");
static_assert(__builtin_offsetof(UnkShotManager_b0011, field_0x1238) == 0x1238, "UnkShotManager_b0011 layout");
static_assert(__builtin_offsetof(UnkShotManager_b0011, field_0x1260) == 0x1260, "UnkShotManager_b0011 layout");

struct UnkSoundRef;

struct UnkPlayer;
struct UnkActor;
struct UnkEntityWithSound;
struct UnkDropTarget;

UnkSoundHandle fn_7100d7c42c(UnkEntityWithSound* self, void* a, void* b, void* c, void* d);
void fn_7101025560(UnkDropTarget* self, const UnkStrIdPathPair* ids, f32 blend, f32 start);

// The player's sound calls (src/game/CEntity_2.cpp) and the drop manager's, as called here: with a
// flag last and the handle returned.
using UnkPlayFn_b0011 = UnkSoundHandle (*)(UnkEntityWithSound*, const UnkStrIdPathPair*, void*, bool);
using UnkPlayAtFn_b0011 = UnkSoundHandle (*)(UnkEntityWithSound*, const UnkStrIdPathPair*, void*, void*, u32);
using UnkDropPlayAtFn_b0011 = UnkSoundHandle (*)(UnkDropTarget*, const UnkStrIdPathPair*, void*, void*, u32);

// Plays the sound of a shot type and variant at `source` through the player, or through the drop
// manager when there is no player; the empty handle when the sound has no id or path.
UnkSoundHandle fn_7100ed6910(UnkShotManager_b0011* self, void* source, u32 type, u32 variant, u32 flags) {
    UnkShotSound_b0011& sound = self->field_0x6c8[type][variant];
    if (sound.ids.id.data == &g_unk_7101d47108->empty || !self->HasPath(type, variant))
        return g_unk_7108b6c460;
    const UnkStrIdPathPair* ids = &sound.ids;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
        return reinterpret_cast<UnkPlayAtFn_b0011>(&fn_7100d7c42c)(reinterpret_cast<UnkEntityWithSound*>(player),
                                                                   ids, source, nullptr, flags);
    return reinterpret_cast<UnkDropPlayAtFn_b0011>(&fn_7101024b28)(g_unk_7101d47318, ids, source, nullptr, flags);
}

// The same without a source.
UnkSoundHandle fn_7100ed6a10(UnkShotManager_b0011* self, u32 type, u32 variant) {
    UnkShotSound_b0011& sound = self->field_0x6c8[type][variant];
    if (sound.ids.id.data == &g_unk_7101d47108->empty || !self->HasPath(type, variant))
        return g_unk_7108b6c460;
    const UnkStrIdPathPair* ids = &sound.ids;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
        return reinterpret_cast<UnkPlayFn_b0011>(&fn_7100d7c45c)(reinterpret_cast<UnkEntityWithSound*>(player), ids,
                                                                 nullptr, false);
    return fn_7101024e28(g_unk_7101d47318, ids, nullptr, false);
}

// Stops the sound of a shot type and variant (the drop manager's with a 0.2 blend).
void fn_7100ed6af0(UnkShotManager_b0011* self, u32 type, u32 variant) {
    UnkShotSound_b0011& sound = self->field_0x6c8[type][variant];
    if (sound.ids.id.data == &g_unk_7101d47108->empty || !self->HasPath(type, variant))
        return;
    const UnkStrIdPathPair* ids = &sound.ids;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
        fn_7100d7c548(reinterpret_cast<UnkActor*>(player), ids, 0.0f, 0.0f);
    else
        fn_7101025560(g_unk_7101d47318, ids, 0.2f, 0.0f);
}

UnkListedObj* fn_7100e0b770(char* self);
void fn_710020c4b8(UnkToggle* self, bool value);

// A light the light manager hands out; its switch at +0x28.
struct UnkShotLight_b0011 {
    u8 pad_0x0[0x28];
    UnkToggle toggle;
};

static inline char* LightManager_b0011() {
    if (!g_unk_71726b1688) {
        void* p = fn_71000839a0(0x198, 0x3f);
        fn_7100359118(p);
        g_unk_71726b1688 = p;
    }
    return static_cast<char*>(g_unk_71726b1688);
}

// Takes a light from the light manager for each of the light list's slots, switched off.
void fn_7100ed54fc(UnkShotManager_b0011* self) {
    u32 n = self->field_0x1238.capacity;
    for (u32 i = 0; i < n; ++i) {
        UnkPtrList* list = reinterpret_cast<UnkPtrList*>(&self->field_0x1238);
        *fn_7100e0a5ec(list, i) = fn_7100e0b770(LightManager_b0011());
        UnkToggle* toggle = &reinterpret_cast<UnkShotLight_b0011*>(*fn_7100e0a5ec(list, i))->toggle;
        fn_710020c4b8(toggle, false);
        fn_710020c8ac(toggle, false);
    }
    self->field_0x1260 = 0;
}

void fn_7100ddec50(UnkShotEffect_b0011* self, u32 a, u32 b);
void fn_71011aa898(UnkModelManager* self, u64* model);

static inline UnkModelManager* ModelManager_b0011() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* p = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(p);
        g_unk_71726b0cd8 = p;
    }
    return g_unk_71726b0cd8;
}

// Stops every live effect and hands its explosion's three models to the model manager.
void fn_7100ed7144(UnkShotManager_b0011* self) {
    for (UnkShotEffect_b0011 *e = self->field_0x30.data, *end = e + self->field_0x30.count; e != end; ++e) {
        if (!e->state.live)
            continue;
        fn_7100ddec50(e, 0, 1);
        if (UnkShotGfx_b0011* gfx = self->field_0x240[e->state.type][e->state.variant]) {
            fn_71011aa898(ModelManager_b0011(), &gfx->field_0xa0);
            fn_71011aa898(ModelManager_b0011(), &gfx->field_0xa8);
            fn_71011aa898(ModelManager_b0011(), &gfx->field_0xb0);
        }
    }
}

// Destroys the 125 render instances of one list, if it has been created.
static inline void DestroyInstances_b0011(void**& instances) {
    if (!instances)
        return;
    for (u32 i = 0; i < 125; ++i) {
        if (instances[i])
            fn_7100219e10(g_unk_7101d47300, reinterpret_cast<u64>(instances[i]));
        instances[i] = nullptr;
    }
}

// Destroys every render instance the shots were drawn with.
void fn_7100ed7634(UnkShotManager_b0011* self) {
    for (u32 type = 0; type < 5; ++type) {
        for (u32 variant = 0; variant < 2; ++variant) {
            DestroyInstances_b0011(self->field_0x1a0[type][variant][0]);
            DestroyInstances_b0011(self->field_0x1a0[type][variant][1]);
        }
    }
}

// Appends a copy of an effect.
void fn_7100ede44c(base::UnkVector<UnkShotEffect_b0011>* self, const UnkShotEffect_b0011* effect) {
    self->PushBackCopy(*effect);
}

// ---- A key and a value, held or pointed at ----

struct UnkValue_b0011 {
    u8 data[0x40];
};
struct UnkSlot_0x40;

// Either a key and a value held here (kind 1) or pointers to them elsewhere (kind 2); kind 0 is
// empty. The key is stored bytewise, so the kind is read again after it.
struct UnkKeyValueRef_b0011 {
    union {
        struct {
            u64 key;
            UnkValue_b0011 value;
        } own;
        struct {
            u64* key;
            UnkValue_b0011* value;
        } ref;
    };
    u32 kind;  // 0x48

    UnkValue_b0011* Value() { return kind == 2 ? ref.value : &own.value; }
    void SetKey(u64 key) {
        u64* p = kind == 2 ? ref.key : &own.key;
        __builtin_memcpy(p, &key, sizeof(u64));
    }
};
static_assert(__builtin_offsetof(UnkKeyValueRef_b0011, kind) == 0x48, "UnkKeyValueRef_b0011 layout");

// Assigns the key and the value wherever they are, from the other's.
UnkKeyValueRef_b0011* fn_7100edc7cc(UnkKeyValueRef_b0011* self, UnkKeyValueRef_b0011* other) {
    if (self != other) {
        if (other->kind == 1) {
            self->SetKey(other->own.key);
            fn_7100edbef4(reinterpret_cast<UnkSlot_0x40*>(self->Value()), reinterpret_cast<UnkSlot_0x40*>(&other->own.value));
        } else if (other->kind == 2) {
            self->SetKey(*other->ref.key);
            UnkValue_b0011* value = other->ref.value;
            fn_7100edbef4(reinterpret_cast<UnkSlot_0x40*>(self->Value()), reinterpret_cast<UnkSlot_0x40*>(value));
        }
    }
    return self;
}

using UnkPathMember_b0011 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// A key and a file-path id held here, or (kind 2) pointers to them elsewhere.
struct UnkKeyPathRef_b0011 {
    union {
        u64 key;
        u64* key_ref;
    };
    union {
        base::global::UnkStringData* path;
        UnkPathMember_b0011* path_ref;
    };
    u32 kind;  // 0x10

    u64* Key() { return kind == 2 ? key_ref : &key; }
    UnkPathMember_b0011* Path() { return kind == 2 ? path_ref : reinterpret_cast<UnkPathMember_b0011*>(&path); }
    void SetKey(u64 value) {
        u64* p = Key();
        __builtin_memcpy(p, &value, sizeof(u64));
    }
};
static_assert(sizeof(UnkKeyPathRef_b0011) == 0x18, "UnkKeyPathRef_b0011 layout");

UnkKeyPathRef_b0011* fn_7100edad6c(UnkKeyPathRef_b0011* self, UnkKeyPathRef_b0011* other) {
    if (self != other) {
        self->SetKey(*other->Key());
        *self->Path() = *other->Path();
    }
    return self;
}

// ---- File-path ids ----

// Resets the id `it` points at in the array at `data` to the empty path.
CFilePathStrId* fn_7100edc330(CFilePathStrId* data, CFilePathStrId* it) {
    u32 index = it - data;
    data[index] = CFilePathStrId();
    return &data[index];
}

// Resets eight ids to the empty path.
void fn_7100edc1f0(CFilePathStrId* ids) {
    for (u32 i = 0; i < 8; ++i)
        ids[i] = CFilePathStrId();
}

// ---- A callback bound to an object ----

struct UnkFunctor_b0011 {
    u8 data[0x40];
};

// An id and an object, and either a held functor (kind 1) or a binding to the object (kind 2: a
// pointer back to the callback and the object).
struct UnkCallback_b0011 {
    u64 id;
    void* object;
    UnkCallback_b0011* owner;  // 0x10
    union {
        void* bound;               // 0x18
        UnkFunctor_b0011 functor;  // 0x18
    };
    u32 kind;  // 0x58

    UnkCallback_b0011(u64 id_, void* object_) : id(id_), object(object_) {
        kind = 2;
        owner = this;
        bound = object_;
    }
    ~UnkCallback_b0011() { Reset(); }

    void Reset() {
        if (kind == 1) {
            fn_7100ed977c(reinterpret_cast<UnkSlot_0x40*>(&functor));
            kind = 0;
        } else if (kind == 2) {
            kind = 0;
        }
    }

    UnkCallback_b0011& operator=(UnkCallback_b0011&& other) {
        if (this != &other) {
            id = other.id;
            object = other.object;
            void* b = other.object;
            Reset();
            if (b) {
                kind = 2;
                owner = this;
                bound = b;
            }
        }
        return *this;
    }
};
static_assert(__builtin_offsetof(UnkCallback_b0011, kind) == 0x58, "UnkCallback_b0011 layout");

struct UnkCallbackOwner_b0011 {
    u8 pad_0x0[0x3c0];
    u8 field_0x3c0[0x8];
};

// Binds the callback to the object at +0x3c0, with the id 0xf.
void fn_7100edd1bc(UnkCallbackOwner_b0011* self, UnkCallback_b0011* callback) {
    *callback = UnkCallback_b0011(0xf, self->field_0x3c0);
}

// ---- Growable arrays ----

// A reference-counted object: the count at +8, destroyed through its vf_0x10 when it drops to zero.
struct UnkRefCounted_b0011 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys
    // A union member: Clang gives union accesses no type-based alias info, so the pointer is
    // reloaded after the count changes, as in the original.
    union {
        u32 ref_count;
    };
};

// A counted pointer; a copy takes the other's object when it has one.
struct UnkRefPtr_b0011 {
    UnkRefCounted_b0011* ptr;

    UnkRefPtr_b0011(const UnkRefPtr_b0011& other) : ptr(nullptr) {
        if (UnkRefCounted_b0011* p = other.ptr) {
            ++p->ref_count;
            if (UnkRefCounted_b0011* old = ptr) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            ptr = p;
        }
    }
};

// Growable arrays (base::UnkVector's layout) whose Reserve stays out of line; copied by
// reserving the other's capacity and appending each element with PushBack's growth.
template <class T>
struct UnkCopyVec_b0011 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reserve(u32 n);

    __attribute__((always_inline)) void PushBackCopy(const T& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        new (&data[count]) T(value);
        ++count;
    }

    __attribute__((always_inline)) void CopyFrom(const UnkCopyVec_b0011& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(other.capacity);
        for (const T *p = other.data, *e = p + other.count; p != e; ++p)
            PushBackCopy(*p);
    }
};

struct UnkRefRecord10_b0011 {
    UnkRefPtr_b0011 ref;
    union {  // stored without type-based alias info: the count is read again after it
        u32 field_0x8;
    };
};
using UnkRefRecord10Vec_b0011 = UnkCopyVec_b0011<UnkRefRecord10_b0011>;
void fn_7100ecf4c8(UnkRefRecord10Vec_b0011* self, u32 n);
template <>
inline void UnkRefRecord10Vec_b0011::Reserve(u32 n) {
    fn_7100ecf4c8(this, n);
}

struct UnkRefRecord18_b0011 {
    UnkRefPtr_b0011 ref;
    u64 field_0x8;
    u32 field_0x10;
};
using UnkRefRecord18Vec_b0011 = UnkCopyVec_b0011<UnkRefRecord18_b0011>;
void fn_710085c338(UnkRefRecord18Vec_b0011* self, u32 n);
template <>
inline void UnkRefRecord18Vec_b0011::Reserve(u32 n) {
    fn_710085c338(this, n);
}

void fn_7100ede32c(UnkRefRecord10Vec_b0011* self, const UnkRefRecord10Vec_b0011* other) { self->CopyFrom(*other); }

void fn_7100ede1f0(UnkRefRecord18Vec_b0011* self, const UnkRefRecord18Vec_b0011* other) { self->CopyFrom(*other); }

struct UnkShotWeakTarget_b0011;
struct UnkShotWeakRefData_b0011 {
    UnkShotWeakTarget_b0011* target;
    s64 generation;
};
// A weak reference (the manager's +0x1210 holds them): alive while the target's generation is this
// one's. Copied as a block.
struct UnkShotWeakRef_b0011 {
    UnkShotWeakRefData_b0011 ref;

    UnkShotWeakRef_b0011(const UnkShotWeakRef_b0011& other) : ref(other.ref) {}
};

// Copy assignment: cleared, reserved to the other's capacity, and each element appended.
base::UnkVector<UnkShotWeakRef_b0011>* fn_7100eddaac(base::UnkVector<UnkShotWeakRef_b0011>* self,
                                                     const base::UnkVector<UnkShotWeakRef_b0011>* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        for (const UnkShotWeakRef_b0011 *p = other->data, *e = p + other->count; p != e; ++p)
            self->PushBackCopy(*p);
    }
    return self;
}

// The shots (src/game/CShotManager/CTunableShotManager_Resize_b43.cpp).
struct UnkObj290_b43a3 {
    virtual void vf_0x0();  // destroys
    u8 pad_0x8[0x288];
};
static_assert(sizeof(UnkObj290_b43a3) == 0x290, "UnkObj290_b43a3 layout");

// Reallocates to n shots, moving each over and destroying the old one.
static inline void ReallocShots_b0011(base::UnkVector<UnkObj290_b43a3>* self, u32 n) {
    UnkObj290_b43a3* p =
        static_cast<UnkObj290_b43a3*>(self->allocator->vf_0x40((u64)n * sizeof(UnkObj290_b43a3), 8, self->flags));
    if (UnkObj290_b43a3* old = self->data) {
        UnkObj290_b43a3* dst = p;
        for (u32 i = 0; i < self->count; ++i) {
            fn_7100eddc68(dst, old);
            old->vf_0x0();
            ++dst;
            ++old;
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = n;
}

// Copy assignment of the shots: each is destroyed, then the array is grown to the other's
// capacity and the other's shots appended (with PushBack's growth).
base::UnkVector<UnkObj290_b43a3>* fn_7100edd784(base::UnkVector<UnkObj290_b43a3>* self,
                                               base::UnkVector<UnkObj290_b43a3>* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].vf_0x0();
        self->count = 0;
        u32 capacity = other->capacity;
        if (self->capacity < capacity)
            ReallocShots_b0011(self, capacity);
        for (UnkObj290_b43a3 *p = other->data, *e = p + other->count; p != e; ++p) {
            u32 n = self->count;
            u32 old_capacity = self->capacity;
            u32 new_capacity = old_capacity;
            if (old_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            if (old_capacity < new_capacity) {
                ReallocShots_b0011(self, new_capacity);
                n = self->count;
            }
            fn_7100eddc68(&self->data[n], p);
            ++self->count;
        }
    }
    return self;
}
