// The data loader's core (dread_port_loader.h): parses the main NSO, decompresses its segments into
// a mapping at the game's own addresses, applies the dynamic relocations with code pointers
// translated to the port's functions, and protects the read-only part. Freestanding on purpose: no
// allocation and no standard library, only the host services the caller passes, so that
// tools/test_port_loader.py can build this file alone into a DLL and run it on the user's dump.

#include "dread_port_loader.h"

namespace {

// --- the file formats ---------------------------------------------------------------------------

// The NSO header: the three segments (text, rodata, data), each with a file offset, a memory
// offset and a decompressed size, their stored sizes, and the module ID. Offsets from the file.
constexpr u64 kNsoHeaderSize = 0x100;
constexpr u64 kNsoFlags = 0x0c;
constexpr u64 kNsoSegments = 0x10;      // 3 x {file offset, memory offset, size}, 16 bytes apart
constexpr u64 kNsoBssSize = 0x3c;
constexpr u64 kNsoBuildId = 0x40;
constexpr u64 kNsoBuildIdSize = 0x20;
constexpr u64 kNsoFileSizes = 0x60;     // 3 x u32
constexpr u64 kPage = 0x1000;

// The dynamic section's tags the loader reads (ELF).
constexpr s64 DT_NULL = 0, DT_PLTRELSZ = 2, DT_STRTAB = 5, DT_SYMTAB = 6, DT_RELA = 7, DT_RELASZ = 8,
              DT_RELAENT = 9, DT_STRSZ = 10, DT_SYMENT = 11, DT_PLTREL = 20, DT_JMPREL = 23;
// The AArch64 relocations a Switch module carries.
constexpr u32 R_AARCH64_NONE = 0, R_AARCH64_ABS64 = 257, R_AARCH64_GLOB_DAT = 1025,
              R_AARCH64_JUMP_SLOT = 1026, R_AARCH64_RELATIVE = 1027;
constexpr u64 kRelaSize = 24, kSymSize = 24;

u32 Read32(const u8* p) {
    return u32(p[0]) | u32(p[1]) << 8 | u32(p[2]) << 16 | u32(p[3]) << 24;
}

u64 Read64(const u8* p) {
    return u64(Read32(p)) | u64(Read32(p + 4)) << 32;
}

void Write64(u8* p, u64 v) {
    for (int i = 0; i < 8; ++i)
        p[i] = u8(v >> (8 * i));
}

void Copy(u8* to, const u8* from, u64 n) {
    for (u64 i = 0; i < n; ++i)
        to[i] = from[i];
}

u64 PageUp(u64 v) { return (v + kPage - 1) & ~(kPage - 1); }

// An LZ4 block (no frame): a token, literals, a two-byte back offset, a match. Returns false
// unless it decodes to exactly `out_size` bytes without reading or writing out of bounds.
bool Lz4Decompress(const u8* in, u64 in_size, u8* out, u64 out_size) {
    u64 ip = 0, op = 0;
    while (ip < in_size) {
        const u8 token = in[ip++];
        u64 literals = token >> 4;
        if (literals == 15) {
            u8 b;
            do {
                if (ip >= in_size)
                    return false;
                b = in[ip++];
                literals += b;
            } while (b == 255);
        }
        if (literals > in_size - ip || literals > out_size - op)
            return false;
        Copy(out + op, in + ip, literals);
        ip += literals;
        op += literals;
        if (ip == in_size)
            break;  // the last sequence has literals only
        if (in_size - ip < 2)
            return false;
        const u64 offset = u64(in[ip]) | u64(in[ip + 1]) << 8;
        ip += 2;
        if (offset == 0 || offset > op)
            return false;
        u64 length = token & 15;
        if (length == 15) {
            u8 b;
            do {
                if (ip >= in_size)
                    return false;
                b = in[ip++];
                length += b;
            } while (b == 255);
        }
        length += 4;
        if (length > out_size - op)
            return false;
        for (u64 i = 0; i < length; ++i, ++op)  // byte by byte: the match may overlap its output
            out[op] = out[op - offset];
    }
    return op == out_size;
}

struct Segment {
    u64 file_offset, memory_offset, size, stored_size;
    bool compressed;
};

// --- the loader ----------------------------------------------------------------------------------

// A trap thunk, 32 bytes: the argument in rdi (System V) and rcx (Windows x64), the trap's address
// in rax, an indirect jump, padding with int3.
constexpr u64 kThunkSize = 32;

void WriteThunk(u8* at, u64 argument, void* target) {
    u8* p = at;
    *p++ = 0x48, *p++ = 0xbf;  // movabs rdi, argument
    Write64(p, argument), p += 8;
    *p++ = 0x48, *p++ = 0xb9;  // movabs rcx, argument
    Write64(p, argument), p += 8;
    *p++ = 0x48, *p++ = 0xb8;  // movabs rax, target
    Write64(p, u64(target)), p += 8;
    *p++ = 0xff, *p++ = 0xe0;  // jmp rax
    while (p < at + kThunkSize)
        *p++ = 0xcc;
}

bool Equal(const char* a, const char* b) {
    for (; *a && *a == *b; ++a, ++b) {
    }
    return *a == *b;
}

struct Loader {
    const DreadPortLoadOptions& options;
    const DreadPortHost& host;
    DreadPortImageInfo& info;
    u8* image = nullptr;  // == options.base once mapped
    u64 image_size = 0;
    u64 rela = 0, rela_size = 0, jmprel = 0, jmprel_size = 0, symtab = 0, strtab = 0, strtab_size = 0;
    u8* thunks = nullptr;
    u64 thunk_capacity = 0;

