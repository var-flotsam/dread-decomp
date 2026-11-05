// CGooshockerAIComponent's unit (tools/tu_map.py unit 854): fn_710059ea38 runs the base handler
// (fn_710059b8d4) and then, on the event's flag (+0x8), either wakes the owner's part
// (fn_71003be154 -> fn_71007d29d4) and sets the flag at +0x148e8, or clears it and, while the
// counter at +0x147f8 is positive, sets the "on" timeline (fn_710059e4f0) once the flag at +0x148e9
// allows it, else calls fn_710059aa2c. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkGooshockerState;
struct UnkGooplotAI_b55;
struct UnkGooplotOwner_b55;

u32 fn_710059b8d4(void* self, void* event);
char* fn_71003be154(UnkActor*);
void fn_71007d29d4(char* part);
void fn_710059aa2c(UnkGooplotAI_b55* ai, UnkGooplotOwner_b55* owner);
void fn_710059e4f0(UnkGooshockerState* self, bool mode);

struct UnkGooshockerEvent_b63a3 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

struct UnkGooshockerAIMsg_b63a3 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146db - 0x38];
    bool field_0x146db;
    u8 pad_0x146dc[0x147f8 - 0x146dc];
    s32 field_0x147f8;
    u8 pad_0x147fc[0x148e8 - 0x147fc];
    bool field_0x148e8;
    bool field_0x148e9;
};

u32 fn_710059ea38(UnkGooshockerAIMsg_b63a3* self, UnkGooshockerEvent_b63a3* event) {
    u32 result = fn_710059b8d4(self, event);
    if (event->field_0x8) {
        if (char* part = fn_71003be154(self->owner))
            fn_71007d29d4(part);
        self->field_0x148e8 = true;
    } else {
        s32 count = self->field_0x147f8;
        self->field_0x148e8 = false;
        if (count <= 0) {
            // fn_710059aa2c reads only its first argument; the second is left as it is.
            UnkGooplotOwner_b55* unused;
            fn_710059aa2c(reinterpret_cast<UnkGooplotAI_b55*>(self), unused);
        } else if (self->field_0x148e9) {
            fn_710059e4f0(reinterpret_cast<UnkGooshockerState*>(self), 1);
            self->field_0x146db = true;
        }
    }
    return result;
}
