// CChozoWarriorAIComponent unit (tools/tu_map.py): on a hit whose target object has the kind at 0x71726d1260,
// sends the owner's +0x220 sub-object a fixed hash (UnkSpitSend) and, with no attack picked yet and the
// owner's +0x278 object not busy, picks one of two (at random when +0x14999 is set); then defers to
// fn_71011088e4. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAttackComponent.h"
#include "game/CChozoWarriorAIComponent.h"
#include "types.h"

extern "C" int rand();

struct UnkChozoWarriorKind_b67a2;

struct UnkChozoWarriorHitObject_b67a2 {
    u8 pad_0x0[0x20];
    UnkChozoWarriorKind_b67a2* kind;
};

struct UnkChozoWarriorHitTarget_b67a2 {
    u8 pad_0x0[0x10];
    UnkChozoWarriorHitObject_b67a2* object;
};

struct UnkChozoWarriorOwner_l499f78 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x50];
    void* field_0x278;
};

struct UnkChozoWarriorAttack_l499f78 {
    u8 pad_0x0[0x50];
};

struct UnkChozoWarriorAI_l499f78 {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner_l499f78* owner;  // 0x30
    u8 pad_0x38[0x14999 - 0x38];
    bool field_0x14999;
    u8 pad_0x1499a[0x14a30 - 0x1499a];
    UnkChozoWarriorAttack_l499f78 attack_0x14a30;
    u8 pad_0x14a80[0x14ae0 - 0x14a80];
    UnkChozoWarriorAttack_l499f78 attack_0x14ae0;
    UnkChozoWarriorAttack_l499f78* current;  // 0x14b30
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l499f78, current) == 0x14b30, "");

void fn_71011088e4(UnkChozoWarriorAI_l499f78* self, void* arg, UnkChozoWarriorHitTarget_b67a2** hit);

void fn_7100499f78(UnkChozoWarriorAI_l499f78* self, void* arg, UnkChozoWarriorHitTarget_b67a2** hit) {
    if (hit) {
        void* target = self->owner->field_0x220;
        void* object = self->owner->field_0x278;
        if (target && object && *hit && (*hit)->object && (*hit)->object->kind == g_unk_71726d1260) {
            UnkSpitSend(target, 0x972422f600dd3f97, 0xf4061ff25a2163d6);
            if (!self->current && !fn_7101136308(object)) {
                if (self->field_0x14999 && (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) > 0.5f)
                    self->current = &self->attack_0x14ae0;
                else
                    self->current = &self->attack_0x14a30;
            }
        }
    }
    fn_71011088e4(self, arg, hit);
}
