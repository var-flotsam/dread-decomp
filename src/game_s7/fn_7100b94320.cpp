// Sets up the boss rush results screen: looks up its composition ("bossrushresults") through
// the GUI registry, finds its labels, fills in the time, the defeated-boss count and the record
// flag from the stats, and binds the accept (and, outside survival, quit) button to vf_0x48.
// Class names come from reflection (GUI::CLabel, GUI::CSprite); names and layouts are
// placeholders.
#include "UnkGuiS7.h"

using namespace unk_gui;

struct UnkResults_7100b94320 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    UnkOwner* owner;
    u8 unk_0x10[0x20];
    UnkGuiObj* composition;
    UnkGuiObj* title;
    UnkGuiObj* time;
    UnkGuiObj* time_diff;
    UnkGuiObj* stat_label_1;
    UnkGuiObj* stat_value_1;
    UnkGuiObj* stat_label_2;
    UnkGuiObj* stat_value_2;
    u8 unk_0x70[0x8];
    UnkGuiObj* new_record;
    UnkGuiObj* times_defeated;
    UnkGuiObj* accept;
    bool is_record;
    s32 field_0x94;
    f32 field_0x98;
    u8 unk_0x9c[0x4];
    u8 field_0xa0[0x8];

    UnkGuiObj* Find(const char* name, const UnkClass* (*get_class)()) {
        return FindChild(composition, name, get_class);
    }
};

// NON_MATCHING(minor): the five calls after the lookup load the composition through the slot's
// address kept in a register (x22, from the pre-indexed stores that set it); ours addresses it
// from the object. Same behaviour.
void fn_7100b94320(UnkResults_7100b94320* self) {
    UnkGuiRoot* root = g_unk_717271e8f8;
    self->composition = fn_71011b69d8(root, CRntString("bossrushresults")).Get();
    UnkGuiObj*& composition = self->composition;
    fn_71011d968c(g_unk_717271e8f8->menu, composition);
    fn_71011cdce8(composition, 1);
    fn_71011cf2a4(composition, 1);
    fn_71011cde44(composition, 2);
    fn_71011b6bbc(g_unk_717271e8f8, composition, false);

    bool survival = self->owner->mode->survival;
    self->title = self->Find("RushTittle", fn_71011e8dfc);
    self->times_defeated = self->Find("TimesDefeated", fn_71011e8dfc);
    fn_71011cdce8(self->times_defeated, 0);
    self->time = self->Find("RushTime", fn_71011e8dfc);
    fn_71011cdce8(self->time, 0);
    self->time_diff = self->Find("RushTimeDiff", fn_71011e8dfc);
    fn_71011cdce8(self->time_diff, 0);
    self->stat_label_1 = self->Find("StatLabel01", fn_71011e8dfc);
    self->stat_value_1 = self->Find("StatValue01", fn_71011e8dfc);
    self->stat_label_2 = self->Find("StatLabel02", fn_71011e8dfc);
    self->stat_value_2 = self->Find("StatValue02", fn_71011e8dfc);
    self->new_record = self->Find("NewRecordLabel", fn_71011e8dfc);
    self->accept = self->Find("Accept", fn_71011e8dfc);

    f32 alpha = survival ? 0.0f : 1.0f;
    fn_71011cfba8(self->accept, alpha);
    UnkStats* stats = g_unk_71726bc570;
    fn_71011ec424(self->title, CRntString("#GUI_BOSSRUSH_SURVIVAL_LABEL"));
    fn_71011ec424(self->stat_label_1, CRntString("#GUI_BOSSRUSH_TIP_DEFEATED_BOSSES_COUNT"));
    fn_71011cdce8(self->stat_label_1, 1);
    fn_71011cfba8(self->stat_label_1, alpha);
    s32 count = survival ? stats->field_0xd0 : stats->field_0xc60;
    fn_71011ed0f0(self->stat_value_1, count);
    fn_71011cdce8(self->stat_value_1, 1);
    fn_71011cfba8(self->stat_value_1, alpha);
    fn_71011ec424(self->stat_label_2, CRntString("#GUI_BOSSRUSH_ETERNAL_RUSH_TIME_RESULT"));
    fn_71011cdce8(self->stat_label_2, 1);
    fn_71011cfba8(self->stat_label_2, alpha);
    fn_71011ec468(self->stat_value_2, fn_7100af45b0((s32)(survival ? Trunc((f32)stats->field_0xa38) : stats->field_0xc64), 0), false);
    fn_71011cdce8(self->stat_value_2, 1);
    fn_71011cfba8(self->stat_value_2, alpha);
    bool is_record = survival ? stats->field_0xc68 : false;
    self->is_record = is_record;
    fn_71011cdc30(self->new_record, is_record);
    fn_71011ed018(self->new_record, 0);
    fn_71011ccb28(self->new_record, 0.322f);
    fn_71011cc844(self->new_record, count > 99 ? 0.49f : count > 9 ? 0.51f : 0.53f);
    fn_71011cfba8(self->new_record, alpha);
    fn_7100eb7358(self->field_0xa0, 0, 0);
    fn_71011f8bdc(self->Find("Background", fn_71011f6890), g_unk_7172769ca0[0]);

    if (survival) {
        self->field_0x94 = 0;
        self->field_0x98 = -1.0f;
        UnkGuiObj* button = fn_71011d9508(self->composition, &self->composition->field_0x190);
        s32 id = 0x11;
        if (button)
            fn_7100b95760(button, &id, std::bind(&UnkResults_7100b94320::vf_0x48, self), nullptr, 2);
    } else {
        self->field_0x94 = -1;
        fn_71011cdc30(self->accept, false);
        UnkGuiObj* quit = self->Find("Quit", fn_71011e8dfc);
        fn_71011ec424(quit, CRntString("#GUI_GENERAL_LABEL_BACK"));
        fn_71011cdce8(quit, 1);
        UnkGuiObj* button = fn_71011d9508(composition, &composition->field_0x190);
        s32 id = 0x12;
        if (button)
            fn_7100b95760(button, &id, std::bind(&UnkResults_7100b94320::vf_0x48, self), nullptr, 2);
    }
}
