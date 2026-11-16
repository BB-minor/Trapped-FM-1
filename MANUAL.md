# INFUR — workflow manual

INFUR is a trap groovebox firmware for the M-VAVE FM-1, built on SLOOP 2.8.5 (which is built on
Felucca). **[GUIDE.md](GUIDE.md) documents everything SLOOP does and is still the reference for all of
it.** This manual is the shorter thing: what INFUR adds, and how to actually make a track on the device.

Engineering notes, the resource budget and the reasoning behind the changes are in
[INFUR.md](INFUR.md).

---

## 1. The six roles

The FM-1 has **three synth parts and one drum track of sixteen lanes**, not six tracks. The six roles
sit on them like this, with a part to spare:

| Role | Where | Notes |
| --- | --- | --- |
| Kick, snare, hats, percussion | **Drum track** (track 4) | 16 lanes, its own 6 voices |
| 808 | **Synth 1** | ANALOG, a MONO preset |
| Harmony | **Synth 2** | POLY, up to 4 notes a step |
| Melody | **Synth 3** | MONO or POLY |
| Texture | **GLO → TEXTURE** | not a track: no part, no voices |
| — | a synth part spare | |

**Voices.** The three synth parts share **8**; the drum track has its own 6. A 4-note chord + a melody
note + an 808 is 6 of 8. Bass and lead are protected when the budget runs out — dense chords thin from
the top.

The sixteen drum lanes, on the white keys F3 up:

```
KICK  KICK 2  SNARE  CLAP  HAT  OPEN HAT  PEDAL  RIM
SNARE 2  LOW TOM  HI TOM  CRASH  RIDE  SHAKER  CONGA  COWBELL
```

---

## 2. Making a beat

### Set the song up

- **Tempo**: SELECT on the TRACKS screen, or HOME → KNOB 1, or GLO → GLOBAL → BPM.
- **Key and scale**: **SEL + any key** sets the key for all three synth parts; **SEL + KNOB 2** the
  scale (MIN, HARM and PHRY are the trap ones). Per track on the SEL page.
- **Swing**: HOME → KNOB 1, or per track with SEQ + KNOB 3.

### The kit

Use a **SYN kit** (DRUMS → PRESETS → SYN1…SYN4): synthesised, so it costs no sample slots and every
sound is editable in the web editor's **Drum synth** page.

**Drive the kick inside the kit, not on the track.** Every lane has its own `drive` (a tanh saturator
before its filter), its own level, tune, decay and filter. Set the kick's drive there and the hats stay
clean. The drum track's DIST, INSERT and filter are shared by all sixteen lanes, so they are the wrong
place for anything that should only hit one sound.

### Record, do not step-enter

Steps are for editing. Play the parts in:

| Situation | REC does |
| --- | --- |
| Playing | records the selected track at once; each pass overdubs |
| Stopped, project has notes | arms — your first note starts the loop and is step 1 |
| Stopped, empty | free take: play, press REC on the "1" after your last bar |

Notes land where you *heard* them (the ~12 ms between key and sound is taken back). **Turn ALGORITHM
while recording and the take moves to the next track without stopping** — hats, 808, harmony, melody in
one pass.

On the REC screen: mode, length (1/2/4 bars), start (first note or a count-in) and **QUANT**. With QUANT
off your timing is kept as each step's nudge, still editable afterwards.

---

## 3. Hi-hats

### Triplet grooves

For hats as a *groove*, put the drum track on a triplet grid: **SEQ → PATTERN → DIV = 16T**. That is six
steps a beat, so a bar is 24 steps and the beats land on steps 1, 7, 13 and 19 — a kick on 1 and 3 is
exactly on grid. Hats on every step are continuous 1/16 triplets, and ratchets stay free for bursts.

### Rolls, live (INFUR)

Hold **ARP** and the **black keys choose the roll rate while held**, slow on the left to fast on the
right:

| F#3 | G#3 | A#3 | C#4 | D#4 | F#4 | G#4 | A#4 | C#5 | D#5 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1/2 | 1/4 | 4T | 1/8 | 8T | 1/16 | 16T | 1/32 | 32T | 1/64 |

Left hand on a black key, right hand on the white key of the sound: move the left hand and the
subdivision changes mid-roll. Let go and the rate goes back to GLO → MASTER → ROLL. The last black key
pressed wins. **ARP + key + OCT− / OCT+** rolls ghost or hard.

