// Unit 267's property setter (tools/tu_map.py; no class hint): copies a value into the field its
// reflected type names (a 12-byte vector, flags set from a float, the second vector, a flag raised
// from the third one). The type objects are the unit's own (pointers set by its static
// initialiser). Names and layouts are placeholders.
#include "types.h"

struct UnkTypeInfo_ovb0259 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

extern UnkTypeInfo_ovb0259* g_unk_716e5969d0 __attribute__((visibility("hidden")));
extern UnkTypeInfo_ovb0259* g_unk_716e5969c0 __attribute__((visibility("hidden")));
extern UnkTypeInfo_ovb0259* g_unk_716e5969d8 __attribute__((visibility("hidden")));
extern UnkTypeInfo_ovb0259* g_unk_716e5969c8 __attribute__((visibility("hidden")));
extern UnkTypeInfo_ovb0259* g_unk_716e5969e0 __attribute__((visibility("hidden")));

// A 12-byte value copied as a word and a half word.
static inline void Copy12(void* dst, const void* src) {
    u64 lo;
    u32 hi;
    __builtin_memcpy(&lo, src, 8);
    __builtin_memcpy(&hi, static_cast<const char*>(src) + 8, 4);
    __builtin_memcpy(dst, &lo, 8);
    __builtin_memcpy(static_cast<char*>(dst) + 8, &hi, 4);
}

struct UnkProps_ovb0259 {
    u8 pad_0x0[0x140];
    u8 first[12];  // 0x140
    u8 second[12];  // 0x14c
    u8 third[12];  // 0x158
    u32 field_0x164;
    bool field_0x168;
    bool field_0x169;
    bool field_0x16a;
    u8 pad_0x16b[4];
    bool field_0x16f;
};
static_assert(__builtin_offsetof(UnkProps_ovb0259, field_0x16f) == 0x16f, "UnkProps_ovb0259 layout");

void fn_71001e8e7c(UnkProps_ovb0259* self, void* type, const f32* value) {
    if (__builtin_expect(g_unk_716e5969d0->field_0x30 == type, 1)) {
        Copy12(self->first, value);
        return;
    }
    if (__builtin_expect(g_unk_716e5969c0->field_0x30 == type, 1)) {
        self->field_0x168 = value[0] > 0.1f;
        return;
    }
    if (__builtin_expect(g_unk_716e5969d8->field_0x30 == type, 1)) {
        Copy12(self->second, value);
        return;
    }
    if (__builtin_expect(g_unk_716e5969c8->field_0x30 == type, 1)) {
        f32 v = value[0];
        self->field_0x169 = v > 0.5f && v < 1.5f;
        self->field_0x16a = v > 1.5f && v < 2.5f;
        return;
    }
    if (__builtin_expect(g_unk_716e5969e0->field_0x30 == type, 1)) {
        f32 v = value[0];
        bool in_range = (v > 0.5f) & (v < 1.5f);
        u8 prev = self->field_0x16f;
        // the original compares the value again for the reset rather than reusing the flag
        f32 again = v;
        asm("" : "+w"(again));
        if (!prev) {
            if (again > 0.5f && again < 1.5f)
                self->field_0x164 = 0;
        }
        self->field_0x16f = in_range;
        Copy12(self->third, value + 1);
    }
}
