/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"

char
ysrc_select_line        (char *a_entry)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_RECD ];
   int         x_len       =    0;
   char        x_data      [LEN_RECD ];
   strlcpy (t, s_cur->contents + g_sreg.beg, g_sreg.end - g_sreg.beg + 2);
   x_len = strlen (t);
   if (x_len > 60)  sprintf (x_data , "%3då%-.60s>", x_len, t);
   else             sprintf (x_data , "%3då%sæ"    , x_len, t);
   sprintf (a_entry, "%c %3db %3de %3dr %s", g_sreg.active, g_sreg.beg, g_sreg.end, g_sreg.root, x_data);
   return 0;
}

char
ySRC_select_status      (char a_size, short a_wide, char *a_list)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_FULL]   = "";
   /*---(defense)------------------------*/
   --rce;  if (a_list == NULL)   return rce;
   /*---(get info)-----------------------*/
   rc = ysrc_select_line (t);
   --rce;  if (rc < 0)            return rce;
   /*---(generate stats)--------------*/
   sprintf (a_list, " selc %s Ï", t);
   /*---(complete)--------------------*/
   return 0;
}

char
ysrc_sreg_line     (uchar a_abbr, char *a_entry)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   tSREG      *x_sreg      = NULL;
   char       *x_data      [LEN_RECD ];
   char       *x_label     [LEN_LABEL];
   /*---(defense)------------------------*/
   --rce; if (a_entry == NULL)   return rce;
   strlcpy (a_entry, "· n/a", LEN_RECD);
   n = ysrc_sreg_index (a_abbr);
   if (n  < 0)   return rce;
   x_sreg = &g_sregs [n];
   /*---(fill)---------------------------*/
   if (x_sreg->len > 40)  sprintf (x_data , "%3då%-.40s>", x_sreg->len, x_sreg->data);
   else                   sprintf (x_data , "%3då%sæ"    , x_sreg->len, x_sreg->data);
   sprintf (x_label, "å%-.10sæ", x_sreg->label);
   sprintf (a_entry, "%c %c %c %-45.45s %3db %3de %3dr  %s",
         a_abbr, x_sreg->active, x_sreg->source, x_data,
         x_sreg->beg, x_sreg->end, x_sreg->root, x_label);
   /*---(complete)-----------------------*/
   if (x_sreg->active == S_SREG_NOT)  return 0;
   return 1;
}

char
ySRC_sreg_status        (char a_size, short a_wide, char *a_list)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_FULL]   = "";
   /*---(defense)------------------------*/
   --rce;  if (a_list == NULL)   return rce;
   /*---(get info)-----------------------*/
   rc = ysrc_sreg_line (g_psreg, t);
   --rce;  if (rc < 0)            return rce;
   /*---(generate stats)--------------*/
   sprintf (a_list, " sreg %s Ï", t);
   /*---(complete)--------------------*/
   return 0;
}
