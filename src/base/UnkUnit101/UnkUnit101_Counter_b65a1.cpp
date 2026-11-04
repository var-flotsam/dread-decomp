// Unit 101 of tools/tu_map.py (no reflected class): drops a module-local count of live voices by one
// (called by src/base/snd/UnkSoundVoiceList_b56.cpp when a voice stops; fn_710010519c raises it).
// Names are placeholders.
#include "types.h"

extern s32 g_unk_7108b6c470 __attribute__((visibility("hidden")));

void fn_7100104ef4() { g_unk_7108b6c470--; }
