/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"




/*====================------------------------------------====================*/
/*===----                        current selection                     ----===*/
/*====================------------------------------------====================*/
static void  o___SELECTION_______o () { return; }

ysrc_select__trim       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   short       a           =    0;
   /*---(check lower)--------------------*/
   if (g_sreg.beg < 0)              g_sreg.beg = 0;
   if (g_sreg.end < 0)              g_sreg.end = 0;
   /*---(check upper)--------------------*/
   if (g_sreg.beg >= ysrc_npos ())  g_sreg.beg = ysrc_npos () - 1;
   if (g_sreg.end >= ysrc_npos ())  g_sreg.end = ysrc_npos () - 1;
   /*---(correct directions)-------------*/
   if (g_sreg.beg > g_sreg.end) {
      a          = g_sreg.beg;
      g_sreg.beg = g_sreg.end;
      g_sreg.end = a;
   }
   /*---(correct root)-------------------*/
   if (g_sreg.root < g_sreg.beg)  g_sreg.root = g_sreg.beg;
   if (g_sreg.root > g_sreg.end)  g_sreg.root = g_sreg.end;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> go back to defaults ----------------[ shoot  [gz.530.011.00]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
ysrc_select_reset       (char a_type)
{
   short       x_pos       =    0;
   switch (a_type) {
   case G_SREG_ZERO  :  x_pos =  0;              break;
   case G_SREG_CURR  :  x_pos = ysrc_cpos ();    break;
   case G_SREG_BEG   :  x_pos = g_sreg.beg;      break;
   case G_SREG_END   :  x_pos = g_sreg.end;      break;
   case G_SREG_ROOT  :  x_pos = g_sreg.root;     break;
   }
   /*---(defaults)-----------------------*/
   ysrc_sreg__wipeall ('-', &g_sreg);
   g_sreg.root  = g_sreg.end   = g_sreg.beg   = x_pos;
   g_csreg = '"';
   g_sreg.active = S_SREG_NOT;
   s_cur->cpos = x_pos;
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_select_update      (short a_pos)
{
   /*---(if selection off)---------------*/
   if (g_sreg.active != S_SREG_YES) {
      g_sreg.root = g_sreg.beg = g_sreg.end = a_pos;
   }
   /*---(if current below root)----------*/
   else if (a_pos < g_sreg.root) {
      g_sreg.beg = a_pos;
      g_sreg.end = g_sreg.root;
   }
   /*---(if current above root)----------*/
   else {
      g_sreg.beg = g_sreg.root;
      g_sreg.end = a_pos;
   }
   /*---(check)--------------------------*/
   ysrc_select__trim ();
   /*---(complete)-----------------------*/
   return 0;
}

short
ysrc_select_reverse     (void)
{
   /*---(switch root)--------------------*/
   if      (g_sreg.root == g_sreg.beg) {
      g_sreg.root      = g_sreg.end;
      return g_sreg.beg;
   }
   else if (g_sreg.root == g_sreg.end) {
      g_sreg.root = g_sreg.beg;
      return g_sreg.end;
   }
   /*---(check)--------------------------*/
   ysrc_select__trim ();
   /*---(complete)-----------------------*/
   return g_sreg.end;
}

char
ysrc_select_exact       (short a_beg, short a_end, short a_root)
{
   /*---(save values)--------------------*/
   g_sreg.beg    = a_beg;
   g_sreg.end    = a_end;
   g_sreg.root   = a_root;
   g_sreg.active = S_SREG_YES;
   /*---(check)--------------------------*/
   ysrc_select__trim ();
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_select_curr        (short *a_beg, short *a_end, short *a_root)
{
   /*---(check first)--------------------*/
   ysrc_select__trim ();
   /*---(save back)----------------------*/
   if (a_beg  != NULL)  *a_beg  = g_sreg.beg;
   if (a_end  != NULL)  *a_end  = g_sreg.end;
   if (a_root != NULL)  *a_root = g_sreg.root;
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_select_get         (short *a_beg, short *a_end, short *a_root, short *a_len, char *a_data)
{
   /*---(check first)--------------------*/
   ysrc_select__trim ();
   /*---(save back)----------------------*/
   if (a_beg  != NULL)  *a_beg  = g_sreg.beg;
   if (a_end  != NULL)  *a_end  = g_sreg.end;
   if (a_root != NULL)  *a_root = g_sreg.root;
   if (a_len  != NULL)  *a_len  = g_sreg.end - g_sreg.beg + 1;
   if (a_data != NULL)  ystrlcpy (a_data, s_cur->contents + g_sreg.beg, g_sreg.end - g_sreg.beg + 2);
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_select_all         (void)
{
   /*---(set values)---------------------*/
   g_sreg.beg    = 0;
   g_sreg.end    = ysrc_npos () - 1;
   g_sreg.root   = 0;
   g_sreg.active = S_SREG_YES;
   /*---(complete)-----------------------*/
   return 0;
}

char ySRC_select_islive     (void) { if (g_sreg.active == S_SREG_YES)  return 1; return 0; }
char ysrc_select_makelive   (void) { g_sreg.active = S_SREG_YES; return 0; }



