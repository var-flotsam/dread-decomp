// GUI::CHUD (unit 2751): the HUD's missile-change callback (bound in fn_71011df648); passes a copy of the
// item-change message to the missile counter update. Names and layouts are placeholders.
// MATCHES only when defined in src/game/GUI/CHUD/CHUD_MissileCount_l1e2950.cpp, after fn_71011e2950:
// Clang then infers that fn_71011e2950 only reads its message, and keeps the copy's id in a register
// for the destructor (as the original, one unit, does). Append the function below to that file.

bool fn_71011e7c18(UnkHud_2950* self, const UnkItemChange_2950* change) {
    UnkItemChange_2950 copy = *change;
    CHUD_UpdateMissileCounterOnItemChange(self, &copy);
    return false;
}
