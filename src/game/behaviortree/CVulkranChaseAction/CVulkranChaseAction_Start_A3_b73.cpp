// behaviortree::CVulkranChaseAction (tools/tu_map.py): starting the action. After the base start
// (fn_7100394bd0), resets the fields at +0x48..+0x63, the id at +0x40 to the empty id, caches the owner's
// (+0x18) AI component (+0x240) cast to CVulkranAIComponent (+0x30) and its +0x10 object cast to
// CCharClassVulkranAIComponent (+0x38), then hooks the callback fn_71006d847c (link at +0x68, handle at +0x80)
// up to the owner's character class (+0x220), as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkSelf;
void fn_7100394bd0(UnkSelf*);
void fn_71006d847c();

struct UnkVulkranTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkVulkranChase_A3_b73 {
    u8 pad_0x0[0x18];
    char* owner;
    u8 pad_0x20[0x10];
    UnkReflObject* ai;     // 0x30
    UnkReflObject* cls;    // 0x38
    CStrId id;             // 0x40
    bool field_0x48;
    bool field_0x49;
    u8 pad_0x4a[0x2];
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    bool field_0x58;
    bool field_0x59;
    bool field_0x5a;
    bool field_0x5b;
    f32 field_0x5c;
    f32 field_0x60;
    u8 pad_0x64[0x4];
    UnkVulkranTrackLink_A3_b73 link;  // 0x68
    UnkWeakHandle* handle;            // 0x80
};

void fn_71006d73f0(UnkVulkranChase_A3_b73* self) {
    fn_7100394bd0(reinterpret_cast<UnkSelf*>(self));
    self->field_0x48 = false;
    self->field_0x49 = true;
    self->field_0x4c = 0;
    self->field_0x50 = 0;
    self->field_0x54 = 0;
    self->field_0x58 = true;
    self->field_0x59 = false;
    self->field_0x5a = false;
    self->field_0x5b = true;
    self->field_0x5c = -1.0f;
    self->field_0x60 = -1.0f;
    self->id = g_unk_7101d470e0;
    UnkReflObject* ai = *reinterpret_cast<UnkReflObject**>(self->owner + 0x240);
    UnkReflObject* vulkran = ai && fn_710009d50c(ai->vf_0x0(), fn_71006db2b8()) ? ai : nullptr;
    self->ai = vulkran;
    UnkReflObject* cls = vulkran ? *reinterpret_cast<UnkReflObject**>(reinterpret_cast<char*>(vulkran) + 0x10) : nullptr;
    self->cls = cls && fn_710009d50c(cls->vf_0x0(), fn_71006d85b4()) ? cls : nullptr;
    if (*reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220)) {
        UnkVulkranTrackLink_A3_b73* link = &self->link;
        link->context = self;
        link->callback = fn_71006d847c;
        UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        if (track) {
            asm("" : "+r"(self));
            fn_7100316018(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
}
