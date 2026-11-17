#pragma once
// CFactionComponent's unit (unit 2707, 0x7101138930-0x7101139a5c): the faction lists the component registers in.
// One declaration per symbol: include this instead of declaring one. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkFactionListTag_ovb0139;
using UnkFactionList_ovb0139 = base::UnkSList<UnkFactionListTag_ovb0139>;

extern UnkFactionList_ovb0139 g_unk_7172a08ad8;  // players
extern UnkFactionList_ovb0139 g_unk_7172a08b48;  // neutral
extern UnkFactionList_ovb0139 g_unk_7172a08bb8;  // enemies
extern UnkFactionList_ovb0139 g_unk_7172a08c28;  // all
