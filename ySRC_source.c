/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


tEDIT   s_src;
tEDIT   s_cmd;
tEDIT  *s_cur    = &s_src;

int   ysrc_cpos        (void) { return s_cur->cpos; }
int   ysrc_npos        (void) { return s_cur->npos; }
char *ysrc_contents    (void) { return s_cur->contents; }

char
ysrc__source_biggies    (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(quick out)----------------------*/
   if (a_major != G_KEY_SPACE)  return 0;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(major keys)---------------------*/
   switch (a_minor) {
   case G_KEY_SPACE  :
      DEBUG_USER   yLOG_note    ("space, nothing to do");
      break;
   case G_KEY_ESCAPE :
      if (ysrc_select_islive ()) {
         DEBUG_USER   yLOG_note    ("escape, means get out of selection");
         ysrc_select_reset (s_cur->cpos);
      } else {
         DEBUG_USER   yLOG_note    ("escape, means forget and return to previous mode");
         ysrc_reset  ();
         ysrc_after  ();
         yMODE_exit  ();
      }
      break;
   case G_KEY_RETURN :
      DEBUG_USER   yLOG_note    ("enter, means save and return to previous mode");
      ysrc_accept ();
      ysrc_after  ();
      yMODE_exit  ();
      break;
   case G_KEY_BTICK :
      DEBUG_USER   yLOG_note    ("backtick, means re-wander or move label");
      /*> yvikeys_map_wander_prep ();                                                 <*/
      rc = yMODE_enter (UMOD_WANDER);
      if (rc < 0) {
         DEBUG_USER   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      break;
   case  '\0'     :
      break;
   default        :
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
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
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(macros modes)-------------------*/
   switch (a_minor) {
   case '@'      :
      DEBUG_USER   yLOG_note    ("macro execution");
      yMODE_enter  (SMOD_MACRO   );
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return a_minor;
      break;
   case 'q'      :
      IF_MACRO_RECORDING {
         DEBUG_USER   yLOG_note    ("end macro recording");
         rc = yMACRO_rec_end ();
      } else {
         DEBUG_USER   yLOG_note    ("begin macro recording");
         yMODE_enter  (SMOD_MACRO   );
         rc = a_minor;
      }
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'Q'      :
      DEBUG_USER   yLOG_note    ("reset macro recording");
      rc = yMACRO_reset_all ();
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
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
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(sub modes)----------------------*/
   switch (a_minor) {
   case  'v' :
      if (ysrc_select_islive ()) s_cur->cpos = ysrc_select_reverse  ();
      else                       ysrc_select_makelive ();
      rc = 0;
      break;
   case  '"' :
      DEBUG_USER   yLOG_note    ("switch to a text register mode (¶)");
      yMODE_enter (SMOD_SREG);
      rc = a_minor;
      break;
   case  'r' :
      DEBUG_USER   yLOG_note    ("enter replace mode");
      yMODE_enter (UMOD_REPLACE);
      rc = a_minor;
      /*> if (yKEYS_repeats () == 0) {                                                          <* 
       *>    /+> SRC_REPL_umode ('m', a_minor);                                           <+/   <* 
       *>    rc = 0;                                                                            <* 
       *> } else {                                                                              <* 
       *>    rc = a_minor;                                                                      <* 
       *> }                                                                                     <*/
      break;
   case  'R' :
      DEBUG_USER   yLOG_note    ("enter replace mode");
      yMODE_enter (UMOD_REPLACE);
      /*> SRC_REPL_umode ('m', a_minor);                                              <*/
      /*> rc = 0;                                                                     <*/
      rc = a_minor;
      break;
   case  'i' : case  'a' : case  'I' : case  'A' :
      DEBUG_USER   yLOG_note    ("enter input mode");
      if (a_minor == 'A')  s_cur->cpos = s_cur->npos - 1;
      if (a_minor == 'I')  s_cur->cpos = 0;
      yMODE_enter (UMOD_INPUT);
      /*> SRC_INPT_umode ('m', tolower (a_minor));                                    <*/
      rc = tolower (a_minor);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc__source_findchar   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
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
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(single keys)--------------------*/
   if (a_major != G_KEY_SPACE) {
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   switch (a_minor) {
   case  '#' :
      DEBUG_USER   yLOG_note    ("current char find");
      rc = ysrc_move_char_next (1);
      break;
   case  'n' :
      DEBUG_USER   yLOG_note    ("char find next");
      rc = ysrc_move_char_next (0);
      break;
   case  'N' :
      DEBUG_USER   yLOG_note    ("char find reverse");
      rc = ysrc_move_char_prev (0);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
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
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(delete/clear)----------------*/
   switch (a_minor) {
   case  G_KEY_DEL : case  G_CHAR_DEL : case  'D' :
      rc = ysrc_multi_pure ('d', 'l');
      if (rc >= 0)  rc = 0;
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case  G_KEY_BS  : case  G_CHAR_BS  : case  'X' :
      rc = ysrc_multi_pure ('d', 'h');
      if (rc >= 0)  rc = 0;
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(other stuff)-----------------*/
   switch (a_minor) {
   case '|' :
      DEBUG_USER   yLOG_value   ("repeats"   , yKEYS_repeats);
      s_cur->cpos = yKEYS_repeat_useall ();
      DEBUG_USER   yLOG_value   ("cpos"      , s_cur->cpos);
      rc     = 0;
      ysrc_after ();
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'u' :
      ysrc_select_reset (s_cur->cpos);
      rc = ysrc_sundo_undo (&(s_cur->cpos));
      ysrc_after ();
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'U' :
      ysrc_select_reset (s_cur->cpos);
      rc = ysrc_sundo_redo (&(s_cur->cpos));
      ysrc_after ();
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc__source_multikey   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(quick out)----------------------*/
   if (a_major == G_KEY_SPACE)  return 0;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(multi-key)----------------------*/
   switch (a_major) {
   case 'd' : case 'x' :
      rc = ysrc_multi_pure (a_major, a_minor);
      break;
   case 'g' :
      rc = ysrc_move_goto    (a_major, a_minor);
      break;
   case 'z' :
      rc = ysrc_move_scroll  (a_major, a_minor);
      break;
   case 'f' : case 'F' :
      rc = ysrc__source_findchar (a_major, a_minor);
      break;
   case 'c' :
      /*> n = yKEYS_repeat_useall ();                                              <* 
       *> for (i = 0; i <= n; ++i) {                                               <* 
       *>    ysrc_multi_pure  ('d', a_minor);                                      <* 
       *> }                                                                        <* 
       *> yMODE_enter (UMOD_INPUT);                                                <* 
       *> SRC_INPT_umode ('m', tolower (a_minor));                                 <* 
       *> rc = tolower (a_minor);                                                  <* 
       *> DEBUG_USER   yLOG_exit    (__FUNCTION__);                                <* 
       *> return rc;                                                               <* 
       *> break;                                                                   <*/
   default  :
      rc = -1;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*-> process keys for cell edit mode ----[ leaf   [ge.#R6.23#.HG]*/ /*-[05.0000.102.#]-*/ /*-[--.---.---.--]-*/
ysrc_source_mode        (uchar a_major, uchar a_minor)
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
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_char    ("a_major"   , a_major);
   DEBUG_USER   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   /*---(defenses)-----------------------*/
   DEBUG_USER   yLOG_char    ("mode"      , yMODE_curr ());
   DEBUG_USER   yLOG_info    ("valid"     , MODES_ONELINE);
   --rce;  if (strchr (MODES_ONELINE, yMODE_curr ()) == NULL ) {
      DEBUG_USER   yLOG_note    ("not an acceptable mode");
      yMODE_exit  ();
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(universal)----------------------*/
   rc = ysrc__source_biggies (a_major, a_minor);
   DEBUG_USER   yLOG_value   ("biggies"   , rc);
   if (rc > 0) {
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(single letter)------------------*/
   if (a_major == G_KEY_SPACE) {
      /*---(repeats)---------------------*/
      if (yKEYS_is_repeater (a_minor)) {
         DEBUG_USER   yLOG_note    ("repeat character 1-9");
         yMODE_enter  (PMOD_REPEAT);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(select related)--------------*/
      /*> if (strchr ("yYpP", a_minor) != 0) {                                        <* 
       *>    DEBUG_USER   yLOG_note    ("switch to a text register mode (yYpP)");     <* 
       *>    yvikeys_sreg_setreg ('"');                                               <* 
       *>    yMODE_enter (SMOD_SREG);                                                 <* 
       *>    rc = yvikeys_sreg_smode (G_KEY_SPACE, a_minor);                          <* 
       *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                <* 
       *>    return rc;                                                               <* 
       *> }                                                                           <*/
      /*> if (yvikeys_sreg_islive () && strchr ("xXdD", a_minor) != 0) {              <* 
       *>    DEBUG_USER   yLOG_note    ("switch to a text register mode (xXdD)");     <* 
       *>    yvikeys_sreg_setreg ('"');                                               <* 
       *>    yMODE_enter (SMOD_SREG);                                                 <* 
       *>    rc = yvikeys_sreg_smode (G_KEY_SPACE, a_minor);                          <* 
       *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                <* 
       *>    return rc;                                                               <* 
       *> }                                                                           <*/
      /*---(multikey prefixes)-----------*/
      if (yKEYS_is_multisrc (a_minor)) {
         DEBUG_USER   yLOG_note    ("beginning of multi-key command");
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(sub-modes)-------------------*/
      if (strchr ("v\"rRiIaA", a_minor) != NULL) {
         rc = ysrc__source_subs (a_major, a_minor);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(basic movement)--------------*/
      if (yKEYS_is_horz_simple (a_minor)) {
         rc = ysrc_move_simple (a_major, a_minor);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(word movement)---------------*/
      if (yKEYS_is_horz_word   (a_minor)) {
         rc = ysrc_word_hmode (a_major, a_minor);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(findchar)--------------------*/
      if (yKEYS_is_findchar (a_minor)) {
         rc = ysrc__source_findchar (a_major, a_minor);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(macro use)-------------------*/
      if (strchr ("DX¾¿|uU", a_minor) != NULL || a_minor == G_KEY_BS || a_minor == G_KEY_DEL) {
         rc = ysrc__source_editing (a_major, a_minor);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(macro use)-------------------*/
      if (yKEYS_is_macro    (a_minor)) {
         rc = ysrc__source_macro (a_major, a_minor);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return rc;
      }
   }
   /*---(multi-key)----------------------*/
   else {
      DEBUG_USER   yLOG_note    ("multikey handling");
      rc = ysrc__source_multikey (a_major, a_minor);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(complete)-----------------------*/
   if (rc < 0) {
      DEBUG_USER   yLOG_note    ("return code is negative");
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}


