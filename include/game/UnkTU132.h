#pragma once

// UnkTU132: functions of this translation unit that other files call (tools/tu_map.py unit 132,
// 0x710014569c-0x7100145bdc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkOverlayModel;

void fn_71001457fc(void* effect, void* scene, u32 flag);  // Attaches an effect to a scene.
void fn_710014580c(void* effect);  // Detaches an effect.
void fn_7100145818(UnkOverlayModel* model, u32 flag);
