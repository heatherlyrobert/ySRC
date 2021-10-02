/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"




/*====================------------------------------------====================*/
/*===----                        current selection                     ----===*/
/*====================------------------------------------====================*/
static void  o___SELECTION_______o () { return; }

char         /*-> go back to defaults ----------------[ shoot  [gz.530.011.00]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
ysrc_select_reset       (int a_pos)
{
   ysrc_sreg__wipeall ('-', &g_sreg);
   g_sreg.root  = g_sreg.end   = g_sreg.beg   = a_pos;
   g_csreg = '"';
   return 0;
}

char
ysrc_select_update      (int a_pos)
{
   if (g_sreg.active != S_SREG_YES) {
      g_sreg.root = g_sreg.beg = g_sreg.end = a_pos;
   } else if (a_pos < g_sreg.root) {
      g_sreg.beg = a_pos;
      g_sreg.end = g_sreg.root;
   } else {
      g_sreg.beg = g_sreg.root;
      g_sreg.end = a_pos;
   }
   return 0;
}

int
ysrc_select_reverse     (void)
{
   if      (g_sreg.root == g_sreg.beg) {
      g_sreg.root      = g_sreg.end;
      return g_sreg.beg;
   }
   else if (g_sreg.root == g_sreg.end) {
      g_sreg.root = g_sreg.beg;
      return g_sreg.end;
   }
}

char
ysrc_select_exact       (int a_beg, int a_end, int a_root)
{
   g_sreg.beg  = a_beg;
   g_sreg.end  = a_end;
   g_sreg.root = a_root;
   return 0;
}

char
ysrc_select_curr        (int *a_beg, int *a_end, int *a_root)
{
   if (a_beg  != NULL)  *a_beg  = g_sreg.beg;
   if (a_end  != NULL)  *a_end  = g_sreg.end;
   if (a_root != NULL)  *a_root = g_sreg.root;
   return 0;
}

char
ysrc_select_all         (void)
{
   g_sreg.beg  = 0;
   g_sreg.end  = ysrc_npos() - 1;
   g_sreg.root = 0;
   return 0;
}

char ysrc_select_getlive    (void) { return g_sreg.active; }
char ysrc_select_islive     (void) { if (g_sreg.active == S_SREG_YES)  return 1; return 0; }
char ysrc_select_isdead     (void) { if (g_sreg.active == S_SREG_NOT)  return 1; return 0; }
char ysrc_select_makelive   (void) { g_sreg.active = S_SREG_YES; return 0; }
char ysrc_select_makedead   (void) { g_sreg.active = S_SREG_NOT; return 0; }



