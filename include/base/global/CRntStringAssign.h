#pragma once

// base::global::CRntString: assigning a counted prefix of a C string. (CRntString.h has the
// type and the other string functions; this is a separate header so that file stays as it is.)

#include "base/global/CRntString.h"

// Replaces the string with the first `length` characters of `str` (fewer when it is shorter); a
// length of 0 clears it (frees the heap copy, the key becomes -1). A borrowed string is left
// alone. When `str` is the string's own text the copy goes through a temporary, with inline
// storage for 255 characters or fewer.
void fn_710007e7f8(base::global::CRntString* self, const char* str, u32 length);