The slow rates are not for hats. The ROLL layer works on the synth parts too, and **4T on the 808**
gives quarter-note-triplet patterns from one held key.

### The pitch dive (INFUR)

**F#5**, the eleventh black key, dives the selected track's pitch while held: down by
**GLO → MASTER 2 → DIVE** semitones (0–24, default 12) over about 120 ms, and back when you let go.

On a synth part it bends the sounding voices. On the drum track it offsets **each hit as it starts**, so
a running roll walks downwards — a sweep inside a 40 ms hat would not be audible, but a descending roll
very much is. *Synthesised kits only: a sampled kit does not go through the same path and will not dive.*

### Shaping a roll (INFUR)

**GLO → MASTER 2 → ROLL~**: `FLAT · UP · DOWN · UPDN`. It scales every hit of a **ratchet** across the
ratchet, down to 35 % at the quietest, so rolls crescendo or decay instead of sitting flat. One setting
for the whole kit, not per lane. Live rolls keep their own level (OCT− / OCT+).

### What records, and what does not

A live roll is written as **ratchets** — but only when the roll rate is a whole multiple of the step:

| Roll | Records as ratchets on |
| --- | --- |
| 1/32 | a 1/16 grid (x2) |
| 32T | 1/16 (x3) or 16T (x2) |
| 1/64 | 1/16 (x4) |
| 16T | a **1/8** grid (x3) |
| 8T | a **1/4** grid (x3) |
| 4T | a **1/2** grid (x3) |

A 16T roll on a 1/16 grid is one and a half steps: it plays correctly but records as a single plain hit,
not a wrong ratchet. If a roll must end up in the pattern, match the grid to its family.

---

## 4. The 808

Synth 1, ANALOG. The presets are there already: **808 BOOM, 808 DIRTY, 808 SLIDE, SUB BASS, PLUGG
BASS**, and ACID 303 / FUNK BASS nearby. Each carries GLIDE, a pitch envelope and TRANS −24.

- **Glide between notes**: the VOICE page's GLIDE, with GLMOD = RATE (a fixed speed for any interval) or
  TIME (the same time for any interval).
- **A slide on one step**: the NOTES page, OCT+ held, **SLIDE** — it glides into that note in ~50 ms
  even with GLIDE at 0, like an acid slide.
- **Drive for small speakers**: the track's **DIST** (FX page, KNOB 1) is a low cut, then an asymmetric
  soft clip that puts even harmonics on the upper partials, then a tone low-pass that closes as the
  drive rises. 808 DIRTY ships at DIST 24, 808 SLIDE at 30.
- These are **MONO** presets. That is right for a bass, but it means a note held under another will say
  *MONO: SET VOICE POLY*.

---

## 5. Harmony and melody

**CHORD** (SEL + KNOB 1): `OFF · TRIAD · 7TH · 9TH · SUS4 · POWER`. 9TH is 1-3-7-9, the lo-fi voicing.
With a chord mode on, the white keys walk the scale from C4 — one finger per degree, always in key.

**CHORD+**: hold a black key while playing —

| F# | G# | A# | C# | D# |
| --- | --- | --- | --- | --- |
| major ↔ minor | add 7th | sus4 | add 9th | inversion |

Stackable, and recorded as they sound. **SEL 2** has STRUM (1–60 ms, either direction) and **VLEAD**,
which voices each chord nearest the last so progressions move instead of jumping.

**Melody** on the NOTES page (tap SEQ): notes as bars, KNOB 1 walks them, KNOB 2 pitch, KNOB 3 length,
KNOB 4 force (GHOST → SOFT → NORM → HARD → ACC). **OCT+ held** opens a note's sheet: SLIDE, NUDGE, FILL,
MOVE, COPY, DELETE. Undo and redo on EDIT + OCT− / OCT+.

**Four notes a step** is the ceiling. A 9th chord is exactly four.

---

## 6. Texture (INFUR)

**GLO → TEXTURE** is a bed under the whole mix. It is not a track: it costs no synth part and no voices.
One source at a time.

