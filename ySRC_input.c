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
   int         x_repeats   =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT   yLOG_note    ("mark insert/append position and save existing");
   /*---(wipe)---------------------------*/
   ysrc__input_wipe ();
   /*---(save key information)-----------*/
   s_mode     = yKEYS_current ();
   DEBUG_EDIT   yLOG_char    ("s_mode"    , s_mode);
   s_curr     = yMODE_curr ();
   DEBUG_EDIT   yLOG_char    ("s_curr"    , s_curr);
   s_prev     = yMODE_prev ();
   DEBUG_EDIT   yLOG_char    ("s_prev"    , s_prev);
   switch (s_mode) {
   case  '=' : case  '#' : case  '+' : case  '-' :
   case  'A' : case  ':' : case  '/' : case  ';' :
      s_mode = 'A';
      s_cur->cpos = s_cur->npos - 1;
   case  'a' :
      s_dir = 'a';
      break;
   case  'I' :
      s_cur->cpos = 0;
   case  'i' :
      s_dir = 'i';
      break;
   }
   DEBUG_EDIT   yLOG_char    ("s_mode"    , s_mode);
   DEBUG_EDIT   yLOG_value   ("npos"      , s_cur->npos);
   DEBUG_EDIT   yLOG_value   ("cpos"      , s_cur->cpos);
   DEBUG_EDIT   yLOG_char    ("s_dir"     , s_dir);
   /*---(check repeats)------------------*/
   x_repeats = yKEYS_repeats ();
   yKEYS_repeat_reset ();   /* no repeats allowed */
   --rce;  if (x_repeats > 0) {
      yKEYS_lock (); /* lock so following keys do not cause trouble */
      ysrc__input_wipe ();
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add placeholder)----------------*/
   if (s_dir == 'a')    ysrc_append_one (G_CHAR_PLACE);
   else                 ysrc_insert_one (G_CHAR_PLACE);
   DEBUG_EDIT   yLOG_complex ("contents"  , "%2dn %2dc å%sæ", s_cur->npos, s_cur->cpos, s_cur->contents);
   /*---(ready for use)------------------*/
   UPDATE_AFTER_CHANGES;
   ysrc_sundo_beg ();
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
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
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   switch (a_minor) {
   case G_KEY_ESCAPE : case G_KEY_RETURN :
      /*---(get rid of placeholder)------*/
      DEBUG_EDIT   yLOG_note    ("escape/return, return to source mode");
      rc = ysrc_delete_one ();
      if (s_dir == 'a' && s_cur->cpos < s_cur->npos - 1)  --s_cur->cpos;
      ysrc_sundo_end ();
      yMODE_exit     ();
      DEBUG_EDIT   yLOG_value   ("mode"     , yMODE_curr ());
      if (a_minor == G_KEY_RETURN && strchr (MODES_ONELINE, yMODE_curr ()) != NULL) {
         DEBUG_EDIT   yLOG_note    ("fast path back to map mode");
         ySRC_mode (G_KEY_SPACE, G_KEY_RETURN);
      }
      ysrc__input_wipe ();
      UPDATE_AFTER_CHANGES;
      break;
   default        :
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ysrc__input_escaped         (uchar a_major, uchar a_minor)
{
   /*---(normal)-------------------------*/
   if (s_escaping == G_CHAR_SPACE) {
      if (a_minor == '\\') {
         DEBUG_EDIT   yLOG_note    ("found a leading backslash");
         s_escaping = G_CHAR_UP;
         return 1;
      }
      return 0;
   }
   /*---(backslashed)--------------------*/
   if (s_escaping == G_CHAR_UP) {
      if (a_minor == '_') {
         DEBUG_EDIT   yLOG_note    ("found a leading backslash/underscore");
         s_escaping = G_CHAR_RIGHT;
         return 2;
      }
      DEBUG_EDIT   yLOG_note    ("converting backslash character");
      ysrc_input__add (chrslashed (a_minor));
      s_escaping = G_CHAR_SPACE;
      return 3;
   }
   /*---(backslash/more)-----------------*/
   if (s_escaping == G_CHAR_RIGHT) {
      DEBUG_EDIT   yLOG_note    ("converting backslash/underscore");
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
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT   yLOG_complex ("positions" , "%3dn, %3dc", s_cur->npos, s_cur->cpos);
   /*---(delete/clear)----------------*/
   switch (a_minor) {
   case  G_KEY_DEL : case  G_CHAR_DEL :
      DEBUG_EDIT   yLOG_note    ("delete handler");
      if (s_cur->cpos < s_cur->npos - 1) {
         ++s_cur->cpos;
         ysrc_sundo_add ('d', 'l', s_cur->cpos - 1, s_cur->contents [s_cur->cpos], G_KEY_NULL);
         rc = ysrc_delete_one ();
         --s_cur->cpos;
         UPDATE_AFTER_CHANGES;
         if (rc >= 0)  rc = 1;
      } else {
         DEBUG_EDIT   yLOG_note    ("delete rejected at end of string");
         rc = -11;
      }
      break;
   case  G_KEY_BS  : case  G_CHAR_BS  :
      if (s_cur->cpos > 0) {
         DEBUG_EDIT   yLOG_note    ("backspace handler");
         --s_cur->cpos;
         ysrc_sundo_add ('d', 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_KEY_NULL);
         rc = ysrc_delete_one ();
         UPDATE_AFTER_CHANGES;
         if (rc >= 0)  rc = 1;
      } else {
         DEBUG_EDIT   yLOG_note    ("backspace rejected at beginning of string");
         rc = -12;
      }
      break;
   default   :
      rc = 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc_input__add         (uchar a_minor)
{
   DEBUG_EDIT   yLOG_note    ("add and move remaining chars to the right");
   a_minor = chrvisible (a_minor);
   ysrc_sundo_single (s_dir, s_cur->cpos, G_CHAR_NULL, a_minor);
   ysrc_insert_one   (a_minor);
   ++s_cur->cpos;
   DEBUG_EDIT   yLOG_complex ("contents"  , "%2dn %2dc å%sæ", s_cur->npos, s_cur->cpos, s_cur->contents);
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
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT   yLOG_char    ("a_major"   , a_major);
   DEBUG_EDIT   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   DEBUG_EDIT   yLOG_char    ("mode"      , s_quoting);
   /*---(defenses)-----------------------*/
   DEBUG_EDIT   yLOG_char    ("mode"      , yMODE_curr ());
   --rce;  if (yMODE_not (UMOD_INPUT)) {
      DEBUG_EDIT   yLOG_note    ("not the correct mode");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   s_last  = s_minor;
   s_major = a_major;
   s_minor = a_minor;
   UPDATE_BEFORE_CHANGES;
   /*---(universal)----------------------*/
   rc = ysrc__input_biggies (a_major, a_minor);
   DEBUG_EDIT   yLOG_value   ("biggies"   , rc);
   if (rc > 0) {
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(escaped chars)------------------*/
   rc = ysrc__input_escaped (a_major, a_minor);
   DEBUG_EDIT   yLOG_value   ("escaped"   , rc);
   if (rc > 0) {
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(editing)------------------------*/
   rc = ysrc__input_editing (a_major, a_minor);
   if (rc != 0) {
      if (rc > 0)  rc = 0;
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(wander mode)--------------------*/
   else if (a_minor == G_KEY_BTICK) {
      rc = yMODE_enter (UMOD_WANDER);
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(normal text)--------------------*/
   if (a_minor >= 32 && a_minor != 127) {
      if (a_minor == G_KEY_SPACE)  a_minor = G_CHAR_STORAGE;
      DEBUG_EDIT   yLOG_note    ("handle normal keys");
      rc = ysrc_input__add (a_minor);
   }
   /*---(check for history)--------------*/
   /*> s_curr = yMODE_prev ();                                                                       <* 
    *> sprintf (x_history, "%c%c%c¤"  , s_curr, s_curr, s_curr);                                     <* 
    *> if (strcmp (s_cur->contents, x_history) == 0) {                                               <* 
    *>    rc = yvikeys_src_one_delete ();                                                            <* 
    *>    --s_cur->cpos;                                                                             <* 
    *>    yvikeys_sundo_end (__FUNCTION__);                                                          <* 
    *>    UPDATE_AFTER_CHANGES;                                                                      <* 
    *>    yMODE_exit ();                                                                             <* 
    *>    /+> if (s_curr == MODE_SEARCH)  strlcpy (s_cur->contents, "/", LEN_RECD);   <*             <* 
    *>     *> else                        strlcpy (s_cur->contents, ":", LEN_RECD);   <+/            <* 
    *>    /+> UPDATE_AFTER_CHANGES;                                                            <+/   <* 
    *>    yvikeys_hist__bounds ();                                                                   <* 
    *>    yMODE_enter  (UMOD_HISTORY);                                                               <* 
    *>    myVIKEYS.cursor = '-';                                                                     <* 
    *>    DEBUG_EDIT   yLOG_note    ("change to history mode");                                      <* 
    *> }                                                                                             <*/
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
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


