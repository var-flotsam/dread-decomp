// Translation unit 1119 (no reflected class): passes an event to the base handler (fn_71006fe894) and, when
// the event's state (+0x34) is 0 or 4, also to the member at +0x12ca8 (fn_71003878c8). Names and layouts are
// placeholders.
#include "types.h"

struct UnkUnit1119Event_b54 {
    u8 pad_0x0[0x34];
    u32 state;
};

void fn_71006fe894(char* self, UnkUnit1119Event_b54* event, bool flag);
void fn_71003878c8(char* member, UnkUnit1119Event_b54* event, bool flag);

void fn_7100700e18(char* self, UnkUnit1119Event_b54* event, bool flag) {
    fn_71006fe894(self, event, flag);
    if (event->state == 0 || event->state == 4)
        fn_71003878c8(self + 0x12ca8, event, flag);
}
