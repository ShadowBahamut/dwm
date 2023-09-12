static const char norm_fg[] = "#e3cce0";
static const char norm_bg[] = "#0f0915";
static const char norm_border[] = "#9e8e9c";

static const char sel_fg[] = "#e3cce0";
static const char sel_bg[] = "#67549C";
static const char sel_border[] = "#e3cce0";

static const char urg_fg[] = "#e3cce0";
static const char urg_bg[] = "#3E56BF";
static const char urg_border[] = "#3E56BF";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
    [SchemeUrg] =  { urg_fg,      urg_bg,    urg_border },
};
