#pragma once

// fn_7100de8f78 (src/game_hi/fn_7100de8f78.cpp): returns a new reference to a string id and records
// the id once in a function-local static registry. The static initialisers of the reflected
// units build their class-name ids through it.

#include "base/global/CStrId.h"

base::global::CStrId fn_7100de8f78(const base::global::CStrId& name);
