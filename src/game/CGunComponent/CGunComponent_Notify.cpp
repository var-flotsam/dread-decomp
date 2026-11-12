// CGunComponent: fn_71008409d0 tells the deferred callback set at +0x198 `true`
// (fn_71000303c0). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;

struct UnkGunComponent {
    u8 pad_0x0[0x198];
    UnkDeferredSet<UnkDeferredCallback<bool>> field_0x198;
};

void fn_71008409d0(UnkGunComponent* self) {
    bool arg = true;
    fn_71000303c0(&self->field_0x198, &arg);
}
