// The C++ library's templates that `main` instantiates in its own code (status L in
// data/functions/, named by the dump's dynamic symbols), instantiated here from libc++ 9's headers
// (the port's, port_sweep.py libcxx9): the same mangled names, which tools/port_link.py binds those
// addresses to (cxx_by_address). The host's libc++ cannot stand in: it exports none of them, or only
// under a newer ABI tag (__put_character_sequence[abi:ne180100]).
//
// GUI::CSlotInfoComposition's unit (fn_7100b79ef8, the slot's text) formats with a
// std::stringstream: its destructors and thunks, the string buffer's virtual functions (the game's
// own vtables point at its copies) and the character output under operator<<.

#include <ostream>
#include <sstream>

template class std::basic_stringbuf<char>;
template class std::basic_stringstream<char>;
template std::ostream& std::__put_character_sequence<char, std::char_traits<char>>(std::ostream&, const char*, size_t);
