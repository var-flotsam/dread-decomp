// Adds a string id to, or removes it from, the key list `self` (game/UnkGuiRoot.h's UnkGuiKeyList:
// fn_710002fd10 adds, fn_710002ffb4 removes); the same shape as
// src/game/CAimComponent/UnkAimKeyListToggle_b43.cpp. Called by CCharClassBaseTriggerComponent_b43.cpp.
#include "game/UnkGuiRoot.h"
#include "game/UnkTU39.h"

void fn_7100030128(char* self, bool flag, void* arg) {
    UnkGuiKeyList* list = reinterpret_cast<UnkGuiKeyList*>(self);
    base::global::CStrId* key = static_cast<base::global::CStrId*>(arg);
    if (flag)
        fn_710002fd10(list, key);
    else
        fn_710002ffb4(list, key);
}
