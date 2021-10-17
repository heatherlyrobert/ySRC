/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"

/*
 *   replace can not change the length of content, except to append at end
 *
 *   is has two variations -- 'r' for single char and 'R' for string
 *
 *   'r' exits automatically after one character is replaced
 *
 *   'R' must be specifically exited with ¥ to calling mode
 *
 *   when called from source mode, ¦ goes all the war by to source's parent
 *
 *   it can handle µ and µ_ backspace escapes in either variation
 *
 *   'R' allows left and right movement with backspace and delete (no change)
 *
 *   'r' needs to handle repeat (not append though), but 'R' never repeats
 *
 *
 *
 */

static  uchar  s_mode      = '-';           /* r=single, R=multiple           */
static  uchar  s_major     = '-';           /* major key                      */
static  uchar  s_minor     = '-';           /* minor key                      */
static  uchar  s_last      = '-';           /* last minor/key pressed         */
static  uchar  s_save      = '\0';          /* character being replaced       */
static  uchar  s_escaping  = G_CHAR_SPACE;  /* in backslash/escape handling   */
static  int    s_repeats   =   0;

char
ysrc_replace_init       (void)
{
   ysrc__replace_wipe ();
   return 0;
}


char
ysrc__replace_wipe      (void)
{
   s_mode      = '-';
   s_major     = '-';
   s_minor     = '-';
   s_last      = G_KEY_SPACE;
   s_save      = '\0';
   s_escaping  = G_CHAR_SPACE;
   s_repeats   =   0;
   return 0;
}

char
ysrc__replace_place     (void)
{
   char        rc          =    0;
   if (s_cur->cpos < 0)                { rc = -1;  s_cur->cpos = 0; }
   /*> if (s_cur->cpos >  s_cur->npos)     { rc = -2;  s_cur->cpos = s_cur->npos; }   <*/
   s_save = s_cur->contents [s_cur->cpos];
   if (s_cur->cpos < s_cur->npos) {
      ysrc_replace_one (G_CHAR_PLACE);
   } else {
      --s_cur->cpos;
      ysrc_append_one  (G_CHAR_PLACE);
   }
   if (s_save == '\0')  s_cur->contents [s_cur->cpos + 1] = '\0';
   return rc;
}

