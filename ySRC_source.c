/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


tEDIT   s_src;
tEDIT   s_cmd;
tEDIT  *s_cur    = &s_src;

static  uchar *s_valid = "abcdefghijklmnopqrstuvwxyz<>";
static  uchar  s_escaping  = G_CHAR_SPACE;  /* in backslash/escape handling   */


/*====================------------------------------------====================*/
/*===----                    source marks                              ----===*/
/*====================------------------------------------====================*/
static void  o___SMARKS__________o () { return; }

char
ysrc_source_smark_set   (uchar a_mark)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar      *p           = NULL;
   char        n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC_U   yLOG_senter  (__FUNCTION__);
   DEBUG_YSRC_U   yLOG_sint    (a_mark);
   /*---(defense)------------------------*/
   --rce;  if (a_mark == 0 || (p = strchr (s_valid, a_mark)) == NULL) {
      DEBUG_YSRC_U   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC_U   yLOG_schar   (a_mark);
   /*---(set)----------------------------*/
   n = p - s_valid;
   DEBUG_YSRC_U   yLOG_sint    (n);
   mySRC.s_marks [n] = s_cur->cpos;
   DEBUG_YSRC_U   yLOG_sint    (mySRC.s_marks [n]);
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_source_smark_clear (uchar a_mark)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar      *p           = NULL;
   char        n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC_U   yLOG_senter  (__FUNCTION__);
   DEBUG_YSRC_U   yLOG_sint    (a_mark);
   /*---(defense)------------------------*/
   --rce;  if (a_mark == 0 || (p = strchr (s_valid, a_mark)) == NULL) {
      DEBUG_YSRC_U   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC_U   yLOG_schar   (a_mark);
   /*---(set)----------------------------*/
   n = p - s_valid;
   mySRC.s_marks [n] = -1;
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_source_smark_purge (void)
{
   int         i           =    0;
   int         l           =    0;
   DEBUG_YSRC_U   yLOG_senter  (__FUNCTION__);
   l = strlen (s_valid);
   for (i = 0; i < l; ++i)  mySRC.s_marks [i] = -1;
   DEBUG_YSRC_U   yLOG_sexit   (__FUNCTION__);
   return 0;
}

short
ysrc_source_smark_get   (uchar a_mark)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar      *p           = NULL;
   char        n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC_U   yLOG_senter  (__FUNCTION__);
   DEBUG_YSRC_U   yLOG_sint    (a_mark);
   /*---(defense)------------------------*/
   --rce;  if (a_mark == 0 || (p = strchr (s_valid, a_mark)) == NULL) {
      DEBUG_YSRC_U   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC_U   yLOG_schar   (a_mark);
   /*---(go)-----------------------------*/
   DEBUG_YSRC_U   yLOG_sint    (n);
   n = p - s_valid;
   DEBUG_YSRC_U   yLOG_sint    (mySRC.s_marks [n]);
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U   yLOG_sexit   (__FUNCTION__);
   return mySRC.s_marks [n];
}

char
ysrc_source_smark_go    (uchar a_mark)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar      *p           = NULL;
   char        n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC_U   yLOG_senter  (__FUNCTION__);
   DEBUG_YSRC_U   yLOG_sint    (a_mark);
   /*---(defense)------------------------*/
   --rce;  if (a_mark == 0 || (p = strchr (s_valid, a_mark)) == NULL) {
      DEBUG_YSRC_U   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC_U   yLOG_schar   (a_mark);
   /*---(go)-----------------------------*/
   n = p - s_valid;
   --rce;  if (mySRC.s_marks [n] <  0) {
      DEBUG_YSRC_U   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   s_cur->cpos  = mySRC.s_marks [n];
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       source set                             ----===*/
/*====================------------------------------------====================*/
static void  o___SETS____________o () { return; }

char
ysrc_source_set_purge   (void)
{
   int         i           =    0;
   for (i = 0; i < 256; ++i) {
      mySRC.s_set  [i] = '·';
      mySRC.s_tes  [i] = '·';
   }
   strcpy (mySRC.s_text, "");
   strcpy (mySRC.s_code, "");
   strcpy (mySRC.s_once, "");
   mySRC.s_act = '·';
   return 0;
}

char
ysrc_source_set_set     (uchar a_major, uchar *a_text)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         b           =    0;
   int         i, j;
   int         a_beg       =   -1;
   uchar       x_set       [256];
   /*---(header)-------------------------*/
   DEBUG_YSRC_U   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC_U   yLOG_spoint  (a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_YSRC_U   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC_U   yLOG_snote   (a_text);
   /*---(cleanse)------------------------*/
   for (i = 0; i < 256; ++i)  mySRC.s_set [i] = '·';
   /*---(set)----------------------------*/
   if (a_text [0] == '^')  ++b;
   for (i = b; i < strlen (a_text); ++i) {
      if (i != b && a_text [i] == '-') {
         DEBUG_YSRC_U   yLOG_schar   ('>');
         a_beg = a_text [i - 1];
         continue;
      }
      if (a_beg >= 0) {
         for (j = a_beg; j <= a_text [i]; ++j)  {
            DEBUG_YSRC_U   yLOG_schar   (j);
            mySRC.s_set [j]       = 'y';
         }
      } else {
         DEBUG_YSRC_U   yLOG_schar   (j);
         mySRC.s_set [a_text [i]] = 'y';
      }
      a_beg = -1;
   }
   /*---(check reverse)------------------*/
   if (a_text [0] == '^')  {
      for (i = 0; i < 256; ++i) {
         if (mySRC.s_set [i] == 'y')  mySRC.s_set [i] = '·';
         else                         mySRC.s_set [i] = 'y';
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_source_set_code    (uchar a_major, uchar *a_text)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         b           =    0;
   int         i, j;
   int         a_beg       =   -1;
   uchar       x_set       [256];
   /*---(header)-------------------------*/
   DEBUG_YSRC_U   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC_U   yLOG_spoint  (a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_YSRC_U   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC_U   yLOG_snote   (a_text);
   /*---(cleanse)------------------------*/
   for (i = 0; i < 256; ++i)  mySRC.s_tes [i] = '·';
   /*---(set)----------------------------*/
   if (a_text [0] == '^')  ++b;
   for (i = b; i < strlen (a_text); ++i) {
      if (i != b && a_text [i] == '-') {
         DEBUG_YSRC_U   yLOG_schar   ('>');
         a_beg = a_text [i - 1];
         continue;
      }
      if (a_beg >= 0) {
         for (j = a_beg; j <= a_text [i]; ++j)  {
            DEBUG_YSRC_U   yLOG_schar   (j);
            mySRC.s_tes [j]       = 'y';
         }
      } else {
         DEBUG_YSRC_U   yLOG_schar   (j);
         mySRC.s_tes [a_text [i]] = 'y';
      }
      a_beg = -1;
   }
   /*---(check reverse)------------------*/
   if (a_text [0] == '^')  {
      for (i = 0; i < 256; ++i) {
         if (mySRC.s_tes [i] == 'y')  mySRC.s_tes [i] = '·';
         else                         mySRC.s_tes [i] = 'y';
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_source_set_check   (uchar a_char)
{
   if (mySRC.s_set [a_char] == 'y')  return 1;
   return 0;
}

char
ysrc_source_set_exec    (uchar a_major, uchar a_act, uchar a_minor)
{
   char        rce         =  -10;
   int         i, j;
   int         b, e;
   /*---(header)-------------------------*/
   DEBUG_YSRC_U   yLOG_enter   (__FUNCTION__);
   /*---(check major)--------------------*/
   DEBUG_YSRC_U   yLOG_value   ("a_major"   , a_major);
   --rce;  switch (a_major) {
   case '?' :
      b = e = s_cur->cpos;
      break;
   case 'A' :
      b = ysrc_source_smark_get ('<');
      DEBUG_YSRC_U   yLOG_value   ("b"         , b);
      if (b < 0)  b = 0;
      e = ysrc_source_smark_get ('>');
      DEBUG_YSRC_U   yLOG_value   ("e"         , e);
      if (e < 0)  e = s_cur->npos;
      break;
   default  :
      DEBUG_YSRC_U   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_YSRC_U   yLOG_complex ("a_major"   , "%c, %3db, %3de", a_major, b, e);
   /*---(check length)-------------------*/
   DEBUG_YSRC_U   yLOG_value   ("npos"      , s_cur->npos);
   --rce;  if (s_cur->npos <= 0) {
      DEBUG_YSRC_U   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense on minor)---------------*/
   DEBUG_YSRC_U   yLOG_value   ("a_minor"   , a_major);
   --rce;  if (a_act == 0 || strchr ("rxdia", a_act) == NULL) {
      DEBUG_YSRC_U   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC_U   yLOG_char    ("a_minor"   , a_major);
   /*---(handle update)------------------*/
   for (i = b; i <= e; ++i) {
      if (ysrc_source_set_check (s_cur->contents [i])) {
         switch (a_act) {
         case 'r' :
            s_cur->contents [i] = a_minor;
            break;
         case 'x' :
            s_cur->contents [i] = ' ';
            break;
         case 'd' :
            for (j = i; j < s_cur->npos; ++j) {
               s_cur->contents [j] = s_cur->contents [j + 1];
            }
            if  (e == s_cur->npos) --e;;
            --(s_cur->npos);
            break;
         case 'i' :
            for (j = s_cur->npos; j >= i; --j) {
               s_cur->contents [j + 1] = s_cur->contents [j];
            }
            s_cur->contents [i++] = a_minor;
            if  (e == s_cur->npos) ++e;;
            ++(s_cur->npos);
            break;
         case 'a' :
            for (j = s_cur->npos; j >  i; --j) {
               s_cur->contents [j + 1] = s_cur->contents [j];
            }
            s_cur->contents [++i] = a_minor;
            if  (e == s_cur->npos) ++e;;
            ++(s_cur->npos);
            break;
         }
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       source mdoe                            ----===*/
/*====================------------------------------------====================*/
static void  o___MODE____________o () { return; }

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
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
   /*---(major keys)---------------------*/
   switch (a_minor) {
   case G_KEY_SPACE  :
      DEBUG_YSRC_U  yLOG_note    ("space, nothing to do");
      break;
   case G_KEY_ESCAPE :
      if (ySRC_select_islive ()) {
         DEBUG_YSRC_U  yLOG_note    ("escape, means get out of selection");
         x_pos = g_sreg.root;
         ysrc_select_reset (G_SREG_ROOT);
         s_cur->cpos = x_pos;
      } else {
         DEBUG_YSRC_U  yLOG_note    ("escape, means forget and return to previous mode");
         ysrc_reset  ();
         ysrc_after  ();
         s_escaping = G_CHAR_SPACE;
         mySRC.s_act = '·';
         yMODE_exit  ();
      }
      break;
   case G_KEY_RETURN :
      DEBUG_YSRC_U  yLOG_note    ("enter, means save and return to previous mode");
      ysrc_accept ();
      ysrc_after  ();
      s_escaping = G_CHAR_SPACE;
      mySRC.s_act = '·';
      yMODE_exit  ();
      break;
   case G_KEY_BTICK :
      DEBUG_YSRC_U  yLOG_note    ("backtick, means re-wander or move label");
      rc = yMODE_enter (UMOD_WANDER);
      if (rc < 0) {
         DEBUG_YSRC_U  yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      break;
   case  '\0'     :
      break;
   default        :
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
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
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
   /*---(macros modes)-------------------*/
   switch (a_minor) {
   case '@'      :
      DEBUG_YSRC_U  yLOG_note    ("macro execution");
      yMODE_enter  (SMOD_MACRO   );
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return a_minor;
      break;
   case 'q'      :
      IF_MACRO_RECORDING {
         DEBUG_YSRC_U  yLOG_note    ("end macro recording");
         rc = yMACRO_rec_end ();
      } else {
         DEBUG_YSRC_U  yLOG_note    ("begin macro recording");
         yMODE_enter  (SMOD_MACRO   );
         rc = a_minor;
      }
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'Q'      :
      DEBUG_YSRC_U  yLOG_note    ("reset macro recording");
      rc = yMACRO_reset_all ();
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
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
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
   /*---(sub modes)----------------------*/
   switch (a_minor) {
   case  'v' :
      if (ySRC_select_islive ()) s_cur->cpos = ysrc_select_reverse  ();
      else                       ysrc_select_makelive ();
      rc = 0;
      break;
   case  '"' :
      DEBUG_YSRC_U  yLOG_note    ("switch to a text register mode (¶)");
      rc = yMODE_enter (SMOD_SREG);
      if (rc >= 0)  rc = a_minor;
      break;
   case  'r' :
      DEBUG_YSRC_U  yLOG_note    ("enter replace mode");
      rc = yMODE_enter (UMOD_REPLACE);
      /*> if (rc >= 0)  rc = a_minor;                                                 <*/
      break;
   case  'R' :
      DEBUG_YSRC_U  yLOG_note    ("enter replace mode");
      rc = yMODE_enter (UMOD_REPLACE);
      /*> if (rc >= 0)  rc = a_minor;                                                 <*/
      break;
   case  'i' : case  'a' : case  'I' : case  'A' :
      DEBUG_YSRC_U  yLOG_note    ("enter input mode");
      rc = yMODE_enter (UMOD_INPUT);
      /*> if (rc >= 0)  rc = tolower (a_minor);                                       <*/
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc__source_findchar   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
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
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(single keys)--------------------*/
   if (a_major != G_KEY_SPACE) {
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   switch (a_minor) {
   case  '#' :
      DEBUG_YSRC_U  yLOG_note    ("current char find");
      rc = ysrc_move_char_next (1);
      break;
   case  'n' :
      DEBUG_YSRC_U  yLOG_note    ("char find next");
      rc = ysrc_move_char_next (0);
      break;
   case  'N' :
      DEBUG_YSRC_U  yLOG_note    ("char find reverse");
      rc = ysrc_move_char_prev (0);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
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
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
   /*---(delete/clear)----------------*/
   switch (a_minor) {
   case  G_KEY_DEL : case  G_CHAR_DEL : case  'D' :
      rc = ysrc_multi_pure ('d', 'l');
      if (rc >= 0)  rc = 0;
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case  G_KEY_BS  : case  G_CHAR_BS  : case  'X' :
      rc = ysrc_multi_pure ('d', 'h');
      if (rc >= 0)  rc = 0;
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(other stuff)-----------------*/
   switch (a_minor) {
   case '|' :
      DEBUG_YSRC_U  yLOG_value   ("repeats"   , yKEYS_repeats);
      s_cur->cpos = yKEYS_repeat_useall ();
      DEBUG_YSRC_U  yLOG_value   ("cpos"      , s_cur->cpos);
      rc     = 0;
      ysrc_after ();
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'u' :
      ysrc_select_reset (s_cur->cpos);
      rc = ysrc_sundo_undo ();
      ysrc_after ();
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   case 'U' :
      ysrc_select_reset (s_cur->cpos);
      rc = ysrc_sundo_redo ();
      ysrc_after ();
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc__source_multikey   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =   -1;
   char        t           [LEN_RECD]  = "";
   int         i, j;
   /*---(quick out)----------------------*/
   if (a_major == G_KEY_SPACE)  return 0;
   if (a_major == 0)            return 0;
   /*---(header)-------------------------*/
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (strchr ("ydxc", a_major) != NULL) {
      if (yKEYS_repeat_beg ()) {
         DEBUG_YSRC_U  yLOG_note    ("initialize yanked text");
         mySRC.yank [0] = '\0';
         mySRC.ylen     = 0;
         mySRC.ybeg     = s_cur->cpos;
      }
   }
   /*---(multi-key)----------------------*/
   switch (a_major) {
   case 'y' : case 'd' : case 'x' : case 'c' :
      DEBUG_YSRC_U  yLOG_note    ("multi-key yank (y), delete (d), clear (x), or change (c)");
      rc = ysrc_multi_pure (a_major, a_minor);
      break;
   case 'g' :
      DEBUG_YSRC_U  yLOG_note    ("multi-key goto (g)");
      rc = ysrc_move_goto (a_major, a_minor);
      break;
   case 'z' :
      DEBUG_YSRC_U  yLOG_note    ("multi-key scroll (z)");
      rc = ysrc_move_scroll (a_major, a_minor);
      break;
   case 'f' : case 'F' :
      DEBUG_YSRC_U  yLOG_note    ("multi-key find char (F or f)");
      rc = ysrc__source_findchar (a_major, a_minor);
      break;
   case 'm' :
      DEBUG_YSRC_U  yLOG_note    ("multi-key (m) set a mark");
      rc = ysrc_source_smark_set (a_minor);
      break;
   case '\'' :
      DEBUG_YSRC_U  yLOG_note    ("multi-key (m) return to a mark");
      rc = ysrc_source_smark_go  (a_minor);
      break;
   case 'S' :
      if (a_minor == (uchar) '˜') {
         DEBUG_YSRC_U  yLOG_note    ("complete base set");
         DEBUG_YSRC_U  yLOG_info    ("s_text"    , mySRC.s_text);
         rc = ysrc_source_set_set (a_major, mySRC.s_text);
      } else {
         DEBUG_YSRC_U  yLOG_note    ("accumulate base set");
         sprintf (t, "%c", a_minor);
         strlcat (mySRC.s_text, t, LEN_HUND);
         DEBUG_YSRC_U  yLOG_info    ("s_text"    , mySRC.s_text);
         rc = a_major;
      }
      break;
   case 'E' :
      if (a_minor == (uchar) '˜') {
         DEBUG_YSRC_U  yLOG_note    ("complete encode set");
         DEBUG_YSRC_U  yLOG_info    ("s_text"    , mySRC.s_text);
         rc = ysrc_source_set_set (a_major, mySRC.s_text);
      } else {
         DEBUG_YSRC_U  yLOG_note    ("accumulate encode set");
         sprintf (t, "%c", a_minor);
         strlcat (mySRC.s_code, t, LEN_HUND);
         DEBUG_YSRC_U  yLOG_info    ("s_code"    , mySRC.s_code);
         rc = a_major;
      }
      break;
   case '?' :
      if (mySRC.s_act == (uchar) '·') {
         DEBUG_YSRC_U  yLOG_note    ("set character check action");
         mySRC.s_act = a_minor;
         rc = a_major;
         if (strchr ("xd", mySRC.s_act) == NULL)  break;
      }
      if (strchr ("RXDIA", mySRC.s_act) != NULL) {
         DEBUG_YSRC_U  yLOG_note    ("capital versions, singl char set");
         rc = a_major;
         sprintf (mySRC.s_once, "%c", a_minor);
         ysrc_source_set_set (a_major, mySRC.s_once);
         mySRC.s_act = tolower (mySRC.s_act);
         if (strchr ("xd", mySRC.s_act) == NULL)  break;
      } else {
         rc = ysrc_source_set_exec    (a_major, mySRC.s_act, a_minor);
      }
      break;
   case 'A' :
      if (mySRC.s_act == (uchar) '·') {
         DEBUG_YSRC_U  yLOG_note    ("set character check action");
         mySRC.s_act = a_minor;
         rc = a_major;
         if (strchr ("xd", mySRC.s_act) == NULL)  break;
      }
      if (strchr ("RXDIA", mySRC.s_act) != NULL) {
         DEBUG_YSRC_U  yLOG_note    ("capital versions, singl char set");
         rc = a_major;
         sprintf (mySRC.s_once, "%c", a_minor);
         ysrc_source_set_set (a_major, mySRC.s_once);
         mySRC.s_act = tolower (mySRC.s_act);
         if (strchr ("xd", mySRC.s_act) == NULL)  break;
      } else {
         rc = ysrc_source_set_exec    (a_major, mySRC.s_act, a_minor);
      }
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
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
   return rc;
}


char
ysrc_trouble            (void)
{
   s_escaping = G_CHAR_SPACE;
   ysrc_source_set_purge   ();
   mySRC.s_act = '·';
   yKEYS_set_lock  ();
   return 0;
}

char
ysrc_source_escaped         (uchar a_major, uchar *a_minor)
{
   char         rce        =  -10;
   uchar        x_minor    =  '·';
   /*---(header)-------------------------*/
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
   /*---(start)--------------------------*/
   x_minor = *a_minor;
   DEBUG_YSRC_U  yLOG_char    ("x_minor"   , x_minor);
   /*---(normal)-------------------------*/
   if (s_escaping == G_CHAR_SPACE) {
      if (x_minor == '\\') {
         DEBUG_YSRC_U  yLOG_note    ("found a leading backslash");
         s_escaping = G_CHAR_UP;
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return 1;
      }
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(backslashed)--------------------*/
   --rce;  if (s_escaping == G_CHAR_UP) {
      if (x_minor == '_') {
         DEBUG_YSRC_U  yLOG_note    ("found a leading backslash/underscore");
         s_escaping = G_CHAR_RIGHT;
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return 2;
      }
      DEBUG_YSRC_U  yLOG_note    ("converting backslash character");
      *a_minor = chrslashed (x_minor);
      s_escaping = G_CHAR_SPACE;
      if (*a_minor == 0) {
         ysrc_trouble ();
         DEBUG_YSRC_U  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(backslash/more)-----------------*/
   --rce;  if (s_escaping == G_CHAR_RIGHT) {
      DEBUG_YSRC_U  yLOG_note    ("converting backslash/underscore");
      *a_minor = chrslashed_more (x_minor);
      s_escaping = G_CHAR_SPACE;
      if (*a_minor == 0) {
         ysrc_trouble ();
         DEBUG_YSRC_U  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
   return 0;
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
   DEBUG_YSRC_U  yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC_U  yLOG_char    ("a_major"   , a_major);
   DEBUG_YSRC_U  yLOG_char    ("a_minor"   , chrvisible (a_minor));
   /*---(defenses)-----------------------*/
   DEBUG_YSRC_U  yLOG_char    ("mode"      , yMODE_curr ());
   DEBUG_YSRC_U  yLOG_info    ("valid"     , MODES_ONELINE);
   --rce;  if (strchr (MODES_ONELINE, yMODE_curr ()) == NULL ) {
      DEBUG_YSRC_U  yLOG_note    ("not an acceptable mode");
      yMODE_exit  ();
      DEBUG_YSRC_U  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(universal)----------------------*/
   rc = ysrc__source_biggies (a_major, a_minor);
   DEBUG_YSRC_U  yLOG_value   ("biggies"   , rc);
   if (rc > 0) {
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(escaping)-----------------------*/
   rc = ysrc_source_escaped         (a_major, &a_minor);
   DEBUG_YSRC_U  yLOG_value   ("escaping"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSRC_U  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (rc > 0) {
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return a_major;
   }
   /*---(single letter)------------------*/
   if (a_major == G_KEY_SPACE) {
      /*---(repeats)---------------------*/
      if (yKEYS_is_repeater (a_minor)) {
         DEBUG_YSRC_U  yLOG_note    ("repeat character 1-9");
         yMODE_enter  (PMOD_REPEAT);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(marks)-----------------------*/
      if (strchr ("m'", a_minor) != NULL) {
         DEBUG_YSRC_U  yLOG_note    ("prepare to use marks");
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(sets)------------------------*/
      if (a_minor == (uchar) 'â') {
         DEBUG_YSRC_U  yLOG_note    ("define the base set");
         ysrc_source_set_purge   ();
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return 'S';
      }
      if (a_minor == (uchar) 'á') {
         DEBUG_YSRC_U  yLOG_note    ("define the encode set");
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return 'C';
      }
      if (a_minor == (uchar) 'ã') {
         DEBUG_YSRC_U  yLOG_note    ("use the set once");
         mySRC.s_act = '·';
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return '?';
      }
      if (a_minor == (uchar) 'ä') {
         DEBUG_YSRC_U  yLOG_note    ("use the everywhere");
         mySRC.s_act = '·';
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return 'A';
      }
      /*---(select related)--------------*/
      if (strchr ("pPY", a_minor) != 0) {
         DEBUG_YSRC_U  yLOG_note    ("switch to a text register mode (pPY)");
         ysrc_sreg_setreg ('"');
         yMODE_enter (SMOD_SREG);
         rc = ysrc_sreg_smode (G_KEY_SPACE, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      if (ySRC_select_islive () && strchr ("yxdrsc", a_minor) != 0) {
         DEBUG_YSRC_U  yLOG_note    ("switch to a text register mode (yxdrsc)");
         ysrc_sreg_setreg ('"');
         yMODE_enter (SMOD_SREG);
         rc = ysrc_sreg_smode (G_KEY_SPACE, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(multikey prefixes)-----------*/
      if (yKEYS_is_multi_src (a_minor)) {
         DEBUG_YSRC_U  yLOG_note    ("beginning of multi-key command");
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return a_minor;
      }
      /*---(sub-modes)-------------------*/
      if (strchr ("v\"rRiIaA", a_minor) != NULL) {
         rc = ysrc__source_subs (a_major, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(basic movement)--------------*/
      if (yKEYS_is_horz_simple (a_minor)) {
         rc = ysrc_move_simple (a_major, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(word movement)---------------*/
      if (yKEYS_is_horz_word   (a_minor)) {
         rc = ysrc_word_hmode (a_major, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(findchar)--------------------*/
      if (yKEYS_is_horz_char (a_minor)) {
         rc = ysrc__source_findchar (a_major, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(macro use)-------------------*/
      if (strchr ("DX¾¿|uU", a_minor) != NULL || a_minor == G_KEY_BS || a_minor == G_KEY_DEL) {
         rc = ysrc__source_editing (a_major, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(macro use)-------------------*/
      if (yKEYS_is_macro    (a_minor)) {
         rc = ysrc__source_macro (a_major, a_minor);
         DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(replay)----------------------*/
      if (a_minor = '.') {
         rc = yKEYS_replay ();
      }
      /*---(done)------------------------*/
   }
   /*---(multi-key)----------------------*/
   else {
      DEBUG_YSRC_U  yLOG_note    ("multikey handling");
      rc = ysrc__source_multikey (a_major, a_minor);
      if (a_major == 'c' && yKEYS_repeat_end ()) {
         DEBUG_YSRC_U  yLOG_note    ("enter input mode");
         rc = yMODE_enter (UMOD_INPUT);
         ysrc_sundo_chain ();
      }
      DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(complete)-----------------------*/
   if (rc < 0) {
      DEBUG_YSRC_U  yLOG_note    ("return code is negative");
      DEBUG_YSRC_U  yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_YSRC_U  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char
ysrc__source_unit       (char *a_text)
{
   snprintf (a_text, LEN_FULL, "%c", s_escaping);
   return 0;
}


