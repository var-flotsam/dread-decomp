// Unit 2730: outside the editor and viewer modes, passes the id of the actor's name to
// fn_710119780c; then fn_7100e010a0 on the game manager, and each listener (+0x30, +0x38 of them)
// is called with the actor and the event 1. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkListener_710119770c {
    void* context;
    void (*callback)(void* context, void* object, u32 event);
};

struct UnkObj_710119770c {
    u8 unk_0x0[0x30];
    UnkListener_710119770c* listeners;
    u32 count;
};

// The pooled data behind the actor's name: the text at +0x10 and a use count at +0x44.
struct UnkNameData_710119770c {
    u8 unk_0x0[0x10];
    CRntString str;
    u8 unk_0x40[0x4];
    union {
        u32 use_count;
    };
};

struct UnkActor_710119770c {
    u8 unk_0x0[0x310];
    UnkNameData_710119770c* name;
};

// An id built from a string out of line, released like CStrId.
struct UnkBuiltId_710119770c {
    union {
        CStrId id;
    };

    explicit UnkBuiltId_710119770c(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltId_710119770c() { id.~CStrId(); }
};

void fn_710119780c(UnkObj_710119770c* self, const CStrId& id);
void fn_7100e010a0(UnkGameManager* self, u32 value);

void fn_710119770c(UnkObj_710119770c* self, UnkActor_710119770c* actor) {
    if (!fn_7100deabe4(g_unk_7101d47310) && !fn_7100deac04(g_unk_7101d47310)) {
        ++actor->name->use_count;
        UnkBuiltId_710119770c id(actor->name->str);
        fn_710119780c(self, id.id);
    }
    fn_7100e010a0(g_unk_7101d47310, 0);
    if (self->count) {
        UnkListener_710119770c* it = self->listeners;
        UnkListener_710119770c* end = it + self->count;
        for (; it != end; ++it)
            it->callback(it->context, actor, 1);
    }
}
