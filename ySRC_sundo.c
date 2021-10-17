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
   rc = ysrc_sundo_trim (0);
   /*> rc = yVIKEYS_view_optionX (YVIKEYS_STATUS, "sundo", ysrc_sundo_status, "source editing undo stack");   <*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       change grouping                        ----===*/
/*====================------------------------------------====================*/
static void  o___GROUPING________o () { return; }

char
ysrc_sundo_beg          (void)
{
   DEBUG_EDIT   yLOG_senter  (__FUNCTION__);
   DEBUG_EDIT   yLOG_schar   (yKEYS_repeating   ());
   DEBUG_EDIT   yLOG_sint    (yKEYS_repeat_orig ());
   DEBUG_EDIT   yLOG_sint    (yKEYS_repeats     ());
   DEBUG_EDIT   yLOG_sint    (g_nseq);
   DEBUG_EDIT   yLOG_sint    (yKEYS_normal      ());
   DEBUG_EDIT   yLOG_sint    (yKEYS_unique ());
   if      (g_nseq < 0)                          ++g_nseq;
   else if (yKEYS_normal () && yKEYS_unique ())  ++g_nseq;
   DEBUG_EDIT   yLOG_sint    (g_nseq);
   DEBUG_EDIT   yLOG_sexit   (__FUNCTION__);
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
   DEBUG_EDIT   yLOG_senter  (__FUNCTION__);
   DEBUG_EDIT   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       recording changes                      ----===*/
/*====================------------------------------------====================*/
static void  o___RECORD__________o () { return; }

char
ysrc_sundo_add          (char a_major, char a_minor, short a_pos, char a_before, char a_after)
{
   DEBUG_EDIT   yLOG_senter  (__FUNCTION__);
   ++g_csundo;
   g_sundos [g_csundo].seq    = g_nseq;
   g_sundos [g_csundo].major  = a_major;
   g_sundos [g_csundo].minor  = a_minor;
   g_sundos [g_csundo].cpos   = a_pos;
   g_sundos [g_csundo].before = chrvisible (a_before);
   g_sundos [g_csundo].after  = chrvisible (a_after);
   g_nsundo = g_csundo + 1;
   DEBUG_EDIT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char ysrc_sundo_single       (char a_minor, short a_pos, char a_before, char a_after) { return ysrc_sundo_add (G_CHAR_SPACE, a_minor, a_pos, a_before, a_after); }



/*====================------------------------------------====================*/
/*===----                       undoing actions                        ----===*/
/*====================------------------------------------====================*/
static void  o___UNDO____________o () { return; }

char
ysrc_sundo__undo        (short *a_pos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar       x_major     =  '-';
   uchar       x_minor     =  '-';
   short       x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_EDIT   yLOG_sint    (g_nsundo);
   DEBUG_EDIT   yLOG_sint    (g_csundo);
   --rce;  if (g_nsundo <= 0) {
      DEBUG_EDIT   yLOG_snote   ("stack too small");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (g_csundo <  0) {
      DEBUG_EDIT   yLOG_snote   ("current too small");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_pos == NULL) {
      DEBUG_EDIT   yLOG_snote   ("no position provided");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(master data)--------------------*/
   DEBUG_EDIT   yLOG_snote   ("master");
   x_major = g_sundos [g_csundo].major;
   x_minor = g_sundos [g_csundo].minor;
   x_pos   = g_sundos [g_csundo].cpos;
   DEBUG_EDIT   yLOG_schar   (x_major);
   DEBUG_EDIT   yLOG_schar   (x_minor);
   *a_pos = x_pos;
   DEBUG_EDIT   yLOG_sint    (*a_pos);
   DEBUG_EDIT   yLOG_sint    (s_cur->cpos);
   /*---(position)-----------------------*/
   DEBUG_EDIT   yLOG_snote   ("before");
   DEBUG_EDIT   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   DEBUG_EDIT   yLOG_schar   (g_sundos [g_csundo].after);
   /*---(single char)--------------------*/
   --rce;  if (strchr (" ·", x_major) != NULL) {
      switch (x_minor) {
      case 'r' : case 'R' :
         DEBUG_EDIT   yLOG_snote   ("replace");
         ysrc_replace_one (g_sundos [g_csundo].before);
         break;
      case 'i' : case 'a' :
         DEBUG_EDIT   yLOG_snote   ("insert/append");
         ysrc_delete_one  ();
         break;
      }
   }
   /*---(multi char)---------------------*/
   else if (x_major == 'd') {
      DEBUG_EDIT   yLOG_snote   ("delete");
      ysrc_insert_one  (g_sundos [g_csundo].before);
   }
   else if (x_major == 'x') {
      DEBUG_EDIT   yLOG_snote   ("clear");
      ysrc_replace_one (g_sundos [g_csundo].before);
   }
   /*---(trouble)------------------------*/
   else {
      DEBUG_EDIT   yLOG_snote   ("UNKNOWN");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   DEBUG_EDIT   yLOG_snote   ("after");
   DEBUG_EDIT   yLOG_schar   (g_sundos [g_csundo].before);
   DEBUG_EDIT   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   --g_csundo;
   DEBUG_EDIT   yLOG_sint    (g_csundo);
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_sundo_undo         (short *a_pos)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_seq       =   -1;
   int         i           =    0;
   int         c           =    0;
   int         rc          =    0;
   /*---(prepare)------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   x_seq = g_sundos [g_csundo].seq;
   DEBUG_EDIT   yLOG_value   ("x_seq"     , x_seq);
   for (i = g_csundo; i >= 0; --i) {
      if (g_sundos [g_csundo].seq != x_seq)  break;
      rc = ysrc_sundo__undo (a_pos);
      if (rc < 0) break;
      ++c;
   }
   if (c <= 0)  rc = -1;
   DEBUG_EDIT   yLOG_value   ("rc"        , rc);
   UPDATE_AFTER_CHANGES;
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       redoing actions                        ----===*/
/*====================------------------------------------====================*/
static void  o___REDO____________o () { return; }

char
ysrc_sundo__redo        (short *a_pos)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   uchar       x_major     =  '-';
   uchar       x_minor     =  '-';
   short       x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_EDIT   yLOG_sint    (g_nsundo);
   DEBUG_EDIT   yLOG_sint    (g_csundo);
   --rce;  if (g_csundo < -1) {
      DEBUG_EDIT   yLOG_snote   ("current too small");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (g_csundo >= g_nsundo - 1) {
      DEBUG_EDIT   yLOG_snote   ("current is too big");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_pos == NULL) {
      DEBUG_EDIT   yLOG_snote   ("no position provided");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   ++g_csundo;
   DEBUG_EDIT   yLOG_sint    (g_csundo);
   /*---(master data)--------------------*/
   DEBUG_EDIT   yLOG_snote   ("master");
   x_major = g_sundos [g_csundo].major;
   x_minor = g_sundos [g_csundo].minor;
   x_pos   = g_sundos [g_csundo].cpos;
   DEBUG_EDIT   yLOG_schar   (x_major);
   DEBUG_EDIT   yLOG_schar   (x_minor);
   *a_pos = x_pos;
   DEBUG_EDIT   yLOG_sint    (*a_pos);
   DEBUG_EDIT   yLOG_sint    (s_cur->cpos);
   /*---(position)-----------------------*/
   DEBUG_EDIT   yLOG_snote   ("before");
   DEBUG_EDIT   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   DEBUG_EDIT   yLOG_schar   (g_sundos [g_csundo].before);
   /*---(position)-----------------------*/
   *a_pos = g_sundos [g_csundo].cpos;
   DEBUG_EDIT   yLOG_value   ("cpos"      , *a_pos);
   /*---(single char)--------------------*/
   --rce;  if (strchr (" ·", x_major) != NULL) {
      switch (x_minor) {
      case 'r' : case 'R' :
         DEBUG_EDIT   yLOG_snote   ("replace");
         ysrc_replace_one (g_sundos [g_csundo].after);
         break;
      case 'i' : case 'a' :
         DEBUG_EDIT   yLOG_snote   ("insert/append");
         ysrc_insert_one  (g_sundos [g_csundo].after);
         break;
      }
   }
   /*---(multi char)---------------------*/
   else if (x_major == 'd') {
      DEBUG_EDIT   yLOG_snote   ("delete");
      ysrc_delete_one  ();
   }
   else if (x_major == 'x') {
      DEBUG_EDIT   yLOG_snote   ("delete");
      ysrc_replace_one (g_sundos [g_csundo].after);
   }
   /*---(trouble)------------------------*/
   else {
      DEBUG_EDIT   yLOG_snote   ("UNKNOWN");
      DEBUG_EDIT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   DEBUG_EDIT   yLOG_snote   ("after");
   DEBUG_EDIT   yLOG_schar   (g_sundos [g_csundo].after);
   DEBUG_EDIT   yLOG_schar   (s_cur->contents [s_cur->cpos]);
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_sundo_redo         (short *a_pos)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_seq       =   -1;
   int         i           =    0;
   int         c           =    0;
   int         rc          =    0;
   /*---(prepare)------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   x_seq = g_sundos [g_csundo + 1].seq;
   DEBUG_EDIT   yLOG_value   ("x_seq"     , x_seq);
   for (i = g_csundo; i < g_nsundo; ++i) {
      if (g_sundos [g_csundo + 1].seq != x_seq)  break;
      rc = ysrc_sundo__redo (a_pos);
      if (rc < 0) break;
      ++c;
   }
   if (c <= 0)  rc = -1;
   DEBUG_EDIT   yLOG_value   ("rc"        , rc);
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    status and information                    ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS__________o () { return; }

char
ysrc_sundo_status       (char *a_list)
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
   /*> if (g_nsundo == 0) {                                                           <* 
    *>    sprintf (a_list, "%3dn, %3dc", g_nsundo, g_csundo);                         <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <*/
   /*---(prepare)---------------------*/
   x_end = g_nsundo;
   x_beg = x_end - 10;
   if (x_beg < 0)  x_beg = 0;
   /*---(write line)------------------*/
   /*> sprintf (a_list, "%3dn, %3dc, ", g_nsundo, g_csundo);                          <*/
   for (i = x_beg; i < x_end; ++i) {
      if (g_sundos [i].seq  < 0)  x_seq = '·';
      else                        x_seq = YSTR_CHARS [g_sundos [i].seq];
      if (g_sundos [i].cpos < 0)  x_pos = '·';
      else                        x_pos = YSTR_CHARS [g_sundos [i].cpos];
      sprintf (t, "%c%c%c%c%c%c,", x_seq, g_sundos [i].major, g_sundos [i].minor, x_pos, g_sundos [i].before, g_sundos [i].after);
      /*> x_ch = g_sundos [i].major;                                                                                                                                        <* 
       *> if (x_ch != G_KEY_SPACE)  sprintf (t, "%d%c%c%d%c%c,", g_sundos [i].seq, x_ch, g_sundos [i].minor, g_sundos [i].cpos, g_sundos [i].before, g_sundos [i].after);   <* 
       *> else                      sprintf (t, "%d%c%d%c%c,"  , g_sundos [i].seq,       g_sundos [i].minor, g_sundos [i].cpos, g_sundos [i].before, g_sundos [i].after);   <*/
      strlcat (a_list, t, LEN_RECD);
   }
   /*---(complete)--------------------*/
   return 0;
}
