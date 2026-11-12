// CDroppableComponent unit (tools/tu_map.py unit 1216): binds the component's two callbacks to
// the object behind the weak handle at +0xa0, after making sure the handle is set (fn_71007ba354);
// fn_71007ba580 stops the playing sound (the handle at +0x78), releases the path at +0x88 and
// drops the unit's live count. Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "game/CComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHolder;
void fn_71007ba354(void* unused, UnkHolder* holder);

void fn_71007bb430();
void fn_71007bb43c();

struct UnkDroppableDelegate_b51 {
    void* owner;
    void (*func)();
};

void fn_71009cdc3c(UnkWeakTarget* target, const UnkDroppableDelegate_b51* delegate);
void fn_71009cdc48(UnkWeakTarget* target, const UnkDroppableDelegate_b51* delegate);

struct UnkDroppable_b51 {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x68];
    UnkWeakHandle* field_0xa0;
};

bool fn_71007ba2d4(UnkDroppable_b51* self) {
    fn_71007ba354(self->field_0x30, reinterpret_cast<UnkHolder*>(&self->field_0xa0));
    if (self->field_0xa0 && self->field_0xa0->target) {
        UnkDroppableDelegate_b51 first = {self, fn_71007bb430};
        fn_71009cdc3c(self->field_0xa0->target, &first);
        UnkDroppableDelegate_b51 second = {self, fn_71007bb43c};
        fn_71009cdc48(self->field_0xa0 ? self->field_0xa0->target : nullptr, &second);
    }
    return true;
}

struct UnkHandleOwner_71010c4230;

struct UnkSoundRef;
extern s32 g_unk_717271ab44;

struct UnkSoundEntry_b51 {
    void* field_0x0;
    s64 serial;
};

struct UnkShipRechargeEntity_b49a4;  // the owner as CShipRechargeComponent_StopSound_b49a4.cpp names it

struct UnkDroppableStop_b51 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x40];
    UnkSoundHandle sound;       // 0x78
    UnkStrIdPathResource path;  // 0x88
};

void fn_71007ba580(UnkDroppableStop_b51* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    UnkSoundEntry_b51* entry = reinterpret_cast<UnkSoundEntry_b51*>(self->sound.ref);
    if (entry && entry->serial == self->sound.serial && self->sound.serial != -1 && entry->field_0x0) {
        fn_7100d7c510(static_cast<UnkShipRechargeEntity_b49a4*>(self->owner), &self->sound, 0.0f, 0.0f);
        self->sound = g_unk_7108b6c460;
    }
    fn_7100027044(&self->path);
    --g_unk_717271ab44;
}
