#pragma once

// A conversion between two reflected types, as the static initialisers register them
// (game/UnkTUInit.h and src/game_5821e4/UnkStaticInit.h fill one per direction, enum <-> int
// for example): the source and target type ids (the word at 0x30 of a type object's info), the
// converting function, and the link to the converter registered before it. Names and layouts
// are placeholders.
struct UnkConverter {
    void* from;
    void* to;
    void (*convert)();
    UnkConverter* next;
};

// The head of the global list of converters: the one registered last (reached through the GOT).
extern UnkConverter* g_unk_7101d5bdc8;

// Registers a converter: links it to the current head, if there is one, and makes it the head
// (src/base/reflection/UnkReflConverter.cpp). Canonical declaration: include this header, never redeclare.
void fn_71000a16c4(UnkConverter* converter);
