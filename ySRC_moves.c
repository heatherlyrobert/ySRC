/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"




char*        /*-> return current source label --------[ shoot  [gz.210.001.01]*/ /*-[00.0000.102.4]-*/ /*-[--.---.---.--]-*/
SOURCE_label            (void)
{
   return s_src.label;
}



char         /*-> tbd --------------------------------[ ------ [ge.DB2.162.62]*/ /*-[01.0000.403.!]-*/ /*-[--.---.---.--]-*/
SOURCE__simple          (int a_major, int a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_EDIT  yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   --rce;  if (a_major != G_KEY_SPACE) {
      DEBUG_EDIT   yLOG_note    ("a_major was not empty");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*> DEBUG_EDIT  yLOG_info    ("g_hsimple" , g_hsimple);                            <*/
   --rce;  if (!yKEYS_is_horz_simple (a_minor)) {
      DEBUG_EDIT   yLOG_note    ("a_minor was not a valid option");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(horizontal moves)---------------*/
   switch (a_minor) {
   case '0' : s_cur->cpos  = 0;                    break;
   case 'H' : s_cur->cpos -= 5;                    break;
   case 'h' : --s_cur->cpos;                       break;
   case 'l' : ++s_cur->cpos;                       break;
   case 'L' : s_cur->cpos += 5;                    break;
   case '$' : s_cur->cpos  = s_cur->npos - 1;      break;
   }
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_EDIT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.DB2.162.62]*/ /*-[01.0000.403.!]-*/ /*-[--.---.---.--]-*/
SOURCE__goto       (int a_major, int a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_qtr       =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT  yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   --rce;  if (a_major != 'g') {
      DEBUG_EDIT   yLOG_note    ("a_major was not g (goto)");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*> DEBUG_EDIT  yLOG_info    ("g_hgoto"  , g_hgoto);                               <*/
   --rce;  if (!yKEYS_is_horz_goto (a_minor)) {
      DEBUG_EDIT   yLOG_note    ("a_minor was not a valid option");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(goto moves)---------------------*/
   x_qtr = s_cur->apos / 4;
   switch (a_minor) {
   case 'S' : s_cur->cpos  = s_cur->bpos - x_qtr * 4;  break;
   case 'H' : s_cur->cpos  = s_cur->bpos - x_qtr * 2;  break;
   case 's' : s_cur->cpos  = s_cur->bpos;              break;
   case 'h' : s_cur->cpos  = s_cur->bpos + x_qtr * 1;  break;
   case 'c' : s_cur->cpos  = s_cur->bpos + x_qtr * 2;  break;
   case 'l' : s_cur->cpos  = s_cur->bpos + x_qtr * 3;  break;
   case 'e' : s_cur->cpos  = s_cur->epos;              break;
   case 'L' : s_cur->cpos  = s_cur->bpos + x_qtr * 6;  break;
   case 'E' : s_cur->cpos  = s_cur->bpos + x_qtr * 8;  break;
   }
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_EDIT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.DB2.162.62]*/ /*-[01.0000.403.!]-*/ /*-[--.---.---.--]-*/
SOURCE__scroll     (char a_major, char a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_qtr       =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT  yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   --rce;  if (a_major != 'z') {
      DEBUG_EDIT   yLOG_note    ("a_major was not z (scroll)");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*> DEBUG_EDIT  yLOG_info    ("g_hscroll" , g_hscroll);                            <*/
   --rce;  if (!yKEYS_is_horz_scroll (a_minor)) {
      DEBUG_EDIT   yLOG_note    ("a_minor was not a valid option");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(goto moves)---------------------*/
   x_qtr = s_cur->apos / 4;
   switch (a_minor) {
   case 's' : s_cur->bpos += s_cur->cpos - (s_cur->bpos + x_qtr * 0);  break;
   case 'h' : s_cur->bpos += s_cur->cpos - (s_cur->bpos + x_qtr * 1);  break;
   case 'c' : s_cur->bpos += s_cur->cpos - (s_cur->bpos + x_qtr * 2);  break;
   case 'l' : s_cur->bpos += s_cur->cpos - (s_cur->bpos + x_qtr * 3);  break;
   case 'e' : s_cur->bpos += s_cur->cpos - (s_cur->epos);              break;
   }
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_EDIT  yLOG_exit    (__FUNCTION__);
   return 0;
}

int
SOURCE__nextword        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(word forward)-------------------*/
   for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
      if (strchr ("<B", s_cur->words [i]) == NULL)  continue;
      return i - s_cur->cpos;
   }
   /*---(complete)-----------------------*/
   return s_cur->npos - s_cur->cpos;
}

int
SOURCE__endword         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(word forward)-------------------*/
   for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
      if (strchr (">B", s_cur->words [i]) == NULL)  continue;
      return i - s_cur->cpos + 1;
   }
   /*---(complete)-----------------------*/
   return s_cur->npos - s_cur->cpos;
}

int
SOURCE__prevword        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         x_len       =    0;
   /*---(word backward)------------------*/
   for (i = s_cur->cpos - 1; i >= 0 ; --i) {
      if (strchr ("<B", s_cur->words [i]) == NULL)  continue;
      x_len       = s_cur->cpos - i;
      s_cur->cpos = i;
      return x_len;
   }
   /*---(all)----------------------------*/
   x_len       = s_cur->cpos - 1;
   s_cur->cpos = 0;
   return x_len;
}

char
SOURCE__word            (int a_major, int a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char       *x_word      = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
   int         x_not       =    0;
   int         x_yes       =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT  yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   /*> DEBUG_EDIT  yLOG_info    ("g_hword"   , g_hword);                              <*/
   --rce;  if (!yKEYS_is_horz_word (a_minor)) {
      DEBUG_EDIT   yLOG_note    ("a_minor was not a valid option");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(words)--------------------------*/
   if (strchr ("wW", a_minor) != 0) {
      for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
         if (strchr ("<B", s_cur->words [i]) == NULL)  continue;
         s_cur->cpos = i;
         break;
      }
   }
   /*---(ends)---------------------------*/
   if (strchr ("eE", a_minor) != 0) {
      for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
         if (strchr (">B", s_cur->words [i]) == NULL)  continue;
         s_cur->cpos = i;
         break;
      }
   }
   /*---(beginnings)---------------------*/
   if (strchr ("bB", a_minor) != 0) {
      for (i = s_cur->cpos - 1; i >= 0; --i) {
         if (strchr ("<B", s_cur->words [i]) == NULL)  continue;
         s_cur->cpos = i;
         break;
      }
   }
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_EDIT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SOURCE__charfindrev     (uchar a_ch)
{
   int         i           =    0;
   for (i = s_cur->cpos - 1; i >= 0; --i) {
      if (s_cur->contents [i] != a_ch)  continue;
      s_cur->cpos = i;
      UPDATE_AFTER_CHANGES;
      return 0;
   }
   return -1;
}

char
SOURCE__charfind        (uchar a_ch)
{
   int         i           =    0;
   for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
      if (s_cur->contents [i] != a_ch)  continue;
      s_cur->cpos = i;
      UPDATE_AFTER_CHANGES;
      return 0;
   }
   return -1;
}

