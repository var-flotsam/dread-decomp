#pragma once

// The pack sets object of unit 66 (tools/tu_map.py: 0x7100089284-0x710008b6d4, whose reflected
// class is base::file::CPackManagerDevice; this object is 0x1348 bytes, far larger than that
// class's reflected size 0x88, so it is another class of the unit). It holds a ring of 128 load
// requests, the pack manager it mounts packs on and the "PackLoaderThread" that serves the ring
// (src/base/file/CPackManagerDevice.cpp). Its vtable is at GOT 0x7101cbc488. Names and layout
// are placeholders.

#include <new>

#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

namespace base::file {

struct CPackManagerDevice;

// A load request's two words (zero for a pack set's packs).
struct UnkPathRequestArgs {
    u64 field_0x0;
    u64 field_0x8;
};

// A load request: a path id with a flag byte, a value (9 when empty) and two words (0x20 bytes).
// The path is released and assigned in two steps (base/UnkDestructors.h).
struct UnkPathSlot {
    UnkStrIdMemberOf<g_unk_7101d47200> path;  // +0x0, built as a CFilePathStrId from the empty string
    u8 field_0x8;
    u8 pad_0x9[3];
    u32 field_0xc;  // 9
    UnkPathRequestArgs args;  // +0x10
};
static_assert(sizeof(UnkPathSlot) == 0x20, "UnkPathSlot placeholder layout");

struct UnkFileDevice {
    void* vtable;       // +0x0
    u8 base_0x8[0xb0];  // a base object built by fn_710008b88c
    u8 field_0xb8;
    u8 pad_0xb9[7];
    UnkPathSlot slots[128];  // +0xc0: the request ring
    u32 queued;              // +0x10c0: requests added
    u32 field_0x10c4;
    u32 next;  // +0x10c8: the slot the next request goes to
    u8 pad_0x10cc[4];
    nn::os::MutexType mutex;        // +0x10d0: guards the ring
    nn::os::MutexType mutex_0x10f0;  // +0x10f0
    CPackManagerDevice* packs;      // +0x1110
    union {
        u16 field_0x1118;  // the constructor clears both flags at once
        struct {
            bool running;  // +0x1118: the loader thread is started
            u8 field_0x1119;
        };
    };
    u8 pad_0x111a[6];
    nn::os::MutexType mutex_0x1120;  // +0x1120
    u8 thread[0x1e4];               // +0x1140: the loader thread (src/base/UnkEngineThread.cpp)
    u8 tail_0x1324[0x14];           // zeroed by overlapping word stores
    nn::os::LightEventType event;   // +0x1338: signalled when there is work
    u8 pad_0x1344[4];
};
static_assert(sizeof(UnkFileDevice) == 0x1348, "UnkFileDevice placeholder layout");

}  // namespace base::file

// Its vtable (GOT 0x7101cbc488) and the constructor of its base object.
extern char g_unk_7101a1b0e8[];
void fn_710008b88c(void* base_object);

// Its constructor's body, inline: fn_71000893a8 is the out-of-line copy (callers pass the
// storage), and the loader thread's singleton accessor (fn_7100089af8) has it inlined.
__attribute__((always_inline)) inline void UnkFileDeviceConstruct(base::file::UnkFileDevice* self) {
    using base::file::UnkPathSlot;
    typedef u64 __attribute__((may_alias)) word;
    // The original computes the flags' address first; the volatile barrier keeps it there when
    // the body is inlined as well.
    u16* flags = &self->field_0x1118;
    asm volatile("" : "+r"(flags));
    self->vtable = g_unk_7101a1b0e8 + 0x10;
    fn_710008b88c(self->base_0x8);
    self->field_0xb8 = 0;
    UnkPathSlot* slot = self->slots;
    do {
        new (&slot->path) base::global::CFilePathStrId("", true);
        slot->field_0x8 = 0;
        slot->field_0xc = 9;
        slot->args.field_0x0 = 0;
        slot->args.field_0x8 = 0;
        ++slot;
    } while (slot != &self->slots[128]);
    self->queued = 0;
    self->field_0x10c4 = 0;
    self->next = 0;
    fn_71000668f8(&self->mutex);
    self->packs = nullptr;
    *flags = 0;
    *reinterpret_cast<u32*>(self->tail_0x1324 + 0x10) = 0;
    *reinterpret_cast<word*>(self->tail_0x1324) = 0;
    *reinterpret_cast<word*>(self->tail_0x1324 + 8) = 0;
}

// The constructor, out of line (src/base/file/UnkFileDevice.cpp).
void fn_71000893a8(base::file::UnkFileDevice* self);