    void Log(const char* line) const {
        if (host.log)
            host.log(line);
    }

    const DreadPortFunction* FindFunction(u64 address) const {
        u64 lo = 0, hi = options.function_count;
        while (lo < hi) {
            const u64 mid = lo + (hi - lo) / 2;
            if (options.functions[mid].address < address)
                lo = mid + 1;
            else
                hi = mid;
        }
        return lo < options.function_count && options.functions[lo].address == address ? &options.functions[lo]
                                                                                         : nullptr;
    }

    void* FindImport(const char* name) const {
        for (u64 i = 0; i < options.import_count; ++i)
            if (Equal(options.imports[i].name, name))
                return options.imports[i].code;
        return nullptr;
    }

    // Counts when `thunks` is null (the first pass), writes otherwise.
    void* Thunk(u64 argument, void* target) {
        if (!thunks) {
            ++thunk_capacity;
            return nullptr;
        }
        u8* at = thunks + info.thunks * kThunkSize;
        WriteThunk(at, argument, target);
        ++info.thunks;
        return at;
    }

    // A pointer the game stores, as an offset from the base: into .text it becomes the port's
    // function (or a trap), anywhere else the mapped address.
    u64 Translate(u64 offset, bool counting) {
        if (offset >= info.text_end)
            return options.base + offset;
        if (!counting)
            ++info.code_pointers;
        if (const DreadPortFunction* f = FindFunction(options.base + offset)) {
            if (!counting)
                ++info.code_translated;
            return u64(f->code);
        }
        return u64(Thunk(options.base + offset, options.missing_function));
    }

    // One table of relocations (.rela.dyn or .rela.plt). With `counting`, only counts the thunks.
    DreadPortLoadStatus Apply(u64 table, u64 size, bool counting) {
        for (u64 at = table; at + kRelaSize <= table + size; at += kRelaSize) {
            const u64 offset = Read64(image + at);
            const u64 r_info = Read64(image + at + 8);
            const u64 addend = Read64(image + at + 16);
            const u32 type = u32(r_info);
            const u64 symbol = r_info >> 32;
            if (type == R_AARCH64_NONE)
                continue;
            if (offset + 8 > info.data_end || offset < info.data_begin) {
                info.first_bad_value = offset;  // the game relocates nothing outside .data
                return DreadPortLoad_BadRelocation;
            }
            u64 value;
            if (type == R_AARCH64_RELATIVE) {
                value = Translate(addend, counting);
            } else if (type == R_AARCH64_ABS64 || type == R_AARCH64_GLOB_DAT || type == R_AARCH64_JUMP_SLOT) {
                const u64 sym = symtab + symbol * kSymSize;
                if (sym + kSymSize > image_size) {
                    info.first_bad_value = symbol;
                    return DreadPortLoad_BadRelocation;
                }
                const u64 name = Read32(image + sym);
                const u16 section = u16(image[sym + 6] | image[sym + 7] << 8);
                const u64 sym_value = Read64(image + sym + 8);
                const u64 add = type == R_AARCH64_ABS64 ? addend : 0;
                if (section != 0) {
                    value = Translate(sym_value + add, counting);
                } else {
                    if (name >= strtab_size) {
                        info.first_bad_value = symbol;
                        return DreadPortLoad_BadRelocation;
                    }
                    const char* import = reinterpret_cast<const char*>(image + strtab + name);
                    if (!counting)
                        ++info.imports;
                    if (void* code = FindImport(import)) {
                        if (!counting)
                            ++info.imports_resolved;
                        value = u64(code) + add;
                    } else if (add == 0) {
                        value = u64(Thunk(u64(import), options.missing_import));
                    } else {
                        value = 0;  // an unresolved object plus an offset (a type-info vtable): no thunk fits
                    }
                }
            } else {
                info.first_bad_value = type;
                return DreadPortLoad_BadRelocation;
            }
            if (!counting) {
                Write64(image + offset, value);
                ++info.relocations;
            }
        }
        return DreadPortLoad_Ok;
    }

