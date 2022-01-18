/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


tEDIT   s_src;
tEDIT   s_cmd;
tEDIT  *s_cur    = &s_src;


char
ySRC_size               (short a_formula, short a_command)
{
   s_src.wide = a_formula;
   s_src.apos = a_formula - 6;
   s_cmd.wide = a_command;
   s_cmd.apos = a_command - 6;
   return 0;
}


int   ysrc_cpos         (void) { return s_cur->cpos; }
int   ysrc_npos         (void) { return s_cur->npos; }
char *ysrc_contents     (void) { return s_cur->contents; }

char
ysrc__source_biggies    (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   char        x_pos       =    0;
   /*---(quick out)----------------------*/
   if (a_major != G_KEY_SPACE)  return 0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(major keys)---------------------*/
   switch (a_minor) {
   case G_KEY_SPACE  :
      DEBUG_YSRC   yLOG_note    ("space, nothing to do");
      break;
   case G_KEY_ESCAPE :
      if (ySRC_select_islive ()) {
         DEBUG_YSRC   yLOG_note    ("escape, means get out of selection");
         x_pos = g_sreg.root;
         ysrc_select_reset (G_SREG_ROOT);
         s_cur->cpos = x_pos;
      } else {
         DEBUG_YSRC   yLOG_note    ("escape, means forget and return to previous mode");
         ysrc_reset  ();
         ysrc_after  ();
         yMODE_exit  ();
      }
      break;
   case G_KEY_RETURN :
      DEBUG_YSRC   yLOG_note    ("enter, means save and return to previous mode");
      ysrc_accept ();
      ysrc_after  ();
      yMODE_exit  ();
      break;
   case G_KEY_BTICK :
      DEBUG_YSRC   yLOG_note    ("backtick, means re-wander or move label");
      rc = yMODE_enter (UMOD_WANDER);
      if (rc < 0) {
         DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      break;
   case  '\0'     :
      break;
   default        :
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysrc__source_macro      (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(quick out)----------------------*/
   if (a_major != G_KEY_SPACE)  return 0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(macros modes)-------------------*/
   switch (a_minor) {
   case '@'      :
      DEBUG_YSRC   yLOG_note    ("macro execution");
      yMODE_enter  (SMOD_MACRO   );
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return a_minor;
      break;
   case 'q'      :
      IF_MACRO_RECORDING {
         DEBUG_YSRC   yLOG_note    ("end macro recording");
         rc = yMACRO_rec_end ();
      } else {
         DEBUG_YSRC   yLOG_note    ("begin macro recording");
         yMODE_enter  (SMOD_MACRO   );
         rc = a_minor;
      }
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'Q'      :
      DEBUG_YSRC   yLOG_note    ("reset macro recording");
      rc = yMACRO_reset_all ();
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc__source_subs       (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(quick out)----------------------*/
   if (a_major != G_KEY_SPACE)  return 0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(sub modes)----------------------*/
   switch (a_minor) {
   case  'v' :
      if (ySRC_select_islive ()) s_cur->cpos = ysrc_select_reverse  ();
      else                       ysrc_select_makelive ();
      rc = 0;
      break;
   case  '"' :
      DEBUG_YSRC   yLOG_note    ("switch to a text register mode (¶)");
      rc = yMODE_enter (SMOD_SREG);
      if (rc >= 0)  rc = a_minor;
      break;
   case  'r' :
      DEBUG_YSRC   yLOG_note    ("enter replace mode");
      rc = yMODE_enter (UMOD_REPLACE);
      /*> if (rc >= 0)  rc = a_minor;                                                 <*/
      break;
   case  'R' :
      DEBUG_YSRC   yLOG_note    ("enter replace mode");
      rc = yMODE_enter (UMOD_REPLACE);
      /*> if (rc >= 0)  rc = a_minor;                                                 <*/
      break;
   case  'i' : case  'a' : case  'I' : case  'A' :
      DEBUG_YSRC   yLOG_note    ("enter input mode");
      rc = yMODE_enter (UMOD_INPUT);
      /*> if (rc >= 0)  rc = tolower (a_minor);                                       <*/
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc__source_findchar   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(multi-keys)---------------------*/
   if (strchr ("fF", a_major) != NULL) {
      switch (a_major) {
      case 'f' :
         rc = ysrc_move_char_next (a_minor);
         break;
      case 'F' :
         rc = ysrc_move_char_prev (a_minor);
         break;
      }
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(single keys)--------------------*/
   if (a_major != G_KEY_SPACE) {
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   switch (a_minor) {
   case  '#' :
      DEBUG_YSRC   yLOG_note    ("current char find");
      rc = ysrc_move_char_next (1);
      break;
   case  'n' :
      DEBUG_YSRC   yLOG_note    ("char find next");
      rc = ysrc_move_char_next (0);
      break;
   case  'N' :
      DEBUG_YSRC   yLOG_note    ("char find reverse");
      rc = ysrc_move_char_prev (0);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc__source_editing    (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(quick out)----------------------*/
   if (a_major != G_KEY_SPACE)  return 0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(delete/clear)----------------*/
   switch (a_minor) {
   case  G_KEY_DEL : case  G_CHAR_DEL : case  'D' :
      rc = ysrc_multi_pure ('d', 'l');
      if (rc >= 0)  rc = 0;
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case  G_KEY_BS  : case  G_CHAR_BS  : case  'X' :
      rc = ysrc_multi_pure ('d', 'h');
      if (rc >= 0)  rc = 0;
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(other stuff)-----------------*/
   switch (a_minor) {
   case '|' :
      DEBUG_YSRC   yLOG_value   ("repeats"   , yKEYS_repeats);
      s_cur->cpos = yKEYS_repeat_useall ();
      DEBUG_YSRC   yLOG_value   ("cpos"      , s_cur->cpos);
      rc     = 0;
      ysrc_after ();
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'u' :
      ysrc_select_reset (s_cur->cpos);
      rc = ysrc_sundo_undo ();
      ysrc_after ();
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'U' :
      ysrc_select_reset (s_cur->cpos);
      rc = ysrc_sundo_redo ();
      ysrc_after ();
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc__source_multikey   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   char        t           [LEN_RECD]  = "";
   /*---(quick out)----------------------*/
   if (a_major == G_KEY_SPACE)  return 0;
   if (a_major == 0)            return 0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (strchr ("ydxc", a_major) != NULL) {
      if (yKEYS_repeat_beg ()) {
         DEBUG_YSRC   yLOG_note    ("initialize yanked text");
         mySRC.yank [0] = '\0';
         mySRC.ylen     = 0;
         mySRC.ybeg     = s_cur->cpos;
      }
   }
   /*---(multi-key)----------------------*/
   switch (a_major) {
   case 'y' : case 'd' : case 'x' : case 'c' :
      DEBUG_YSRC   yLOG_note    ("multi-key yank (y), delete (d), clear (x), or change (c)");
      rc = ysrc_multi_pure (a_major, a_minor);
      break;
   case 'g' :
      DEBUG_YSRC   yLOG_note    ("multi-key goto (g)");
      rc = ysrc_move_goto (a_major, a_minor);
      break;
   case 'z' :
      DEBUG_YSRC   yLOG_note    ("multi-key scroll (z)");
      rc = ysrc_move_scroll (a_major, a_minor);
      break;
   case 'f' : case 'F' :
      DEBUG_YSRC   yLOG_note    ("multi-key find char (F or f)");
      rc = ysrc__source_findchar (a_major, a_minor);
      break;
   }
   /*---(wrap-up)------------------------*/
   if (strchr ("ydxc", a_major) != NULL) {
      if (yKEYS_repeat_end ()) {
         ysrc_select_reset (G_SREG_ROOT);
         if (strchr ("HhBb0" , a_minor) != NULL) {
            strlrev (t, mySRC.yank, LEN_RECD);
            ysrc_sreg_push ('¶', t);
            g_sregs [0].end    = mySRC.ybeg;
            g_sregs [0].beg    = mySRC.ybeg - mySRC.ylen + 1;
            g_sregs [0].root   = g_sregs [0].end;
         } else {
            ysrc_sreg_push ('¶', mySRC.yank);
            g_sregs [0].beg    = mySRC.ybeg;
            g_sregs [0].end    = mySRC.ybeg + mySRC.ylen - 1;
            g_sregs [0].root   = g_sregs [0].beg;
         }
         g_sregs [0].source = 'u';
         g_sregs [0].len    = mySRC.ylen;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*-> process keys for cell edit mode ----[ leaf   [ge.#R6.23#.HG]*/ /*-[05.0000.102.#]-*/ /*-[--.---.---.--]-*/
ySRC_mode               (uchar a_major, uchar a_minor)
{
   /*---(design notes)-------------------*/
   /*
    *   this should imitate the vi-normal mode, but only horizontally.
    *   all formulas and contents should be relatively short or they
    *   should be broken up (formulas) or in a database (text).  so,
    *   forget wordwrap.
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =   -1;
   int         i           =    0;
   int         n           =    0;
   static char x_char      =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_char    ("a_major"   , a_major);
   DEBUG_YSRC   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   /*---(defenses)-----------------------*/
   DEBUG_YSRC   yLOG_char    ("mode"      , yMODE_curr ());
   DEBUG_YSRC   yLOG_info    ("valid"     , MODES_ONELINE);
   --rce;  if (strchr (MODES_ONELINE, yMODE_curr ()) == NULL ) {
      DEBUG_YSRC   yLOG_note    ("not an acceptable mode");
      yMODE_exit  ();
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(universal)----------------------*/
   rc = ysrc__source_biggies (a_major, a_minor);
   DEBUG_YSRC   yLOG_value   ("biggies"   , rc);
   if (rc > 0) {
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(single letter)------------------*/
   if (a_major == G_KEY_SPACE) {
      /*---(repeats)---------------------*/
      if (yKEYS_is_repeater (a_minor)) {
         DEBUG_YSRC   yLOG_note    ("repeat character 1-9");
         yMODE_enter  (PMOD_REPEAT);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(select related)--------------*/
      if (strchr ("pPY", a_minor) != 0) {
         DEBUG_YSRC   yLOG_note    ("switch to a text register mode (pPY)");
         ysrc_sreg_setreg ('"');
         yMODE_enter (SMOD_SREG);
         rc = ysrc_sreg_smode (G_KEY_SPACE, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      if (ySRC_select_islive () && strchr ("yxdrsc", a_minor) != 0) {
         DEBUG_YSRC   yLOG_note    ("switch to a text register mode (yxdrsc)");
         ysrc_sreg_setreg ('"');
         yMODE_enter (SMOD_SREG);
         rc = ysrc_sreg_smode (G_KEY_SPACE, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(multikey prefixes)-----------*/
      if (yKEYS_is_multi_src (a_minor)) {
         DEBUG_YSRC   yLOG_note    ("beginning of multi-key command");
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(sub-modes)-------------------*/
      if (strchr ("v\"rRiIaA", a_minor) != NULL) {
         rc = ysrc__source_subs (a_major, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(basic movement)--------------*/
      if (yKEYS_is_horz_simple (a_minor)) {
         rc = ysrc_move_simple (a_major, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(word movement)---------------*/
      if (yKEYS_is_horz_word   (a_minor)) {
         rc = ysrc_word_hmode (a_major, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(findchar)--------------------*/
      if (yKEYS_is_horz_char (a_minor)) {
         rc = ysrc__source_findchar (a_major, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(macro use)-------------------*/
      if (strchr ("DX¾¿|uU", a_minor) != NULL || a_minor == G_KEY_BS || a_minor == G_KEY_DEL) {
         rc = ysrc__source_editing (a_major, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(macro use)-------------------*/
      if (yKEYS_is_macro    (a_minor)) {
         rc = ysrc__source_macro (a_major, a_minor);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
   }
   /*---(multi-key)----------------------*/
   else {
      DEBUG_YSRC   yLOG_note    ("multikey handling");
      rc = ysrc__source_multikey (a_major, a_minor);
      if (a_major == 'c' && yKEYS_repeat_end ()) {
         DEBUG_YSRC   yLOG_note    ("enter input mode");
         rc = yMODE_enter (UMOD_INPUT);
         ysrc_sundo_chain ();
      }
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(complete)-----------------------*/
   if (rc < 0) {
      DEBUG_YSRC   yLOG_note    ("return code is negative");
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}


