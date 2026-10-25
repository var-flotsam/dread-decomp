// Fifteen functions with one body: sample a float from every entry of the owner's table into a
// local list, gather it with spans from the owner and the caller's pair into a parameter
// block, and hand that to a handler (one per function) unless the manager declines; five pass it to a
// second handler then.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkOffsetTable {
    u8 unk_0x0[0x8];
    u32 count;
    u32 offsets[1];  // relative to &offsets[0]

    void* Get(u32 i) { return reinterpret_cast<u8*>(offsets) + offsets[i]; }
};

struct UnkTableOwner {
    u8 unk_0x0[0x60];
    UnkOffsetTable* table;
};

struct UnkSpan {
    void* data;
    u32 count;
};

struct UnkSampleSource {
    u8 unk_0x0[0x8];
    UnkSpan field_0x8;
    u8 unk_0x18[0x18];
    UnkSpan field_0x30;
    u8 unk_0x40[0x18];
    UnkTableOwner* owner;
};

struct UnkPair {
    u64 a;
    u32 b;
};

struct UnkSampler {
    UnkSampleSource* source;
    f32 field_0x8;
    UnkPair field_0x10;
};

struct UnkFloatList {
    u32 count;
    f32 values[17];
};

// A span with a 64-bit count, as passed to the handlers.
struct UnkWideSpan {
    void* data = nullptr;
    u64 count = 0;

    UnkWideSpan() = default;
    UnkWideSpan(void* data, u64 count) : data(data), count(count) {}
    UnkWideSpan(const UnkSpan& span) : data(span.data), count(span.count) {}
};

struct UnkTail {
    u64 a = 0;
    u32 b = 0;
};

struct UnkSampleParams {
    UnkWideSpan values;
    UnkWideSpan field_0x10;
    UnkWideSpan field_0x20;
    UnkTail field_0x30;
};

bool fn_7100deabe4(UnkGameManager* manager);
bool fn_7100deac04(UnkGameManager* manager);
f32 fn_7100027910(void* entry, f32 value);

// Without Fallback the parameters are dropped when the manager declines.
template <void (*Handler)(UnkSampleParams*), void (*Fallback)(UnkSampleParams*) = nullptr>
static inline void SampleAndSend(UnkSampler* self) {
    UnkTableOwner* owner = self->source->owner;
    UnkFloatList list;
    list.count = 0;
    for (u64 i = 0; i < owner->table->count;) {
        list.values[i] = fn_7100027910(owner->table->Get(i), self->field_0x8);
        list.count = ++i;
    }
    UnkSampleParams params;
    params.values = UnkWideSpan(list.values, list.count);
    params.field_0x10 = UnkWideSpan(self->source->field_0x8);
    params.field_0x20 = UnkWideSpan(self->source->field_0x30);
    __builtin_memcpy(&params.field_0x30, &self->field_0x10, sizeof(UnkTail));
    if (!fn_7100deabe4(g_unk_7101d47310) || fn_7100deac04(g_unk_7101d47310))
        Handler(&params);
    else if (Fallback)
        Fallback(&params);
}

void fn_7100c81264(UnkSampleParams* params);
void fn_7100c80924(UnkSampleParams* params);
void fn_7100c7f13c(UnkSampleParams* params);
void fn_7100c7edec(UnkSampleParams* params);
void fn_7100c7ebe4(UnkSampleParams* params);
void fn_7100c7b4ec(UnkSampleParams* params);
void fn_7100c7e2a4(UnkSampleParams* params);
void fn_7100c7ffe4(UnkSampleParams* params);
void fn_7100c7b8bc(UnkSampleParams* params);
void fn_7100c7b33c(UnkSampleParams* params);
void fn_7100c7f50c(UnkSampleParams* params);
void fn_7100c7f5c8(UnkSampleParams* params);
void fn_7100c7c404(UnkSampleParams* params);
void fn_7100c7c458(UnkSampleParams* params);
void fn_7100c7d894(UnkSampleParams* params);
void fn_7100c7d8e8(UnkSampleParams* params);
void fn_7100c7ce14(UnkSampleParams* params);
void fn_7100c7cea4(UnkSampleParams* params);
void fn_7100c7b9bc(UnkSampleParams* params);
void fn_7100c7ba2c(UnkSampleParams* params);

void fn_7100c79a80(UnkSampler* self) { SampleAndSend<fn_7100c81264>(self); }
void fn_7100c79b80(UnkSampler* self) { SampleAndSend<fn_7100c80924>(self); }
void fn_7100c79c80(UnkSampler* self) { SampleAndSend<fn_7100c7f13c>(self); }
void fn_7100c79d80(UnkSampler* self) { SampleAndSend<fn_7100c7edec>(self); }
void fn_7100c79e80(UnkSampler* self) { SampleAndSend<fn_7100c7ebe4>(self); }
void fn_7100c79f80(UnkSampler* self) { SampleAndSend<fn_7100c7b4ec>(self); }
void fn_7100c7a080(UnkSampler* self) { SampleAndSend<fn_7100c7e2a4>(self); }
void fn_7100c7a180(UnkSampler* self) { SampleAndSend<fn_7100c7ffe4>(self); }
void fn_7100c7a5b0(UnkSampler* self) { SampleAndSend<fn_7100c7b8bc>(self); }
void fn_7100c7a7c0(UnkSampler* self) { SampleAndSend<fn_7100c7b33c>(self); }

void fn_7100c79970(UnkSampler* self) { SampleAndSend<fn_7100c7f50c, fn_7100c7f5c8>(self); }
void fn_7100c7a280(UnkSampler* self) { SampleAndSend<fn_7100c7c404, fn_7100c7c458>(self); }
void fn_7100c7a390(UnkSampler* self) { SampleAndSend<fn_7100c7d894, fn_7100c7d8e8>(self); }
void fn_7100c7a4a0(UnkSampler* self) { SampleAndSend<fn_7100c7ce14, fn_7100c7cea4>(self); }
void fn_7100c7a6b0(UnkSampler* self) { SampleAndSend<fn_7100c7b9bc, fn_7100c7ba2c>(self); }