char         /*-> complex delete action --------------[ ------ [gz.430.031.02]*/ /*-[01.0000.213.!]-*/ /*-[--.---.---.--]-*/
SOURCE_delete          (char a_major, char a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        i           =    0;
   char        x_len       =    0;
   char        x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_EDIT   yLOG_char    ("s_live"    , ysrc_select_getlive ());
   --rce;  if (ysrc_select_islive ()) {
      DEBUG_EDIT   yLOG_note    ("function only handles non-selected text");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_EDIT   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   --rce;  if (a_minor == 0 || strchr ("hlwbe0$", a_minor) == NULL) {
      DEBUG_EDIT   yLOG_note    ("source only allows right and left");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_EDIT   yLOG_value   ("npos"      , s_cur->npos);
   --rce;  if (s_cur->npos <= 0) {
      DEBUG_EDIT   yLOG_note    ("nothing text to delete");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_EDIT   yLOG_value   ("cpos"      , s_cur->cpos);
   --rce;  if (a_minor == 'h' && s_cur->cpos <= 0) {
      DEBUG_EDIT   yLOG_note    ("nothing left to delete");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_minor == 'l' && s_cur->cpos >= s_cur->npos) {
      DEBUG_EDIT   yLOG_note    ("nothing right to delete");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   switch (a_minor) {
   case 'h' : x_len = 1;                             break;
   case 'l' : x_len = 1;                             break;
   case 'w' : x_len = SOURCE__nextword ();           break;
   case 'b' : x_len = SOURCE__prevword ();           break;
   case 'e' : x_len = SOURCE__endword  ();           break;
   case '0' : x_len = s_cur->cpos; s_cur->cpos = 0;  break;
   case '$' : x_len = s_cur->npos - s_cur->cpos;     break;
   }
   if (strchr ("hb0$", a_minor) != NULL)  x_pos = s_cur->cpos;
   else                                   x_pos = s_cur->cpos + x_len;
   /*---(end)----------------------------*/
   ysrc_sundo_beg ();
   for (i = 0; i < x_len; ++i) {
      if (a_major == 'd') {
         if (a_minor == 'h')  --s_cur->cpos;
         ysrc_sundo_add (a_major, 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_KEY_NULL);
         ysrc_delete_one ();
      } else {
         if (a_minor == 'h')  --s_cur->cpos;
         ysrc_sundo_add (a_major, 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_CHAR_STORAGE);
         ysrc_replace_one (G_CHAR_STORAGE);
         if (a_minor != 'h')  ++s_cur->cpos;
      }
   }
   ysrc_sundo_end ();
   if (a_major == 'x' && a_minor != 'h')  s_cur->cpos = x_pos;
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}
