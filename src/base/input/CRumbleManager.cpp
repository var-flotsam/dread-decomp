// The constructor of the object of unit 51, whose reflected class base::input::CRumbleManager::
// CTunableRumbleManager names the file (the object's own type is unverified; base/input/UnkRumbleManager.h).
#include "base/UnkBlockFreeList.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"

using namespace base::input;

extern char g_unk_7101a1a330[];  // its base vtable (GOT 0x7101cbbf68)
extern char g_unk_7101a1a3d0[];  // its vtable (GOT 0x7101cbc0d8)

void fn_710007533c(void* self);  // the 0x1d00-byte member's constructor
// Sets up the hash map's storage (base::refl_HashMapInit's copy: buckets, nodes).
void fn_7100075958(void* map, u32 buckets, u32 nodes);

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// A function-local static word (zero-initialised under its guard) that this constructor reads.
extern u32 g_unk_7101d58738;
extern u64 g_unk_7101d58740;

void fn_71000733e4(UnkRumbleManager* self) {
    typedef u64 __attribute__((may_alias)) word;
    char* base = reinterpret_cast<char*>(self);
    self->vtable = g_unk_7101a1a330 + 0x10;
    *reinterpret_cast<word*>(base + 0x1ec) = 0;
    *reinterpret_cast<word*>(base + 0x1f4) = 0;
    self->field_0x1fc = 0;
    new (&self->mixers[0]) nn::hid::VibrationMixer();
    new (&self->mixers[1]) nn::hid::VibrationMixer();
    new (&self->targets[0]) nn::hid::VibrationTarget();
    new (&self->targets[1]) nn::hid::VibrationTarget();
    new (&self->connections[0]) nn::hid::VibrationNodeConnection();
    new (&self->connections[1]) nn::hid::VibrationNodeConnection();
    fn_710007533c(self->sub_0x3a0);
    self->field_0x20b0 = 0;
    self->field_0x20a8 = 0;
    self->field_0x20a0 = 0;
    *reinterpret_cast<word*>(base + 0x20b8) = 0xe0000000a;
    self->alloc_0x20c0 = fn_7100083990();
    self->vtable = g_unk_7101a1a3d0 + 0x10;
    UnkPooledChainedMap* m = &self->map;
    m->alloc = fn_7100083990();
    m->flags = 0xe;
    char* mb = reinterpret_cast<char*>(m);
    *reinterpret_cast<word*>(mb + 0xc) = 0;
    *reinterpret_cast<word*>(mb + 0x14) = 0;
    *reinterpret_cast<u32*>(mb + 0x1c) = 0;
    *reinterpret_cast<u32*>(mb + 0x20) = 10;  // the growth step
    m->field_0x24 = true;
    m->field_0x28 = 0xe;
    base::UnkAllocator* a = fn_7100083990();
    m->field_0x38 = 0;
    m->field_0x40 = 0;
    m->pool_vtable = g_unk_7101a1a4c8 + 0x10;
    m->alloc_0x30 = a;
    m->field_0x50 = 0;
    fn_710008377c(&m->pool);
    m->field_0x80 = 0;
    m->field_0x84 = false;
    fn_7100075958(m, 0xb, 0x200);
    self->field_0x2160 = 0;
    self->field_0x2158 = 0;
    self->field_0x2168 = 10;
    self->field_0x216c = true;
    self->field_0x2170 = 0xf;
    self->alloc_0x2178 = fn_7100083990();
    self->field_0x2180 = 0;
    fn_710008377c(&self->pool_0x2190);
    self->field_0x21c0 = 0;
    self->field_0x21b8 = 0;
    self->field_0x21c8 = 10;
    self->field_0x21cc = true;
    self->field_0x21d0 = 0xf;
    self->alloc_0x21d8 = fn_7100083990();
    // The zero vector's three words (base/math/UnkZeroVector3.h), copied as words.
    const u32* defaults = reinterpret_cast<const u32*>(&g_unk_710164779c);
    self->field_0x21e8 = defaults[0];
    u32 z = defaults[2];
    self->field_0x21ec = defaults[1];
    self->field_0x21f0 = z;
    self->field_0x21f8 = 0;
    self->field_0x2200 = 0;
    u32* zero = &g_unk_7101d58738;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_7101d58740), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_7101d58740)) {
            *zero = 0;
            __cxa_guard_release(&g_unk_7101d58740);
        }
    }
    self->field_0x2204 = *zero;
    self->field_0x2208 = 0x100;
}
