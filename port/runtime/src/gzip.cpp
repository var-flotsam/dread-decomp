// nn::util::DecompressGzip for the port: a gzip member (RFC 1952) holding one DEFLATE stream
// (RFC 1951), unpacked into the caller's buffer. The game's material loader fn_71001a6644 reads
// packed files through fn_71001409a0, which passes a 0x1bf8-byte work buffer; this inflater keeps
// its state on the stack and ignores it. Written from the two RFCs, with no library, so the port
// needs no zlib on any host.

#include "nn/util.h"

namespace nn::util {

namespace {

struct Inflater {
    const u8* in;
    u64 in_size;
    u64 in_pos = 0;
    u32 bit_buffer = 0;
    u32 bit_count = 0;
    u8* out;
    u64 out_size;
    u64 out_pos = 0;
    bool failed = false;

    // `n` bits (n <= 16), least significant first, as DEFLATE packs them.
    u32 Bits(u32 n) {
        while (bit_count < n) {
            if (in_pos >= in_size) {
                failed = true;
                return 0;
            }
            bit_buffer |= u32(in[in_pos++]) << bit_count;
            bit_count += 8;
        }
        const u32 v = bit_buffer & ((1u << n) - 1);
        bit_buffer >>= n;
        bit_count -= n;
        return v;
    }
};

// A canonical Huffman code: how many codes of each length, and the symbols in code order.
struct Huffman {
    u16 count[16];
    u16 symbol[288];
};

// False when the lengths over-subscribe the code space (an incomplete code is allowed, as for a
// distance code with one symbol).
bool Build(Huffman* h, const u8* lengths, u32 n) {
    for (u16& c : h->count)
        c = 0;
    for (u32 i = 0; i < n; ++i)
        h->count[lengths[i]]++;
    if (h->count[0] == n)
        return true;
    s32 left = 1;
    for (u32 len = 1; len < 16; ++len) {
        left = left * 2 - h->count[len];
        if (left < 0)
            return false;
    }
    u16 offset[16];
    offset[1] = 0;
    for (u32 len = 1; len < 15; ++len)
        offset[len + 1] = u16(offset[len] + h->count[len]);
    for (u32 i = 0; i < n; ++i)
        if (lengths[i] != 0)
            h->symbol[offset[lengths[i]]++] = u16(i);
    return true;
}

// The next symbol, reading the code a bit at a time (codes are packed most significant bit first).
s32 Decode(Inflater* s, const Huffman* h) {
    s32 code = 0, first = 0, index = 0;
    for (u32 len = 1; len < 16; ++len) {
        code |= s32(s->Bits(1));
        if (s->failed)
            return -1;
        const s32 count = h->count[len];
        if (code - count < first)
            return h->symbol[index + (code - first)];
        index += count;
        first = (first + count) << 1;
        code <<= 1;
    }
    return -1;
}

constexpr u16 kLengthBase[29] = {3,  4,  5,  6,  7,  8,  9,  10, 11,  13,  15,  17,  19,  23, 27,
                                 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
constexpr u8 kLengthExtra[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
                                 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
constexpr u16 kDistanceBase[30] = {1,   2,   3,   4,   5,   7,    9,    13,   17,   25,   33,   49,   65,    97,    129,
                                   193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};
constexpr u8 kDistanceExtra[30] = {0, 0, 0, 0, 1, 1, 2, 2,  3,  3,  4,  4,  5,  5,  6,
                                   6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

bool Codes(Inflater* s, const Huffman* lengths, const Huffman* distances) {
    for (;;) {
        const s32 sym = Decode(s, lengths);
        if (sym < 0)
            return false;
        if (sym < 256) {
            if (s->out_pos >= s->out_size)
                return false;
            s->out[s->out_pos++] = u8(sym);
        } else if (sym == 256) {
            return true;
        } else {
            const u32 l = u32(sym - 257);
            if (l >= 29)
                return false;
            const u64 len = kLengthBase[l] + s->Bits(kLengthExtra[l]);
            const s32 d = Decode(s, distances);
            if (d < 0 || d >= 30)
                return false;
            const u64 dist = kDistanceBase[d] + s->Bits(kDistanceExtra[d]);
            if (s->failed || dist > s->out_pos || len > s->out_size - s->out_pos)
                return false;
            for (u64 i = 0; i < len; ++i, ++s->out_pos)
                s->out[s->out_pos] = s->out[s->out_pos - dist];
        }
    }
}

bool Stored(Inflater* s) {
    s->bit_buffer = 0;
    s->bit_count = 0;
    if (s->in_size - s->in_pos < 4)
        return false;
    const u32 len = u32(s->in[s->in_pos]) | u32(s->in[s->in_pos + 1]) << 8;
    const u32 nlen = u32(s->in[s->in_pos + 2]) | u32(s->in[s->in_pos + 3]) << 8;
    s->in_pos += 4;
    if (len != (~nlen & 0xffff) || s->in_size - s->in_pos < len || s->out_size - s->out_pos < len)
        return false;
    for (u32 i = 0; i < len; ++i)
        s->out[s->out_pos++] = s->in[s->in_pos++];
    return true;
}

bool Fixed(Inflater* s) {
    Huffman lengths, distances;
    u8 l[288];
    for (u32 i = 0; i < 288; ++i)
        l[i] = i < 144 ? 8 : i < 256 ? 9 : i < 280 ? 7 : 8;
    Build(&lengths, l, 288);
    for (u32 i = 0; i < 30; ++i)
        l[i] = 5;
    Build(&distances, l, 30);
    return Codes(s, &lengths, &distances);
}

bool Dynamic(Inflater* s) {
    static constexpr u8 kOrder[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    const u32 nlen = s->Bits(5) + 257, ndist = s->Bits(5) + 1, ncode = s->Bits(4) + 4;
    if (s->failed || nlen > 286 || ndist > 30)
        return false;
    u8 l[320] = {};
    for (u32 i = 0; i < ncode; ++i)
        l[kOrder[i]] = u8(s->Bits(3));
    Huffman code_lengths;
    if (!Build(&code_lengths, l, 19))
        return false;
    for (u32 i = 0; i < 19; ++i)
        l[i] = 0;
    for (u32 i = 0; i < nlen + ndist;) {
        const s32 sym = Decode(s, &code_lengths);
        if (sym < 0)
            return false;
        if (sym < 16) {
            l[i++] = u8(sym);
            continue;
        }
        u8 value = 0;
        u32 repeat;
        if (sym == 16) {
            if (i == 0)
                return false;
            value = l[i - 1];
            repeat = 3 + s->Bits(2);
        } else if (sym == 17) {
            repeat = 3 + s->Bits(3);
        } else {
            repeat = 11 + s->Bits(7);
        }
        if (s->failed || i + repeat > nlen + ndist)
            return false;
        while (repeat--)
            l[i++] = value;
    }
    if (l[256] == 0)
        return false;  // no end-of-block code
    Huffman lengths, distances;
    if (!Build(&lengths, l, nlen) || !Build(&distances, l + nlen, ndist))
        return false;
    return Codes(s, &lengths, &distances);
}

u32 Crc32(const u8* p, u64 n) {
    u32 crc = 0xffffffffu;
    for (u64 i = 0; i < n; ++i) {
        crc ^= p[i];
        for (u32 k = 0; k < 8; ++k)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1)));
    }
    return ~crc;
}

u32 Le32(const u8* p) {
    return u32(p[0]) | u32(p[1]) << 8 | u32(p[2]) << 16 | u32(p[3]) << 24;
}

}  // namespace

bool DecompressGzip(void* out, u64 out_size, const void* data, u64 size, void* /*work*/, u64 /*work_size*/) {
    const auto* in = static_cast<const u8*>(data);
    if (size < 18 || in[0] != 0x1f || in[1] != 0x8b || in[2] != 8)
        return false;
    const u8 flags = in[3];
    u64 pos = 10;
    if (flags & 4) {  // FEXTRA
        if (size - pos < 2)
            return false;
        pos += 2 + (u64(in[pos]) | u64(in[pos + 1]) << 8);
    }
    for (u8 field = 8; field <= 16; field = u8(field << 1))  // FNAME, FCOMMENT: zero-terminated
        if (flags & field) {
            while (pos < size && in[pos] != 0)
                ++pos;
            ++pos;
        }
    if (flags & 2)  // FHCRC
        pos += 2;
    if (pos + 8 > size)
        return false;
    Inflater s{in + pos, size - pos - 8, 0, 0, 0, static_cast<u8*>(out), out_size};
    for (u32 last = 0; !last;) {
        last = s.Bits(1);
        const u32 type = s.Bits(2);
        const bool ok = !s.failed && (type == 0 ? Stored(&s) : type == 1 ? Fixed(&s) : type == 2 && Dynamic(&s));
        if (!ok)
            return false;
    }
    const u8* trailer = in + size - 8;
    return Le32(trailer) == Crc32(s.out, s.out_pos) && Le32(trailer + 4) == u32(s.out_pos);
}

}  // namespace nn::util
