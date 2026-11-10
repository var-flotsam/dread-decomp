// Unit 36 of tools/tu_map.py (the NFP worker, see src/base/UnkUnit36/): the machine's answer handlers
// (g_unk_7101a19948, run by fn_710002c1b0) and their helpers. Each handler waits for the worker's
// answer event without blocking, reads the answer and either moves the machine on or clears the
// request (its two slot-event arrays at +0xb8/+0xc0, the slot index at +0xb4 and the words at +0x44)
// and goes back. Names and layouts are placeholders.
#include <nn/os.h>

#include "../../base/UnkUnit36/UnkUnit36_Nfp.h"
#include "base/UnkVector.h"
#include "types.h"

// The sound holder singleton (src/engine/UnkEngineStart.cpp); its first word is a sound group.
struct UnkSoundGroup_b0183 {
    u8 unk_0x0[0x40];
    u32 field_0x40;
};
struct UnkSoundHolder {
    UnkSoundGroup_b0183* group;
};
extern UnkSoundHolder* g_unk_7101d47340;

// Creates the object on first use: storage from the default allocator (flags 0x3f unless given),
// built by its constructor function.
template <class T, u64 Size, void (*Ctor)(T*), u32 Flags = 0x3f>
static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, Flags));
        Ctor(p);
        instance = p;
    }
    return instance;
}

void* SleepFor_b0183(u64 value) asm("_Z13fn_71000810b0j");
void* operator new(unsigned long);
void operator delete(void*);
void* fn_710002cc8c(UnkUnit36Static* self);

struct UnkUnit36Poly {
    virtual bool vf_0x0();
};

namespace nn::os {
void FinalizeEvent(EventType* event);
void ClearSystemEvent(SystemEventType* event);
void DestroySystemEvent(SystemEventType* event);
}

void fn_710006cd9c(UnkSoundHolder* holder);
void fn_710002ccfc(UnkUnit36Static* self, void* slot);
void fn_710002d3e0(UnkUnit36Object* self, nn::os::EventType* done);
UnkUnit36Static* fn_710002cc34();
void fn_710002cc44();
u32 fn_710002d320(UnkUnit36Static* self);

extern "C" int memcmp(const void*, const void*, unsigned long);

// The worker's answer table: the entry the last query stopped on and the ones before it.
struct UnkUnit36Table {
    u8 unk_0x0[0x10];
    u32 first;  // 0x10
    u32 entries[0x10f];  // 0x14
    s32 count;  // 0x450
    u32 key;  // 0x454
    f32 value;  // 0x458
};

bool fn_710002d328(UnkUnit36Table* self, f32* out, u32 skip) {
    u32 cur = self->first;
    s64 i = 0;
    while (cur != self->key) {
        if (i >= self->count)
            return false;
        if (cur == skip) {
            *out = 0.0f;
            return true;
        }
        cur = self->entries[i++];
    }
    *out = self->value;
    return true;
}

// One of the request's 0x30-byte event slots; the flag says the event exists.
struct UnkUnit36Slot {
    u8 unk_0x0[0x28];
    bool created;  // 0x28
    u8 unk_0x29[7];
};

static inline char* Raw(UnkUnit36Request& request) {
    return reinterpret_cast<char*>(&request);
}

static inline void DestroySlots(char* raw, u32 array_offset) {
    UnkUnit36Slot* slots = *reinterpret_cast<UnkUnit36Slot**>(raw + array_offset);
    if (!slots)
        return;
    s32 count = *reinterpret_cast<s32*>(raw + 0xb0);
    for (s64 i = 0; i < count; i++) {
        UnkUnit36Slot* slot = &(*reinterpret_cast<UnkUnit36Slot**>(raw + array_offset))[i];
        if (slot->created) {
            nn::os::DestroySystemEvent(reinterpret_cast<nn::os::SystemEventType*>(slot));
            count = *reinterpret_cast<s32*>(raw + 0xb0);
        }
    }
    *reinterpret_cast<UnkUnit36Slot**>(raw + array_offset) = nullptr;
}

static inline void ClearRequest(char* raw) {
    DestroySlots(raw, 0xb8);
    DestroySlots(raw, 0xc0);
    *reinterpret_cast<s32*>(raw + 0xb4) = 0;
    __builtin_memset(raw + 0x44, 0, 0x70);
}

