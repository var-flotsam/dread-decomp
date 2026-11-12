#pragma once

// CTriggerLogicAction: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1575,
// 0x7100a54720-0x7100a57e38). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkChangeSetupLogicActionDef;
struct UnkLogicActionBase;
struct UnkTutoExitAction_b47;
void fn_7100a55ac8(UnkTutoExitAction_b47* self);
void fn_7100a55aa4(UnkChangeSetupLogicActionDef* self);  // The logic-action base's constructor.
void fn_7100a55ab8(UnkLogicActionBase*);  // Derived classes call it as their base's destructor.
extern base::reflection::CType g_unk_717274caa0;
