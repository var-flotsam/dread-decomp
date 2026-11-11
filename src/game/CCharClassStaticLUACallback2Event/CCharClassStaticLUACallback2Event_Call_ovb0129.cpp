// CCharClassStaticLUACallback2Event unit (tools/tu_map.py): the event calls its script function (id at +0x10)
// with two booleans, whether each argument id (+0x18, +0x20) reads "true" (a function-local id); and the
// event's copy-assignment (fn_7100ce0c40). Names and layouts are placeholders.
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
struct UnkLuaIdData_b0129 {
    u8 pad_0x0[0x10];
    CRntString text;
    u8 pad_0x40[0x4];
    union {
        s32 reads;
    };
};

struct UnkLuaId_b0129 {
    base::global::UnkStringData* data;

    UnkLuaIdData_b0129* Data() const { return reinterpret_cast<UnkLuaIdData_b0129*>(data); }
};

// The function-local id "true" and its guard.
struct UnkLuaStaticId_b0129 {
    UnkLuaId_b0129 id;
    u64 guard;
};
extern UnkLuaStaticId_b0129 g_unk_717279ea68 __attribute__((visibility("hidden")));

static inline UnkLuaId_b0129& TrueId() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_717279ea68.guard), __ATOMIC_ACQUIRE) & 1),
                         0)) {
        if (__cxa_guard_acquire(&g_unk_717279ea68.guard)) {
            new (&g_unk_717279ea68.id) UnkStrIdBase("true", true);
            fn_7100000250(fn_710000051c, &g_unk_717279ea68.id, g_unk_7101a19000);
            __cxa_guard_release(&g_unk_717279ea68.guard);
        }
    }
    return g_unk_717279ea68.id;
}

static inline const char* Text(UnkLuaId_b0129& id) {
    ++id.Data()->reads;
    return id.Data()->text.data ? id.Data()->text.data : g_unk_710162e2ed;
}

// Whether the id's text contains the key's (false for the empty id).
static inline bool Contains(UnkLuaId_b0129& id, UnkLuaId_b0129& key) {
    ++key.Data()->reads;
    const char* k = key.Data()->text.data;
    ++id.Data()->reads;
    return id.data != &g_unk_7101d47108->empty && fn_710007fb9c(&id.Data()->text, k);
}

struct UnkLuaCallback2Event_b0129 {
    u8 pad_0x0[0x10];
    UnkLuaId_b0129 function;  // 0x10
    UnkLuaId_b0129 arg0;      // 0x18
    UnkLuaId_b0129 arg1;      // 0x20
};

// Calls the script function with two booleans: whether each argument reads "true".
void fn_7100ce0858(UnkLuaCallback2Event_b0129* self) {
    UnkLuaId_b0129& yes = TrueId();
    const char* function = Text(self->function);
    bool a = Contains(self->arg0, yes);
    bool b = Contains(self->arg1, yes);
    fn_7100000790(function, "bb", a, b);
}

// The event's copy-assignment: the word at +0x8 and the three ids.
struct UnkLuaCallback2EventData_b0129 {
    void* vtable;
    u32 field_0x8;
    base::UnkStrIdMember function;  // 0x10
    base::UnkStrIdMember arg0;      // 0x18
    base::UnkStrIdMember arg1;      // 0x20
};

UnkLuaCallback2EventData_b0129* fn_7100ce0c40(UnkLuaCallback2EventData_b0129* self,
                                              const UnkLuaCallback2EventData_b0129* other) {
    self->field_0x8 = other->field_0x8;
    self->function = other->function;
    self->arg0 = other->arg0;
    self->arg1 = other->arg1;
    return self;
}
