// Destructor body: destroys the member at +0xc0, drops two ref-counted pointers and releases
// twelve string ids, then runs the base destructor.
#include "game/UnkComponent.h"
#include "types.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id member; its release decrements and tests the count in two steps.
struct UnkStrIdField {
    UnkStringData* data;

    void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    s32 ref_count;
};

struct UnkRefPtr {
    UnkRefCounted* ptr;

    void Release() {
        if (UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkObj_7100a02c54 : UnkComponent {
    u8 pad_0x40[0x8];
    UnkStrIdField field_0x48;
    UnkStrIdField field_0x50;
    UnkStrIdField field_0x58;
    UnkStrIdField field_0x60;
    UnkStrIdField field_0x68;
    UnkStrIdField field_0x70;
    UnkStrIdField field_0x78;
    UnkStrIdField field_0x80;
    UnkStrIdField field_0x88;
    UnkStrIdField field_0x90;
    UnkStrIdField field_0x98;
    UnkStrIdField field_0xa0;
    u8 pad_0xa8[0x8];
    UnkRefPtr field_0xb0;
    UnkRefPtr field_0xb8;
    u8 field_0xc0[0x8];
};

extern void* g_unk_7101a8ad98[];
void fn_7100431cec(void* p);

void fn_7100a02c54(UnkObj_7100a02c54* self) {
    self->vtable = g_unk_7101a8ad98 + 2;
    fn_7100431cec(self->field_0xc0);
    self->field_0xb8.Release();
    self->field_0xb0.Release();
    self->field_0xa0.Release();
    self->field_0x98.Release();
    self->field_0x90.Release();
    self->field_0x88.Release();
    self->field_0x80.Release();
    self->field_0x78.Release();
    self->field_0x70.Release();
    self->field_0x68.Release();
    self->field_0x60.Release();
    self->field_0x58.Release();
    self->field_0x50.Release();
    self->field_0x48.Release();
    fn_71010c4194(self);
}
