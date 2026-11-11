// CDaivoAIComponent's unit (tools/tu_map.py unit 766): runs the base handler (fn_7101108c9c) and,
// when it succeeds, finds the "CONTROLLER" child (fn_71004dc4b4, a typed reflection lookup) of the
// object at +0x10 of the one at +0x14890 and notifies it (fn_71004e10dc). Returns the base's result.
// Names and layouts are placeholders.
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkDaivoArg_b46a1;
struct UnkDaivoController_b46a1;

struct UnkDaivoLink_b46a1 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkDaivoAIComponent_b46a1 {
    u8 pad_0x0[0x14890];
    UnkDaivoLink_b46a1* field_0x14890;
};

UnkDaivoController_b46a1* fn_71004dc4b4(void* owner);
void fn_71004e10dc(UnkDaivoController_b46a1* controller);

bool fn_71004df7fc(UnkDaivoAIComponent_b46a1* self, UnkDaivoArg_b46a1* arg) {
    bool result = fn_7101108c9c(self, arg);
    if (result) {
        if (UnkDaivoLink_b46a1* link = self->field_0x14890) {
            if (link->field_0x10) {
                if (UnkDaivoController_b46a1* controller = fn_71004dc4b4(link->field_0x10))
                    fn_71004e10dc(controller);
            }
        }
    }
    return result;
}
