// Destructor body of the object whose state machine fn_7100a9ea64 sets up: detaches its
// listener, releases its members, destroys the seven states and the state machine, and runs
// the base destructor.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHandle {
    u8 unk_0x0[0x10];
    void* target;
};

struct UnkDelegate {
    void* object;
    void* function;

    ~UnkDelegate() {
        function = nullptr;
        object = nullptr;
    }
};

struct UnkState {
    void** vtable;
    u8 unk_0x8[0x10];
    base::reflection::UnkReflValue field_0x18;
    UnkDelegate delegates[4];
};
static_assert(sizeof(UnkState) == 0x70, "UnkState layout");

struct UnkStateObject {
    void** vtable;
    u8 unk_0x8[0x38];
    u8 field_0x40[0xa0];
    UnkRefObject* field_0xe0;
    UnkRefObject* field_0xe8;
    u8 unk_0xf0[0x10];
    void** machine_vtable;  // the state machine at +0x100
    u8 unk_0x108[0x8];
    u64 field_0x110;
    u64 field_0x118;
    u32 field_0x120;
    u8 unk_0x124[0x8c];
    UnkState states[7];
    u8 unk_0x4c0[0x8];
    UnkStringData* field_0x4c8;  // CStrId
    void* field_0x4d0;
    void* field_0x4d8;
    u8 unk_0x4e0[0x10];
    base::UnkAllocator* field_0x4f0;
    u8 unk_0x4f8[0x18];
    UnkRefObject* field_0x510;
    void** listener_vtable;  // a listener at +0x518
    u8 unk_0x520[0x10];
    UnkRefObject* field_0x530;
};
static_assert(__builtin_offsetof(UnkStateObject, states) == 0x1b0, "layout");
static_assert(__builtin_offsetof(UnkStateObject, field_0x530) == 0x530, "layout");

extern void* g_unk_7101a94220[];
extern void* g_unk_7101a77000[];
extern void* g_unk_7101a946c0[];
extern void* g_unk_7101a94718[];
void fn_7100a2d4d0(void* target, void* listener);
void fn_71004f11f8(void* self);
void fn_7100431cec(void* self);
void fn_71010c4194(UnkStateObject* self);

static inline void Release(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

static inline void DestroyState(UnkState* state) {
    for (int i = 3; i >= 0; --i)
        state->delegates[i].~UnkDelegate();
    state->vtable = &g_unk_7101a946c0[2];
    fn_71000a1c60(&state->field_0x18);
}

void fn_7100a9e6bc(UnkStateObject* self) {
    self->vtable = &g_unk_7101a94220[2];
    self->listener_vtable = &g_unk_7101a77000[2];
    if (UnkRefObject* handle = self->field_0x530) {
        if (void* target = reinterpret_cast<UnkHandle*>(handle)->target)
            fn_7100a2d4d0(target, &self->listener_vtable);
    }
    Release(self->field_0x530);
    Release(self->field_0x510);
    fn_71004f11f8(&self->field_0x4d0);
    if (self->field_0x4d8)
        self->field_0x4f0->vf_0x48(self->field_0x4d8);
    if (self->field_0x4d0)
        self->field_0x4f0->vf_0x48(self->field_0x4d0);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->field_0x4c8;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    for (int i = 6; i >= 0; --i)
        DestroyState(&self->states[i]);
    self->machine_vtable = &g_unk_7101a94718[2];
    self->field_0x110 = 0;
    self->field_0x118 = 0;
    self->field_0x120 = 0;
    Release(self->field_0xe8);
    Release(self->field_0xe0);
    fn_7100431cec(self->field_0x40);
    fn_71010c4194(self);
}
