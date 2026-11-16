// CRobotAIComponent's unit: runs the base step; when it gives 0, answers 0 if the event's owner AI is a
// CEmmyAIComponent and the event carries one of a set of flags, else 2. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkSluggerEvent_l661e24;
struct UnkEventAI_l661e24 {
    virtual const base::reflection::CClass* vf_0x0();
};
struct UnkEventActor_l661e24 {
    u8 pad_0x0[0x240];
    UnkEventAI_l661e24* ai;
};
struct UnkEventLink_l661e24 {
    u8 pad_0x0[0x10];
    UnkEventActor_l661e24* actor;
};
struct UnkSluggerEvent {
    u8 pad_0x0[0x8];
    UnkEventLink_l661e24* link;
    u8 pad_0x10[0xb4 - 0x10];
    u32 flags;
};

u32 fn_7100661e24(UnkSluggerAI* self, UnkSluggerEvent* event) {
    u32 result = fn_7101105e44(self, event);
    if (!result) {
        u32 answer = 2;
        if (event->link && event->link->actor) {
            UnkEventAI_l661e24* ai = event->link->actor->ai;
            if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
                answer = 0;
        }
        if (event->flags & 0x2a05d6fe)
            return result;
        return answer;
    }
    return result;
}
