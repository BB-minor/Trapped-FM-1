/* SPDX-License-Identifier: GPL-3.0-only */
/* (INFUR) the two rightmost black keys, D#5 and F#5, as soft and hard on a synth track (seq.c
 * key_vel_syn). The FM-1's keys have no velocity sensor and a synth part played 100 whatever you did.
 *   with QNT WHITE (the default) those two are silent, so they modify instead: 72 and 127
 *   nothing held is still 100, as it always was
 *   with QNT OFF they are notes again, and a key plays 100
 *   what is recorded is what was played */
#define main hostsim_main
#include "hostsim.c"
#undef main
static void run_block(void) { int32_t out[CTL * 2]; mix_block(out, CTL); keyboard_block(); }
static int fails;
static void check(int ok, const char *what)
{
    printf("keyvel: %-66s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        fails++;
}
int main(void)
{
    track_t *t = &trk[0];
    host_tracks_init();
    t->p[P_QUANT] = 2;                                   /* WHITE, the default */
    check(kb_map(t, KEY_VSOFT) == KB_SILENT && kb_map(t, KEY_VHARD) == KB_SILENT,
          "QNT WHITE: D#5 and F#5 are silent, so they are free to mean something");
    fm1_in.notes = 0;
    check(key_vel_syn(t) == 100u, "nothing held: 100, as a synth key always played");
    fm1_in.notes = 1u << KEY_VSOFT;
    check(key_vel_syn(t) == 72u, "D#5 held: soft (72, lvl_vel's SOFT)");
    fm1_in.notes = 1u << KEY_VHARD;
    check(key_vel_syn(t) == 127u, "F#5 held: hard (127, lvl_vel's HARD)");
    fm1_in.notes = (1u << KEY_VSOFT) | (1u << KEY_VHARD);
    check(key_vel_syn(t) == 127u, "both held: hard wins, one rule rather than a surprise");
    t->p[P_QUANT] = 0;                                   /* OFF: chromatic, the black keys are notes */
    fm1_in.notes = 1u << KEY_VSOFT;
    check(kb_map(t, KEY_VSOFT) != KB_SILENT && key_vel_syn(t) == 100u,
          "QNT OFF: D#5 is a note again and modifies nothing");
    t->p[P_QUANT] = 2;
    {   /* played and recorded: the step keeps the velocity the key was played at */
        uint32_t white = 7u;                             /* C4 */
        t->p[P_SLEN] = 16;
        steps_clear(t);
        song.sel = 0;
        song.rec = 1u;
        transport_req = 1;
        run_block();
        fm1_in.notes = 1u << KEY_VHARD;                  /* hold hard, then play */
        run_block();
        fm1_in.notes |= 1u << white;
        run_block();
        fm1_in.notes = 0;
        run_block();
        check(t->step[0].n == 1u && t->step[0].vel == 127u,
              "recorded with F#5 held: the step keeps 127");
    }
    printf("keyvel: %s\n", fails ? "FAILED" : "soft and hard on the two rightmost black keys PASS");
    return fails != 0;
}