| Source | What it is |
| --- | --- |
| **VINYL** | rumble, crackle and hiss |
| **TAPE** | drifting level, tilted dark noise |
| **DRONE** | three partials (root, fifth, octave) drifting in pitch and level over 25, 18 and 14 s, high-passed at ~110 Hz |
| **VOX** | a small choir: three band-limited saws a few cents apart, 5 Hz vibrato, breath, female formants, the vowel drifting across /a/–/u/ over 24 s |
| **GRAIN** | four overlapping grains of 1–4 s on the root's chord, panned at random |
| **USR1–4** | your own looped samples (about 7 s a slot at 22 kHz; a lower rate buys length) |

Controls: **LEVEL**, **TONE** (left closes the highs, right the lows), **MOVE** (how much it breathes),
and on **TEXTURE 2**: **RATE**, **WHEN** (with the transport, or always), **DUCK** (the kick pumps it),
and **NOTE** — the root DRONE and GRAIN sing on, MIDI 48–79.

**Keeping out of the 808's way.** DRONE and GRAIN put **0.00 %** of their energy below 110 Hz, measured;
VOX 3.6 %. **VINYL is 43 % below 60 Hz** — it is the one source that will mud an 808. Use TONE to high-
pass it, or pick DRONE.

---

## 7. The master chain (INFUR)

**GLO → MASTER**: DUST, **DUCK** (the kick ducks the parts — the sidechain), FILT (a DJ filter), ROLL.

**GLO → MASTER 2**: **CLIP · CEIL · DIVE · ROLL~**

- **CLIP** (0–127) drives the mix into the output stage's tanh knee and lifts the limiter's threshold to
  match, so the mix *clips* instead of being turned down. Measured over its range: **+10.8 dB RMS, crest
  10.9 → 5.0 dB**, and it never reaches full scale. At CLIP 0 the output is bit for bit what it was.
- **CEIL** (0–24, quarter-dB steps) trims *after* the knee. MASTER sits before the clipper, so backing it
  off would take the drive with it; CEIL is the other half a clipper needs. Because the knee saturates at
  32178, the output is bounded whatever the drive:

| CEIL | Hard ceiling |
| --- | --- |
| 0 | −0.16 dBFS / dBTP |
| **8** | **−2.16 dBFS / dBTP** |
| 24 | −6.16 dBFS / dBTP |

  Inter-sample overshoot measured at **+0.00 dB** (the knee is a soft clipper, not a hard one), so true
  peak tracks sample peak. **CEIL 8 is a guaranteed −2 dBTP** for lossy-codec headroom, however hard you
  drive CLIP.

CEIL, DIVE and ROLL~ are settings of the FM-1: they survive a power cycle and a project load. CLIP
belongs to the project.

---

## 8. Bouncing to a DAW

On USB the FM-1 is an audio input named **Felucca** — 44.1 or 48 kHz, 16-bit stereo, class compliant, no
driver. MIDI, the editor and the installer keep working on the same cable.

Before you bounce:

- **HOME → AUDIO → USB AUDIO = FULL**, or the level follows the volume knob
- **GLO → GLOBAL → CLICK = OFF**, or the click is recorded

There are no per-track outputs, so stems are mute-and-pass: mute what you do not want (GLO layer; the
drum track has per-lane live mutes), record, repeat. **CLIP and CEIL apply to the USB path too**, so
stems arrive already under your ceiling.

---

## 9. The look (INFUR)

**Theme** — the web editor's Theme page, over USB, no flash and no risk. Ground `#111512`, bone text,
sage accent; the track colours are a teal family chosen so that the inactive (one-third) state of each
is still visible on the ground.

**Boot splash** — the INFUR mark and wordmark, 240x62, sage on near-black.

---

## 10. What the device holds

The FM-1 has a 1 MB flash. After the firmware, the sample sets and the settings, this is what is left
for your work (`storage.c`, the map at the top of it):

| | How many | Where |
| --- | --- | --- |
| **Sections** (A, B, C, D) | **4**, 3,816 B each | `0x97000..0x9EFFF` |
| The one you are working on | autosaved, comes back at power-on | `0x9F000` and `0xFE000` |
| **Song** (one arrangement) | **1**, up to **16 rows** — each row is *play section X for N bars*, N = 1–64, plus a loop flag | in the settings, 36 B |
| **Your synth kits** SYN1–4 | **4** | in the settings |
| **Your sample slots** USR1–4 | **4**, 80 KiB each (about 7 s at 22 kHz; USR3+4 join into one of ~15 s) | `0xA0000..0xDBFFF`, `0xE7000..` |
| **Your sounds** (user presets) | **32**, in two banks of 16 | `0xDC000..0xDFFFF` |
| **FM6 patches** B1–B27 | **27** | with the samples |

