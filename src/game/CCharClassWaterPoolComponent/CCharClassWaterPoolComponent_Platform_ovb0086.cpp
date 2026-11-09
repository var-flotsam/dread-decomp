// CCharClassWaterPoolComponent's unit (tools/tu_map.py): the copy assignment of the callback-set array
// iterator's cached value (fn_710090c404), the start that links the pool's platform through its weak
// handle (fn_7100909368) and the component's destructor (fn_7100908424). Names and layouts are
// placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A callback set element as the iterator's cached value holds it (the UnkCallbackSetElem of
// fn_710090dbb4.cpp): a vtable, the callbacks, a word, the owner handle and the element's own word.
struct UnkWaterPoolElem_b0086 {
    void* vtable;
    UnkErasedVector callbacks;  // 0x8
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    u32 field_0x40;
};
static_assert(__builtin_offsetof(UnkWaterPoolElem_b0086, field_0x40) == 0x40, "UnkWaterPoolElem_b0086 layout");

// The cached (index, element) value: empty (0), owned (1) or referring to an iterator's (2).
struct UnkWaterPoolIterValue_b0086 {
    union {
        struct {
            u32 index;
            UnkWaterPoolElem_b0086 element;
        } value;
        struct {
            u32* index;
            UnkWaterPoolElem_b0086* element;
        } ref;
    };
    u32 state;  // 0x50

    u32& Index() { return state == 2 ? *ref.index : value.index; }
    UnkWaterPoolElem_b0086& Element() { return state == 2 ? *ref.element : value.element; }
};

UnkWaterPoolIterValue_b0086* fn_710090c404(UnkWaterPoolIterValue_b0086* self, UnkWaterPoolIterValue_b0086* other) {
    if (self == other)
        return self;
    self->Index() = other->Index();
    UnkWaterPoolElem_b0086& src = other->Element();
    UnkWaterPoolElem_b0086& dst = self->Element();
    const base::UnkRefHandle* src_owner = &src.owner;
    fn_710031d208(&dst.owner, src_owner);
    dst.field_0x30 = src.field_0x30;
    fn_710023b29c(&dst.callbacks, &src.callbacks);
    dst.field_0x40 = src.field_0x40;
    return self;
}

struct UnkWaterPoolPlatform_b0086 : UnkWeakTarget {};

struct UnkWaterPoolComponent_b0086 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual UnkVec3f vf_0x178();
    u8 pad_0x8[0x28];
    void* field_0x30;
    u8 pad_0x38[0xc88 - 0x38];
    UnkWeakHandle* platform;  // 0xc88
};

void fn_7100900c44(UnkWaterPoolComponent_b0086* self);
UnkWaterPoolPlatform_b0086* fn_71008fbfc0(void* source);
void fn_7100905bdc(UnkWaterPoolPlatform_b0086* platform, f32 value, bool flag);

void fn_7100909368(UnkWaterPoolComponent_b0086* self) {
    fn_7100900c44(self);
    UnkWaterPoolPlatform_b0086* platform = fn_71008fbfc0(self->field_0x30);
    if (!platform)
        return;
    fn_7100905bdc(platform, self->vf_0x178().x, false);
    AssignRef(&self->platform, GetWeakHandle(platform));
}

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a80ed8[];  // CCharClassWaterPoolComponent's vtable
void fn_7100900414(UnkBaseComponent* self);
struct UnkCallbackSetVector;
void fn_710090d2f4(UnkCallbackSetVector* v, u32 size);  // Truncate

// The collision manager, created on first use (as src/game/CSubAreaManager.cpp declares it).
struct UnkManager_7100863a8c;
extern UnkManager_7100863a8c* g_unk_71726a4788;
void fn_7100d46804(UnkManager_7100863a8c* self);  // constructor
struct UnkCollisionObject;
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* obj);

static inline UnkManager_7100863a8c* CollisionManager() {
    if (!g_unk_71726a4788) {
        UnkManager_7100863a8c* m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return g_unk_71726a4788;
}

// An engine array (base::UnkVector's layout) whose elements need no destructor here.
struct UnkWaterPoolArray_b0086 {
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;  // +0x20
};

// Releases a string id in two steps (the id and its count reloaded after the decrement).
static inline void ReleaseId(UnkStringData** id) {
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
}

struct UnkWaterPoolComponent2_b0086 : UnkBaseComponent {
    u8 pad_0x40[0xc90 - 0x40];
    UnkWaterPoolArray_b0086 callbacks;  // 0xc90, the callback sets (fn_710090dbb4.cpp)
    u8 pad_0xcb8[0x10];
    UnkStringData* field_0xcc8;  // CStrId
    UnkStrIdTable field_0xcd0;
    u8 pad_0xcf8[8];
    UnkWaterPoolArray_b0086 field_0xd00;
    UnkStringData* field_0xd28;  // CStrId
    u8 pad_0xd30[8];
    UnkCollisionObject* collision;  // 0xd38
};
static_assert(__builtin_offsetof(UnkWaterPoolComponent2_b0086, collision) == 0xd38, "layout");

// The destructor: the collision object is handed back to the collision manager, the members are
// released in reverse order, then the base destructor runs.
void fn_7100908424(UnkWaterPoolComponent2_b0086* self) {
    self->vtable = g_unk_7101a80ed8 + 2;
    if (self->collision) {
        fn_7100d46c60(CollisionManager(), self->collision);
        self->collision = nullptr;
    }
    ReleaseId(&self->field_0xd28);
    if (self->field_0xd00.data) {
        self->field_0xd00.count = 0;
        self->field_0xd00.allocator->vf_0x48(self->field_0xd00.data);
        self->field_0xd00.capacity = 0;
    }
    fn_71004f11f8(&self->field_0xcd0);
    if (self->field_0xcd0.keys)
        self->field_0xcd0.allocator->vf_0x48(self->field_0xcd0.keys);
    if (self->field_0xcd0.values)
        self->field_0xcd0.allocator->vf_0x48(self->field_0xcd0.values);
    ReleaseId(&self->field_0xcc8);
    if (self->callbacks.data) {
        fn_710090d2f4(reinterpret_cast<UnkCallbackSetVector*>(&self->callbacks), 0);
        self->callbacks.allocator->vf_0x48(self->callbacks.data);
        self->callbacks.capacity = 0;
    }
    fn_7100900414(self);
}