    DreadPortLoadStatus ReadDynamic(u64 dynamic) {
        u64 relaent = kRelaSize, syment = kSymSize, pltrel = DT_RELA;
        for (u64 at = dynamic;; at += 16) {
            if (at + 16 > info.data_end)
                return DreadPortLoad_BadModule;
            const s64 tag = s64(Read64(image + at));
            const u64 value = Read64(image + at + 8);
            if (tag == DT_NULL)
                break;
            switch (tag) {
            case DT_RELA: rela = value; break;
            case DT_RELASZ: rela_size = value; break;
            case DT_RELAENT: relaent = value; break;
            case DT_JMPREL: jmprel = value; break;
            case DT_PLTRELSZ: jmprel_size = value; break;
            case DT_PLTREL: pltrel = value; break;
            case DT_SYMTAB: symtab = value; break;
            case DT_STRTAB: strtab = value; break;
            case DT_STRSZ: strtab_size = value; break;
            case DT_SYMENT: syment = value; break;
            default: break;
            }
        }
        if (relaent != kRelaSize || syment != kSymSize || (jmprel_size && pltrel != DT_RELA))
            return DreadPortLoad_BadModule;
        if (rela + rela_size > image_size || jmprel + jmprel_size > image_size || strtab + strtab_size > image_size ||
            symtab >= image_size)
            return DreadPortLoad_BadModule;
        return DreadPortLoad_Ok;
    }

