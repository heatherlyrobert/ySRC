/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



/*====================------------------------------------====================*/
/*===----                     identify word boundaries                 ----===*/
/*====================------------------------------------====================*/
static void  o___IDENTIFY________o () { return; }

uchar
ysrc_word__type         (char a_type, char a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_mode      =  ' ';
   /*---(words)--------------------------*/
   if      (strchr (YSTR_CHARS, a_curr) != NULL)   x_mode = 'w';
   else if (a_type == 'f' && a_curr == '_')        x_mode = 'w';
   else if (a_type == '-' && a_curr == '-')        x_mode = 'w';
   /*---(spaces)-------------------------*/
   else if (strchr (" ·²"     , a_curr) != NULL)   x_mode = 's';
   /*---(others)-------------------------*/
   else if (strchr (YSTR_PUNCT, a_curr) != NULL)   x_mode = '²';
   else if (strchr (YSTR_GREEK, a_curr) != NULL)   x_mode = '²';
   else                                            x_mode = '²';
   /*---(complete)-----------------------*/
   return x_mode;
}

char
ysrc_word__curr         (char a_type, char a_save, char a_mode)
{
   /*---(fast track)---------------------*/
   if (a_mode == a_save)  return ' ';
   /*---(beginning of string)------------*/
   if      (a_save == '\0' && a_mode == 'w')                     return '[';
   else if (a_save == '\0' && a_mode == '²'  && a_type == '-')   return '[';
   /*---(after space)--------------------*/
   else if (a_save == 's'  && a_mode == 'w')                     return '[';
   else if (a_save == 's'  && a_mode == '²'  && a_type == '-')   return '[';
   /*---(after other)--------------------*/
   else if (a_save == '²'  && a_mode == 'w'  && a_type == '-')   return '<';
   else if (a_save == '²'  && a_mode == 'w'  && a_type == 'f')   return '[';
   /*---(complete)-----------------------*/
   return ' ';
}

char
ysrc_word__prev         (char a_type, char a_save, char a_mode)
{
   /*---(fast track)---------------------*/
   if (a_mode == a_save)  return ' ';
   /*---(end of string)------------------*/
   if      (a_save == 'w'  && a_mode == '\0')                    return ']';
   else if (a_save == '²'  && a_mode == '\0' && a_type == '-')   return ']';
   /*---(before space)-------------------*/
   else if (a_save == 'w'  && a_mode == 's')                     return ']';
   else if (a_save == '²'  && a_mode == 's'  && a_type == '-')   return ']';
   /*---(before other)-------------------*/
   else if (a_save == 'w'  && a_mode == '²'  && a_type == '-')   return '>'; 
   else if (a_save == 'w'  && a_mode == '²'  && a_type == 'f')   return ']'; 
   /*---(complete)-----------------------*/
   return ' ';
}

