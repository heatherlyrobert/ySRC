/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char           unit_answer [LEN_FULL];

char       /*----: set up program urgents/debugging --------------------------*/
ysrc__unit_quiet        (void)
{
   char        rc           =    0;
   int         x_narg       = 1;
   char       *x_args [20]  = {"ySRC_unit" };
   /*> yURG_logger   (x_narg, x_args);                                                <*/
   /*> yURG_urgs     (x_narg, x_args);                                                <*/
   rc = yMODE_init  (MODE_MAP);
   rc = yMODE_init_after ();
   yKEYS_unit_handlers ();
   yKEYS_init  ();
   yKEYS_init_after ();
   ySRC_init   ();
   ySRC_init_after  ();
   ySRC_config (NULL, NULL);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysrc__unit_loud         (void)
{
   char        rc           =    0;
   int         x_narg       = 1;
   char       *x_args [20]  = {"ySRC_unit" };
   yURG_logger   (x_narg, x_args);
   yURG_urgs     (x_narg, x_args);
   yURG_by_name  ("kitchen"      , YURG_ON);
   yURG_by_name  ("edit"         , YURG_ON);
   yURG_by_name  ("ystr"         , YURG_ON);
   yURG_by_name  ("yvihub"       , YURG_ON);
   yURG_by_name  ("ymode"        , YURG_ON);
   yURG_by_name  ("ykeys"        , YURG_ON);
   yURG_by_name  ("ymap"         , YURG_ON);
   yURG_by_name  ("ysrc"         , YURG_ON);
   DEBUG_YSRC  yLOG_info     ("ySRC"      , ySRC_version   ());
   yMODE_init (MODE_MAP);
   rc = yMODE_init_after ();
   yKEYS_unit_handlers ();
   yKEYS_init  ();
   yKEYS_init_after ();
   ySRC_init   ();
   ySRC_init_after  ();
   ySRC_config (NULL, NULL);
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ysrc__unit_end          (void)
{
   DEBUG_YSRC  yLOG_enter   (__FUNCTION__);
   ySRC_wrap    ();
   DEBUG_YSRC  yLOG_exit    (__FUNCTION__);
   yLOGS_end    ();
   return 0;
}

char*        /*-> unit test accessor -----------------[ light  [us.420.111.11]*/ /*-[01.0000.00#.Z]-*/ /*-[--.---.---.--]-*/
ySRC__unit              (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        r           [LEN_FULL]  = "";
   char        s           [LEN_FULL]  = "";
   char        t           [LEN_FULL]  = "";
   int         x_len       =    0;
   uchar       x_abbr      =  '-';
   uchar       x_char      =  '-';
   /*---(preprare)-----------------------*/
   strcpy  (unit_answer, "SRC unit         : question not understood");
   /*---(selection)----------------------*/
   if      (strcmp (a_question, "undo"           )   == 0) {
      ysrc_sundo_status (0, 0, t);
      snprintf (unit_answer, LEN_FULL, "SRC undo (%2d/%2d) : å%sæ", g_nsundo, g_csundo, t);
      return unit_answer;
   }
   else if (strcmp (a_question, "selection"      )   == 0) {
      /*> ystrlcpy (t, s_cur->contents + g_sreg.beg, g_sreg.end - g_sreg.beg + 2);     <*/
      /*> t [g_sreg.root - g_sreg.beg] = 'Ï';                                         <*/
      /*> snprintf (unit_answer, LEN_FULL, "SRC selection    : %c %2db %2de %2d# %2dr å%sæ", g_sreg.active, g_sreg.beg, g_sreg.end, g_sreg.end - g_sreg.beg + 1, g_sreg.root, t);   <*/
      ysrc_select_line (t);
      snprintf (unit_answer, LEN_FULL, "SRC selection    : %s", t);
      return unit_answer;
   }
   else if (strcmp (a_question, "inventory"      )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SRC reg inven    : %c  %c  å%sæ", g_csreg, g_wsreg, G_SREG_LIST);
      return unit_answer;
   }
   else if (strcmp (a_question, "contents"       )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SRC contents     : å%-.80sæ", s_cur->contents);
      return unit_answer;
   }
   else if (strcmp (a_question, "words"          )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SRC words        : å%-.80sæ", s_cur->words);
      return unit_answer;
   }
   else if (strcmp (a_question, "details"        )   == 0) {
      sprintf (s, "å%-.10sæ", s_cur->label);
      sprintf (t, "%3då%-.40sæ", s_cur->npos, s_cur->contents);
      snprintf (unit_answer, LEN_FULL, "SRC details      : %3dw %3da %3db %3dc %3de %-12.12s %s", s_cur->wide, s_cur->apos, s_cur->bpos, s_cur->cpos, s_cur->epos, s, t);
      return unit_answer;
   }
   else if (strcmp (a_question, "display"        )   == 0) {
      if (strlen (s_cur->contents) > 0) {
         ystrlcpy (t, s_cur->contents + s_cur->bpos, s_cur->apos + 1);
         x_char = chrvisible (t [s_cur->cpos - s_cur->bpos]);
         t [s_cur->cpos - s_cur->bpos] = 'Ï';
      } else {
         x_char = G_CHAR_NULL;
      }
      snprintf (unit_answer, LEN_FULL, "SRC display (%2d) : %c å%sæ", s_cur->apos, x_char, t);
      return unit_answer;
   }
   else if (strcmp (a_question, "source"         )   == 0) {
      ysrc__source_unit (t);
      snprintf (unit_answer, LEN_FULL, "SRC source       : %s", t);
      return unit_answer;
   }
   else if (strcmp (a_question, "input"          )   == 0) {
      ysrc__input_unit (t);
      snprintf (unit_answer, LEN_FULL, "SRC input        : %s", t);
      return unit_answer;
   }
   else if (strcmp (a_question, "replace"        )   == 0) {
      ysrc__replace_unit (t);
      snprintf (unit_answer, LEN_FULL, "SRC replace      : %s", t);
      return unit_answer;
   }
   else if (strcmp (a_question, "set"            )   == 0) {
      if (n < 0 || n > 3) {
         snprintf (unit_answer, LEN_FULL, "SRC set          : unknown section");
      } else {
         sprintf (t, "%-64.64s", mySRC.s_set + n * 64);
         snprintf (unit_answer, LEN_FULL, "SRC set      (%d) : %s", n, t);
      }
      return unit_answer;
   }
   else if (strcmp (a_question, "raw_set"        )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SRC raw set      : %2då%sæ", strlen (mySRC.s_text), mySRC.s_text);
      return unit_answer;
   }
   /*---(complex)------------------------*/
   x_abbr = n;
   n = ysrc_sreg_index  (x_abbr);
   if (n < 0) {
      ystrlcpy  (unit_answer, "SRC sreg         : not a valid register name", LEN_FULL);
      return unit_answer;
   }
   if      (strcmp (a_question, "saved"          )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SRC reg save (%c) : %c %3d[%.40s]", n, g_sregs [n].active, g_sregs [n].len, g_sregs [n].data);
   }
   if      (strcmp (a_question, "register"       )   == 0) {
      /*> sprintf (r, "å%.10sæ", g_sregs [n].label);                                  <*/
      /*> sprintf (t, "%2då%.40sæ", g_sregs [n].len, g_sregs [n].data);               <*/
      /*> snprintf (unit_answer, LEN_FULL, "SRC reg   (%c/%2d) : %c %c %-12.12s %2d %2d %2d %s", x_abbr, n, g_sregs [n].active, g_sregs [n].source, r, g_sregs [n].beg, g_sregs [n].end, g_sregs [n].root, t);   <*/
      ysrc_sreg_line (x_abbr, t);
      snprintf (unit_answer, LEN_FULL, "SRC reg      (%c) : %s", t [0], t + 2);
   }

   /*---(complete)-----------------------*/
   return unit_answer;
}