**The song is one list, not sixteen of them.** Sixteen is how many *rows* that one arrangement has, and
a row is "play this section for this many bars". Sections repeat freely, so a row 2 and a row 4 can both
be B:

```
1. A x 4    intro        5. C x 8    chorus
2. B x 8    verse        6. D x 4    bridge
3. C x 8    chorus       7. C x 16   chorus out
4. B x 8    verse            ... up to 16 rows, loop on or off
```

Length is never the problem: 16 rows of up to 64 bars is **1,024 bars**, about half an hour at 140 bpm.
The ceilings that bite are **four distinct patterns** and **sixteen section changes**.

**And the device holds one song at a time.** Four sections is an intro, a verse, a chorus and a bridge,
and that is all of it until you move the song off.

To keep more than that, save an editor **Backup** file per song (Song & sections tab) and restore the
one you want. As many as you like on the computer. The workflow on the move is: finish a song, back it
up over USB, start the next. The autosave means nothing is lost by switching off, but starting a new
song overwrites the last one's sections if it was not backed up.

### What the computer can and cannot do without the FM-1

The editor is storage and a remote control. It is **not a player**: a backup is restored *to the FM-1*,
which plays it.

| | Needs the FM-1 |
| --- | --- |
| Keeping many songs, as backup files | no |
| Editing a song's patterns, notes, kits, mix | no |
| **Hearing it** | **yes** |
| **Getting audio out of it** | **yes, and in real time** |
| Getting the notes into a DAW | no — **Export MIDI**, a track at a time |

**There is no offline bounce.** The editor's **record set** records the FM-1's USB audio to a WAV while
it plays, so the device has to be connected and running. You cannot open a backup away from the device
and render it.

The browser does carry a WebAssembly build of the firmware's engines, but it is for the editor's
simulated mode (`?mock=1`, no device) and auditions patterns rather than arrangements — it will not
play a song back.

**Export MIDI** is the way out without the hardware: a track's pattern as a `.mid` with its notes,
lengths, levels and accents at the song's tempo, the drum track as GM notes on channel 10. A track and
a pattern at a time, not the arrangement — the notes travel, the sound does not.

## 11. Limits worth knowing

- **Four notes a step.** A 9th chord fits exactly; nothing larger.
- **Nudge, swing and fill are per step on the drum track**, not per lane: the whole step moves, all
  sixteen sounds with it. Laid-back hats over a straight kick cannot be expressed.
- **DIV is per track**, so the whole kit shares a grid. Ratchets and the 16T grid cover most of it.
- **The drum track's DIST, INSERT and filter are shared** by all sixteen lanes. Per-lane drive, level,
  tune, decay and filter live in the kit instead.
- **ROLL~ is one shape for the kit**, not per lane.
- **The pitch dive works on synthesised kits only.**
- **Three synth parts.** More is possible but would not give more voices: the 8 are shared.

---

## 12. Before the first flash

**Not yet done — read this before installing anything.**

1. **Read the flash chip** (ABOUT, or `flash` on the console). SLOOP 2.8.5 exists because units with the
   **UC25HQ80** (JEDEC 0xB36014, sold from late 2026) were taken for an unknown part and refused to save
   or update. Do not install anything older than 2.8.5 on such a unit.
2. **Have M-VAVE's M-UPGRADE and the official FM-1 V16 package downloaded** before you start.
3. **Back up** with the web editor.

Recovery, in order of convenience:

| Route | |
| --- | --- |
| **USB RESCUE** | hold **OCT−** alone at power-on, then install again from the browser |
| **M-UPGRADE + V16** | m-vave.com → Downloads → PC Software; close other MIDI apps first |
| **Clipper's installer page** | *Return to the official firmware* — works with SLOOP installed, takes V16 or V15 |
| **FM1 Open workbench** | returns to original V15 |
| **FM-1-transporter** | if it will not boot at all |

The installer checks the package (SHA-256) before writing, the update loader checks it again (CRC)
before starting it, and an interrupted install resumes when you press Install again.