char
ysrc_word__update       (short a_pos, char a_curr)
{
   /*---(before start)-------------------*/
   if (a_curr < 0)  return 0;
   /*---(beginning)----------------------*/
   if (a_curr == '<')   s_cur->words [a_pos] = '<';
   if (a_curr == '[')   s_cur->words [a_pos] = '[';
   /*---(ending)-------------------------*/
   if (a_curr == '>') {
      if      (s_cur->words [a_pos] == '<')  s_cur->words [a_pos] = 'b';
      else if (s_cur->words [a_pos] == '[')  s_cur->words [a_pos] = 'º';
      else                                   s_cur->words [a_pos] = '>';
   }
   if (a_curr == ']') {
      if      (s_cur->words [a_pos] == '<')  s_cur->words [a_pos] = '»';
      else if (s_cur->words [a_pos] == '[')  s_cur->words [a_pos] = 'B';
      else                                   s_cur->words [a_pos] = ']';
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_word_update        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_change    =  '-';
   int         i           =    0;
   uchar       x_save      = '\0';
   uchar       x_mode      =  '²';
   char        x_type      =  '-';
   char        x_update    =  ' ';
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT   yLOG_value   ("s_npos"    , s_cur->npos);
   DEBUG_EDIT   yLOG_info    ("contents"  , s_cur->contents);
   /*---(mark tail)----------------------*/
   s_cur->words [s_cur->npos] = '\0';
   if (s_cur->npos <= 0) {
      s_cur->words [0] = '\0';
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   if (strchr ("=#", s_cur->contents [0]) != NULL)   x_type = 'f';
   /*---(walk string)--------------------*/
   for (i = 0; i <  s_cur->npos; ++i) {
      /*---(initialize)------------------*/
      s_cur->words [i] = '²';
      /*---(check)-----------------------*/
      x_mode = ysrc_word__type   (x_type, s_cur->contents [i]);
      DEBUG_EDIT   yLOG_complex ("values"    , "%3d, save %3d/%c, curr %3d/%c", i, x_save, chrvisible (x_save), x_mode, chrvisible (x_mode));
      if (x_mode == x_save)  continue;
      /*---(update current)--------------*/
      x_update = ysrc_word__curr (x_type, x_save, x_mode);
      if (x_update != ' ') ysrc_word__update   (i    , x_update);
      DEBUG_EDIT   yLOG_char    ("curr"      , x_update);
      /*---(update previous)-------------*/
      x_update = ysrc_word__prev (x_type, x_save, x_mode);
      if (x_update != ' ') ysrc_word__update   (i - 1, x_update);
      DEBUG_EDIT   yLOG_char    ("prev"      , x_update);
      /*---(save)------------------------*/
      x_save = x_mode;
      /*---(done)------------------------*/
   }
   /*---(mark end)-----------------------*/
   x_mode = '\0';
   DEBUG_EDIT   yLOG_complex ("values"    , "%3d, save %3d/%c, curr %3d/%c", i, x_save, chrvisible (x_save), x_mode, chrvisible (x_mode));
   x_update = ysrc_word__prev (x_type, x_save, '\0');
   if (x_update != ' ') ysrc_word__update   (i - 1, x_update);
   DEBUG_EDIT   yLOG_char    ("prev"      , x_update);
   /*> if (x_type != 'f' && x_mode != '²')  ysrc_word__update   (s_cur->npos - 1, '>');   <*/
   DEBUG_EDIT   yLOG_info    ("words"     , s_cur->words);
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     finding word boundaries                  ----===*/
/*====================------------------------------------====================*/
static void  o___FINDING_________o () { return; }

char
ysrc_word_next          (char a_minor, short *a_cur, short *a_len)
{
   /*---(design notes)-------------------*/
   /*
    *   this is for 'w' key
    */
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =   -1;
   /*---(defaults)-----------------------*/
   if (a_len != NULL)  *a_len = 0;
   if (a_cur != NULL)  *a_cur = s_cur->cpos;
   /*---(word forward)-------------------*/
   for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
      if (a_minor == 'W' && strchr ("[ºB"  , s_cur->words [i]) == NULL)  continue;
      if (a_minor == 'w' && strchr ("[º<Bb", s_cur->words [i]) == NULL)  continue;
      n = i;
      break;
   }
   /*---(check)--------------------------*/
   if (n < 0)  return n;
   /*---(save-back)----------------------*/
   if (a_len != NULL)  *a_len = n - s_cur->cpos + 1;
   if (a_cur != NULL)  *a_cur = n;
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_word_end           (char a_minor, short *a_cur, short *a_len)
{
   /*---(design notes)-------------------*/
   /*
    *   this is for 'e' key
    */
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =   -1;
   /*---(defaults)-----------------------*/
   if (a_len != NULL)  *a_len = 0;
   if (a_cur != NULL)  *a_cur = s_cur->cpos;
   /*---(word forward)-------------------*/
   for (i = s_cur->cpos + 1; i < s_cur->npos; ++i) {
      if (a_minor == 'E' && strchr ("]»B"  , s_cur->words [i]) == NULL)  continue;
      if (a_minor == 'e' && strchr ("]»>Bb", s_cur->words [i]) == NULL)  continue;
      n = i;
      break;
   }
   /*---(check)--------------------------*/
   if (n < 0)  return n;
   /*---(save-back)----------------------*/
   if (a_len != NULL)  *a_len = n - s_cur->cpos + 1;
   if (a_cur != NULL)  *a_cur = n;
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_word_prev          (char a_minor, short *a_cur, short *a_len)
{
   /*---(design notes)-------------------*/
   /*
    *   this is for 'b' key
    */
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =   -1;
   int         x_len       =    0;
   /*---(defaults)-----------------------*/
   if (a_len != NULL)  *a_len = 0;
   if (a_cur != NULL)  *a_cur = s_cur->cpos;
   /*---(word backward)------------------*/
   for (i = s_cur->cpos - 1; i >= 0 ; --i) {
      if (a_minor == 'B' && strchr ("[ºB"  , s_cur->words [i]) == NULL)  continue;
      if (a_minor == 'b' && strchr ("[º<Bb", s_cur->words [i]) == NULL)  continue;
      n = i;
      break;
   }
   /*---(check)--------------------------*/
   if (n < 0)  return n;
   /*---(save-back)----------------------*/
   if (a_len != NULL)  *a_len = s_cur->cpos - n + 1;
   if (a_cur != NULL)  *a_cur = n;
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       mode support                           ----===*/
/*====================------------------------------------====================*/
static void  o___MODE____________o () { return; }

char
ysrc_word_hmode        (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT  yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT  yLOG_char    ("a_minor"   , a_minor);
   /*---(defense)------------------------*/
   --rce;  if (!yKEYS_is_horz_word (a_minor)) {
      DEBUG_EDIT   yLOG_note    ("a_minor was not a valid option");
      DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(words)--------------------------*/
   if (strchr ("wW", a_minor) != 0) {
      rc = ysrc_word_next (a_minor, &(s_cur->cpos), NULL);
   }
   /*---(ends)---------------------------*/
   else if (strchr ("eE", a_minor) != 0) {
      rc = ysrc_word_end  (a_minor, &(s_cur->cpos), NULL);
   }
   /*---(beginnings)---------------------*/
   else if (strchr ("bB", a_minor) != 0) {
      rc = ysrc_word_prev (a_minor, &(s_cur->cpos), NULL);
   }
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)--------------------*/
   DEBUG_EDIT  yLOG_exit    (__FUNCTION__);
   return rc;
}


