#pragma once

// The string id and file-path id a path text names ("id:path"), as a temporary or a scope local:
// built by fn_710001f074, which empties both ids and parses the text into them (fn_710001a8b4,
// src/base/UnkStrIdPathPair.cpp), and released in reverse order by the pair's destructor. Callers
// hand `ids` on (most often to fn_71000267f8) and let it go. Names are placeholders.
//
// fn_710001f074 is the constructor itself, so the ids are constructed once, by it, and need no
// union to stay unset before the call.

#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntString.h"

struct UnkPathIdsFromText {
    UnkStrIdPathPair ids;

    explicit UnkPathIdsFromText(const base::global::CRntString& text) asm("fn_710001f074");
};
static_assert(sizeof(UnkPathIdsFromText) == 0x10, "UnkPathIdsFromText layout");
