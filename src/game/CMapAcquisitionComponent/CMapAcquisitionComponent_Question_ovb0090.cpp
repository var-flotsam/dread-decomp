// CMapAcquisitionComponent's unit (tools/tu_map.py unit 1594): the save-station question through the
// GUI composition (the game manager's +0x2258, its +0x10) with "#GUI_SAVESTATION_QUESTION", accept and
// back texts and two callbacks on the component: after the delay at +0x804 runs out (fn_7100a86138,
// callbacks fn_7100a88610/fn_7100a8861c) and directly (fn_7100a86d00, callbacks
// fn_7100a88640/fn_7100a88644). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkMapAcquisition_b50a5;
void fn_7100a88610(UnkMapAcquisition_b50a5* self);
void fn_7100a8861c(UnkMapAcquisition_b50a5* self);
void fn_7100a88640(UnkMapAcquisition_b50a5* self);
void fn_7100a88644(UnkMapAcquisition_b50a5* self);

// A callback: an object and a function to call on it.
struct UnkMapCallback_ovb0090 {
    UnkMapAcquisition_b50a5* object;
    void (*function)(UnkMapAcquisition_b50a5*);
};

struct UnkGuiComposition_ovb0090;

struct UnkGuiFlagRef_ovb0090 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

// Asks the save-station question through the GUI composition, with accept and back callbacks.
static inline __attribute__((always_inline)) void AskSaveQuestion(UnkMapAcquisition_b50a5* self,
                                                                  void (*on_accept)(UnkMapAcquisition_b50a5*),
                                                                  void (*on_back)(UnkMapAcquisition_b50a5*)) {
    UnkGuiFlagRef_ovb0090* ref = static_cast<UnkGuiFlagRef_ovb0090*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    CStrId text("#GUI_SAVESTATION_QUESTION", true);
    UnkMapCallback_ovb0090 accept_callback = {self, on_accept};
    UnkMapCallback_ovb0090 back_callback = {self, on_back};
    fn_7100b5fde0(gui, &text, &accept, &back, &accept_callback, &back_callback, 0, -1.0f);
}

struct UnkMapAcquisitionQ_ovb0090 {
    u8 pad_0x0[0x804];
    f32 question_delay;  // 0x804
};

void fn_7100a86138(UnkMapAcquisitionQ_ovb0090* self, f32 dt) {
    if (!(self->question_delay > 0.0f))
        return;
    self->question_delay -= dt;
    if (!(self->question_delay <= 0.0f))
        return;
    AskSaveQuestion(reinterpret_cast<UnkMapAcquisition_b50a5*>(self), fn_7100a88610, fn_7100a8861c);
}

void fn_7100a86d00(UnkMapAcquisition_b50a5* self) {
    AskSaveQuestion(self, fn_7100a88640, fn_7100a88644);
}
