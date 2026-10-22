// Puts an object's state index in a property bag and sends it to the GUI root as a message
// (built by fn_710120235c around the object's target). Layouts are placeholders.
#include "../game_s7/UnkPropBagS7.h"

using namespace unk_prop_bag;

struct UnkObj_7100af65b4 {
    u8 unk_0x0[0x2a4];
    u32 field_0x2a4;
};

struct UnkMsg_7100af65b4;
void fn_710120235c(UnkMsg_7100af65b4* self, void* target, const u32& kind, UnkPropBag* bag);
void fn_7101202488(UnkMsg_7100af65b4* self);  // destructor

struct UnkMsg_7100af65b4 {
    u8 unk_0x0[0x28];

    UnkMsg_7100af65b4(void* target, const u32& kind, UnkPropBag* bag) { fn_710120235c(this, target, kind, bag); }
    ~UnkMsg_7100af65b4() { fn_7101202488(this); }
};

void* fn_71011c068c(UnkObj_7100af65b4* self);
extern void* g_unk_717271e8f8;
void fn_71011b6e0c(void* root, UnkMsg_7100af65b4* msg);

void fn_7100af65b4(UnkObj_7100af65b4* self) {
    UnkPropBag bag;
    fn_7100aa3b9c(&bag, CStrId("StateIdx", true), self->field_0x2a4);
    UnkMsg_7100af65b4 msg(fn_71011c068c(self), 4, &bag);
    fn_71011b6e0c(g_unk_717271e8f8, &msg);
}
