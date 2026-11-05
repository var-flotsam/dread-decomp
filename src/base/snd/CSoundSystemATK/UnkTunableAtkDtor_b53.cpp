// base::snd::CSoundSystemATK::CTunableSoundSystemATK's unit (tools/tu_map.py): the destructor of an
// object holding mutexes, three locked lists (a base with one mutex, extended by two more) and a
// list with only the base, and a system event at +0x90 destroyed when it was set up. The vtables
// are the original's (reached through the GOT). Names and layouts are placeholders.
#include <nn/os.h>

#include "types.h"

namespace nn::os {
void DestroySystemEvent(SystemEventType* event);
}  // namespace nn::os

extern char g_unk_7101cb8e00[];  // the base's vtable block
extern char g_unk_7101cb8e70[];  // the derived list's vtable block

struct UnkLockedListBase {
    void* vtable;
    nn::os::MutexType mutex;  // 0x8
    u8 pad_0x28[0x28];
    ~UnkLockedListBase() {
        vtable = g_unk_7101cb8e00 + 16;
        nn::os::FinalizeMutex(&mutex);
    }
};
static_assert(sizeof(UnkLockedListBase) == 0x50, "size");

struct UnkLockedList : UnkLockedListBase {
    u8 pad_0x50[0x60];
    nn::os::MutexType mutex_0xb0;
    nn::os::MutexType mutex_0xd0;
    u8 pad_0xf0[8];
    ~UnkLockedList() {
        vtable = g_unk_7101cb8e70 + 16;
        nn::os::FinalizeMutex(&mutex_0xd0);
        nn::os::FinalizeMutex(&mutex_0xb0);
    }
};
static_assert(sizeof(UnkLockedList) == 0xf8, "size");

struct UnkScopedSystemEvent_b53 {
    u8 storage[0x28];
    u8 state;
    ~UnkScopedSystemEvent_b53() {
        if (state)
            nn::os::DestroySystemEvent(reinterpret_cast<nn::os::SystemEventType*>(this));
    }
};

struct UnkTunableAtk {
    u8 pad_0x0[0x90];
    UnkScopedSystemEvent_b53 event;  // 0x90
    u8 pad_0xb9[0x558 - 0xb9];
    UnkLockedListBase list_0x558;
    UnkLockedList list_0x5a8;
    UnkLockedList list_0x6a0;
    UnkLockedList list_0x798;
    u8 pad_0x890[0x10];
    nn::os::MutexType mutex_0x8a0;
    u8 pad_0x8c0[0x960 - 0x8c0];
    nn::os::MutexType mutex_0x960;
    nn::os::MutexType mutex_0x980;
    nn::os::MutexType mutex_0x9a0;
    nn::os::MutexType mutex_0x9c0;
    nn::os::MutexType mutex_0x9e0;
    __attribute__((always_inline)) ~UnkTunableAtk() {
        nn::os::FinalizeMutex(&mutex_0x9e0);
        nn::os::FinalizeMutex(&mutex_0x9c0);
        nn::os::FinalizeMutex(&mutex_0x9a0);
        nn::os::FinalizeMutex(&mutex_0x980);
        nn::os::FinalizeMutex(&mutex_0x960);
        nn::os::FinalizeMutex(&mutex_0x8a0);
    }
};
static_assert(__builtin_offsetof(UnkTunableAtk, list_0x798) == 0x798, "layout");
static_assert(__builtin_offsetof(UnkTunableAtk, mutex_0x9e0) == 0x9e0, "layout");

void fn_7100103a70(UnkTunableAtk* self) {
    self->~UnkTunableAtk();
}
