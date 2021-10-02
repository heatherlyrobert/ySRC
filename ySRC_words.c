/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



char
ysrc_words__type        (char a_type, char a_curr, char a_save)
{
   /*---(locals)-----------+-----+-----+-*/
   char       *x_form      = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_$";
   char       *x_word      = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
   char       *x_math      = "² #=(),+-*/%";
   char        x_mode      =  ' ';
   if      (strchr (YSTR_CHARS, a_curr) != NULL)     x_mode = 'w';
   else if (a_type == 'f' && strchr ("_$", a_curr) != NULL)     x_mode = 'w';
   else if (a_type == '-' && strchr ("-" , a_curr) != NULL)     x_mode = 'w';
   else if (strchr (YSTR_PUNCT, a_curr) != NULL)     x_mode = '²';
   else if (strchr (YSTR_GREEK, a_curr) != NULL)     x_mode = '²';
   else                                              x_mode = 's';
   /*---(complete)-----------------------*/
   return x_mode;
}

char
ysrc_words__update      (int a_pos, char a_curr)
{
   /*---(before start)-------------------*/
   if (a_curr < 0)  return 0;
   /*---(beginning)----------------------*/
   if (a_curr == '<')   s_cur->words [a_pos] = '<';
   /*---(ending)-------------------------*/
   if (a_curr == '>') {
      if (s_cur->words [a_pos] == '<')  s_cur->words [a_pos] = 'B';
      else                              s_cur->words [a_pos] = '>';
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
ysrc_words             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_change    =  '-';
   int         i           =    0;
   char        x_save      =  '²';
   char        x_mode      =  '²';
   char        x_type      =  '-';
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
   for (i = 0; i < s_cur->npos; ++i) {
      /*---(initialize)------------------*/
      s_cur->words [i] = '²';
      /*---(check)-----------------------*/
      x_mode = ysrc_words__type   (x_type, s_cur->contents [i], x_save);
      if (x_mode == x_save)  continue;
      /*---(update current)--------------*/
      if (x_save == '²' && x_mode == 'w')   ysrc_words__update   (i    , '<');
      if (x_save == '²' && x_mode == 's')   ysrc_words__update   (i    , '<');
      if (x_save == 'w' && x_mode == 's')   ysrc_words__update   (i    , '<');
      if (x_save == 's' && x_mode == 'w')   ysrc_words__update   (i    , '<');
      /*---(update previous)-------------*/
      if (x_save == 'w' && x_mode == '²')   ysrc_words__update   (i - 1, '>'); 
      if (x_save == 's' && x_mode == '²')   ysrc_words__update   (i - 1, '>'); 
      if (x_save == 'w' && x_mode == 's')   ysrc_words__update   (i - 1, '>');
      if (x_save == 's' && x_mode == 'w')   ysrc_words__update   (i - 1, '>');
      /*---(save)------------------------*/
      x_save = x_mode;
      /*---(done)------------------------*/
   }
   /*---(mark end)-----------------------*/
   if (x_type != 'f' && x_mode != '²')  ysrc_words__update   (s_cur->npos - 1, '>');
   DEBUG_EDIT   yLOG_info    ("words"     , s_cur->words);
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}


