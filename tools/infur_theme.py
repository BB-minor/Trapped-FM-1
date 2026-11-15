#!/usr/bin/env python3
"""Final INFUR theme_t: validated, CRC'd."""
import struct, zlib
def to565(h):
    h=h.lstrip("#"); r,g,b=(int(h[i:i+2],16) for i in (0,2,4))
    return ((r>>3)<<11)|((g>>2)<<5)|(b>>3)
def ch(c,k): return ((c>>11)&31)*8 if k==0 else ((c>>5)&63)*4 if k==1 else (c&31)*8
def apart(c,bg): return max(abs(ch(c,k)-ch(bg,k)) for k in range(3))>=96
def d(c,bg): return max(abs(ch(c,k)-ch(bg,k)) for k in range(3))

BG   = "#111512"
STEP = ["#1C2C26","#3F6B66","#79A394","#9DB5A6","#ECE6D8"]   # c[0..4] faint -> strong
TRK  = ["#BDD6CD","#8BC6BD","#73AAAC","#EEE6DE"]             # 808, melodic, spare, drums
HI   = "#79A394"

bg = to565(BG)
print("screen steps:")
for i,s in enumerate(STEP):
    print(f"  c[{i}] {s}  delta {d(to565(s),bg):3d}" + ("   <- CHECKED" if i==4 else ""))
print(f"accent  hi {HI}  delta {d(to565(HI),bg):3d}   <- CHECKED")
print("tracks:")
for n,t in zip(["1 808","2 melodic","3 spare","4 drums"], TRK):
    print(f"  trk {n:10s} {t}  delta {d(to565(t),bg):3d}")

ok = apart(to565(STEP[4]), bg) and apart(to565(HI), bg)
print(f"\ntheme_check -> {'ACCEPTED' if ok else 'REJECTED'}")

body = struct.pack("<I8s5H4HHHH", 0x324D4854, b"INFUR\0\0\0",
                   *[to565(s) for s in STEP], *[to565(t) for t in TRK], bg, to565(HI), 0)
assert len(body)==36
blob = body + struct.pack("<I", zlib.crc32(body) & 0xFFFFFFFF)
open("infur_theme.bin","wb").write(blob)
print(f"theme_t 40 B, CRC {zlib.crc32(body)&0xFFFFFFFF:#010x} -> infur_theme.bin")
print("\nRGB565 words for the editor Theme page:")
print("  c[] =", [f"0x{to565(s):04X}" for s in STEP])
print("  trk[]=", [f"0x{to565(t):04X}" for t in TRK])
print(f"  bg = 0x{bg:04X}   hi = 0x{to565(HI):04X}")
