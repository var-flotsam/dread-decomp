// CCharClassStaticLUACallback1Event's unit (tools/tu_map.py): the event calls its script function (id at +0x10)
// with its argument id (+0x18): as a boolean (whether it reads "true") when it contains "true" or "false",
// otherwise as a string. Same helpers as src/game/CCharClassStaticLUACallback2Event/
// CCharClassStaticLUACallback2Event_Call_ovb0129.cpp. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrIdBase;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// A string id's pooled data: its text at +0x10 and a count of reads at +0x44 (a union member: the
// original reloads the id's data pointer after the increment).
struct UnkLuaIdData_ovb0139 {
    u8 pad_0x0[0x10];
    CRntString text;
    u8 pad_0x40[0x4];
    union {
        s32 reads;
    };
};

struct UnkLuaId_ovb0139 {
    base::global::UnkStringData* data;

    UnkLuaIdData_ovb0139* Data() const { return reinterpret_cast<UnkLuaIdData_ovb0139*>(data); }
};

// The function-local id "true" and its guard.
struct UnkLuaStaticId_ovb0139 {
    UnkLuaId_ovb0139 id;
    u64 guard;
};
extern UnkLuaStaticId_ovb0139 g_unk_717279e8f8 __attribute__((visibility("hidden")));

static inline UnkLuaId_ovb0139& TrueId() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_717279e8f8.guard), __ATOMIC_ACQUIRE) & 1),
                         0)) {
        if (__cxa_guard_acquire(&g_unk_717279e8f8.guard)) {
            new (&g_unk_717279e8f8.id) UnkStrIdBase("true", true);
            fn_7100000250(fn_710000051c, &g_unk_717279e8f8.id, g_unk_7101a19000);
            __cxa_guard_release(&g_unk_717279e8f8.guard);
        }
    }
    return g_unk_717279e8f8.id;
}

static inline const char* Text(UnkLuaId_ovb0139& id) {
    ++id.Data()->reads;
    return id.Data()->text.data ? id.Data()->text.data : g_unk_710162e2ed;
}

// Whether the id's text contains the key's (false for the empty id).
static inline bool Contains(UnkLuaId_ovb0139& id, UnkLuaId_ovb0139& key) {
    ++key.Data()->reads;
    const char* k = key.Data()->text.data;
    ++id.Data()->reads;
    return id.data != &g_unk_7101d47108->empty && fn_710007fb9c(&id.Data()->text, k);
}
// Whether the id's text contains a literal (false for the empty id).
static inline bool ContainsText(UnkLuaId_ovb0139& id, const char* key) {
    ++id.Data()->reads;
    return id.data != &g_unk_7101d47108->empty && fn_710007fb9c(&id.Data()->text, key);
}

struct UnkLuaCallback1Event_ovb0139 {
    u8 pad_0x0[0x10];
    UnkLuaId_ovb0139 function;  // 0x10
    UnkLuaId_ovb0139 arg;       // 0x18
};

void fn_7100cdfa54(UnkLuaCallback1Event_ovb0139* self) {
    UnkLuaId_ovb0139& yes = TrueId();
    if (Contains(self->arg, yes) || ContainsText(self->arg, "false")) {
        const char* function = Text(self->function);
        bool value = Contains(self->arg, yes);
        fn_7100000790(function, "b", value);
        return;
    }
    const char* function = Text(self->function);
    const char* arg = Text(self->arg);
    fn_7100000790(function, "s", arg);
}
