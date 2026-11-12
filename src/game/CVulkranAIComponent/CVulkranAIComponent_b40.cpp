// CVulkranAIComponent unit: fn_71006dcaa4 clears the flag at 0x14731, hands the owner (+0x30) to the
// registry at +0x368 of the lazily created manager (fn_71006ded20), and, when asked to and the owner has
// its object at +0x220, resets its entries 0xb and 6 (fn_7101178e08) and sets entry 8 (fn_710117662c),
// the third argument telling whether the object at +0x218 is in a state other than 4. Ends with
// fn_71006dd25c. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

struct UnkActorTarget;
void fn_710117662c(UnkActorTarget* target, u32 value, u32 extra, bool apply, bool flag, f32 time);
void fn_7101178e08(UnkActorTarget* target, u32 a, bool, bool, f32 value);

struct UnkOwner_vk {
    u8 pad_0x0[0x218];
    void* field_0x218;
    UnkActorTarget* field_0x220;
};

struct UnkVulkranAIComponent {
    u8 pad_0x0[0x30];
    UnkOwner_vk* owner;
    u8 pad_0x38[0x146f9];
    bool field_0x14731;
};

void fn_71006ded20(void* registry, UnkOwner_vk* owner);
void fn_71006dd25c(UnkVulkranAIComponent* self);

void fn_71006dcaa4(UnkVulkranAIComponent* self, bool reset) {
    self->field_0x14731 = false;
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    fn_71006ded20(reinterpret_cast<char*>(m) + 0x368, self->owner);
    UnkOwner_vk* owner = self->owner;
    UnkActorTarget* target = owner->field_0x220;
    if (target && reset) {
        void* other = owner->field_0x218;
        fn_7101178e08(target, 0xb, 0, 0, -1.0f);
        fn_7101178e08(target, 6, 0, 0, -1.0f);
        fn_710117662c(target, 8, 0, other ? fn_71010cc3a4(other) != 4 : false, true, -1.0f);
    }
    fn_71006dd25c(self);
}
