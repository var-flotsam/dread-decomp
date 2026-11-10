// Unit 212 (a node with a transform and an id list, no reliable name): its default and move
// constructors, and the refresh of its matrices from the parent's. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdInline;

// Zeroed, then the diagonal set in reverse (the order the original stores).
static inline void SetIdentity(UnkMatrix44* m) {
    for (u32 i = 0; i < 16; ++i)
        m->m[i] = 0.0f;
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

struct UnkNode_b0253 {
    u64 field_0x0;   // 0x0
    u64 field_0x8;   // 0x8
    u64 field_0x10;  // 0x10
    UnkMatrix44 matrix;  // 0x18
    void* field_0x58;  // 0x58
    void* field_0x60;  // 0x60
    CStrId id;  // 0x68
    base::UnkVector<void*> list;  // 0x70
};
static_assert(__builtin_offsetof(UnkNode_b0253, id) == 0x68, "UnkNode_b0253 layout");
static_assert(__builtin_offsetof(UnkNode_b0253, list) == 0x70, "UnkNode_b0253 layout");
static_assert(sizeof(UnkNode_b0253) == 0x98, "UnkNode_b0253 layout");

void fn_7100194a9c(UnkNode_b0253* self) {
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    SetIdentity(&self->matrix);
    self->field_0x58 = nullptr;
    self->field_0x60 = nullptr;
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    new (&self->list) base::UnkVector<void*>(10);
}

void fn_7100194ba4(UnkNode_b0253* self, UnkNode_b0253* other) {
    self->field_0x0 = other->field_0x0;
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    SetIdentity(&self->matrix);
    self->field_0x58 = other->field_0x58;
    self->field_0x60 = other->field_0x60;
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    self->list.data = other->list.data;
    self->list.count = other->list.count;
    self->list.capacity = other->list.capacity;
    self->list.step = other->list.step;
    self->list.field_0x14 = other->list.field_0x14;
    self->list.flags = other->list.flags;
    self->list.allocator = other->list.allocator;
    __builtin_memset(&other->list, 0, 0x10);
    __builtin_memset(&other->field_0x0, 0, 0x18);
    __builtin_memset(&other->field_0x58, 0, 0x10);
}

struct UnkXformOwner_b0253 {
    u8 pad_0x0[0x18];
    UnkMatrix44 matrix;  // 0x18
};

struct UnkXformNode_b0253 {
    UnkXformOwner_b0253* parent;  // 0x0
    u8 pad_0x8[0x8];
    const UnkMatrix44* local;  // 0x10
    UnkMatrix44 matrix;  // 0x18
    u8 pad_0x58[0x0];
    const UnkMatrix44* other;  // 0x58
    UnkMatrix44* out;  // 0x60
};
static_assert(__builtin_offsetof(UnkXformNode_b0253, out) == 0x60, "UnkXformNode_b0253 layout");

void fn_7100195114(UnkXformNode_b0253* self) {
    if (self->parent) {
        UnkMatrix44 m = fn_71000127c8(self->parent->matrix, *self->local);
        UnkCopyMatrix(&self->matrix, m);
    } else {
        UnkCopyMatrix(&self->matrix, *self->local);
    }
    if (self->out) {
        UnkMatrix44 m = fn_71000127c8(self->matrix, *self->other);
        UnkCopyMatrix(self->out, m);
    }
}
