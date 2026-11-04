// CAutomperAIComponent (fn_71003bed68) and CAutsniperAIComponent (fn_71003db188): the same handler, which
// passes the call on to the base handler (fn_71006619f8) unless the message's (+0x60) id at +0x20 is the
// one kept at +0xec8 of the id table built once by fn_7100031894 (the branch hint keeps the original's
// block order). Names and layouts are placeholders.
#include "types.h"

// As src/game/CEmmyAIComponent/CEmmyAIComponent_IsStateId_b63a3.cpp declares it.
struct UnkGameManagerIdTable {
    u8 pad_0x0[0xec8];
    u64 field_0xec8;
    u8 pad_0xed0[0x2578 - 0xed0];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkFilteredMessageInfo_b64a2 {
    u8 pad_0x0[0x20];
    u64 id;
};

struct UnkFilteredMessage_b64a2 {
    u8 pad_0x0[0x60];
    UnkFilteredMessageInfo_b64a2* info;
};

void fn_71006619f8(void* self, void* arg, UnkFilteredMessage_b64a2* msg);

static inline __attribute__((always_inline)) void Handle(void* self, void* arg, UnkFilteredMessage_b64a2* msg) {
    UnkFilteredMessageInfo_b64a2* info = msg->info;
    UnkGameManagerIdTable& table = GameManagerIdTable();
    if (__builtin_expect(info->id != table.field_0xec8, 1))
        fn_71006619f8(self, arg, msg);
}

void fn_71003bed68(void* self, void* arg, UnkFilteredMessage_b64a2* msg) { Handle(self, arg, msg); }
void fn_71003db188(void* self, void* arg, UnkFilteredMessage_b64a2* msg) { Handle(self, arg, msg); }
