// GUI::CSlotInfoComposition's unit (tools/tu_map.py): libc++'s __pad_and_output for char, instantiated in
// this unit: writes [begin, pad point), then the stream's width minus the length in fill characters, then
// [pad point, end) through the buffer's xsputn, and clears the width; null when a write falls short.
#include <string>

#include "types.h"

// std::basic_streambuf<char> as far as used here: xsputn is vtable slot 0x60.
struct UnkStreamBuf_ovb0133 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual s64 xsputn(const char* s, s64 n);

    s64 sputn(const char* s, s64 n) { return xsputn(s, n); }
};

// std::ios_base as far as used here: the field width at +0x18.
struct UnkIosBase_ovb0133 {
    u8 pad_0x0[0x18];
    s64 width_;

    s64 width() const { return width_; }
    void width(s64 w) { width_ = w; }
};

UnkStreamBuf_ovb0133* fn_7100b7b8d0(UnkStreamBuf_ovb0133* s, const char* ob, const char* op, const char* oe,
                                    UnkIosBase_ovb0133& iob, char fl) {
    if (s == nullptr)
        return s;
    s64 sz = oe - ob;
    s64 ns = iob.width();
    if (ns > sz)
        ns -= sz;
    else
        ns = 0;
    s64 np = op - ob;
    if (np > 0) {
        if (s->sputn(ob, np) != np)
            return nullptr;
    }
    if (ns > 0) {
        std::string sp(ns, fl);
        if (s->sputn(sp.data(), ns) != ns)
            return nullptr;
    }
    np = oe - op;
    if (np > 0) {
        if (s->sputn(op, np) != np)
            return nullptr;
    }
    iob.width(0);
    return s;
}