void fn_710002af50() {
    UnkUnit36Request& request = g_unk_7101d4f508;
    if (!nn::os::TryWaitEvent(UnkUnit36RequestEvent(request)))
        return;
    u32 result = fn_710002d320(fn_710002cc34());
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    if (result == 0) {
        machine.state = 9;
        machine.field_0x218 = 0;
        machine.field_0x21c = 0;
        return;
    }
    machine.state = 2;
    machine.field_0x218 = 0;
    machine.field_0x21c = 0;
    ClearRequest(Raw(request));
}

// The answer to the request: the words +0x8c.. of the request go to the machine at +0x14.. and
// the machine moves to state 5; an error clears the request and goes back to state 2.
void fn_710002ac00() {
    UnkUnit36Request& request = g_unk_7101d4f508;
    if (!nn::os::TryWaitEvent(UnkUnit36RequestEvent(request)))
        return;
    u32 result = fn_710002d320(fn_710002cc34());
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    if (result == 0) {
        char* raw = Raw(request);
        char* mach = reinterpret_cast<char*>(&machine);
        __builtin_memcpy(mach + 0x1c, raw + 0x94, 8);
        __builtin_memcpy(mach + 0x24, raw + 0x9c, 8);
        __builtin_memcpy(mach + 0x2c, raw + 0xa4, 8);
        __builtin_memcpy(mach + 0x34, raw + 0xac, 4);
        __builtin_memcpy(mach + 0x38, raw + 0xb0, 4);
        __builtin_memcpy(mach + 0x14, raw + 0x8c, 8);
        machine.state = 5;
        machine.field_0x218 = 0;
        machine.field_0x21c = 0;
        return;
    }
    machine.state = 2;
    machine.field_0x218 = 0;
    machine.field_0x21c = 0;
    ClearRequest(Raw(request));
}

// A state handler of the machine: in the phase the previous step left in +0x21c, either an error
// ends the machine (state 0x16), or the request is either left alone (no slots), cleared (the
// machine's word at +0x8 says the module-0x73 busy error) or kept, and the machine goes back.
void fn_710002be20() {
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    u32 phase = machine.field_0x21c;
    machine.field_0x218 = 5;
    if (phase == 1) {
        UnkUnit36Request& request = g_unk_7101d4f508;
        char* raw = Raw(request);
        if (*reinterpret_cast<s32*>(raw + 0xb0) >= 1) {
            if ((machine.field_0x8 & 0x3fe1ff) == 0x8073) {
                machine.state = 2;
                machine.field_0x218 = 0;
                machine.field_0x21c = 0;
                ClearRequest(raw);
                machine.field_0x8 = 0;
            } else {
                machine.state = 5;
                machine.field_0x218 = 0;
                machine.field_0x21c = 0;
                machine.field_0x8 = 0;
            }
        } else {
            machine.state = 2;
            machine.field_0x218 = 0;
            machine.field_0x21c = 0;
            machine.field_0x8 = 0;
        }
    } else if (phase == 2) {
        machine.state = 0x16;
        machine.field_0x21c = 0;
        machine.field_0x218 = 0;
    }
}

