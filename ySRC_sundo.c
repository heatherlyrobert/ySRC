/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



tSUNDO g_sundos  [S_SUNDO_MAX];
int    g_nsundo    =  0;
int    g_csundo    = -1;
int    g_nseq      = -1;



/*====================------------------------------------====================*/
/*===----                         program level                        ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
ysrc_sundo_trim         (int a_start)
{
   int         i           =    0;
   for (i = a_start; i < S_SUNDO_MAX; ++i) {
      g_sundos [i].seq    = -1;
      g_sundos [i].major  = '-';
      g_sundos [i].minor  = '-';
      g_sundos [i].cpos   = -1;
      g_sundos [i].before = '-';
      g_sundos [i].after  = '-';
   }
   g_nsundo =  a_start;
   g_csundo =  a_start - 1;
   if (a_start == 0)   g_nseq   = -1;
   return 0;
}

char ysrc_sundo_reset        (void) { return ysrc_sundo_trim (0); }


char
ysrc_sundo_init         (void)
{
   char        rc          =    0;
   rc = ysrc_sundo_reset ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       change grouping                        ----===*/
/*====================------------------------------------====================*/
static void  o___GROUPING________o () { return; }

char
ysrc_sundo_beg          (void)
{
   char n = yKEYS_normal ();
   char u = yKEYS_unique ();
   char b = yKEYS_repeat_beg ();
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   DEBUG_YSRC   yLOG_sint    (yKEYS_repeating   ());
   DEBUG_YSRC   yLOG_sint    (yKEYS_repeat_orig ());
   DEBUG_YSRC   yLOG_sint    (yKEYS_repeats     ());
   if (g_csundo == -1)  g_nseq = -1;
   DEBUG_YSRC   yLOG_sint    (g_nseq);
   DEBUG_YSRC   yLOG_svalue  ("n", n);
   DEBUG_YSRC   yLOG_svalue  ("u", u);
   DEBUG_YSRC   yLOG_svalue  ("b", b);
   if (b) {
      if      (g_nseq < 0)  ++g_nseq;
      else if (n && u)      ++g_nseq;
   }
   DEBUG_YSRC   yLOG_sint    (g_nseq);
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_sundo_chain        (void)
{
   --g_nseq;
   return 0;
}


char
ysrc_sundo_end          (void)
{
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       recording changes                      ----===*/
/*====================------------------------------------====================*/
static void  o___RECORD__________o () { return; }

char
ysrc_sundo_add          (char a_major, char a_minor, short a_pos, char a_before, char a_after)
{
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   ++g_csundo;
   DEBUG_YSRC   yLOG_sint    (g_csundo);
   DEBUG_YSRC   yLOG_sint    (g_nseq);
   DEBUG_YSRC   yLOG_schar   (a_major);
   DEBUG_YSRC   yLOG_schar   (a_minor);
   DEBUG_YSRC   yLOG_sint    (a_pos);
   DEBUG_YSRC   yLOG_schar   (a_before);
   DEBUG_YSRC   yLOG_schar   (a_after);
   g_sundos [g_csundo].seq    = g_nseq;
   g_sundos [g_csundo].major  = a_major;
   g_sundos [g_csundo].minor  = a_minor;
   g_sundos [g_csundo].cpos   = a_pos;
   if (a_before != G_KEY_SPACE)  a_before = chrvisible (a_before);
   g_sundos [g_csundo].before = a_before;
   if (a_after  != G_KEY_SPACE)  a_after  = chrvisible (a_after);
   g_sundos [g_csundo].after  = a_after;
   g_nsundo = g_csundo + 1;
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char ysrc_sundo_single       (char a_minor, short a_pos, char a_before, char a_after) { return ysrc_sundo_add (G_CHAR_SPACE, a_minor, a_pos, a_before, a_after); }



/*====================------------------------------------====================*/
/*===----                       undoing actions                        ----===*/
/*====================------------------------------------====================*/
static void  o___UNDO____________o () { return; }

char
ysrc_sundo__undo        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar       x_major     =  '-';
   uchar       x_minor     =  '-';
   short       x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_sint    (g_nsundo);
   DEBUG_YSRC   yLOG_sint    (g_csundo);
   --rce;  if (g_nsundo <= 0) {
      DEBUG_YSRC   yLOG_snote   ("stack too small");
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (g_csundo <  0) {
      DEBUG_YSRC   yLOG_snote   ("current too small");
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(master data)--------------------*/
   DEBUG_YSRC   yLOG_snote   ("master");
   x_major = g_sundos [g_csundo].major;
   x_minor = g_sundos [g_csundo].minor;
   x_pos   = g_sundos [g_csundo].cpos;
   DEBUG_YSRC   yLOG_schar   (x_major);
   DEBUG_YSRC   yLOG_schar   (x_minor);
   /*---(position)-----------------------*/
   DEBUG_YSRC   yLOG_snote   ("before");
   s_cur->cpos = x_pos;
   DEBUG_YSRC   yLOG_sint    (s_cur->cpos);
   DEBUG_YSRC   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   DEBUG_YSRC   yLOG_schar   (g_sundos [g_csundo].after);
   /*---(single char)--------------------*/
   --rce;  if (strchr (" ·", x_major) != NULL) {
      switch (x_minor) {
      case 'r' : case 'R' :
         DEBUG_YSRC   yLOG_snote   ("replace");
         ysrc_replace_one (g_sundos [g_csundo].before);
         break;
      case 'i' : case 'a' :
         DEBUG_YSRC   yLOG_snote   ("insert/append");
         ysrc_delete_one  ();
         if (x_minor == 'a')  --s_cur->cpos;
         break;
      }
   }
   /*---(multi char)---------------------*/
   else if (x_major == 'd') {
      DEBUG_YSRC   yLOG_snote   ("delete");
      ysrc_insert_one  (g_sundos [g_csundo].before);
   }
   else if (x_major == 'x') {
      DEBUG_YSRC   yLOG_snote   ("clear");
      ysrc_replace_one (g_sundos [g_csundo].before);
   }
   /*---(trouble)------------------------*/
   else {
      DEBUG_YSRC   yLOG_snote   ("UNKNOWN");
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   DEBUG_YSRC   yLOG_snote   ("after");
   DEBUG_YSRC   yLOG_schar   (g_sundos [g_csundo].before);
   DEBUG_YSRC   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   --g_csundo;
   DEBUG_YSRC   yLOG_sint    (g_csundo);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_sundo_undo         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_seq       =   -1;
   int         i           =    0;
   int         c           =    0;
   int         rc          =    0;
   /*---(prepare)------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   x_seq = g_sundos [g_csundo].seq;
   DEBUG_YSRC   yLOG_value   ("x_seq"     , x_seq);
   for (i = g_csundo; i >= 0; --i) {
      if (g_sundos [g_csundo].seq != x_seq)  break;
      rc = ysrc_sundo__undo ();
      if (rc < 0) break;
      ++c;
   }
   if (c <= 0)  rc = -1;
   DEBUG_YSRC   yLOG_value   ("rc"        , rc);
   UPDATE_AFTER_CHANGES;
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       redoing actions                        ----===*/
/*====================------------------------------------====================*/
static void  o___REDO____________o () { return; }

char
ysrc_sundo__redo        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar       x_major     =  '-';
   uchar       x_minor     =  '-';
   short       x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_sint    (g_nsundo);
   DEBUG_YSRC   yLOG_sint    (g_csundo);
   --rce;  if (g_csundo < -1) {
      DEBUG_YSRC   yLOG_snote   ("current too small");
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (g_csundo >= g_nsundo - 1) {
      DEBUG_YSRC   yLOG_snote   ("current is too big");
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   ++g_csundo;
   DEBUG_YSRC   yLOG_sint    (g_csundo);
   /*---(master data)--------------------*/
   DEBUG_YSRC   yLOG_snote   ("master");
   x_major = g_sundos [g_csundo].major;
   x_minor = g_sundos [g_csundo].minor;
   x_pos   = g_sundos [g_csundo].cpos;
   DEBUG_YSRC   yLOG_schar   (x_major);
   DEBUG_YSRC   yLOG_schar   (x_minor);
   /*---(position)-----------------------*/
   DEBUG_YSRC   yLOG_snote   ("before");
   s_cur->cpos = x_pos;
   DEBUG_YSRC   yLOG_sint    (s_cur->cpos);
   DEBUG_YSRC   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   DEBUG_YSRC   yLOG_schar   (g_sundos [g_csundo].before);
   /*---(single char)--------------------*/
   --rce;  if (strchr (" ·", x_major) != NULL) {
      switch (x_minor) {
      case 'r' : case 'R' :
         DEBUG_YSRC   yLOG_snote   ("replace");
         ysrc_replace_one (g_sundos [g_csundo].after);
         break;
      case 'i' : case 'a' :
         DEBUG_YSRC   yLOG_snote   ("insert/append");
         ysrc_insert_one  (g_sundos [g_csundo].after);
         break;
      }
   }
   /*---(multi char)---------------------*/
   else if (x_major == 'd') {
      DEBUG_YSRC   yLOG_snote   ("delete right");
      ysrc_delete_one  ();
      if (x_minor == 'h' && s_cur->cpos != s_cur->npos - 1)  --s_cur->cpos;
   }
   else if (x_major == 'x') {
      DEBUG_YSRC   yLOG_snote   ("delete");
      ysrc_replace_one (g_sundos [g_csundo].after);
   }
   /*---(trouble)------------------------*/
   else {
      DEBUG_YSRC   yLOG_snote   ("UNKNOWN");
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   DEBUG_YSRC   yLOG_snote   ("after");
   DEBUG_YSRC   yLOG_schar   (g_sundos [g_csundo].after);
   DEBUG_YSRC   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_sundo_redo         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_seq       =   -1;
   int         i           =    0;
   int         c           =    0;
   int         rc          =    0;
   /*---(prepare)------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   x_seq = g_sundos [g_csundo + 1].seq;
   DEBUG_YSRC   yLOG_value   ("x_seq"     , x_seq);
   for (i = g_csundo; i < g_nsundo - 1; ++i) {
      if (g_sundos [g_csundo + 1].seq != x_seq)  break;
      rc = ysrc_sundo__redo ();
      if (rc < 0) break;
      ++c;
   }
   if (c <= 0)  rc = -1;
   DEBUG_YSRC   yLOG_value   ("rc"        , rc);
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    status and information                    ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS__________o () { return; }

char
ysrc_sundo_status       (char a_size, short a_wide, char *a_list)
{
   char        rce         =  -10;
   int         i           =    0;
   int         x_beg       =    0;
   int         x_end       =    0;
   char        t           [LEN_LABEL] = "";
   char        x_ch        =  ' ';
   uchar       x_seq       =  '·';
   uchar       x_pos       =  '·';
   /*---(defenses)--------------------*/
   --rce;  if (a_list  == NULL) return rce;
   strcpy (a_list, "");
   /*---(fast path)-------------------*/
   if (g_nsundo == 0) {
      sprintf (a_list, " sundo   (empty) Ï", g_nsundo);
      return 0;
   }
   /*---(prepare)---------------------*/
   x_end = g_nsundo;
   x_beg = x_end - 10;
   if (x_beg < 0)  x_beg = 0;
   /*---(write line)------------------*/
   sprintf (a_list, " sundo   %3dn, %3dc, ", g_nsundo, g_csundo);
   for (i = x_beg; i < x_end; ++i) {
      if (g_sundos [i].seq  < 0)  x_seq = '·';
      else                        x_seq = YSTR_CHARS [g_sundos [i].seq];
      if (g_sundos [i].cpos < 0)  x_pos = '·';
      else                        x_pos = YSTR_CHARS [g_sundos [i].cpos];
      sprintf (t, "%c%c%c%c%c%c,", x_seq, g_sundos [i].major, g_sundos [i].minor, x_pos, g_sundos [i].before, g_sundos [i].after);
      /*> x_ch = g_sundos [i].major;                                                                                                                                        <* 
       *> if (x_ch != G_KEY_SPACE)  sprintf (t, "%d%c%c%d%c%c,", g_sundos [i].seq, x_ch, g_sundos [i].minor, g_sundos [i].cpos, g_sundos [i].before, g_sundos [i].after);   <* 
       *> else                      sprintf (t, "%d%c%d%c%c,"  , g_sundos [i].seq,       g_sundos [i].minor, g_sundos [i].cpos, g_sundos [i].before, g_sundos [i].after);   <*/
      ystrlcat (a_list, t, LEN_RECD);
   }
   ystrlcat (a_list, "Ï", LEN_RECD);
   /*---(complete)--------------------*/
   return 0;
}
