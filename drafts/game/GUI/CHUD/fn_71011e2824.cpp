// GUI::CHUD (unit 2751): refreshes the missile counter by sending the missile update (fn_71011e2950) a
// change of ITEM_WEAPON_MISSILE_MAX with nothing filled. Names and layouts are placeholders.
// Graded (72%) only appended to src/game/GUI/CHUD/CHUD_MissileCount_l1e2950.cpp (the callee's file, the
// original's unit); in a file of its own the message's id is reloaded after the call. Remaining
// difference: the original tail-calls the last release (fn_71000803a4); this form calls it and returns.

// The item-change message built from an id taken by value.
struct UnkItemChangeMsg_2824 : UnkItemChange_2950 {
    UnkItemChangeMsg_2824(base::global::CStrId id, bool filled) : UnkItemChange_2950{id, {}, filled} {}
};

static inline bool Notify_2824(UnkHud_2950* self, const UnkItemChange_2950& change) {
    return fn_71011e2950(self, &change);
}

void fn_71011e2824(UnkHud_2950* self) {
    Notify_2824(self, UnkItemChangeMsg_2824(Qid_2950(g_unk_717275a218).id, false));
}