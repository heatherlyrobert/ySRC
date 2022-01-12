/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



uchar      g_char      = ' ';
uchar      g_goto      = 'c';



/*====================------------------------------------====================*/
/*===----                    absolute position moves                   ----===*/
/*====================------------------------------------====================*/
static void      o___ABSOLUTE________________o (void) {;}

char         /*-> tbd --------------------------------[ ------ [ge.DB2.162.62]*/ /*-[01.0000.403.!]-*/ /*-[--.---.---.--]-*/
ysrc_move_simple        (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_allow     =  '-';
   /*---(header)-------------------------*/
   DEBUG_YSRC  yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC  yLOG_char    ("a_major"   , a_major);
   DEBUG_YSRC  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   --rce;  if (a_major == 0 || a_minor == 0) {
      DEBUG_YSRC   yLOG_note    ("either a_major or a_minor were null");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   if (a_major == G_KEY_SPACE && yKEYS_is_horz_simple (a_minor))  x_allow = 'y';
   if (a_major == 'c' && strchr ("HhlL", a_minor) != NULL)        x_allow = 'y';
   --rce;  if (x_allow != 'y') {
      DEBUG_YSRC   yLOG_note    ("a_major/a_minor combination not allowed");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(normal horizontal)--------------*/
   if (a_major == G_KEY_SPACE) {
      switch (a_minor) {
      case '0' : s_cur->cpos  =  0;                   break;
      case 'H' : s_cur->cpos -=  5;                   break;
      case 'h' : --s_cur->cpos;                       break;
      case 'l' : ++s_cur->cpos;                       break;
      case 'L' : s_cur->cpos +=  5;                   break;
      case '$' : s_cur->cpos  = s_cur->npos - 1;      break;
      }
   }
   /*---(big horizonal)------------------*/
   else if (a_major == 'c') {
      switch (a_minor) {
      case 'H' : s_cur->cpos -= 125;                  break;
      case 'h' : s_cur->cpos -=  25;                  break;
      case 'l' : s_cur->cpos +=  25;                  break;
      case 'L' : s_cur->cpos += 125;                  break;
      }
   }
   /*---(wrapup)-------------------------*/
   rc = UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_YSRC  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc_move_char_next     (uchar a_ch)
{
   char        rc          =    0;
   int         i           =    0;
   if      (a_ch ==  0)  a_ch = g_char;
   else if (a_ch ==  1)  a_ch = s_cur->contents [s_cur->cpos];
   for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
      if (s_cur->contents [i] != a_ch)  continue;
      s_cur->cpos = i;
      g_char      = a_ch;
      rc = UPDATE_AFTER_CHANGES;
      return rc;
   }
   return -1;
}

char
ysrc_move_char_prev     (uchar a_ch)
{
   char        rc          =    0;
   int         i           =    0;
   if      (a_ch ==  0)  a_ch = g_char;
   else if (a_ch ==  1)  a_ch = s_cur->contents [s_cur->cpos];
   for (i = s_cur->cpos - 1; i >= 0; --i) {
      if (s_cur->contents [i] != a_ch)  continue;
      s_cur->cpos = i;
      g_char      = a_ch;
      rc = UPDATE_AFTER_CHANGES;
      return rc;
   }
   return -1;
}



/*====================------------------------------------====================*/
/*===----                     screen relative moves                    ----===*/
/*====================------------------------------------====================*/
static void      o___SCREEN__________________o (void) {;}

char         /*-> tbd --------------------------------[ ------ [ge.DB2.162.62]*/ /*-[01.0000.403.!]-*/ /*-[--.---.---.--]-*/
ysrc_move_goto          (uchar a_major, uchar a_minor)
{
   /*---(design notes)-------------------*/
   /*
    *  SHLE move screen, but maintain relative cursor position
    *
    *  SE   overlay the new and old screens by one column
    *  H    begining goes to center (mostly, except if even columns)
    *  L    end goes to center (mostly, except if even columns)
    *
    *  gg   means repeat last goto
    *
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_qtr       =    0;
   int         x_haf       =    0;
   int         x_ful       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC  yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   --rce;  if (a_major != 'g') {
      DEBUG_YSRC   yLOG_note    ("a_major was not g (goto)");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_minor == 0 || !yKEYS_is_horz_goto (a_minor)) {
      DEBUG_YSRC   yLOG_note    ("a_minor was not a valid option");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(distances)----------------------*/
   x_qtr  = s_cur->apos / 4;
   x_haf  = s_cur->apos / 2;
   x_ful  = s_cur->apos;
   /*---(repeating)----------------------*/
   if (a_minor == 'g')  a_minor = g_goto;
   /*---(goto moves)---------------------*/
   switch (a_minor) {
   case 'S' : s_cur->bpos  = s_cur->bpos - x_ful + 1;
              s_cur->cpos  = s_cur->cpos - x_ful + 1;
              break;
   case 'H' : s_cur->bpos  = s_cur->bpos - x_haf + 1;
              s_cur->cpos  = s_cur->cpos - x_haf + 1;
              break;
   case 's' : s_cur->cpos  = s_cur->bpos;                         break;
   case 'h' : s_cur->cpos  = s_cur->bpos + x_qtr * 1 - 1;         break;
   case 'c' : s_cur->cpos  = s_cur->bpos + x_haf     - 1;         break;
   case 'l' : s_cur->cpos  = s_cur->bpos + x_qtr * 3 - 1;         break;
   case 'e' : s_cur->cpos  = s_cur->epos;                         break;
   case 'L' : s_cur->bpos  = s_cur->bpos + x_haf - 1;
              s_cur->cpos  = s_cur->cpos + x_haf - 1;
              break;
   case 'E' : s_cur->bpos  = s_cur->bpos + x_ful - 1;
              s_cur->cpos  = s_cur->cpos + x_ful - 1;
              break;
   }
   g_goto = a_minor;
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_YSRC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.DB2.162.62]*/ /*-[01.0000.403.!]-*/ /*-[--.---.---.--]-*/
ysrc_move_scroll        (uchar a_major, uchar a_minor)
{
   /*---(design notes)-------------------*/
   /*
    *  all positions should match where goto's end up (shcle)
    *
    *  current position stays the same, beg/end move
    *
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_qtr       =    0;
   int         x_haf       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC  yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   --rce;  if (a_major != 'z') {
      DEBUG_YSRC   yLOG_note    ("a_major was not z (scroll)");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_minor == 0 || !yKEYS_is_horz_scroll (a_minor)) {
      DEBUG_YSRC   yLOG_note    ("a_minor was not a valid option");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(distances)----------------------*/
   x_qtr  = s_cur->apos / 4;
   x_haf  = s_cur->apos / 2;
   /*---(scrolls)------------------------*/
   switch (a_minor) {
   case 's' : s_cur->bpos += s_cur->cpos -  s_cur->bpos;               break;
   case 'h' : s_cur->bpos += s_cur->cpos - (s_cur->bpos + x_qtr * 1) + 1;  break;
   case 'c' : s_cur->bpos += s_cur->cpos - (s_cur->bpos + x_haf) + 1;  break;
   case 'l' : s_cur->bpos += s_cur->cpos - (s_cur->bpos + x_qtr * 3) + 1;  break;
   case 'e' : s_cur->bpos += s_cur->cpos -  s_cur->epos;               break;
   }
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_YSRC  yLOG_exit    (__FUNCTION__);
   return 0;
}



