// Writes one of an object's named settings (a name, through the object at +0x40). Other keys go
// to the base handler (fn_71010c4250). Layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "game/UnkComponent.h"

using namespace unk_prop;
using base::global::CRntString;

extern base::global::CRntString g_unk_7101d58d10;

// A shared record with a name; the use count is a union member, so the owner's pointer is
// reloaded after the increment.
struct UnkNamed_7101181d70 {
    u8 unk_0x0[0x10];
    CRntString name;
    u8 unk_pad[0x44 - 0x10 - sizeof(CRntString)];
    union {
        u32 use_count;
    };
};

struct UnkSub_7101181d70 {
    u8 unk_0x0[0x30];
    UnkNamed_7101181d70* named;
};

struct UnkHolder_7101181d70 {
    u8 unk_0x0[0x10];
    UnkSub_7101181d70* sub;
};

struct UnkObj_7101181d70 {
    u8 unk_0x0[0x40];
    UnkHolder_7101181d70* holder;
};

u32 fn_7101181d70(UnkObj_7101181d70* self, UnkProp* p) {
    if (!p->set && Is(p, Key(0x22d8))) {
        void* stream = p->stream;
        const CRntString* name = &g_unk_7101d58d10;
        if (self->holder && self->holder->sub) {
            UnkSub_7101181d70* sub = self->holder->sub;
            ++sub->named->use_count;
            name = &sub->named->name;
        }
        return fn_710007cb4c(stream, "s", name->data ? name->data : "") != 0;
    }
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
