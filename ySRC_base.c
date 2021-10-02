/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



uchar     *g_stub   = "";



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        ySRC_ver [200] = "";

char*        /*--> return library versioning info --------[ leaf-- [ ------ ]-*/
ySRC_version            (void)
{
   char        t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", 18);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (ySRC_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return ySRC_ver;
}

char
ySRC_init               (void)
{
   ysrc_sundo_trim (0);
   return 0;
}

char
ySRC_wrap               (void)
{
   ysrc_sundo_trim (0);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      start and stop                          ----===*/
/*====================------------------------------------====================*/
static void      o___ENDS____________________o (void) {;}

char         /*-> prepare contents for new edit ------[ leaf   [gz.311.101.00]*/ /*-[00.0000.203.!]-*/ /*-[--.---.---.--]-*/
SOURCE_start       (char *a_prefix)
{
   /*> /+---(locals)-----------+-----+-----+-+/                                       <* 
    *> char        rce         =  -10;                                                <* 
    *> /+---(header)-------------------------+/                                       <* 
    *> DEBUG_EDIT   yLOG_enter   (__FUNCTION__);                                      <* 
    *> /+---(defense)------------------------+/                                       <* 
    *> DEBUG_EDIT   yLOG_point   ("a_prefix"  , a_prefix);                            <* 
    *> --rce;  if (a_prefix == NULL) {                                                <* 
    *>    DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <* 
    *> DEBUG_EDIT   yLOG_info    ("a_prefix"  , a_prefix);                            <* 
    *> /+---(select mode)--------------------+/                                       <* 
    *> switch (a_prefix [0]) {                                                        <* 
    *> case ':' :                                                                     <* 
    *>    s_cur = &s_cmd;                                                             <* 
    *>    yMODE_enter  (MODE_COMMAND);                                                <* 
    *>    break;                                                                      <* 
    *> case ';' :                                                                     <* 
    *>    s_cur = &s_cmd;                                                             <* 
    *>    yMODE_enter  (SMOD_HINT);                                                   <* 
    *>    break;                                                                      <* 
    *> case '/' :                                                                     <* 
    *>    s_cur = &s_cmd;                                                             <* 
    *>    yMODE_enter  (MODE_SEARCH );                                                <* 
    *>    break;                                                                      <* 
    *> case '�' :                                                                     <* 
    *>    s_cur = &s_src;                                                             <* 
    *>    yMODE_enter  (MODE_SOURCE );                                                <* 
    *>    break;                                                                      <* 
    *> default  :                                                                     <* 
    *>    s_cur = &s_src;                                                             <* 
    *>    yMODE_enter  (MODE_SOURCE );                                                <* 
    *>    break;                                                                      <* 
    *> }                                                                              <* 
    *> /+---(clear sundo)--------------------+/                                       <* 
    *> yvikeys_sundo_purge (0);                                                       <* 
    *> /+---(populate globals)---------------+/                                       <* 
    *> SOURCE_menu_prep ();                                                           <* 
    *> if (a_prefix [0] != '�')  strlcpy (s_cur->contents, a_prefix , LEN_RECD);      <* 
    *> s_cur->npos = strllen (s_cur->contents, LEN_RECD);                             <* 
    *> s_cur->bpos = s_cur->cpos = 0;                                                 <* 
    *> s_cur->epos = s_cur->npos;                                                     <* 
    *> SOURCE__done ();                                                               <* 
    *> /+---(go info input)------------------+/                                       <* 
    *> if (a_prefix [0] != '�') {                                                     <* 
    *>    yMODE_enter  (UMOD_INPUT);                                                  <* 
    *>    SRC_INPT_umode  ('m', 'a');                                                 <* 
    *> }                                                                              <* 
    *> /+---(complete)-----------------------+/                                       <* 
    *> DEBUG_EDIT   yLOG_exit    (__FUNCTION__);                                      <* 
    *> return 0;                                                                      <*/
}

char         /*-> complete a source mode move --------[ leaf   [gz.E45.001.A0]*/ /*-[02.0000.513.!]-*/ /*-[--.---.---.--]-*/
SOURCE__done            (void)
{
   /*---(prepare)------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT   yLOG_info    ("s_contents", s_cur->contents);
   s_cur->npos     = strlen (s_cur->contents);
   /*---(display debugging)--------------*/
   DEBUG_EDIT   yLOG_value   ("s_npos"   , s_cur->npos);
   DEBUG_EDIT   yLOG_value   ("s_apos"   , s_cur->apos);
   DEBUG_EDIT   yLOG_value   ("s_bpos"   , s_cur->bpos);
   DEBUG_EDIT   yLOG_value   ("s_cpos"   , s_cur->cpos);
   DEBUG_EDIT   yLOG_value   ("s_epos"   , s_cur->epos);
   /*---(check over/underrun)---------*/
   DEBUG_EDIT   yLOG_note    ("correct s_npos over/underruns");
   if (s_cur->npos  >= LEN_RECD) {
      s_cur->npos = LEN_RECD - 1;
      s_cur->contents [s_cur->npos] = '\0';
   }
   if (s_cur->npos < 0) {
      s_cur->npos = 0;
   }
   /*---(check min/max)---------------*/
   DEBUG_EDIT   yLOG_note    ("correct min/max limits");
   if (s_cur->cpos >=  s_cur->npos)     s_cur->cpos = s_cur->npos - 1;
   if (s_cur->cpos <   0      )    s_cur->cpos = 0;
   if (s_cur->bpos <   0      )    s_cur->bpos = 0;
   DEBUG_EDIT   yLOG_complex ("limits"    , "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(small strings)---------------*/
   if (s_cur->npos == 0) {
      DEBUG_EDIT   yLOG_note    ("check and handle small string");
      s_cur->cpos = s_cur->bpos = s_cur->epos = 0;
   }
   /*---(small strings)---------------*/
   else if (s_cur->npos <= s_cur->apos) {
      DEBUG_EDIT   yLOG_note    ("check and handle small string");
      s_cur->bpos = 0;
      s_cur->epos = s_cur->npos - 1;
   }
   /*---(long strings)----------------*/
   else {
      DEBUG_EDIT   yLOG_note    ("check and handle long string");
      s_cur->epos = s_cur->bpos + s_cur->apos - 1;
      /*---(check off left side)------*/
      DEBUG_EDIT   yLOG_note    ("check off left side of screen");
      if (s_cur->cpos <   s_cur->bpos)  {
         s_cur->bpos = s_cur->cpos;
         s_cur->epos = s_cur->bpos + s_cur->apos - 1;
      }
      /*---(check off right side)-----*/
      DEBUG_EDIT   yLOG_note    ("check off right side of screen");
      if (s_cur->cpos >   s_cur->epos)  {
         s_cur->epos = s_cur->cpos;
         s_cur->bpos = s_cur->epos - s_cur->apos + 1;
      }
      /*---(check scrolling-----------*/
      DEBUG_EDIT   yLOG_note    ("make sure right is locked to end");
      if (s_cur->epos >=  s_cur->npos)  {
         s_cur->epos = s_cur->npos - 1;
         s_cur->bpos = s_cur->epos - s_cur->apos + 1;
      }
   }
   DEBUG_EDIT   yLOG_complex ("post-limit", "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(selection update)---------------*/
   ysrc_select_update (s_cur->cpos);
   DEBUG_EDIT   yLOG_complex ("post-sreg" , "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(update word breaks)-------------*/
   ysrc_words ();
   DEBUG_EDIT   yLOG_complex ("post-words", "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(display debugging)--------------*/
   DEBUG_EDIT   yLOG_value   ("s_npos"   , s_cur->npos);
   DEBUG_EDIT   yLOG_value   ("s_apos"   , s_cur->apos);
   DEBUG_EDIT   yLOG_value   ("s_bpos"   , s_cur->bpos);
   DEBUG_EDIT   yLOG_value   ("s_cpos"   , s_cur->cpos);
   DEBUG_EDIT   yLOG_value   ("s_epos"   , s_cur->epos);
   /*---(complete)--------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> accept all source changes ----------[ shoot  [gz.210.001.01]*/ /*-[00.0000.102.4]-*/ /*-[--.---.---.--]-*/
ysrc_accept             (void)
{
   /*> char        rc          =    0;                                                <* 
    *> char        t           [LEN_RECD] = "";                                       <* 
    *> DEBUG_EDIT   yLOG_enter   (__FUNCTION__);                                      <* 
    *> switch (yMODE_curr ()) {                                                       <* 
    *> case MODE_SOURCE  :                                                            <* 
    *>    DEBUG_EDIT   yLOG_note    ("save source back");                             <* 
    *>    if (s_saver != NULL && s_cur->npos > 0) {                                   <* 
    *>       strlcpy (t, s_cur->contents, LEN_RECD);                                  <* 
    *>       strldchg (t, G_CHAR_STORAGE, G_KEY_SPACE, LEN_RECD);                     <* 
    *>       s_saver (t);                                                             <* 
    *>    }                                                                           <* 
    *>    break;                                                                      <* 
    *> case MODE_COMMAND :                                                            <* 
    *>    DEBUG_EDIT   yLOG_note    ("execute command");                              <* 
    *>    rc = yVIKEYS_cmds_direct (s_cur->contents);                                 <* 
    *>    strlcpy (s_cur->contents, "", LEN_RECD);                                    <* 
    *>    break;                                                                      <* 
    *> case SMOD_HINT    :                                                            <* 
    *>    DEBUG_EDIT   yLOG_note    ("execute hint");                                 <* 
    *>    rc = yvikeys_hint_direct (s_cur->contents);                                 <* 
    *>    strlcpy (s_cur->contents, "", LEN_RECD);                                    <* 
    *>    break;                                                                      <* 
    *> case MODE_SEARCH  :                                                            <* 
    *>    DEBUG_EDIT   yLOG_note    ("execute search");                               <* 
    *>    rc = yvikeys_srch_direct (s_cur->contents);                                 <* 
    *>    strlcpy (s_cur->contents, "", LEN_RECD);                                    <* 
    *>    break;                                                                      <* 
    *> }                                                                              <* 
    *> DEBUG_EDIT   yLOG_value   ("rc"        , rc);                                  <* 
    *> strlcpy (s_cur->original, s_cur->contents, LEN_RECD);                          <* 
    *> s_cur->npos  = s_cur->bpos  = s_cur->cpos  = s_cur->epos  = 0;                 <* 
    *> yvikeys_sreg_reset  (0);                                                       <* 
    *> yvikeys_sundo_purge (0);                                                       <* 
    *> yVIKEYS_source (s_src.label, s_src.original);                                  <* 
    *> yvikeys_map_reposition  ();                                                    <* 
    *> DEBUG_EDIT   yLOG_value   ("rc"        , rc);                                  <* 
    *> DEBUG_EDIT   yLOG_exit    (__FUNCTION__);                                      <* 
    *> return rc;                                                                     <*/
}

char         /*-> reject all source changes ----------[ shoot  [gz.210.001.01]*/ /*-[00.0000.102.4]-*/ /*-[--.---.---.--]-*/
ysrc_reset              (void)
{
   /*> switch (yMODE_curr ()) {                                                                                               <* 
    *> case MODE_COMMAND :                                                                                                    <* 
    *> case MODE_SEARCH  :                                                                                                    <* 
    *> case SMOD_HINT    :                                                                                                    <* 
    *>    strlcpy (s_cur->original, "", LEN_RECD);                                                                            <* 
    *>    break;                                                                                                              <* 
    *> }                                                                                                                      <* 
    *> strlcpy (s_cur->contents, s_cur->original, LEN_RECD);                                                                  <* 
    *> s_cur->npos  = s_cur->bpos  = s_cur->cpos  = s_cur->epos  = 0;                                                         <* 
    *> DEBUG_EDIT   yLOG_complex ("reset"     , "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);   <* 
    *> yvikeys_sreg_reset  (0);                                                                                               <* 
    *> yvikeys_sundo_purge (0);                                                                                               <* 
    *> yVIKEYS_source (s_src.label, s_src.original);                                                                          <* 
    *> yvikeys_map_reposition  ();                                                                                            <* 
    *> DEBUG_EDIT   yLOG_complex ("post-reset", "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);   <* 
    *> return 0;                                                                                                              <*/
}


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char           unit_answer [LEN_FULL];

char       /*----: set up program urgents/debugging --------------------------*/
ysrc__unit_quiet        (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"ySRC_unit" };
   /*> yURG_logger   (x_narg, x_args);                                                <*/
   /*> yURG_urgs     (x_narg, x_args);                                                <*/
   ySRC_init ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysrc__unit_loud         (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"ySRC_unit" };
   yURG_logger   (x_narg, x_args);
   yURG_urgs     (x_narg, x_args);
   yURG_name  ("kitchen"      , YURG_ON);
   yURG_name  ("edit"         , YURG_ON);
   yURG_name  ("ystr"         , YURG_ON);
   DEBUG_MODE  yLOG_info     ("ySRC"      , ySRC_version   ());
   ySRC_init ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ysrc__unit_end          (void)
{
   DEBUG_MODE  yLOG_enter   (__FUNCTION__);
   ySRC_wrap    ();
   DEBUG_MODE  yLOG_exit    (__FUNCTION__);
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
   /*---(preprare)-----------------------*/
   strcpy  (unit_answer, "SRC unit         : question not understood");
   /*---(selection)----------------------*/
   if      (strcmp (a_question, "undo"           )   == 0) {
      ysrc_sundo_status (t);
      snprintf (unit_answer, LEN_FULL, "SRC undo (%2d/%2d) : �%s�", g_nsundo, g_csundo, t);
   }
   else if (strcmp (a_question, "selection"      )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SREG selection   :    %c, %3db, %3de, %3d#, %3dr", g_sreg.active, g_sreg.beg, g_sreg.end, g_sreg.end - g_sreg.beg + 1, g_sreg.root);
      return unit_answer;
   }
   else if (strcmp (a_question, "register"       )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SREG register    : %c  %c", g_csreg, g_wsreg);
      return unit_answer;
   }
   /*> else if (strcmp (a_question, "clip"           )   == 0) {                             <* 
    *>    yvikeys_dump_read (n, t, &x_len);                                                  <* 
    *>    snprintf (unit_answer, LEN_RECD, "SREG clip   (%2d) : %2d[%.40s]", n, x_len, t);   <* 
    *>    return unit_answer;                                                                <* 
    *> }                                                                                     <*/
   /*---(complex)------------------------*/
   n = ysrc_sreg_index  (n);
   if (n < 0) {
      strlcpy  (unit_answer, "SREG unit        : not a valid register name", LEN_FULL);
      return unit_answer;
   }
   if      (strcmp (a_question, "saved"          )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SREG saved   (%c) : %c %3d[%.40s]", n, g_sregs [n].active, g_sregs [n].len, g_sregs [n].data);
   }
   if      (strcmp (a_question, "source"         )   == 0) {
      snprintf (unit_answer, LEN_FULL, "SREG source  (%c) : %c  %c  %-12.12s  %3db  %3de  %3dr", n, g_sregs [n].active, g_sregs [n].source, g_sregs [n].label, g_sregs [n].beg, g_sregs [n].end, g_sregs [n].root);
   }

   /*---(complete)-----------------------*/
   return unit_answer;
}


