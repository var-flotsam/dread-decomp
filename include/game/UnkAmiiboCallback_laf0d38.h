#pragma once

#include "types.h"

struct UnkAmiiboComposition_laf0d38;

// A function bound to its object, called when the message closes. It is passed by value; its copy
// constructor is out of line, so it goes in memory.
struct UnkAmiiboCallback_laf0d38 {
    UnkAmiiboComposition_laf0d38* object;
    void (*function)(UnkAmiiboComposition_laf0d38*);

    UnkAmiiboCallback_laf0d38(UnkAmiiboComposition_laf0d38* object, void (*function)(UnkAmiiboComposition_laf0d38*))
        : object(object), function(function) {}
    UnkAmiiboCallback_laf0d38(const UnkAmiiboCallback_laf0d38& other);
};
