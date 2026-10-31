// CLuaCallsLogicAction's destructors (complete and deleting): restore the vtable, destroy the two
// strings at +0x48 and +0x18 (fn_710007ecd4) and run the base destructor (fn_7100a55ab8). Names
// and the layout are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkLogicActionBase {
    void** vtable;
    u8 unk_0x8[0x10];
    ~UnkLogicActionBase();
};
void fn_7100a55ab8(UnkLogicActionBase* self);  // the base destructor

inline UnkLogicActionBase::~UnkLogicActionBase() { fn_7100a55ab8(this); }

struct UnkLogicActionString {
    u64 unk_0x0[0x30 / 8];
    ~UnkLogicActionString() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this)); }
};

extern void* g_unk_7101a8f358[];

struct CLuaCallsLogicAction : UnkLogicActionBase {
    UnkLogicActionString field_0x18;
    UnkLogicActionString field_0x48;

    ~CLuaCallsLogicAction() { vtable = g_unk_7101a8f358 + 2; }
};

void fn_7100a50508(CLuaCallsLogicAction* self) { self->~CLuaCallsLogicAction(); }

void fn_7100a5054c(CLuaCallsLogicAction* self) {
    self->~CLuaCallsLogicAction();
    operator delete(self);
}