    DreadPortLoadStatus Load(const u8* nso, u64 nso_size) {
        if (nso_size < kNsoHeaderSize || Read32(nso) != 0x304f534e)  // "NSO0"
            return DreadPortLoad_NotNso;
        Copy(info.build_id, nso + kNsoBuildId, kNsoBuildIdSize);
        if (options.expected_build_id) {
            for (u64 i = 0; i < kNsoBuildIdSize; ++i)
                if (nso[kNsoBuildId + i] != options.expected_build_id[i])
                    return DreadPortLoad_WrongBuild;
        }
        const u32 flags = Read32(nso + kNsoFlags);
        Segment seg[3];
        for (int i = 0; i < 3; ++i) {
            const u8* h = nso + kNsoSegments + 16 * i;
            seg[i] = Segment{Read32(h), Read32(h + 4), Read32(h + 8), Read32(nso + kNsoFileSizes + 4 * i),
                             (flags >> i & 1) != 0};
            if (seg[i].file_offset + seg[i].stored_size > nso_size)
                return DreadPortLoad_NotNso;
            if (!seg[i].compressed && seg[i].stored_size != seg[i].size)
                return DreadPortLoad_BadSegment;
        }
        // text at 0, then rodata, then data, each page-aligned and in order, as the console maps them
        if (seg[0].memory_offset != 0 || seg[1].memory_offset < seg[0].size || seg[2].memory_offset < seg[1].memory_offset + seg[1].size ||
            (seg[1].memory_offset | seg[2].memory_offset) % kPage)
            return DreadPortLoad_BadSegment;
        info.base = options.base;
        info.text_end = seg[0].size;
        info.rodata_begin = seg[1].memory_offset;
        info.rodata_end = seg[1].memory_offset + seg[1].size;
        info.data_begin = seg[2].memory_offset;
        info.data_end = seg[2].memory_offset + seg[2].size;
        info.bss_end = PageUp(info.data_end + Read32(nso + kNsoBssSize));
        image_size = info.bss_end;

        for (u64 i = 1; i < options.function_count; ++i)
            if (options.functions[i - 1].address >= options.functions[i].address) {
                info.first_bad_value = options.functions[i].address;
                return DreadPortLoad_BadTable;
            }
        for (u64 i = 0; i < options.data_reference_count; ++i) {
            const u64 a = options.data_references[i];
            if (a < options.base || a >= options.base + info.bss_end) {
                info.first_bad_value = a;
                return DreadPortLoad_DataOutsideImage;
            }
        }
        info.data_references_checked = options.data_reference_count;

        image = static_cast<u8*>(host.map_fixed(options.base, image_size));
        if (image != reinterpret_cast<u8*>(options.base)) {
            info.first_bad_value = options.base;
            return DreadPortLoad_MapFailed;
        }
        for (int i = 0; i < 3; ++i) {
            const u8* from = nso + seg[i].file_offset;
            u8* to = image + seg[i].memory_offset;
            if (seg[i].compressed ? !Lz4Decompress(from, seg[i].stored_size, to, seg[i].size)
                                  : (Copy(to, from, seg[i].size), false))
                return DreadPortLoad_BadSegment;
        }

        // MOD0: its offset is the text's second word; the dynamic section and the .bss bounds are
        // relative to the header itself.
        const u64 mod0 = Read32(image + 4);
        if (mod0 + 0x1c > info.text_end || Read32(image + mod0) != 0x30444f4d)  // "MOD0"
            return DreadPortLoad_BadModule;
        const u64 dynamic = mod0 + u64(s64(s32(Read32(image + mod0 + 4))));
        const u64 bss_end = mod0 + u64(s64(s32(Read32(image + mod0 + 12))));
        if (bss_end > info.bss_end)
            return DreadPortLoad_BadModule;
        if (DreadPortLoadStatus s = ReadDynamic(dynamic))
            return s;

        // Two passes: count the thunks, map them, apply.
        if (DreadPortLoadStatus s = Apply(rela, rela_size, true))
            return s;
        if (DreadPortLoadStatus s = Apply(jmprel, jmprel_size, true))
            return s;
        const u64 thunk_bytes = PageUp(thunk_capacity * kThunkSize);
        if (thunk_capacity) {
            thunks = static_cast<u8*>(host.map_executable(thunk_bytes));
            if (!thunks)
                return DreadPortLoad_ThunkMapFailed;
        }
        if (DreadPortLoadStatus s = Apply(rela, rela_size, false))
            return s;
        if (DreadPortLoadStatus s = Apply(jmprel, jmprel_size, false))
            return s;
        if (thunks && !host.seal_executable(thunks, thunk_bytes))
            return DreadPortLoad_ThunkMapFailed;

        // .text and .rodata hold no relocation (Apply refuses one), so they are final now.
        if (!host.protect_read_only(image, info.data_begin))
            return DreadPortLoad_ProtectFailed;
        Log("dread: game image mapped");
        return DreadPortLoad_Ok;
    }
};

}  // namespace

extern "C" DreadPortLoadStatus dread_port_load_image(const u8* nso, u64 nso_size, const DreadPortLoadOptions* options,
                                                     const DreadPortHost* host, DreadPortImageInfo* info) {
    *info = DreadPortImageInfo{};
    Loader loader{*options, *host, *info};
    return loader.Load(nso, nso_size);
}

extern "C" const char* dread_port_load_status_text(DreadPortLoadStatus status) {
    switch (status) {
    case DreadPortLoad_Ok: return "ok";
    case DreadPortLoad_NotNso: return "not an NSO, or a segment outside the file";
    case DreadPortLoad_WrongBuild: return "the NSO's module ID is not the expected build's";
    case DreadPortLoad_BadSegment: return "a segment does not decompress to its size, or the segments are out of order";
    case DreadPortLoad_BadModule: return "no MOD0 header or no usable dynamic section";
    case DreadPortLoad_MapFailed: return "the image's address range is not free";
    case DreadPortLoad_BadRelocation: return "a relocation the loader does not know";
    case DreadPortLoad_BadTable: return "the function table is not sorted by address";
    case DreadPortLoad_ThunkMapFailed: return "no executable memory for the trap thunks";
    case DreadPortLoad_DataOutsideImage: return "a data symbol lies outside the image";
    case DreadPortLoad_ProtectFailed: return "the read-only part could not be protected";
    }
    return "unknown status";
}
