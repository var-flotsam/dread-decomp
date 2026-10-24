// Static initialiser of a global with five embedded sub-objects (a vtable pointer at the start
// of each, and a back pointer 0x20 bytes on); registers its destructor with atexit.
#include "base/UnkAtExit.h"
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

struct UnkMultiObj_0xe8 {
    void* vtable_0x0;
    u8 pad_0x8[0x18];
    UnkMultiObj_0xe8* self_0x20;
    u8 pad_0x28[8];
    void* vtable_0x30;
    u8 pad_0x38[0x18];
    void* self_0x50;
    u8 pad_0x58[8];
    void* vtable_0x60;
    u8 pad_0x68[0x18];
    void* self_0x80;
    u8 pad_0x88[8];
    void* vtable_0x90;
    u8 pad_0x98[0x18];
    void* self_0xb0;
    u8 pad_0xb8[8];
    void* vtable_0xc0;
    u8 pad_0xc8[0x18];
    void* self_0xe0;
};

extern UnkMultiObj_0xe8 g_unk_7101d4ee20 HIDDEN;
extern u8 g_unk_7101a197b0[] HIDDEN, g_unk_7101a197f8[] HIDDEN, g_unk_7101a19840[] HIDDEN,
    g_unk_7101a19888[] HIDDEN, g_unk_7101a198d0[] HIDDEN;

void fn_7100027780(void*) HIDDEN;  // destructor of g_unk_7101d4ee20

void fn_7100027890() {
    UnkMultiObj_0xe8& o = g_unk_7101d4ee20;
    o.self_0x20 = &o;
    o.vtable_0x0 = g_unk_7101a197b0;
    o.vtable_0x30 = g_unk_7101a197f8;
    o.self_0x50 = &o.vtable_0x30;
    o.vtable_0x60 = g_unk_7101a19840;
    o.self_0x80 = &o.vtable_0x60;
    o.vtable_0x90 = g_unk_7101a19888;
    o.self_0xb0 = &o.vtable_0x90;
    o.vtable_0xc0 = g_unk_7101a198d0;
    o.self_0xe0 = &o.vtable_0xc0;
    fn_7100000250(fn_7100027780, nullptr, &g_unk_7101a19000);
}
