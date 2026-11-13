#pragma once

// UnkTU54: functions of this translation unit that other files call (tools/tu_map.py unit 54,
// 0x71000769f0-0x710007b430). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMatrix44_7100078c9c;
struct UnkKeyedEntry;
struct UnkMatrix44;
struct UnkXform;
struct UnkAim;
struct UnkMat4f;
f32 fn_710007a1d0(f32 angle);  // Wraps an angle into (-pi, pi].
void fn_7100077bbc(UnkMat4f* m);
void fn_710007a170(UnkAim* aim, f32 current);
UnkMatrix44 fn_7100078fc8(const UnkMatrix44& m);
void fn_71000774fc(UnkXform* x);
UnkMatrix44 fn_7100078834(const UnkKeyedEntry* entry, const UnkMatrix44* matrix, f32 weight);
UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);
UnkVec3f fn_7100076acc(const UnkXform* x);  // The rotation angles of a transform.
UnkVec3f fn_710007a7f0(UnkVec3f dir);  // Direction to rotation angles.
UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);
UnkVec3f fn_710007a220(const UnkVec3f* v);
UnkVec3f fn_7100077c04(const UnkMatrix44& m, const UnkVec3f& v);
UnkMat4f fn_7100079c60(const UnkVec3f* from, const UnkVec3f* to);
// The point `v` transformed by the matrix.
UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v);
