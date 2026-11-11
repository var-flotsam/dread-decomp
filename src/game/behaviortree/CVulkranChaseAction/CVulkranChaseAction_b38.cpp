// behaviortree::CVulkranChaseAction: the update. Runs its three steps (each always), and answers 2
// (running) when all of them hold, else 6. Names and layouts are placeholders.
#include "game/behaviortree/CVulkranChaseAction.h"
#include "types.h"

struct UnkVulkranChaseAction;

bool fn_71006d78e0(UnkVulkranChaseAction* self);
bool fn_71006d7d80(UnkVulkranChaseAction* self, f32 dt);

u32 fn_71006d7884(UnkVulkranChaseAction* self, f32 dt) {
    bool ok = fn_71006d78e0(self);
    ok = ok & fn_71006d7d80(self, dt);
    ok = ok & fn_71006d7f78(self);
    return ok ? 2 : 6;
}
