// For every active entry whose target matches the given id, asks the manager to fade it out over
// the given time, or over the entry's own time when the given time is negative.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStrBody_0x38 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkHandle {
    UnkStrBody_0x38* field_0x0;
};

struct UnkEntry_0x60 {
    u8 pad_0x0[0x8];
    f32 field_0x8;
    u8 pad_0xc[0x1c];
    UnkHandle* field_0x28;
    u32 field_0x30;
    u8 pad_0x34[0x2c];
};

struct UnkArray {
    UnkEntry_0x60* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    void Reserve(u32 n);   // fn_7100fddcb4
    void Resize(u32 n);    // fn_7100fde4d8

    UnkEntry_0x60& operator[](u64 i);
};

void fn_7100fddcb4(UnkArray*, u32);
void fn_7100fde4d8(UnkArray*, u32);

inline UnkEntry_0x60& UnkArray::operator[](u64 i) {
    if (autoGrow && i >= count) {
        u32 n = capacity;
        if (i >= capacity && step != 0)
            n = ((u32)i / step + 1) * step;
        fn_7100fddcb4(this, n);
        fn_7100fde4d8(this, i + 1);
    }
    return data[i];
}

struct UnkStruct_0xd0 {
    u8 pad_0x0[0xb8];
    UnkArray field_0xb8;
};

struct UnkMgr;

extern UnkMgr* g_unk_7101d4d820;

void fn_7100108abc(UnkMgr*, UnkHandle*, f32, f32);

static inline f32 Max(f32 a, f32 b) { return a < b ? b : a; }

void fn_7100fdd2b4(UnkStruct_0xd0* self, u64* id, f32 time) {
    UnkArray& arr = self->field_0xb8;
    for (u64 i = 0; i < arr.count; ++i) {
        UnkEntry_0x60& e = arr.data[i];
        if (e.field_0x30 == 0)
            continue;
        UnkHandle* h = e.field_0x28;
        if (h->field_0x0 == (UnkStrBody_0x38*)((char*)g_unk_7101d47200 + 0x10))
            continue;
        if (h->field_0x0->field_0x30 != *id)
            continue;
        f32 t = time >= 0.0f ? time : arr[i].field_0x8;
        fn_7100108abc(g_unk_7101d4d820, h, 0.2f, Max(t, 0.001f));
    }
}
