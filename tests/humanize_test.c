/* SPDX-License-Identifier: GPL-3.0-only */
/* (INFUR) SAVE > TOOLS > HUMAN (seq.c humanize): a quantised pattern is spread in time, and on a synth
 * track in velocity, by enough to stop the grid showing and not enough to sound sloppy.
 *   the spread stays inside HUMAN_T / HUMAN_V and is centred on zero (a pattern must not walk off the beat)
 *   it is triangular, not flat: most steps barely move, a big move is rare
 *   an accented step keeps its velocity; the drum track is moved in time only (its level is 2 bits)
 *   empty steps are left alone, and one undo takes the lot back */
#define main hostsim_main
#include "hostsim.c"
#undef main
static int fails;
static void check(int ok, const char *what)
{
    printf("human: %-72s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        fails++;
}
int main(void)
{
    uint32_t i, n, runs;
    int mn = 99, mx = -99, vn = 999, vx = -999, centre = 0, edge = 0;
    long sm = 0, sv = 0, steps = 0;
    track_t *t = &trk[0], *d;
    host_tracks_init();
    /* a flat, quantised pattern over many runs, so the distribution is worth looking at */
    for (runs = 0; runs < 40u; runs++) {
        t->p[P_SLEN] = 64;
        for (i = 0; i < 64u; i++) {
            uint8_t note = 60;
            put_step(t, i, 1u, &note, ST_NOTE, 0);
            t->step[i].vel = 100;
            t->micro[i] = 0;
        }
        t->step[7].flags |= SF_ACCENT;
        n = humanize(t);
        if (n != 64u)
            check(0, "every step with a note is touched");
        for (i = 0; i < 64u; i++) {
            int m = t->micro[i], dv = (int)t->step[i].vel - 100;
            if (m < mn) mn = m;
            if (m > mx) mx = m;
            if (i != 7u) { if (dv < vn) vn = dv; if (dv > vx) vx = dv; sv += dv; }
            sm += m; steps++;
            if (m >= -1 && m <= 1) centre++;
            if (m <= -HUMAN_T + 1 || m >= HUMAN_T - 1) edge++;
        }
        if (t->step[7].vel != 100u)
            check(0, "the accented step keeps its velocity");
    }
    check(mn >= -HUMAN_T && mx <= HUMAN_T, "the timing spread stays inside HUMAN_T");
    check(vn >= -HUMAN_V && vx <= HUMAN_V, "the velocity spread stays inside HUMAN_V");
    printf("human:   timing %+d..%+d mean %+.3f; velocity %+d..%+d mean %+.3f; centre %ld%% edge %ld%%\n",
           mn, mx, (double)sm / (double)steps, vn, vx, (double)sv / (double)steps,
           100L * centre / steps, 100L * edge / steps);
    check(sm * 4 > -steps && sm * 4 < steps,   /* |mean| < 0.25 of a 64th: 4 % of the spread */
          "centred on zero: a humanised pattern does not walk off the beat");
    check(centre > edge * 2, "triangular, not flat: most steps barely move, a big move is rare");
    /* the drum track: time only, and empty steps left alone */
    d = TDRUM;
    d->p[P_SLEN] = 64;
    for (i = 0; i < 64u; i++) {
        d->micro[i] = 0;
        memset(&d->dstep[i], 0, sizeof d->dstep[i]);
        if (i % 2u == 0u)
            dstep_set(&d->dstep[i], 0u, LV_NORM, 0u);
    }
    n = humanize(d);
    {
        int moved = 0, lvl = 0, empty_moved = 0;
        for (i = 0; i < 64u; i++) {
            if (i % 2u == 0u) {
                if (d->micro[i]) moved++;
                if (dstep_lvl(&d->dstep[i], 0u) != (uint32_t)LV_NORM) lvl++;
            } else if (d->micro[i]) {
                empty_moved++;
            }
        }
        check(n == 32u && moved > 16, "the drum track: its hits are moved in time");
        check(!lvl, "the drum track keeps its levels (two bits: a move would be a whole level)");
        check(!empty_moved, "empty steps are left alone");
    }
    printf("human: %s\n", fails ? "FAILED" : "timing and velocity spread, centred, triangular, drums in time only PASS");
    return fails != 0;
}
