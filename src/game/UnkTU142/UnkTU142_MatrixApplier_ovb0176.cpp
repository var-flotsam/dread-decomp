// Unit 142 (no reflected class): an object that applies a matrix to a keyed pose entry. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "types.h"

struct UnkKeyedEntry;

extern char g_unk_7101a1cd78[];

struct UnkMatrixApplier_ovb0176 {
    void* vtable;  // 0x0
    f32 weight;  // 0x8
    u8 pad_0xc[4];
    void* field_0x10;
    UnkMatrix44 matrix;  // 0x18
    u64 key;  // 0x58
    u8 field_0x60[4];
    u8 field_0x64;
    u8 field_0x65;
};

void fn_710014a32c(UnkMatrixApplier_ovb0176* self) {
    self->weight = 0.0f;
    self->field_0x10 = nullptr;
    self->vtable = g_unk_7101a1cd78 + 0x10;
    f32* m = self->matrix.m;
    __builtin_memset(m, 0, sizeof(self->matrix));
    m[15] = 1.0f;
    m[10] = 1.0f;
    m[5] = 1.0f;
    m[0] = 1.0f;
    self->key = ~0ull;
    self->field_0x64 = 1;
    self->field_0x65 = 0;
}

void fn_710014a5c4(UnkMatrixApplier_ovb0176* self, UnkKeyedEntry* entry, const UnkMatrix44* matrix) {
    UnkMatrix44* out = reinterpret_cast<UnkMatrix44*>(entry);
    if (self->weight == 1.0f)
        UnkCopyMatrix(out, *matrix);
    else
        UnkCopyMatrix(out, fn_7100078834(entry, matrix, self->weight));
}
static_assert(__builtin_offsetof(UnkMatrixApplier_ovb0176, matrix) == 0x18, "layout");
static_assert(__builtin_offsetof(UnkMatrixApplier_ovb0176, field_0x65) == 0x65, "layout");
