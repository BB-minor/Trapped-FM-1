# INFUR — a trap groovebox on the FM-1

Work in progress on top of SLOOP 2.8.5 (with the `dualcore` and `ram` branches merged).
Nothing here has run on hardware yet: the device has not arrived. Everything is built and
verified on the host and in the browser preview until it does.

## What the hardware already gives us

SLOOP is three synth parts plus a drum track of sixteen lanes, not six tracks. The six
roles map onto it with a part to spare:

| Role | Where |
| --- | --- |
| kick | drum lane 0 (lane 14: kick 2) |
| snare / clap | drum lanes 1, 2 (lane 15: snare 2) |
| closed hat | drum lane 3 |
| open hat / perc | lane 4, plus toms, shaker, conga, rim, cowbell, clave |
| 808 bass | synth part 1, ANALOG (`808 BOOM`, `808 DIRTY`, `808 SLIDE`, `SUB BASS`, `PLUGG BASS`) |
| melodic / sample | synth part 2 (twelve engines: SAMPLE, FM6, SLICE, GRAIN, ...) |
| — | synth part 3 spare |

The 808 needs no new code: GLIDE with RATE / TIME modes, a per-step SLIDE that glides in
~50 ms, ENV -> PITCH, `P_TRANS -24`, and the track's DIST (asymmetric soft clip, even
harmonics, tone low-pass closing with drive) for the upper-harmonic presence.

### Drive on the kick without driving the hats

The drum track has one shared DIST / INSERT / filter, but every lane carries its **own**
drive inside the kit: `dsnd_t.drive` (0..127, `softclip((x * drive) >> 4)` per voice,
`drum_synth.c`), plus a per-sound level and the kit's crush. The web editor sends all 22
`dsnd_t` bytes per lane, so the kick is saturated in the kit and the track DIST stays at 0.

## Resource ceiling (measured, `./build.sh` at 2.8.5)

| Budget | Used | Free |
| --- | --- | --- |
| **Code (XIP)** | 571,820 / 581,564 | **9,744 B — the binding constraint** |
| Pool | 333,900 / 344,064 | 10,164 B, but the build fails under 8,192 headroom: ~2 KB usable |
| RAM | 59,340 / 98,304 | 38,964 B |
| CPU | — | 23 % on two cores, 31 % on one, on a real pattern |

Not RAM — flash. The `ram` branch already bought the RAM back. Upstream hit the code
ceiling at 2.7: `tools/build.py` rebuilds with the boot logo as text (4.6 KB) on overflow.

Replacing the boot splash frees **1,881 B**, taking the working budget to **11,625 B**.

## Look

- **Theme** (`assets/infur/infur-theme.bin`, `tools/infur_theme.py`): 40 bytes over USB from
  the editor's Theme page. Costs no flash. Validated against `gfx.c theme_check`.
- **Splash** (`tools/infur_splash.py`): the mark + wordmark lockup, 240x62, 2,754 B RLE.
  The canvas is cropped to the lockup — `splash.c` reads width and height from the header,
  so a tighter canvas simply costs less. Index 0 is never drawn, so the ground is index 1.
  Colour is free: the RLE byte carries the palette index in its low nibble.

### Palette

Ground `#111512`, accent sage `#79A394`, strongest step bone `#ECE6D8`.

The brand's seven colours could not drive four track colours: each track colour derives a
DIM (1/3) variant for the inactive state, and deep teal `#3F6B66` dims to `#102020` —
invisible on the ground. Dropping it left three pale greens 19.3 apart, too close to read on
a small meter. The track colours below were searched inside the brand's own hue (148-184)
and saturation (0.12-0.33) range: worst DIM visibility 40, worst separation 25.5.

| | Colour | DIM visibility |
| --- | --- | --- |
| trk 1 · 808 | `#BDD6CD` | 48 |
| trk 2 · melodic | `#8BC6BD` | 44 |
| trk 3 · spare | `#73AAAC` | 40 |
| trk 4 · drums | `#EEE6DE` | 56 |

Screen steps keep the brand colours: `#1C2C26`, `#3F6B66`, `#79A394`, `#9DB5A6`, `#ECE6D8`.

## To build

- **Master soft clipper** (`G_CLIP`, a MASTER 2 page): the master is already
  limiter -> tanh soft clipper (`fx.c`), but `LIM_T` (18000, about -5 dBFS) holds peaks
  below where tanh bends, so the clipper never works. Drive into it and lift `LIM_T` with
  CLIP, keeping the look-ahead limiter as the ceiling. **CLIP 0 must be bit-identical** to
  today, so the 182 golden renders stay green at the default.

## Auditioning without the device

`firmware/host/preview.c` compiles `engines.c`, `drums.c`, `seq.c` and all of `fx.c`
(limiter and clipper included); `tests/preview_native.c` renders it on the host. The engines,
the kit and the master chain can all be heard before anything is flashed.

## Before the first flash

Not yet done — the device has not arrived.

1. Read the flash chip (ABOUT, or `flash` on the console). 2.8.5 exists because units with
   the **UC25HQ80** (JEDEC 0xB36014, sold from late 2026) were taken for an unknown chip.
2. M-VAVE's **M-UPGRADE** and the official **FM-1 V16** package downloaded and to hand.
3. Back up with the web editor first.

Recovery routes: USB RESCUE (hold OCT- at power-on) · M-UPGRADE with V16 · Clipper's
installer page "Return to the official firmware" (works with SLOOP installed) ·
FM1 Open's workbench (returns to V15) · FM-1-transporter if it will not boot at all.

## Agenda — raised, parked, to come back to

Asked for on 16/11/2026, to be brought up again rather than answered on the spot. Some of these are
questions about what the firmware already does, not work to be done; they are listed as they were asked.

1. **Overall tempo setting** — where the song's tempo lives and how it is set.
2. **Pattern length: how many bars?** — LEN is in steps (1..64) and the bars depend on DIV, so a 64-step
   track is four bars at 1/16 and two and two thirds at 16T.
3. **Drums quantised by default?** — what REC's QUANT does and what it is set to out of the box.
4. **Overall scale setting** — one key and scale across the three synth tracks.
5. **Harmony quantised by default?** — as 3, for the synth tracks.
6. **Humanisation** — nothing like it exists today. QUANT off keeps the timing played, as a per-step
   nudge; a humanise that *adds* a spread does not exist and would be new work.
7. **Generative harmony pattern** — does not exist. New work; worth scoping against the voice budget and
   the four-notes-a-step ceiling before anything is promised.

## Known

`tests/run_tests.sh` reports four `target:` entries over budget (`sample_render`,
`grain_render`, `drums_mix`, `fm1_alnk0_irq`). `tests/target_budget.txt` was last written at
SLOOP **2.4** and HEAD is **2.8.5**; the measured CPU check and all 182 golden renders pass.
Refresh with `BUDGET_UPDATE=1 sh tests/run_tests.sh` before relying on it to catch a
regression.
