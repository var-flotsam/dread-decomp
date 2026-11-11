// From unit 1123 of tools/tu_map.py (0x71007032cc-0x710070368c, no reflected class), with the same
// handler from units 1122 and 1125: each keeps the optional settings of the parameter blocks it
// receives (kind 0 and its own kind) after passing them to the shared step fn_71007038f4 (the
// sending side of unit 1123, fn_71007033c8, is not matched yet). Names and layouts are placeholders.
#include "game/CEnemyMovement.h"
#include "game/UnkTU1124.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkVec3 {
    u32 x;
    u32 y;
    u32 z;
};

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkWords4 {
    u64_a4 lo;
    u64_a4 hi;
};

template <class T>
struct UnkOpt {
    T value;
    bool has;
};

struct UnkOptByte {
    u8 value;
    bool has;
};

// The parameter block: the settings every kind carries, the kind, then the kind's own (Tail).
template <class Tail>
struct UnkHandlerParams {
    void** vtable;
    UnkOpt<UnkVec3> field_0x8;
    UnkOpt<u32> field_0x18;
    UnkOpt<UnkWords4> field_0x20;
    u32 kind;  // 0x34
    Tail tail;  // 0x38
};

// The handler: the unit 1124 object (fn_7100703814 constructs it) and the kept settings, a
// parameter block of the handler's own kind.
template <class Tail>
struct UnkHandler {
    void** vtable;
    u8 pad_0x8[0xa8];
    UnkHandlerParams<Tail> params;  // 0xb0
};

struct UnkBytes3 {
    UnkOptByte field_0x0;
    UnkOptByte field_0x2;
    UnkOptByte field_0x4;
};

struct UnkBytes1 {
    UnkOptByte field_0x0;
};

struct UnkWordVec {
    UnkOpt<u32> field_0x0;
    UnkOpt<UnkVec3> field_0x8;
};

// Its first argument is the handler, its second the block.

template <class Tail>
__attribute__((always_inline)) static inline void TakeCommon(UnkHandler<Tail>* self, UnkHandlerParams<Tail>* params,
                                                             bool reset) {
    if (reset) {
        self->params.field_0x8.has = false;
        self->params.field_0x18.has = false;
        self->params.field_0x20.has = false;
    }
    if (params->field_0x8.has) {
        self->params.field_0x8.value = params->field_0x8.value;
        self->params.field_0x8.has = true;
    }
    if (params->field_0x18.has) {
        self->params.field_0x18.value = params->field_0x18.value;
        self->params.field_0x18.has = true;
    }
    if (params->field_0x20.has) {
        self->params.field_0x20.value = params->field_0x20.value;
        self->params.field_0x20.has = true;
    }
}

template <class T>
__attribute__((always_inline)) static inline void Take(T* dst, const T* src) {
    if (src->has) {
        dst->value = src->value;
        dst->has = true;
    }
}

// Kind 5 (unit 1123): three optional bytes.
u64 fn_71007034e8(UnkHandler<UnkBytes3>* self, UnkHandlerParams<UnkBytes3>* params, bool reset) {
    u64 result = fn_71007038f4(self, params, reset);
    if (params->kind == 5 || params->kind == 0) {
        TakeCommon(self, params, reset);
        if (reset) {
            self->params.tail.field_0x0.has = false;
            self->params.tail.field_0x2.has = false;
            self->params.tail.field_0x4.has = false;
        }
        if (params->kind == self->params.kind) {
            Take(&self->params.tail.field_0x0, &params->tail.field_0x0);
            Take(&self->params.tail.field_0x2, &params->tail.field_0x2);
            Take(&self->params.tail.field_0x4, &params->tail.field_0x4);
        }
    }
    return result;
}

// Kind 6 (unit 1122): one optional byte.
u64 fn_7100703164(UnkHandler<UnkBytes1>* self, UnkHandlerParams<UnkBytes1>* params, bool reset) {
    u64 result = fn_71007038f4(self, params, reset);
    if (params->kind == 6 || params->kind == 0) {
        TakeCommon(self, params, reset);
        if (reset)
            self->params.tail.field_0x0.has = false;
        if (params->kind == self->params.kind)
            Take(&self->params.tail.field_0x0, &params->tail.field_0x0);
    }
    return result;
}

// Kind 8 (unit 1125): an optional word and an optional vector.
u64 fn_71007047d0(UnkHandler<UnkWordVec>* self, UnkHandlerParams<UnkWordVec>* params, bool reset) {
    u64 result = fn_71007038f4(self, params, reset);
    if (params->kind == 8 || params->kind == 0) {
        TakeCommon(self, params, reset);
        if (reset) {
            self->params.tail.field_0x0.has = false;
            self->params.tail.field_0x8.has = false;
        }
        if (params->kind == self->params.kind) {
            Take(&self->params.tail.field_0x0, &params->tail.field_0x0);
            Take(&self->params.tail.field_0x8, &params->tail.field_0x8);
        }
    }
    return result;
}

struct UnkUnit1124Object;

extern void* g_unk_7101a6bcd8[];  // the kind-5 handler's vtable

// The kind-5 handler's constructor: nothing kept yet.
void fn_71007032cc(UnkHandler<UnkBytes3>* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    UnkHandlerParams<UnkBytes3>* params = &self->params;
    params->field_0x18.has = false;
    __builtin_memset(&params->field_0x20.value, 0, 16);
    params->vtable = g_unk_7101a25b10 + 2;
    self->vtable = g_unk_7101a6bcd8 + 2;
    params->field_0x20.has = false;
    params->tail.field_0x0.has = false;
    params->tail.field_0x2.has = false;
    params->tail.field_0x4.has = false;
    __builtin_memset(&params->field_0x8, 0, 13);
    params->kind = 5;
}

// The block the kind-5 handler sends to its owner's effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkSinkParams {
    void** vtable;
    UnkOpt<u32> field_0x8;
    UnkOpt<f32> field_0x10;
    UnkOpt<UnkVec3> field_0x18;
    UnkOpt<UnkVec3> field_0x28;
    u32 kind;  // 0x38
};

extern void* g_unk_7101a24b80[];

struct UnkEffectSink;

struct UnkHandlerOwner {
    u8 pad_0x0[0x218];
    UnkEffectSink* field_0x218;
};

// Sends the sink a block with the largest float as its value at +0x10.
void fn_7100703358(UnkHandler<UnkBytes3>* self) {
    UnkEffectSink* sink = (*reinterpret_cast<UnkHandlerOwner**>(reinterpret_cast<char*>(self) + 0x30))->field_0x218;
    if (!sink)
        return;
    UnkSinkParams params;
    params.vtable = g_unk_7101a24b80 + 2;
    params.field_0x8.has = false;
    params.field_0x10.has = false;
    params.kind = 0;
    __builtin_memset(&params.field_0x18, 0, 13);
    __builtin_memset(&params.field_0x28, 0, 13);
    params.field_0x10.value = 3.4028235e+38f;
    params.field_0x10.has = true;
    fn_71010c7e6c(sink, &params, 0);
}
