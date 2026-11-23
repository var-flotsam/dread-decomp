// Unit 1691 (a boss rush menu button): when one of the flags at +0xd1/+0xd2 is set, asks on the main
// menu's pop-up composition whether to clear the record of the mode at +0x10 (boss rush, survival or
// dread rush), with accept and back texts and two callbacks on the button. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;
using base::reflection::UnkReflObject;

struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;

struct UnkClearRecordButton_lb4af98 {
    u8 pad_0x0[0x10];
    s32 mode;  // 0x10
    u8 pad_0x14[0xbd];
    bool field_0xd1;
    bool field_0xd2;
};

void fn_7100b4cbb4(UnkClearRecordButton_lb4af98* self);
void fn_7100b4cc3c(UnkClearRecordButton_lb4af98* self);

// A callback: an object and a function to call on it.
struct UnkClearRecordCallback_lb4af98 {
    UnkClearRecordButton_lb4af98* object;
    void (*function)(UnkClearRecordButton_lb4af98*);
};

struct UnkMainMenu_lb4af98 {
    u8 pad_0x0[0x540];
    UnkReflObject* popup;  // 0x540
};

// The prompt's text: built empty, then given the mode's text (the old string released, the new one
// taken from the other id, which is left empty).
struct UnkPromptText_lb4af98 : UnkEmptyStrId {
    void Take(CStrId&& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
        }
        data = other.data;
        other.data = &pool->empty;
    }
    ~UnkPromptText_lb4af98() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    const CStrId* Id() const { return reinterpret_cast<const CStrId*>(this); }
};

void fn_7100b4af98(UnkClearRecordButton_lb4af98* self) {
    if (!self->field_0xd1 && !self->field_0xd2)
        return;
    UnkReflObject* object = fn_7100e033a8(g_unk_7101d47310);
    UnkMainMenu_lb4af98* menu =
        fn_710009d50c(object->vf_0x0(), fn_7100b34230()) ? reinterpret_cast<UnkMainMenu_lb4af98*>(object) : nullptr;
    UnkReflObject* popup = menu->popup;
    if (!popup || !fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()))
        return;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    UnkPromptText_lb4af98 text;
    switch (self->mode) {
    case 0:
        text.Take(CStrId("#GUI_BOSSRUSH_PROMPT_CLEAR_RECORD_BOSSRUSH", true));
        break;
    case 1:
        text.Take(CStrId("#GUI_BOSSRUSH_PROMPT_CLEAR_RECORD_SURVIVAL", true));
        break;
    case 2:
        text.Take(CStrId("#GUI_BOSSRUSH_PROMPT_CLEAR_RECORD_DREADRUSH", true));
        break;
    }
    UnkClearRecordCallback_lb4af98 accept_callback = {self, fn_7100b4cbb4};
    UnkClearRecordCallback_lb4af98 back_callback = {self, fn_7100b4cc3c};
    fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(popup), text.Id(), &accept, &back,
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}
