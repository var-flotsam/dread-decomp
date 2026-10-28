// Flush of a movie player: runs fn_7101239de0 on the object at 0x50, then empties its two locked
// ring queues (64 4-byte entries at 0x2a0, 64 0x18-byte entries at 0x3e0), stepping the read
// position up to the write position before resetting both to the start.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

template <class T, u32 N>
struct UnkRing_7100225ef8 {
    T* read;
    T* write;
    u64 count;
    T entries[N];

    __attribute__((always_inline)) void Clear() {
        for (T* p = read; p != write;) {
            ++p;
            p = p == entries + N ? entries : p;
        }
        read = entries;
        write = entries;
        count = 0;
    }
};

template <class T, u32 N, u32 Gap>
struct UnkLockedRing_7100225ef8 {
    nn::os::MutexType mutex;
    u8 unk_0x20[Gap];
    UnkRing_7100225ef8<T, N> ring;

    __attribute__((always_inline)) void Clear() {
        fn_710006691c(&mutex);
        ring.Clear();
        fn_7100066920(&mutex);
    }
};

struct UnkEntry_0x18 {
    u8 data[0x18];
};

struct UnkPlayer_7100225ef8 {
    u8 unk_0x0[0x50];
    u8 field_0x50[0x250];
    UnkLockedRing_7100225ef8<u32, 64, 0x8> field_0x2a0;
    UnkLockedRing_7100225ef8<UnkEntry_0x18, 64, 0x10> field_0x3e0;
};

void fn_7101239de0(void* object);

void fn_7100225ef8(UnkPlayer_7100225ef8* self) {
    fn_7101239de0(self->field_0x50);
    self->field_0x2a0.Clear();
    self->field_0x3e0.Clear();
}