char
ysrc_replace_prepper    (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_note    ("mark replacement position and save existing");
   /*---(wipe)---------------------------*/
   ysrc__replace_wipe ();
   /*---(save key information)-----------*/
   s_mode   = yKEYS_current ();
   if (s_mode == 'r')  s_repeats = yKEYS_repeats ();
   yKEYS_repeat_reset ();
   /*---(show placeholder)---------------*/
   ysrc__replace_place ();
   /*---(ready for use)------------------*/
   ysrc_after     ();
   ysrc_sundo_beg ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc__replace_biggies   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   int         c           =    0;
   int         i           =    0;
   uchar       x_ch        = '\0';
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   switch (a_minor) {
   case G_KEY_ESCAPE : case G_KEY_RETURN :
      /*---(get rid of placeholder)------*/
      DEBUG_USER   yLOG_note    ("escape/return, return to source mode");
      if (s_cur->contents [s_cur->cpos] == G_CHAR_PLACE)  ysrc_replace_one (s_save);
      /*---(handle repeats)--------------*/
      c = yKEYS_repeats ();
      DEBUG_USER   yLOG_value   ("repeats"   , s_repeats);
      if (s_mode == 'r') {
         x_ch    = s_cur->contents [s_cur->cpos];
         DEBUG_USER   yLOG_char    ("x_ch"      , x_ch);
         for (i = 0; i < s_repeats; ++i) {
            ++s_cur->cpos;
            DEBUG_USER   yLOG_complex ("replace"   , "%2d, %2d, %c, %c", i, s_cur->cpos, s_cur->contents [s_cur->cpos], x_ch);
            ysrc_sundo_single ('r', s_cur->cpos, s_cur->contents [s_cur->cpos], x_ch);
            ysrc_replace_one  (x_ch);
         }
      }
      yMODE_exit     ();
      ysrc_sundo_end ();
      DEBUG_USER   yLOG_value   ("mode"     , yMODE_curr ());
      if (a_minor == G_KEY_RETURN && strchr (MODES_ONELINE, yMODE_curr ()) != NULL) {
         DEBUG_USER   yLOG_note    ("fast path back to map mode");
         ysrc_source_mode (G_KEY_SPACE, G_KEY_RETURN);
      }
      ysrc__replace_wipe ();
      ysrc_after  ();
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
ysrc__replace_escaped       (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(quick out)----------------------*/
   /*> if (a_major != G_KEY_SPACE)  return 0;                                         <*/
   /*---(normal)-------------------------*/
   if (s_escaping == G_CHAR_SPACE) {
      if (a_minor == '\\') {
         DEBUG_USER   yLOG_note    ("found a leading backslash");
         s_escaping = G_CHAR_UP;
         rc = 1;
      }
   }
   /*---(backslashed)--------------------*/
   else if (s_escaping == G_CHAR_UP) {
      if (a_minor == '_') {
         DEBUG_USER   yLOG_note    ("found a leading backslash/underscore");
         s_escaping = G_CHAR_RIGHT;
         rc = 2;
      } else {
         DEBUG_USER   yLOG_note    ("converting backslash character");
         ysrc_sundo_single ('r', s_cur->cpos, s_save, chrslashed (a_minor));
         ysrc_replace_one (chrslashed (a_minor));
         s_escaping = G_CHAR_SPACE;
         rc = 3;
      }
   }
   /*---(backslash/more)-----------------*/
   else if (s_escaping == G_CHAR_RIGHT) {
      DEBUG_USER   yLOG_note    ("converting backslash/underscore");
      ysrc_sundo_single ('r', s_cur->cpos, s_save, chrslashed_more (a_minor));
      ysrc_replace_one (chrslashed_more (a_minor));
      s_escaping = G_CHAR_SPACE;
      rc = 4;
   }
   /*---(preparation)--------------------*/
   if (rc >= 3) {
      DEBUG_USER   yLOG_note    ("escape sequence completed");
      DEBUG_USER   yLOG_value   ("old cpos"  , s_cur->cpos);
      DEBUG_USER   yLOG_value   ("repeats"   , yKEYS_repeats ());
      if (s_mode == 'r') {
         DEBUG_USER   yLOG_note    ("character replace mode");
         /*> DEBUG_USER   yLOG_value   ("repeats"   , yKEYS_repeats ());              <*/
         /*> if (yKEYS_repeats () > 0) {                                              <* 
          *>    DEBUG_USER   yLOG_note    ("advance for next char replace");          <* 
          *>    ++s_cur->cpos;                                                        <* 
          *>    if (s_cur->cpos > s_cur->npos - 1)  rc = -2;                          <* 
          *>    ysrc__replace_place ();                                               <* 
          *> }                                                                        <*/
         ysrc__replace_biggies (G_KEY_SPACE, G_KEY_ESCAPE);
      } else {
         DEBUG_USER   yLOG_note    ("string replace mode");
         ++s_cur->cpos;
         ysrc__replace_place ();
      }
      DEBUG_USER   yLOG_value   ("new cpos"  , s_cur->cpos);
   }
   /*---(complete)-----------------------*/
   return rc;
}

char
ysrc__replace_editing   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   char        rc2         =    1;
   /*---(quick out)----------------------*/
   /*> if (a_major != G_KEY_SPACE)  return 0;                                         <*/
   if (s_mode  != 'R')          return 0;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(delete/clear)----------------*/
   switch (a_minor) {
   case  G_KEY_DEL : case  G_CHAR_DEL :
      if (s_cur->contents [s_cur->cpos] == G_CHAR_PLACE)  ysrc_replace_one (s_save);
      ++s_cur->cpos;
      if (s_cur->cpos > s_cur->npos - 1) { rc2 = -2; --s_cur->cpos; }
      rc = ysrc__replace_place ();
      if (rc >= 0)  rc = 1;
      if (rc2 < 0)  rc = rc2;
      break;
   case  G_KEY_BS  : case  G_CHAR_BS  :
      if (s_cur->contents [s_cur->cpos] == G_CHAR_PLACE)  ysrc_replace_one (s_save);
      --s_cur->cpos;
      rc = ysrc__replace_place ();
      if (rc >= 0)  rc = 1;
      break;
   default   :
      rc = 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*-> replace sub-mode -------------------[ ------ [ge.RG6.25#.E5]*/ /*-[02.0000.112.E]-*/ /*-[--.---.---.--]-*/
ysrc_replace_umode      (uchar a_major, uchar a_minor)
{  /*---(design notes)-------------------*/
   /*
    *   very limited sub-mode of SOURCE mode.  allows a user to "type over"
    *   existing text including past the end of the line.  there are no
    *   special keys except ESCAPE/RETURN to return to SOURCE mode.
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   static char x_saved     = '\0';
   static char x_prev      =  '-';
   static char x_mode      =  '-';
   uchar       x_ch        =  '·';
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_char    ("a_major"   , a_major);
   DEBUG_USER   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   /*---(defenses)-----------------------*/
   DEBUG_USER   yLOG_char    ("mode"      , yMODE_curr ());
   --rce;  if (yMODE_not (UMOD_REPLACE)) {
      DEBUG_USER   yLOG_note    ("not the correct mode");
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   ysrc_before ();
   /*---(universal)----------------------*/
   rc = ysrc__replace_biggies (a_major, a_minor);
   DEBUG_USER   yLOG_value   ("biggies"   , rc);
   if (rc > 0) {
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(escaped chars)------------------*/
   rc = ysrc__replace_escaped (a_major, a_minor);
   if (rc > 0) {
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(editing)------------------------*/
   rc = ysrc__replace_editing (a_major, a_minor);
   if (rc != 0) {
      if (rc > 0)  rc = 0;
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(normal text)--------------------*/
   if (a_minor >= 32 && a_minor != 127) {
      if (a_minor == G_KEY_SPACE)  a_minor = G_CHAR_STORAGE;
      ysrc_sundo_single ('r', s_cur->cpos, s_save, a_minor);
      ysrc_replace_one  (a_minor);
   } else {
      ysrc_replace_one  (s_save);
   }
   /*---(finish)-------------------------*/
   if (s_mode == 'r') {
      ysrc__replace_biggies (G_KEY_SPACE, G_KEY_ESCAPE);
      rc = 0;
   } else {
      ++s_cur->cpos;
      ysrc__replace_place ();
      rc = 0;
   }
   /*---(wrap up)------------------------*/
   ysrc_after  ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char
ysrc__replace_unit      (char *a_text)
{
   snprintf (a_text, LEN_FULL, "%c   %c %c %c   %c %c", s_mode, s_major, s_minor, chrvisible (s_last), chrvisible (s_save), s_escaping);
   return 0;
}


