// GUI::CHUD (unit 2751): the HUD's bomb-change callback (bound in fn_71011df648); passes a copy of the
// item-change message to the bomb counter update. Names and layouts are placeholders.
// MATCHES only when defined in src/game/GUI/CHUD/CHUD_BombCount_l1e42cc.cpp, after fn_71011e42cc:
// Clang then infers that fn_71011e42cc only reads its message, and keeps the copy's id in a register
// for the destructor (as the original, one unit, does). Append the function below to that file.

bool fn_71011e7cbc(UnkHud_42cc* self, const UnkItemChange_42cc* change) {
    UnkItemChange_42cc copy = *change;
    fn_71011e42cc(self, &copy);
    return false;
}