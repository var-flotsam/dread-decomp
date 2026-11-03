// The user data's owner (g_unk_7101d47308; unit 2768, see src/game/UnkUserDataOwner.cpp): sets the
// flag at 0x2a8, first signalling the light event at 0x254 when the flag at 0x49 (read under the
// mutex at 0x280) is set. Names and layout are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

void fn_710006669c(nn::os::LightEventType*);

struct UnkUserDataOwnerFlag_b48a3 {
    u8 pad_0x0[0x49];
    bool field_0x49;
    u8 pad_0x4a[0x254 - 0x4a];
    nn::os::LightEventType event;
    u8 pad_0x260[0x280 - 0x254 - sizeof(nn::os::LightEventType)];
    nn::os::MutexType mutex;
    u8 pad_0x2a0[0x2a8 - 0x280 - sizeof(nn::os::MutexType)];
    bool field_0x2a8;
};

void fn_71012037fc(UnkUserDataOwnerFlag_b48a3* self, bool flag) {
    fn_710006691c(&self->mutex);
    bool ready = self->field_0x49;
    fn_7100066920(&self->mutex);
    if (ready)
        fn_710006669c(&self->event);
    self->field_0x2a8 = flag;
}
