// Constructor of a component: runs the base constructor, sets its defaults (a timeout of 1000.0, an
// angle of pi/4, a third), copies the file path id passed by pointer, makes a second id the empty
// path and creates three empty arrays (flags 0xf).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkComponent {
    void** vtable;
    u8 pad_0x8[0x24];
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    u64 field_0x48;
    u64 field_0x50;
    u8 field_0x58;
    u32 field_0x5c;
    u8 field_0x60;
    CFilePathStrId field_0x68;
    base::global::UnkFilePathStrIdBase field_0x70;
    u16 field_0x78;
    base::UnkVector<void*> field_0x80;
    base::UnkVector<void*> field_0xa8;
    base::UnkVector<void*> field_0xd0;
    u8 field_0xf8;
};

extern void* g_unk_7101a49310[];

void fn_7100573104(UnkComponent* self);

void fn_71005732dc(UnkComponent* self, void* unused, const CFilePathStrId* path) {
    fn_7100573104(self);
    self->vtable = g_unk_7101a49310 + 2;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
    self->field_0x34 = 0;
    self->field_0x38 = 0;
    self->field_0x3c = 1000.0f;
    self->field_0x40 = 0.7853982f;
    self->field_0x44 = 0.16666667f;
    self->field_0x48 = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    self->field_0x5c = 0;
    self->field_0x60 = 0;
    new (&self->field_0x68) CFilePathStrId(*path);
    // The empty path id, built by hand: its inline constructor reloads `data` after the pool is made.
    self->field_0x70.data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->field_0x70.data = &g_unk_7101d47200->empty;
    self->field_0x78 = 0;
    new (&self->field_0x80) base::UnkVector<void*>(0xf);
    new (&self->field_0xa8) base::UnkVector<void*>(0xf);
    new (&self->field_0xd0) base::UnkVector<void*>(0xf);
    self->field_0xf8 = 0;
}
