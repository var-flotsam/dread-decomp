// CInputComponent unit: for each input action bit, asks the game manager's input mapping
// (fn_7100f0cc48) which buttons it is bound to, and appends the action to a fixed-capacity list
// when every pressed button is part of that binding. Names and layouts are placeholders.
#include "game/CPlayerInfo.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkEventTable_b0188;

struct UnkActionList_l872d00 {
    u32 capacity;
    u32 count;
    u32* data;

    void PushBack(u32 value) {
        if (count < capacity) {
            data[count] = value;
            count++;
        }
    }
};

struct UnkActionOwner_l872d00 {
    u8 pad_0x0[0x38];
    UnkActionList_l872d00 actions;
};

// Appends `action` when every pressed button is part of its binding.
inline void AddIfBound(UnkEventTable_b0188* mapping, u32 buttons, UnkActionList_l872d00& actions, u32 action) {
    if ((buttons & ~CPlayerInfo_LookupBindingInSelectedSlot(mapping, action)) == 0)
        actions.PushBack(action);
}

void fn_7100872d00(u32 buttons, UnkActionOwner_l872d00* owner) {
    UnkEventTable_b0188* mapping = reinterpret_cast<UnkEventTable_b0188*>(g_unk_7101d47310->field_0x38);
    UnkActionList_l872d00& actions = owner->actions;
    AddIfBound(mapping, buttons, actions, 0x1);
    AddIfBound(mapping, buttons, actions, 0x2);
    AddIfBound(mapping, buttons, actions, 0x4);
    AddIfBound(mapping, buttons, actions, 0x10);
    AddIfBound(mapping, buttons, actions, 0x20);
    AddIfBound(mapping, buttons, actions, 0x40);
    AddIfBound(mapping, buttons, actions, 0x80);
    AddIfBound(mapping, buttons, actions, 0x100);
    AddIfBound(mapping, buttons, actions, 0x200);
    AddIfBound(mapping, buttons, actions, 0x400);
    AddIfBound(mapping, buttons, actions, 0x800);
    AddIfBound(mapping, buttons, actions, 0x1000);
    AddIfBound(mapping, buttons, actions, 0x2000);
    AddIfBound(mapping, buttons, actions, 0x4000);
    AddIfBound(mapping, buttons, actions, 0x8000);
    AddIfBound(mapping, buttons, actions, 0x10000);
    AddIfBound(mapping, buttons, actions, 0x20000);
    AddIfBound(mapping, buttons, actions, 0x40000);
    AddIfBound(mapping, buttons, actions, 0x80000);
    AddIfBound(mapping, buttons, actions, 0x100000);
    AddIfBound(mapping, buttons, actions, 0x200000);
    AddIfBound(mapping, buttons, actions, 0x400000);
}
