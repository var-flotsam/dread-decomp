// Sets up the boss rush practice results screen: looks up its composition ("bossrushresults"),
// finds its labels, shows the practice time, the boss's name and the difference from the boss's
// best time (coloured by sign), and binds the buttons to vf_0x48 and fn_7100b92b3c. Names and
// layouts are placeholders.
#include "UnkGuiS7.h"

using namespace unk_gui;

struct UnkPractice_7100b921e0 {
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
    u8 unk_0x8[0x28];
    UnkGuiObj* composition;
    UnkGuiObj* title;
    UnkGuiObj* time;
    UnkGuiObj* time_diff;
    UnkGuiObj* stat_label;
    UnkGuiObj* stat_value;
    UnkGuiObj* stat_value_diff;
    UnkGuiObj* new_record;
    UnkGuiObj* times_defeated;
    UnkGuiObj* accept;
    UnkGuiObj* quit;
    u8 unk_0x88[0x4];
    s32 field_0x8c;
    f32 field_0x90;

    UnkGuiObj* Find(const char* name, const UnkClass* (*get_class)()) {
        return FindChild(composition, name, get_class);
    }

    void fn_7100b92b3c();
};

// NON_MATCHING(minor): the colour's copy differs: the original sinks all four member copies
// below the branch (one address per member), ours only the last. Same behaviour.
void fn_7100b921e0(UnkPractice_7100b921e0* self) {
    UnkGuiRoot* root = g_unk_717271e8f8;
    self->composition = fn_71011b69d8(root, CRntString("bossrushresults")).Get();
    UnkGuiObj*& composition = self->composition;
    fn_71011d968c(g_unk_717271e8f8->menu, composition);
    fn_71011cdce8(composition, 1);
    fn_71011cf2a4(composition, 1);
    fn_71011cde44(composition, 2);
    fn_71011b6bbc(g_unk_717271e8f8, composition, false);

    self->title = FindChild(composition, "RushTittle", fn_71011e8dfc);
    self->times_defeated = self->Find("TimesDefeated", fn_71011e8dfc);
    self->time = self->Find("RushTime", fn_71011e8dfc);
    fn_71011cdce8(self->time, 0);
    self->time_diff = self->Find("RushTimeDiff", fn_71011e8dfc);
    fn_71011cdce8(self->time_diff, 0);
    self->stat_label = self->Find("StatLabel01", fn_71011e8dfc);
    self->stat_value = self->Find("StatValue01", fn_71011e8dfc);
    self->stat_value_diff = self->Find("StatValueDiff01", fn_71011e8dfc);
    fn_71011cdce8(self->stat_value_diff, 1);
    self->new_record = self->Find("NewRecordLabel", fn_71011e8dfc);
    self->accept = self->Find("Accept", fn_71011e8dfc);
    self->quit = self->Find("Quit", fn_71011e8dfc);
    fn_71011cdce8(self->quit, 1);
    fn_71011ec424(self->accept, CRntString("#GUI_BOSSRUSH_A_REPEAT"));
    fn_71011cfba8(self->accept, 0.0f);
    fn_71011cfba8(self->quit, 0.0f);

    UnkStats* stats = g_unk_71726bc570;
    bool dread = stats->field_0xb8 ? stats->field_0xb9 : false;
    fn_71011ec424(self->stat_label, CRntString("#GUI_BOSSRUSH_PRACTICE_TIME_RESULT"));
    fn_71011cdce8(self->stat_label, 1);
    fn_71011cfba8(self->stat_label, 0.0f);
    fn_71011ec468(self->stat_value, fn_7100af45b0((s32)Trunc((f32)stats->field_0xa38), 0), false);
    fn_71011cdce8(self->stat_value, 1);
    fn_71011cfba8(self->stat_value, 0.0f);
    fn_71011f8bdc(self->Find("Background", fn_71011f6890), dread ? g_unk_7172769ca0[1] : g_unk_7172769ca0[0]);
    fn_71011ec424(self->title, CRntString(dread ? "#GUI_BOSSRUSH_DREADRUSH_PRACTICE_TITLE"
                                                : "#GUI_BOSSRUSH_BOSSRUSH_PRACTICE_TITLE"));
    s32 boss = stats->field_0x100;
    fn_71011ecfbc(self->times_defeated, fn_7100af4650(boss, 0));
    fn_71011ed084(self->times_defeated, 0);
    {
        UnkVec2f offset = {0.035f, -0.3f};
        fn_71011cde64(self->times_defeated, &offset, 2, 1);
    }
    fn_71011cdce8(self->new_record, stats->field_0xc68);
    fn_71011ed018(self->new_record, 0);
    fn_71011ccb28(self->new_record, 0.322f);
    fn_71011cc844(self->new_record, 0.385f);
    fn_71011cfba8(self->new_record, 0.0f);

    f32* best = stats->times.Find(boss);
    if (best != stats->times.End()) {
        s32 diff = (s32)fn_7100dce130(stats) - (s32)*best;
        fn_71011ec468(self->stat_value_diff, fn_7100af45b0(diff, 1), false);
        if (diff == 0)
            fn_71011cfc24(self->stat_value_diff, UnkColor(g_unk_7101aa00b8), 1);
        else
            fn_71011cfc24(self->stat_value_diff, UnkColor(diff < 0 ? g_unk_7101aa00a8 : g_unk_7101aa00c8), 1);
        fn_71011cfba8(self->stat_value_diff, 0.0f);
    } else {
        fn_71011cdc30(self->stat_value_diff, false);
    }

    {
        s32 id = 0x12;
        if (UnkGuiObj* c = composition)
            fn_7100b951f4(c, &id, std::bind(&UnkPractice_7100b921e0::vf_0x48, self), nullptr, 2);
    }
    {
        s32 id = 0x11;
        if (UnkGuiObj* c = composition)
            fn_7100b951f4(c, &id, std::bind(&UnkPractice_7100b921e0::fn_7100b92b3c, self), nullptr, 2);
    }
    self->field_0x8c = 0;
    self->field_0x90 = -1.0f;
}
