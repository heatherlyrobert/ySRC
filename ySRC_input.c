/*============================---beg-of-source---=============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



static  uchar  s_mode      = '-';           /* detailed A, a, I, i            */
static  uchar  s_dir       = '-';           /* a=append, i=input, -=unknown   */
static  uchar  s_curr      = '-';           /* source, command, search, hint  */
static  uchar  s_prev      = '-';           /* mode before                    */
static  uchar  s_major     = '-';           /* major key                      */
static  uchar  s_minor     = '-';           /* minor key                      */
static  uchar  s_last      = '-';           /* last keyy                      */
static  uchar  s_escaping  = G_CHAR_SPACE;  /* in backslash/escape handling   */
static  uchar  s_quoting   = G_CHAR_SPACE;  /* inside quotes                  */

/*
 *  missing backslash 
 *
 *
 *  \a  alt
 *  \c  control
 *  \m
 *  \t  tab
 *  \u  undo micro mode
 *  \w  wander (also `)
 *  \J  join lines
 *  \V
 *  \;
 *  \=
 *  \^  
 *  
 * 
 */



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
ysrc__input_wipe        (void)
{
   s_mode      = '-';
   s_curr      = '-';
   s_dir       = '-';
   s_major     = '-';
   s_minor     = '-';
   s_last      = '-';
   s_escaping  = G_CHAR_SPACE;
   s_quoting   = G_CHAR_SPACE;
   return 0;
}

