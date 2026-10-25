#pragma once

// The destructors of the reflected type objects, as the per-TU static initialisers register them
// with __cxa_atexit (base/UnkAtExit.h): one per kind of type object. Every TU initialiser helper
// (include/game/UnkTUInit.h, src/game_5821e4/UnkStaticInit.h, base/UnkCloneHelpers.h) declares
// them from here; a unit's own file never redeclares them.

void fn_7100096604(void* type);   // a class type's destructor
void fn_7100099468(void* type);   // an enum type's destructor
void fn_710009891c(void* type);   // a vector type's destructor
void fn_710009a368(void* type);   // a flag-set type's destructor
void fn_710010c424(void* value);  // a reflected value's destructor (UnkTUInit.h's UnkValue)
