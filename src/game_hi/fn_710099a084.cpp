// Destructor body: drops a ref-counted pointer, then tears down three nested event-like members
// (each a vtable'd object holding callback slot arrays and small reset-on-destroy records) and a
// last record, then runs the base destructor.
#include "types.h"

void fn_71000a1c60(void* p);
void fn_710117b3f0(void* owner, void* listener);
void fn_71009b0800(void* self);

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


// The common base: a vtable and a member at +0x18 (0x30 bytes).
struct UnkEventBase {
    void* vtable;
    u8 pad_0x8[0x10];
    u8 field_0x18[0x18];

    void DestroyBase(void** vt) {
        vtable = vt + 2;
        fn_71000a1c60(field_0x18);
    }
};

// Eight callback slots after the base (0x70 bytes).
struct UnkSlots : UnkEventBase {
    void* slots[8];

    void Destroy(void** vt) {
        slots[7] = nullptr;
        slots[6] = nullptr;
        slots[5] = nullptr;
        slots[4] = nullptr;
        slots[3] = nullptr;
        slots[2] = nullptr;
        slots[1] = nullptr;
        slots[0] = nullptr;
        DestroyBase(vt);
    }
};

// A small record with its own vtable (0x28 bytes).
struct UnkRecord {
    void* vtable;
    u8 pad_0x8[0x8];
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;

    void Destroy(void** vt) {
        vtable = vt + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = 0;
    }
};

struct UnkListenerOwner : UnkRefCounted {
    void* field_0x10;
};

struct UnkListener {
    void* vtable;
    u8 pad_0x8[0x10];
    UnkListenerOwner* owner;
};

extern void* g_unk_7101a86c60[];
extern void* g_unk_7101a85e98[];
extern void* g_unk_7101a85f98[];
extern void* g_unk_7101a85ff0[];
extern void* g_unk_7101a85c80[];
extern void* g_unk_7101a85bd8[];
extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a85d58[];
extern void* g_unk_7101a85db0[];
extern void* g_unk_7101a86030[];
extern void* g_unk_7101a86130[];
extern void* g_unk_7101a861e0[];
extern void* g_unk_7101a86238[];
extern void* g_unk_7101a86278[];
extern void* g_unk_7101a86e28[];

struct UnkObj_710099a084 {
    void* vtable;
    u8 pad_0x8[0x180];
    UnkRecord field_0x188;
    u8 pad_0x1b0[0x88];
    UnkEventBase field_0x238;
    UnkEventBase field_0x268;
    UnkRecord field_0x298;
    u8 pad_0x2c0[0x88];
    UnkRecord field_0x348;
    u8 pad_0x370[0x88];
    UnkSlots field_0x3f8;
    UnkSlots field_0x468;
    UnkSlots field_0x4d8;
    UnkSlots field_0x548;
    UnkSlots field_0x5b8;
    u8 pad_0x628[0x38];
    UnkEventBase field_0x660;
    UnkRecord field_0x690;
    u8 pad_0x6b8[0x88];
    UnkSlots field_0x740;
    UnkSlots field_0x7b0;
    UnkSlots field_0x820;
    u8 pad_0x890[0x28];
    UnkListener field_0x8b8;
    u8 pad_0x8d8[0x8];
    UnkEventBase field_0x8e0;
    UnkRecord field_0x910;
    u8 pad_0x938[0x90];
    UnkSlots field_0x9c8;
    UnkSlots field_0xa38;
    UnkSlots field_0xaa8;
    UnkRefPtr field_0xb18;
};

void fn_710099a084(UnkObj_710099a084* self) {
    self->vtable = g_unk_7101a86c60 + 2;
    self->field_0xb18.Release();
    self->field_0x8e0.vtable = g_unk_7101a85e98 + 2;
    self->field_0xaa8.Destroy(g_unk_7101a85f98);
    self->field_0xa38.Destroy(g_unk_7101a85f98);
    self->field_0x9c8.Destroy(g_unk_7101a85f98);
    self->field_0x910.Destroy(g_unk_7101a85ff0);
    self->field_0x8e0.DestroyBase(g_unk_7101a85c80);
    self->field_0x660.vtable = g_unk_7101a85bd8 + 2;
    self->field_0x8b8.vtable = g_unk_7101a22d48 + 2;
    if (UnkListenerOwner* owner = self->field_0x8b8.owner) {
        if (owner->field_0x10)
            fn_710117b3f0(owner->field_0x10, &self->field_0x8b8);
        if (UnkListenerOwner* o = self->field_0x8b8.owner) {
            if (--o->ref_count == 0)
                o->Delete();
        }
    }
    self->field_0x820.Destroy(g_unk_7101a85d58);
    self->field_0x7b0.Destroy(g_unk_7101a85d58);
    self->field_0x740.Destroy(g_unk_7101a85d58);
    self->field_0x690.Destroy(g_unk_7101a85db0);
    self->field_0x660.DestroyBase(g_unk_7101a85c80);
    self->field_0x268.vtable = g_unk_7101a86030 + 2;
    self->field_0x5b8.Destroy(g_unk_7101a86130);
    self->field_0x548.Destroy(g_unk_7101a86130);
    self->field_0x4d8.Destroy(g_unk_7101a861e0);
    self->field_0x468.Destroy(g_unk_7101a861e0);
    self->field_0x3f8.Destroy(g_unk_7101a861e0);
    self->field_0x348.Destroy(g_unk_7101a86238);
    self->field_0x298.Destroy(g_unk_7101a86278);
    self->field_0x268.DestroyBase(g_unk_7101a85c80);
    self->field_0x238.DestroyBase(g_unk_7101a85c80);
    self->field_0x188.Destroy(g_unk_7101a86e28);
    fn_71009b0800(self);
}
