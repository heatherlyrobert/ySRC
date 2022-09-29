/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


static char s_prev      [LEN_LABEL] = "";
static char s_curr      [LEN_LABEL] = "";


char
ysrc__wander_biggies    (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
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
      DEBUG_YSRC   yLOG_note    ("escape, means wandering thrown away");
      ysrc__wander_done       ('¥');
      break;
   case G_KEY_RETURN :
      DEBUG_YSRC   yLOG_note    ("enter, means wandering accepted");
      ysrc__wander_done       ('¦');
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
      DEBUG_YSRC   yLOG_note    ("null, nothing to do");
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
ysrc__wander_pos_show   (void)
{
   DEBUG_YSRC   yLOG_complex ("position"  , "%3db, %3dc, %3de, %3dn", s_cur->bpos, s_cur->cpos, s_cur->epos, s_cur->npos);
   return 0;
}

char
ysrc__wander_pos_set  (int a_cpos)
{
   s_cur->cpos = a_cpos;
   return 0;
}

char
ysrc__wander_save        (char *a_new)
{
   DEBUG_YSRC   yLOG_complex ("position"  , "%3db, %3dc, %3de, %3dn", s_cur->bpos, s_cur->cpos, s_cur->epos, s_cur->npos);
   strlcpy (s_cur->contents, a_new, LEN_RECD);
   s_cur->npos = strlen (s_cur->contents);
   ysrc_after ();
   DEBUG_YSRC   yLOG_complex ("position"  , "%3db, %3dc, %3de, %3dn", s_cur->bpos, s_cur->cpos, s_cur->epos, s_cur->npos);
   return 0;
}

char
ysrc__wander_update     (void)
{
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   s_cur->npos = strlen (s_cur->contents);
   DEBUG_YSRC   yLOG_value   ("npos"      , s_cur->npos);
   for (i = 0; i < LEN_RECD; ++i) {
      if (s_cur->contents [i] != G_CHAR_PLACE)  continue;
      s_cur->cpos = i;
      break;
   }
   DEBUG_YSRC   yLOG_value   ("cpos"      , s_cur->cpos);
   ysrc_after ();
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}





char
ysrc__wander_loc        (char *a_label, char *a_len)
{
   /*---(find new)-----------------------*/
   yMAP_current  (NULL, &(mySRC.w_univ), &(mySRC.w_xpos), &(mySRC.w_ypos), &(mySRC.w_zpos));
   str4gyges (mySRC.w_univ, mySRC.w_xpos, mySRC.w_ypos, mySRC.w_zpos, mySRC.w_abs, s_curr, YSTR_USABLE);
   DEBUG_YSRC   yLOG_complex ("current"   , "%-10.10s, %2dt, %3dx, %4dy, %4dz", s_curr, mySRC.w_univ, mySRC.w_xpos, mySRC.w_ypos, mySRC.w_zpos);
   /*---(save back)----------------------*/
   if (a_label != NULL)  strlcpy (a_label, s_curr, LEN_LABEL);
   if (a_len   != NULL)  *a_len = strlen (s_curr);
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_wander_prepper     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        rc          =    0;
   char        x_label     [LEN_LABEL] = "";
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZ®¯abcdefghijklmnopqrstuvwxyz0123456789$@";
   int         x_cpos      =    0;
   int         x_beg       =   -1;
   int         x_end       =   -1;
   int         x_len       =    0;
   ushort      u, x, y, z;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(get current)--------------------*/
   yMAP_current  (NULL, &(mySRC.w_uorig), &(mySRC.w_xorig), &(mySRC.w_yorig), &(mySRC.w_zorig));
   DEBUG_YSRC   yLOG_complex ("original"  , "%2dt, %3dx, %4dy, %4dz", mySRC.w_uorig, mySRC.w_xorig, mySRC.w_yorig, mySRC.w_zorig);
   yMAP_beg      (NULL, NULL           , &(mySRC.w_xbeg), &(mySRC.w_ybeg), &(mySRC.w_zbeg));
   DEBUG_YSRC   yLOG_complex ("begininng" , "%2dt, %3dx, %4dy, %4dz", mySRC.w_uorig, mySRC.w_xbeg, mySRC.w_ybeg, mySRC.w_zbeg);
   /*---(save contents)------------------*/
   strlcpy (mySRC.w_contents, ysrc_contents (), LEN_RECD);
   strcpy  (mySRC.w_pre   , "");
   strcpy  (mySRC.w_suf   , "");
   strcpy  (mySRC.w_new   , "");
   mySRC.w_abs = 0;
   /*---(divide existing input)----------*/
   if (yMODE_curr () == UMOD_INPUT) {
      DEBUG_YSRC   yLOG_note    ("already in input mode");
      for (i = 0; i < LEN_RECD; ++i) {
         if (mySRC.w_contents [i] != G_CHAR_PLACE)  continue;
         break;
      }
      mySRC.w_contents [i] = '\0';
      strlcpy (mySRC.w_pre   , mySRC.w_contents               , LEN_RECD);
      strlcpy (mySRC.w_suf   , mySRC.w_contents + i + 1       , LEN_RECD);
      mySRC.w_contents [i] = G_CHAR_PLACE;
   }
   /*---(identify current label)---------*/
   else if (yMODE_curr () == MODE_SOURCE) {
      DEBUG_YSRC   yLOG_note    ("coming from source, moving label");
      x_cpos = ysrc_cpos ();
      /*---(walk back)-------------------*/
      for (i = x_cpos; i >= 0; --i) {
         if (strchr (x_valid, mySRC.w_contents [i]) == NULL) break;
         x_beg = i;
      }
      DEBUG_YSRC   yLOG_value   ("x_beg"     , x_beg);
      if (x_beg < 0) {
         yKEYS_set_error ();
         DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(walk forward)----------------*/
      x_len = strlen (mySRC.w_contents);
      for (i = x_beg; i < x_len; ++i) {
         if (strchr (x_valid, mySRC.w_contents [i]) == NULL) break;
         x_end = i;
      }
      if (x_end < 0) {
         yKEYS_set_error ();
         DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(divide existing input)-------*/
      DEBUG_YSRC   yLOG_value   ("x_end"     , x_end);
      strlcpy (mySRC.w_pre   , mySRC.w_contents               , x_beg + 1);
      DEBUG_YSRC   yLOG_info    ("mySRC.w_pre"     , mySRC.w_pre);
      strlcpy (mySRC.w_suf   , mySRC.w_contents + x_end + 1   , LEN_RECD);
      DEBUG_YSRC   yLOG_info    ("mySRC.w_suf"     , mySRC.w_suf);
      strlcpy (x_label, mySRC.w_contents + x_beg, x_end - x_beg + 2);
      DEBUG_YSRC   yLOG_info    ("x_label"   , x_label);
      rc = str2gyges (x_label, &u, &x, &y, &z, NULL, 0, YSTR_USABLE);
      DEBUG_YSRC   yLOG_value   ("str2gyges" , rc);
      if (rc < 0) {
         yKEYS_set_error ();
         DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSRC   yLOG_complex ("x_label"   , "%2dt, %3dx, %4dy, %4dz", u, x, y, z);
      yMAP_jump (u, x, y, z);
      ysrc__wander_pos_set (strlen (mySRC.w_pre));
   }
   ysrc__wander_loc (x_label, NULL);
   strlcpy (s_prev, x_label, LEN_LABEL);
   sprintf (mySRC.w_new, "%s%s%s" , mySRC.w_pre, x_label, mySRC.w_suf);
   ysrc__wander_save    (mySRC.w_new);
   DEBUG_YSRC   yLOG_info    ("mySRC.w_contents"  , mySRC.w_contents);
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc__wander_return     (void)
{
   DEBUG_YSRC   yLOG_complex ("original"  , "%2dt, %3dx, %4dy, %4dz", mySRC.w_uorig, mySRC.w_xorig, mySRC.w_yorig, mySRC.w_zorig);
   yMAP_jump (mySRC.w_uorig, mySRC.w_xorig, mySRC.w_yorig, mySRC.w_zorig);
   return 0;
}

char
ysrc__wander_done       (uchar a_key)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_label     [LEN_LABEL] = "";
   char        l           =    0;
   int         i           =    0;
   int         x_prelen    =    0;
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   yMODE_exit  ();
   ysrc__wander_loc (x_label, &l);
   ysrc__wander_return    ();
   if (yMODE_curr () == MODE_SOURCE) sprintf (mySRC.w_new, "%s%s%s"   , mySRC.w_pre, x_label, mySRC.w_suf);
   else if (a_key == '(')            sprintf (mySRC.w_new, "%s(%c%s"  , mySRC.w_pre, G_CHAR_PLACE, mySRC.w_suf);
   else if (a_key == ')')            sprintf (mySRC.w_new, "%s%s)%c%s", mySRC.w_pre, x_label, G_CHAR_PLACE, mySRC.w_suf);
   else                              sprintf (mySRC.w_new, "%s%s%c%s" , mySRC.w_pre, x_label, G_CHAR_PLACE, mySRC.w_suf);
   DEBUG_YSRC   yLOG_info    ("w_new"     , mySRC.w_new);
   if (a_key == G_CHAR_ESCAPE) {
      DEBUG_YSRC   yLOG_note    ("escape mode, forget last change");
      ysrc__wander_save    (mySRC.w_contents);
   }
   else {
      DEBUG_YSRC   yLOG_note    ("return mode, save and update");
      DEBUG_YSRC   yLOG_note    ("handle sundo operations");
      DEBUG_YSRC   yLOG_value   ("l"         , l);
      ysrc_sundo_beg ();
      x_prelen = strlen (mySRC.w_pre);
      DEBUG_YSRC   yLOG_value   ("x_prelen"  , x_prelen);
      for (i = 0; i < l; ++i) {
         DEBUG_YSRC   yLOG_value   ("sundo"     , i);
         ysrc_sundo_single ('i', i + x_prelen, G_CHAR_NULL, x_label [i]);
      }
      ysrc_sundo_end ();
      ysrc__wander_save    (mySRC.w_new);
   }
   ysrc__wander_update  ();
   mySRC.w_abs = 0;
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc__wander_sundo      (char *a_add)
{
   /*---(locals)-----------+-----+-----+-*/
   char        l           =    0;
   int         x_prelen    =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(lengths)------------------------*/
   l = strlen (a_add);
   DEBUG_YSRC   yLOG_sint    (l);
   x_prelen = strlen (mySRC.w_pre);
   DEBUG_YSRC   yLOG_sint    (x_prelen);
   /*---(begin)--------------------------*/
   DEBUG_YSRC   yLOG_snote   ("beg");
   ysrc_sundo_beg ();
   /*---(sundo)--------------------------*/
   for (i = 0; i < l; ++i) {
      DEBUG_YSRC   yLOG_sint    (i);
      ysrc_sundo_single ('i', i + x_prelen, G_CHAR_NULL, a_add [i]);
   }
   /*---(end)----------------------------*/
   ysrc_sundo_end ();
   DEBUG_YSRC   yLOG_snote   ("end");
   /*---(prefix)-------------------------*/
   sprintf (mySRC.w_pre, "%s%s", mySRC.w_pre, a_add);
   /*---(lock in)------------------------*/
   sprintf (mySRC.w_contents, "%s%c%s" , mySRC.w_pre, G_CHAR_PLACE, mySRC.w_suf);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc__wander_and        (uchar a_math)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_label     [LEN_LABEL] = "";
   char        l           =    0;
   int         i           =    0;
   char        x_math      [LEN_TERSE] = "";
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(prepare operator)---------------*/
   if (a_math == ':')  strlcpy (x_math, "..", LEN_TERSE);
   else                sprintf (x_math, "%c", a_math);
   /*---(current location)---------------*/
   ysrc__wander_loc (x_label, &l);
   DEBUG_YSRC   yLOG_info    ("x_label"   , x_label);
   DEBUG_YSRC   yLOG_info    ("s_prev"    , s_prev);
   /*---(double operator)----------------*/
   if (strcmp (x_label, s_prev) == 0) {
      DEBUG_YSRC   yLOG_note    ("double operator");
      ysrc__wander_sundo   (x_math);
      sprintf (mySRC.w_new, "%s%s%s", mySRC.w_pre, x_label, mySRC.w_suf);
      DEBUG_YSRC   yLOG_info    ("w_new"     , mySRC.w_new);
      ysrc__wander_save    (mySRC.w_new);
      ysrc__wander_update  ();
      mySRC.w_abs = 0;
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(sundo label)--------------------*/
   DEBUG_YSRC   yLOG_note    ("handle sundo operations");
   ysrc__wander_sundo (x_label);
   /*> DEBUG_YSRC   yLOG_value   ("l"         , l);                                   <*/
   /*> ysrc_sundo_beg ();                                                             <* 
    *> x_prelen = strlen (mySRC.w_pre);                                               <* 
    *> DEBUG_YSRC   yLOG_value   ("x_prelen"  , x_prelen);                            <* 
    *> for (i = 0; i < l; ++i) {                                                      <* 
    *>    DEBUG_YSRC   yLOG_value   ("sundo"     , i);                                <* 
    *>    ysrc_sundo_single ('i', i + x_prelen, G_CHAR_NULL, x_label [i]);            <* 
    *> }                                                                              <* 
    *> ysrc_sundo_end ();                                                             <*/
   /*---(sundo operator)-----------------*/
   ysrc__wander_sundo (x_math);
   /*---(save and update)----------------*/
   sprintf (mySRC.w_new, "%s%s%s", mySRC.w_pre, x_label, mySRC.w_suf);
   DEBUG_YSRC   yLOG_info    ("w_new"     , mySRC.w_new);
   ysrc__wander_save    (mySRC.w_new);
   ysrc__wander_update  ();
   /*> sprintf (mySRC.w_pre, "%s%s%s", mySRC.w_pre, x_label, x_math);                 <*/
   mySRC.w_abs = 0;
   /*---(save prev)----------------------*/
   strlcpy (s_prev, s_curr, LEN_LABEL);
   DEBUG_YSRC   yLOG_info    ("previous"  , s_prev);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*---(locals)-----------+-----+-----+-*/
/*> char        rce         =  -10;                                             <* 
 *> char        rc          =    0;                                             <* 
 *> char        x_grid      =    0;                                             <* 
 *> char        t           [5];                                                <* 
 *> char        post = ' ';                                                     <* 
 *> char        x_label     [LEN_LABEL] = "";                                   <* 
 *> char        x_new       [LEN_RECD ] = "";                                   <* 
 *> char       *x_math      = "+-+/%#:";                                        <*/

static  uchar  s_escaping  = G_CHAR_SPACE;  /* in backslash/escape handling   */

char
ysrc__wander_escaped    (uchar *a_minor)
{
   uchar       x_minor       = *a_minor;
   /*---(normal)-------------------------*/
   if (s_escaping == G_CHAR_SPACE) {
      if (x_minor == '\\') {
         DEBUG_YSRC   yLOG_note    ("found a leading backslash");
         s_escaping = G_CHAR_UP;
         return 1;
      }
      return 0;
   }
   /*---(backslashed)--------------------*/
   if (s_escaping == G_CHAR_UP) {
      if (x_minor == '_') {
         DEBUG_YSRC   yLOG_note    ("found a leading backslash/underscore");
         s_escaping = G_CHAR_RIGHT;
         return 2;
      }
      DEBUG_YSRC   yLOG_note    ("converting backslash character");
      *a_minor = chrslashed (x_minor);
      s_escaping = G_CHAR_SPACE;
      return 0;
   }
   /*---(backslash/more)-----------------*/
   if (s_escaping == G_CHAR_RIGHT) {
      DEBUG_YSRC   yLOG_note    ("converting backslash/underscore");
      *a_minor = chrslashed_more (x_minor);
      s_escaping = G_CHAR_SPACE;
      return 0;
   }
   /*---(complete)-----------------------*/
   return 0;
}


char         /*-> process keys for wander mode -------[ ------ [ge.FE0.223.65]*/ /*-[05.0000.102.!]-*/ /*-[--.---.---.--]-*/
ysrc_wander_umode       (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_label     [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_char    ("a_major"   , a_major);
   DEBUG_YSRC   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   /*---(defenses)-----------------------*/
   DEBUG_YSRC   yLOG_char    ("mode"      , yMODE_curr ());
   --rce;  if (yMODE_not (UMOD_WANDER )) {
      DEBUG_YSRC   yLOG_note    ("not the correct mode");
      yMODE_exit  ();
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(display location)---------------*/
   strlcpy (x_label, s_curr, LEN_LABEL);
   ysrc__wander_pos_show  ();
   /*---(universal)----------------------*/
   rc = ysrc__wander_biggies (a_major, a_minor);
   DEBUG_YSRC   yLOG_value   ("biggies"   , rc);
   if (rc > 0) {
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(escaped chars)------------------*/
   rc = ysrc__wander_escaped (&a_minor);
   DEBUG_YSRC   yLOG_value   ("escaped"   , rc);
   if (rc > 0) {
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(repeat)-------------------------*/
   if (yKEYS_is_repeater (a_minor)) {
      DEBUG_YMAP   yLOG_note    ("repeating");
      yMODE_enter  (PMOD_REPEAT);
      DEBUG_YMAP   yLOG_exit    (__FUNCTION__);
      return a_minor;
   }
   /*---(moves)--------------------------*/
   rc = yMAP_move_hmode (a_major, a_minor);
   DEBUG_YMAP   yLOG_value   ("moves"     , rc);
   if (rc != 0) {  /* also for failed goto, scroll, or end multikeys */
      DEBUG_YMAP   yLOG_note    ("yMAP_move_hmove");
      rc = 0;
   }
   /*---(non-movements)------------------*/
   else {
      /*---(reset repeating)----------------*/
      yKEYS_repeat_reset ();
      /*---(absolute markers)------------*/
      if (a_minor == '\'') {
         DEBUG_YMAP   yLOG_note    ("tick, change absolute markers");
         mySRC.w_abs += 2;
         if (mySRC.w_abs > 14)  mySRC.w_abs = 0;
      }
      else if (a_minor == '.') {
         DEBUG_YMAP   yLOG_note    ("period, reset absolute markers");
         mySRC.w_abs = 0;
      }
      /*---(new operation)---------------*/
      else if (strchr (YSTR_OPER, a_minor) != 0) {
         DEBUG_YMAP   yLOG_note    ("operator, prepare for next");
         rc = ysrc__wander_and        (a_minor);
      }
      else if (strchr (YSTR_GROUP, a_minor) != NULL) {
         DEBUG_YMAP   yLOG_note    ("grouping, prepare for next");
         rc = ysrc__wander_and        (a_minor);
      }
      /*---(return to origin)------------*/
      else if (a_minor == G_KEY_BTICK) {
         DEBUG_YMAP   yLOG_note    ("backtick, head to starting point");
         ysrc__wander_return    ();
      }
      /*---(wrap-up)------------------------*/
      else {
         DEBUG_YMAP   yLOG_note    ("bad character, ignore");
         yKEYS_set_error ();
         DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(wrap-up)------------------------*/
   ysrc__wander_pos_show  ();
   ysrc__wander_loc (x_label, NULL);
   sprintf (mySRC.w_new, "%s%s%s", mySRC.w_pre, x_label, mySRC.w_suf);
   DEBUG_YSRC   yLOG_info    ("mySRC.w_new"     , mySRC.w_new);
   ysrc__wander_save    (mySRC.w_new);
   ysrc__wander_pos_set (strlen (mySRC.w_pre));
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}