// The next step of the machine: asks the worker for the slot of the sound holder's group. With no
// slots left it ends back in state 2; otherwise the slot's events are cleared, the next request
// (fn_710002ccfc, fn_710002d3e0) is posted and the machine goes to state 6.
void fn_710002ad90() {
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    machine.field_0x218 = 6;
    s32 next;
    if (machine.field_0x21c == 2) {
        next = 0x16;
    } else {
        u32* group_word = reinterpret_cast<u32*>(reinterpret_cast<char*>(&machine) + 0x3c);
        *group_word = 0;
        UnkSoundHolder* holder = Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340);
        u32 slot;
        if (holder->group) {
            slot = holder->group->field_0x40;
            *group_word = slot;
        } else {
            slot = *group_word;
        }
        UnkUnit36Request& request = g_unk_7101d4f508;
        char* raw = Raw(request);
        s32 count = *reinterpret_cast<s32*>(raw + 0xb0);
        *reinterpret_cast<u32*>(raw + 0xb4) = slot;
        if (count >= 1) {
            if (static_cast<s32>(slot) >= count)
                *reinterpret_cast<s32*>(raw + 0xb4) = 0;
            request.field_0x28 = machine.state;
            request.field_0x2c = 0xb;
            request.field_0x30 = 0;
            request.field_0x38 = 0;
            for (u64 i = 0; i < *reinterpret_cast<u32*>(raw + 0xb0); i++) {
                if (*reinterpret_cast<char**>(raw + 0xb8))
                    nn::os::ClearSystemEvent(reinterpret_cast<nn::os::SystemEventType*>(*reinterpret_cast<char**>(raw + 0xb8) + i * 0x30));
                if (*reinterpret_cast<char**>(raw + 0xc0))
                    nn::os::ClearSystemEvent(reinterpret_cast<nn::os::SystemEventType*>(*reinterpret_cast<char**>(raw + 0xc0) + i * 0x30));
            }
            nn::os::ClearSystemEvent(reinterpret_cast<nn::os::SystemEventType*>(request.buffer));
            UnkUnit36Static* worker = fn_710002cc34();
            fn_710002ccfc(worker, raw + 0x44 + (s64) * reinterpret_cast<s32*>(raw + 0xb4) * 8);
            fn_710002d3e0(reinterpret_cast<UnkUnit36Object*>(worker), UnkUnit36RequestEvent(request));
            next = 6;
        } else {
            machine.state = 2;
            machine.field_0x218 = 0;
            machine.field_0x21c = 0;
            if (*reinterpret_cast<void**>(raw + 0xb8))
                *reinterpret_cast<void**>(raw + 0xb8) = nullptr;
            if (*reinterpret_cast<void**>(raw + 0xc0))
                *reinterpret_cast<void**>(raw + 0xc0) = nullptr;
            *reinterpret_cast<s32*>(raw + 0xb4) = 0;
            __builtin_memset(raw + 0x44, 0, 0x70);
            return;
        }
    }
    machine.state = next;
    machine.field_0x218 = 0;
    machine.field_0x21c = 0;
}

// Shuts the worker down: while the machine is not in its final state the worker is asked for the
// last request and waited for; once the answer event fired the worker object, the request's
// events and its buffer are released and the machine goes back to state 0.
void fn_710002bc50() {
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    if (reinterpret_cast<UnkStateMachine_710002c1b0&>(machine).last_state != 0x16) {
        UnkUnit36Static* worker = fn_710002cc34();
        fn_710002cc8c(worker);
        fn_710002d3e0(reinterpret_cast<UnkUnit36Object*>(worker), UnkUnit36RequestEvent(g_unk_7101d4f508));
        UnkUnit36Poly* poly = reinterpret_cast<UnkUnit36Poly*>(worker);
        while (poly->vf_0x0())
            SleepFor_b0183(10);
    }
    UnkUnit36Request& request = g_unk_7101d4f508;
    if (nn::os::TryWaitEvent(UnkUnit36RequestEvent(request))) {
        fn_710002cc44();
        nn::os::FinalizeEvent(UnkUnit36RequestEvent(request));
        machine.state = 0;
        machine.field_0x218 = 0;
        machine.field_0x21c = 0;
        ClearRequest(Raw(request));
        UnkUnit36Buffer* buffer = request.buffer;
        if (buffer) {
            if (buffer->field_0x28)
                nn::os::DestroySystemEvent(reinterpret_cast<nn::os::SystemEventType*>(buffer));
            operator delete(buffer);
        }
    }
}

void fn_710002b710() {
    UnkUnit36Request& request = g_unk_7101d4f508;
    if (!nn::os::TryWaitEvent(UnkUnit36RequestEvent(request)))
        return;
    u32 result = fn_710002d320(fn_710002cc34());
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    char* raw = reinterpret_cast<char*>(&request);
    char* mach = reinterpret_cast<char*>(&machine);
    s32 next = 8;
    if (result != 0) {
        request.field_0x28 = machine.state;
        request.field_0x2c = 0x17;
        request.field_0x38 = result;
    } else {
        u8 length = static_cast<u8>(raw[0xda]);
        if (length == static_cast<u8>(mach[0x4a]) && memcmp(raw + 0xd0, mach + 0x40, length) == 0) {
            reinterpret_cast<void (*)()>(request.field_0x30)();
            next = 0xd;
        } else {
            machine.field_0x2f8 = 0xffff;
            machine.field_0x2fc = 4;
            request.field_0x28 = machine.state;
            request.field_0x2c = 0x17;
            request.field_0x38 = 0xc073;
        }
    }
    machine.state = next;
    machine.field_0x218 = 0;
    machine.field_0x21c = 0;
}