char
ysrc_input_init         (void)
{
   ysrc__input_wipe   ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     worker functions                         ----===*/
/*====================------------------------------------====================*/
static void  o___WORKERS_________o () { return; }

char
ysrc_input_prepper      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_mode      =    0;
   uchar       x_latest    =    0;
   uchar       x_multi     =    0;
   uchar       x_menu      =    0;
   int         x_repeats   =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_note    ("mark insert/append position and save existing");
   /*---(wipe)---------------------------*/
   ysrc__input_wipe ();
   /*---(real modes)---------------------*/
   s_curr     = yMODE_curr ();
   DEBUG_YSRC   yLOG_char    ("s_curr"    , s_curr);
   s_prev     = yMODE_prev ();
   DEBUG_YSRC   yLOG_char    ("s_prev"    , s_prev);
   /*---(save key information)-----------*/
   yKEYS_every_current (&x_mode, &x_latest, &x_multi, &x_menu, NULL, NULL);
   DEBUG_YSRC   yLOG_char    ("x_mode"    , x_mode);
   DEBUG_YSRC   yLOG_char    ("x_latest"  , x_latest);
   DEBUG_YSRC   yLOG_char    ("x_multi"   , x_multi);
   DEBUG_YSRC   yLOG_char    ("x_menu"    , x_menu);
   s_mode     = x_latest;
   if (x_menu)  s_mode = '\\';
   if (x_multi == 'c')  s_mode = 'c';
   DEBUG_YSRC   yLOG_char    ("s_mode"    , s_mode);
   /*---(mode overrides)-----------------*/
   switch (s_curr) {
   case MODE_COMMAND :
      DEBUG_YSRC   yLOG_note    ("command input");
      s_mode = 'A';
      break;
   case MODE_SEARCH  :
      DEBUG_YSRC   yLOG_note    ("search input");
      if (yMODE_exited () != UMOD_HISTORY)  s_mode = 'A';
      break;
   case SMOD_HINT    :
      DEBUG_YSRC   yLOG_note    ("hint input");
      s_mode = 'A';
      break;
   }
   DEBUG_YSRC   yLOG_char    ("s_mode(2)" , s_mode);
   /*---(set start)----------------------*/
   switch (s_mode) {
   case  '=' : case  '#' : case  '+' : case  '-' :
   case  'A' : case  '\\' : case G_CHAR_RETURN :
      DEBUG_YSRC   yLOG_note    ("big append mode");
      s_mode = 'A';
      s_cur->cpos = s_cur->npos - 1;
   case  'a' :
      DEBUG_YSRC   yLOG_note    ("little append mode");
      s_dir = 'a';
      if (s_cur->cpos < 0)  s_cur->cpos = 0;
      break;
   case  'I' :
      s_cur->cpos = 0;
      DEBUG_YSRC   yLOG_note    ("big input mode");
   case  'i' :
      DEBUG_YSRC   yLOG_note    ("little input mode");
      s_dir = 'i';
      break;
   case  'c' :
      DEBUG_YSRC   yLOG_note    ("change input mode");
      if       (s_cur->cpos >= s_cur->npos - 1)        s_dir = 'a';
      else if  (s_cur->cpos == 0)                      s_dir = 'i';
      else if  (x_multi != 'c')                        s_dir = 'i';
      else if  (strchr ("$lLwWeE", x_latest) != NULL)  s_dir = 'i';
      else                                             s_dir = 'a';
      break;
   }
   DEBUG_YSRC   yLOG_char    ("s_mode"    , s_mode);
   DEBUG_YSRC   yLOG_value   ("npos"      , s_cur->npos);
   DEBUG_YSRC   yLOG_value   ("cpos"      , s_cur->cpos);
   DEBUG_YSRC   yLOG_char    ("s_dir"     , s_dir);
   /*---(check repeats)------------------*/
   x_repeats = yKEYS_repeats ();
   yKEYS_repeat_reset ();   /* no repeats allowed */
   --rce;  if (x_repeats > 0) {
      yKEYS_lock (); /* lock so following keys do not cause trouble */
      ysrc__input_wipe ();
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add placeholder)----------------*/
   DEBUG_YSRC   yLOG_complex ("contents"  , "%2dn %2dc å%sæ", s_cur->npos, s_cur->cpos, s_cur->contents);
   if (s_dir == 'a')    ysrc_append_one (G_CHAR_PLACE);
   else                 ysrc_insert_one (G_CHAR_PLACE);
   DEBUG_YSRC   yLOG_complex ("contents"  , "%2dn %2dc å%sæ", s_cur->npos, s_cur->cpos, s_cur->contents);
   /*---(ready for use)------------------*/
   UPDATE_AFTER_CHANGES;
   ysrc_sundo_beg ();
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc__input_biggies     (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   int         c           =    0;
   int         i           =    0;
   uchar       x_ch        = '\0';
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_char    ("a_major"   , a_major);
   DEBUG_YSRC   yLOG_char    ("a_minor"   , a_minor);
   DEBUG_YSRC   yLOG_value   ("a_minor"   , a_minor);
   /*---(handler)------------------------*/
   switch (a_minor) {
   case G_KEY_ESCAPE : case G_KEY_RETURN :
      /*---(get rid of placeholder)------*/
      DEBUG_YSRC   yLOG_note    ("escape/return, return to source mode");
      rc = ysrc_delete_one ();
      if (s_dir == 'a' && s_cur->cpos < s_cur->npos - 1)  --s_cur->cpos;
      ysrc_sundo_end ();
      s_escaping = G_KEY_SPACE;
      yMODE_exit     ();
      DEBUG_YSRC   yLOG_value   ("mode"     , yMODE_curr ());
      if (a_minor == G_KEY_RETURN && strchr (MODES_ONELINE, yMODE_curr ()) != NULL) {
         DEBUG_YSRC   yLOG_note    ("fast path back to map mode");
         ySRC_mode (G_KEY_SPACE, G_KEY_RETURN);
      }
      ysrc__input_wipe ();
      UPDATE_AFTER_CHANGES;
      break;
   default        :
      DEBUG_YSRC   yLOG_note    ("character not a biggie");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysrc__input_escaped         (uchar a_major, uchar a_minor)
{
   /*---(normal)-------------------------*/
   if (s_escaping == G_CHAR_SPACE) {
      if (a_minor == '\\') {
         DEBUG_YSRC   yLOG_note    ("found a leading backslash");
         s_escaping = G_CHAR_UP;
         return 1;
      }
      return 0;
   }
   /*---(backslashed)--------------------*/
   if (s_escaping == G_CHAR_UP) {
      if (a_minor == '_') {
         DEBUG_YSRC   yLOG_note    ("found a leading backslash/underscore");
         s_escaping = G_CHAR_RIGHT;
         return 2;
      }
      DEBUG_YSRC   yLOG_note    ("converting backslash character");
      ysrc_input__add (chrslashed (a_minor));
      s_escaping = G_CHAR_SPACE;
      return 3;
   }
   /*---(backslash/more)-----------------*/
   if (s_escaping == G_CHAR_RIGHT) {
      DEBUG_YSRC   yLOG_note    ("converting backslash/underscore");
      ysrc_input__add (chrslashed_more (a_minor));
      s_escaping = G_CHAR_SPACE;
      return 4;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc__input_editing     (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   char        rc2         =    1;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_complex ("positions" , "%3dn, %3dc", s_cur->npos, s_cur->cpos);
   /*---(delete/clear)----------------*/
   switch (a_minor) {
   case  G_KEY_DEL : case  G_CHAR_DEL :
      DEBUG_YSRC   yLOG_note    ("delete handler");
      if (s_cur->cpos < s_cur->npos - 1) {
         ++s_cur->cpos;
         ysrc_sundo_add ('d', 'l', s_cur->cpos - 1, s_cur->contents [s_cur->cpos], G_KEY_NULL);
         rc = ysrc_delete_one ();
         --s_cur->cpos;
         UPDATE_AFTER_CHANGES;
         if (rc >= 0)  rc = 1;
      } else {
         DEBUG_YSRC   yLOG_note    ("delete rejected at end of string");
         rc = -11;
      }
      break;
   case  G_KEY_BS  : case  G_CHAR_BS  :
      if (s_cur->cpos > 0) {
         DEBUG_YSRC   yLOG_note    ("backspace handler");
         --s_cur->cpos;
         ysrc_sundo_add ('d', 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_KEY_NULL);
         rc = ysrc_delete_one ();
         UPDATE_AFTER_CHANGES;
         if (rc >= 0)  rc = 1;
      } else {
         DEBUG_YSRC   yLOG_note    ("backspace rejected at beginning of string");
         rc = -12;
      }
      break;
   default   :
      rc = 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc_input__add         (uchar a_minor)
{
   DEBUG_YSRC   yLOG_note    ("add and move remaining chars to the right");
   DEBUG_YSRC   yLOG_complex ("before"    , "%2dn %2dc å%sæ", s_cur->npos, s_cur->cpos, s_cur->contents);
   a_minor = chrvisible (a_minor);
   ysrc_sundo_single (s_dir, s_cur->cpos, G_CHAR_NULL, a_minor);
   ysrc_insert_one   (a_minor);
   ++s_cur->cpos;
   DEBUG_YSRC   yLOG_complex ("after"     , "%2dn %2dc å%sæ", s_cur->npos, s_cur->cpos, s_cur->contents);
   UPDATE_AFTER_CHANGES;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      mode handler                            ----===*/
/*====================------------------------------------====================*/
static void  o___MODE____________o () { return; }

char         /*-> process keys for input mode --------[ ------ [ge.TQ5.25#.F9]*/ /*-[03.0000.122.R]-*/ /*-[--.---.---.--]-*/
ysrc_input_umode           (uchar a_major, uchar a_minor)
{
   /*---(design notes)-------------------*/
   /*
    *   this should imitate a very basic vi-input mode by handling
    *   all characters, ignoring new line, and popping out with escape
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_history   [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_char    ("a_major"   , a_major);
   DEBUG_YSRC   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   DEBUG_YSRC   yLOG_char    ("s_mode"    , s_mode);
   DEBUG_YSRC   yLOG_char    ("s_dir"     , s_dir);
   DEBUG_YSRC   yLOG_char    ("s_quoting" , s_quoting);
   /*---(defenses)-----------------------*/
   DEBUG_YSRC   yLOG_char    ("mode"      , yMODE_curr ());
   --rce;  if (yMODE_not (UMOD_INPUT)) {
      DEBUG_YSRC   yLOG_note    ("not the correct mode");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   s_last  = s_minor;
   s_major = a_major;
   s_minor = a_minor;
   UPDATE_BEFORE_CHANGES;
   /*---(universal)----------------------*/
   rc = ysrc__input_biggies (a_major, a_minor);
   DEBUG_YSRC   yLOG_value   ("biggies"   , rc);
   if (rc > 0) {
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(escaped chars)------------------*/
   rc = ysrc__input_escaped (a_major, a_minor);
   DEBUG_YSRC   yLOG_value   ("escaped"   , rc);
   if (rc > 0) {
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(editing)------------------------*/
   rc = ysrc__input_editing (a_major, a_minor);
   if (rc != 0) {
      if (rc > 0)  rc = 0;
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(wander mode)--------------------*/
   else if (a_minor == G_KEY_BTICK) {
      DEBUG_YSRC   yLOG_note    ("jump to wander");
      rc = yMODE_enter (UMOD_WANDER);
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(normal text)--------------------*/
   if (a_minor >= 32 && a_minor != 127) {
      if (a_minor == G_KEY_SPACE)  a_minor = G_CHAR_STORAGE;
      DEBUG_YSRC   yLOG_note    ("handle normal keys");
      rc = ysrc_input__add (a_minor);
   }
   /*---(check for history)--------------*/
   s_curr = yMODE_prev ();
   sprintf (x_history, "%c%c%c¤"  , s_curr, s_curr, s_curr);
   DEBUG_YSRC   yLOG_info    ("sentinel"  , x_history);
   if (strcmp (s_cur->contents, x_history) == 0) {
      DEBUG_YSRC   yLOG_note    ("history requested");
      rc = ysrc_delete_one ();
      --s_cur->cpos;
      ysrc_sundo_end ();
      yMODE_exit ();
      if (s_curr == MODE_SEARCH)  strlcpy (s_cur->contents, "/", LEN_RECD);
      else                        strlcpy (s_cur->contents, ":", LEN_RECD);
      UPDATE_AFTER_CHANGES;
      /*> yvikeys_hist__bounds ();                                                    <*/
      DEBUG_YSRC   yLOG_note    ("change to history mode");
      yMODE_enter  (UMOD_HISTORY);
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char
ysrc__input_unit        (char *a_text)
{
   snprintf (a_text, LEN_FULL, "%c %c %c   %c %c %c   %c %c", s_mode, s_dir, s_curr, s_major, s_minor, s_last, s_escaping, s_quoting);
   return 0;
}


