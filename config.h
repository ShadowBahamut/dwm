#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//static const char *fonts[]          = { "monospace:size=14" };
static const char *fonts[]          = { "monospace:size=14", "JoyPixels:pixelsize=14:antialias=true:autohint=true"  };
static const char dmenufont[]       = "monospace:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#000000";
static const char col_yellow[]      = "#f6ec76";
static const char col_urgborder[]   = "#ff0000";
static const char col1[]            = "#ffffff";
static const char col2[]            = "#ffffff";
static const char col3[]            = "#ffffff";
static const char col4[]            = "#ffffff";
static const char col5[]            = "#ffffff";
static const char col6[]            = "#ffffff";


static const char norm_fg[] = "#e3cce0";
static const char norm_bg[] = "#0f0915";
static const char norm_border[] = "#9e8e9c";

static const char sel_fg[] = "#e3cce0";
static const char sel_bg[] = "#67549C";
static const char sel_border[] = "#e3cce0";

static const char urg_fg[] = "#e3cce0";
static const char urg_bg[] = "#3E56BF";
static const char urg_border[] = "#3E56BF";

enum { SchemeNorm, SchemeSel, SchemeUrg }; /* color schemes */
//enum { SchemeNorm, SchemeCol1, SchemeCol2, SchemeCol3, SchemeCol4,
//       SchemeCol5, SchemeCol6, SchemeSel, SchemeUrg }; /* color schemes */

static const unsigned int baralpha = 0xAF;
static const unsigned int borderalpha = 0xFF;
static const char *colors[][3]      = {
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
    [SchemeUrg] =  { urg_fg,      urg_bg,    urg_border },

    /*               fg         bg         border   */
//	[SchemeNorm]  = { col_gray3, col_gray1, col_gray2 },
//	[SchemeCol1]  = { col1,      col_gray1, col_gray2 },
//	[SchemeCol2]  = { col2,      col_gray1, col_gray2 },
//	[SchemeCol3]  = { col3,      col_gray1, col_gray2 },
//	[SchemeCol4]  = { col4,      col_gray1, col_gray2 },
//	[SchemeCol5]  = { col5,      col_gray1, col_gray2 },
//	[SchemeCol6]  = { col6,      col_gray1, col_gray2 },
//	[SchemeSel]   = { col_gray4, col_yellow,  col_yellow  },
//	[SchemeUrg]  = { col_gray4, col_yellow,  col_urgborder  },
};

static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_yellow, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *Lock[]  = { "i3lock", NULL };
static const char *MANAPICKER[] = { "SXIVPICKER" , NULL };
static const char *DOUJINPICKER[] = { "DOUJINPICKER" , NULL };
static const char *volupdate[] =   { "sigdsblocks", "2", NULL };
static const char *volup[] =   { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+2%", NULL };
static const char *voldown[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-2%", NULL };
static const char *volMute[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *MuteMic[] = { "amixer", "set", "Capture", "toggle", NULL };
static const char *brightnessctl[2][3] = {
   { "brightnessctl", "set", "2%+" },
   { "brightnessctl", "set", "2%-" }  };  

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,	                    XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
    {0,      			XF86XK_AudioRaiseVolume, spawn,   {.v=volup} },
	{0,      			XF86XK_AudioRaiseVolume, spawn,   {.v=volupdate } },
 	{0,      			XF86XK_AudioLowerVolume, spawn,   {.v=voldown } },
 	{0,      			XF86XK_AudioLowerVolume, spawn,   {.v=volupdate } },
 	{0,      		 	XF86XK_AudioMute,        spawn,   {.v=volMute} },
 	{0,      		 	XF86XK_AudioMute,        spawn,   {.v=volupdate } },
 	{0,      		 	XF86XK_AudioMicMute,     spawn,   {.v=MuteMic} },
 	{0,      		 	XF86XK_MonBrightnessUp,          spawn,   {.v=brightnessctl[0]} },
 	{0,      		 	XF86XK_MonBrightnessDown,        spawn,   {.v=brightnessctl[1]} },
	{ MODKEY|ControlMask,           XK_x,      spawn,            {.v = MANAPICKER } },
	{ MODKEY,        		        XK_x,      spawn,            {.v = DOUJINPICKER } },
	{ MODKEY|ControlMask,           XK_l,      spawn,            {.v = Lock } },
	
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdsblocks,    {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdsblocks,    {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdsblocks,    {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

