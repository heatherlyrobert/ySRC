/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


tMY         mySRC;

uchar     *g_stub   = "";
char    (*s_saver) (char *a_contents);
static    uchar s_format [LEN_LABEL] = "";



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
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_prep  (MODE_SOURCE)) {
      DEBUG_YSRC   yLOG_note    ("status is not ready for init");
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to yVIHUB)-----------------*/
   yVIHUB_from_ySRC (ySRC_select_islive, ySRC_start, ySRC_size, ySRC_push, ysrc_swap);
   /*---(pointer)------------------------*/
   DEBUG_YSRC   yLOG_note    ("clear s_saver pointer");
   s_saver  = NULL;
   /*---(command/search)-----------------*/
   DEBUG_YSRC   yLOG_note    ("clear command/search values");
   s_cur = &s_cmd;
   s_cur->type  = EDIT_CMDS;
   strlcpy (s_cur->original, "" , LEN_RECD );
   strlcpy (s_cur->label   , "-", LEN_LABEL);
   ysrc_reset ();
   /*---(source)-------------------------*/
   DEBUG_YSRC   yLOG_note    ("clear source values");
   s_cur = &s_src;
   s_cur->type  = EDIT_NORM;
   strlcpy (s_cur->original, "" , LEN_RECD );
   strlcpy (s_cur->label   , "-", LEN_LABEL);
   ysrc_reset ();
   /*---(clear)--------------------------*/
   ysrc_sundo_reset ();
   /*---(update status)------------------*/
   yMODE_init_set   (MODE_SOURCE , NULL                , ySRC_mode);
   DEBUG_YSRC   yLOG_info    ("source"    , yMODE_actual (MODE_SOURCE));
   yMODE_init_set   (UMOD_REPLACE, ysrc_replace_prepper, ysrc_replace_umode);
   DEBUG_YSRC   yLOG_info    ("replace"   , yMODE_actual (UMOD_REPLACE));
   yMODE_init_set   (UMOD_INPUT  , ysrc_input_prepper  , ysrc_input_umode);
   DEBUG_YSRC   yLOG_info    ("input"     , yMODE_actual (UMOD_INPUT));
   yMODE_init_set   (UMOD_SUNDO  , NULL                , yMODE_unit_stub);
   DEBUG_YSRC   yLOG_info    ("sundo"     , yMODE_actual (UMOD_SUNDO));
   yMODE_init_set   (UMOD_WANDER , ysrc_wander_prepper , ysrc_wander_umode);
   DEBUG_YSRC   yLOG_info    ("wander"    , yMODE_actual (UMOD_WANDER));
   /*---(sreg)---------------------------*/
   ysrc_sreg_init    ();
   ysrc_replace_init ();
   ysrc_input_init   ();
   ysrc_source_smark_purge ();
   ysrc_source_set_purge   ();
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySRC_init_after         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(other updates)------------------*/
   rc = yVIHUB_yFILE_dump_add ("sreg"      , "", "inventory of source registers", ysrc_sreg_dump);
   rc = yVIHUB_yVIEW_switch_add ('s', "sreg"  , "sregister"    , ySRC_sreg_status       , "details of current source register"       );
   rc = yVIHUB_yVIEW_switch_add ('s', "sel"   , "selection"    , ySRC_select_status     , "displays selection status"                );
   yMODE_after_set  (MODE_SOURCE);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> initialize source environment ------[ shoot  [gz.210.001.01]*/ /*-[00.0000.102.4]-*/ /*-[--.---.---.--]-*/
ySRC_config             (void *a_saver)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_needs  (MODE_SOURCE)) {
      DEBUG_YSRC   yLOG_note    ("init must complete before config");
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save pointer)-------------------*/
   if (a_saver != NULL)  s_saver  = a_saver;
   /*---(update status)------------------*/
   yMODE_conf_set   (MODE_SOURCE, '1');
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySRC_wrap               (void)
{
   ysrc_sundo_reset ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     navigation updates                       ----===*/
/*====================------------------------------------====================*/
static void      o___UPDATES_________________o (void) {;}

char         /*-> update contents for display --------[ ------ [-------------]*/
ySRC_source             (char *a_label, char *a_format, char *a_contents)
{
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(check inputs)-------------------*/
   DEBUG_YSRC   yLOG_point   ("a_label"   , a_label);
   DEBUG_YSRC   yLOG_info    ("a_label"   , a_label);
   if (a_label    == NULL)  strlcpy (s_src.label   , "-"       , LEN_LABEL);
   else                     strlcpy (s_src.label   , a_label   , LEN_LABEL);
   DEBUG_YSRC   yLOG_info    ("label"     , s_src.label);
   DEBUG_YSRC   yLOG_point   ("a_format"  , a_format);
   DEBUG_YSRC   yLOG_info    ("a_format"  , a_format);
   if (a_format   == NULL)  strlcpy (s_src.format  , "·····"   , LEN_LABEL);
   else                     strlcpy (s_src.format  , a_format  , LEN_LABEL);
   DEBUG_YSRC   yLOG_info    ("format"    , s_src.format);
   DEBUG_YSRC   yLOG_point   ("a_contents", a_contents);
   DEBUG_YSRC   yLOG_info    ("a_contents", a_contents);
   if (a_contents == NULL)  strlcpy (s_src.original, ""        , LEN_RECD);
   else                     strlcpy (s_src.original, a_contents, LEN_RECD);
   DEBUG_YSRC   yLOG_info    ("contents"  , s_src.original);
   strldchg (s_src.original, G_KEY_SPACE, G_CHAR_STORAGE, LEN_RECD);
   strlcpy  (s_src.contents, s_cur->original, LEN_RECD);
   DEBUG_YSRC   yLOG_info    ("contents"  , s_cur->original);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      before editing                          ----===*/
/*====================------------------------------------====================*/
static void      o___BEFORE__________________o (void) {;}

char         /*-> update contents for display --------[ ------ [-------------]*/
ySRC_update             (char *a_label, char *a_format, char *a_contents)
{
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(fix pointer)--------------------*/
   s_cur = &s_src;
   s_cur->type  = EDIT_NORM;
   /*---(check inputs)-------------------*/
   DEBUG_YSRC   yLOG_point   ("a_label"   , a_label);
   DEBUG_YSRC   yLOG_info    ("a_label"   , a_label);
   if (a_label    == NULL)  strlcpy (s_cur->label   , "-"       , LEN_LABEL);
   else                     strlcpy (s_cur->label   , a_label   , LEN_LABEL);
   DEBUG_YSRC   yLOG_info    ("label"     , s_cur->label);
   DEBUG_YSRC   yLOG_point   ("a_format"  , a_format);
   DEBUG_YSRC   yLOG_info    ("a_format"  , a_format);
   if (a_format   == NULL)  strlcpy (s_cur->format  , "·····"   , LEN_LABEL);
   else                     strlcpy (s_cur->format  , a_format  , LEN_LABEL);
   DEBUG_YSRC   yLOG_info    ("format"    , s_cur->format);
   DEBUG_YSRC   yLOG_point   ("a_contents", a_contents);
   DEBUG_YSRC   yLOG_info    ("a_contents", a_contents);
   if (a_contents == NULL)  strlcpy (s_cur->original, ""        , LEN_RECD);
   else                     strlcpy (s_cur->original, a_contents, LEN_RECD);
   DEBUG_YSRC   yLOG_info    ("contents"  , s_cur->original);
   strldchg (s_cur->original, G_KEY_SPACE, G_CHAR_STORAGE, LEN_RECD);
   strlcpy  (s_cur->contents, s_cur->original, LEN_RECD);
   DEBUG_YSRC   yLOG_info    ("contents"  , s_cur->original);
   /*---(reset content)------------------*/
   s_cur->npos = strllen (s_cur->contents, LEN_RECD);
   s_cur->bpos = s_cur->cpos = 0;
   s_cur->epos = s_cur->npos;
   /*---(process)------------------------*/
   UPDATE_AFTER_CHANGES;
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> start a new editing session --------[ ------ [-------------]*/
ySRC_start         (char *a_prefix)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_point   ("a_prefix"  , a_prefix);
   --rce;  if (a_prefix == NULL) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_info    ("a_prefix"  , a_prefix);
   /*---(select mode)--------------------*/
   switch (a_prefix [0]) {
   case ':' :
      DEBUG_YSRC   yLOG_note    ("command setting");
      s_cur = &s_cmd;
      yMODE_enter  (MODE_COMMAND);
      break;
   case ';' :
      DEBUG_YSRC   yLOG_note    ("hints setting");
      s_cur = &s_cmd;
      yMODE_enter  (SMOD_HINT);
      break;
   case '/' :
      DEBUG_YSRC   yLOG_note    ("search setting");
      s_cur = &s_cmd;
      yMODE_enter  (MODE_SEARCH );
      break;
   case 'µ' :
      DEBUG_YSRC   yLOG_note    ("menu setting");
      s_cur = &s_cmd;
      yMODE_enter  (MODE_COMMAND);
      break;
   default  :
      DEBUG_YSRC   yLOG_note    ("source setting");
      s_cur = &s_src;
      yMODE_enter  (MODE_SOURCE );
      break;
   }
   /*---(clear sundo)--------------------*/
   ysrc_sundo_reset ();
   /*---(populate globals)---------------*/
   /*> SOURCE_menu_prep ();                                                           <*/
   if (a_prefix [0] != '¦') {
      strlcpy (s_cur->contents, a_prefix , LEN_RECD);
   }
   DEBUG_YSRC   yLOG_info    ("contents"  , s_cur->contents);
   s_cur->npos = strllen (s_cur->contents, LEN_RECD);
   s_cur->bpos = s_cur->cpos = 0;
   s_cur->epos = s_cur->npos;
   UPDATE_AFTER_CHANGES;
   /*---(go info input)------------------*/
   if (a_prefix [0] != '¦') {
      DEBUG_YSRC   yLOG_note    ("go straight to append");
      ySRC_mode (' ', 'A');
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char                                                                                          <* 
 *> SOURCE_menu_prep        (void)                                                                <* 
 *> {                                                                                             <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                   <* 
 *>    char        x_on        =  '-';                                                            <* 
 *>    int         x_wide      =    0;                                                            <* 
 *>    tEDIT      *x_cur       = NULL;                                                            <* 
 *>    /+---(header)----------------------+/                                                      <* 
 *>    DEBUG_EDIT   yLOG_enter   (__FUNCTION__);                                                  <* 
 *>    /+---(get wide)--------------------+/                                                      <* 
 *>    x_on = yVIKEYS_view_size     (YVIKEYS_COMMAND, NULL, &x_wide, NULL, NULL, NULL);           <* 
 *>    DEBUG_EDIT   yLOG_char    ("on"        , x_on);                                            <* 
 *>    if (x_on != 'y') yVIKEYS_view_size     (YVIKEYS_FLOAT, NULL, &x_wide, NULL, NULL, NULL);   <* 
 *>    DEBUG_EDIT   yLOG_char    ("x_wide"    , x_wide);                                          <* 
 *>    /+---(set current)-----------------+/                                                      <* 
 *>    x_cur  = &s_cmd;                                                                           <* 
 *>    /+---(set wide)--------------------+/                                                      <* 
 *>    DEBUG_EDIT   yLOG_char    ("env"       , myVIKEYS.env);                                    <* 
 *>    if (myVIKEYS.env == YVIKEYS_CURSES)  x_cur->wide = x_wide;                                 <* 
 *>    else                                 x_cur->wide = x_wide / myVIKEYS.font_scale;           <* 
 *>    /+---(set apos)--------------------+/                                                      <* 
 *>    x_cur->apos = x_cur->wide - 6;                                                             <* 
 *>    DEBUG_EDIT   yLOG_char    ("apos"      , x_cur->apos);                                     <* 
 *>    /+---(complete)--------------------+/                                                      <* 
 *>    DEBUG_EDIT   yLOG_exit    (__FUNCTION__);                                                  <* 
 *>    return 0;                                                                                  <* 
 *> }                                                                                             <*/


/*====================------------------------------------====================*/
/*===----                       while editing                          ----===*/
/*====================------------------------------------====================*/
static void      o___DURING__________________o (void) {;}

char         /*-> prepare for source mode move -------[ leaf   [gz.412.001.00]*/ /*-[00.0000.313.!]-*/ /*-[--.---.---.--]-*/
ysrc_before             (void)
{
   /*---(prepare)------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_info    ("contents"  , s_cur->contents);
   s_cur->npos     = strlen (s_cur->contents);
   /*---(display debugging)--------------*/
   DEBUG_YSRC   yLOG_value   ("npos"     , s_cur->npos);
   DEBUG_YSRC   yLOG_value   ("apos"     , s_cur->apos);
   DEBUG_YSRC   yLOG_value   ("bpos"     , s_cur->bpos);
   DEBUG_YSRC   yLOG_value   ("cpos"     , s_cur->cpos);
   DEBUG_YSRC   yLOG_value   ("epos"     , s_cur->epos);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ySRC_contents           (char a_part, char *m, char *l, short *n, short *a, short *b, short *c, short *e, char *f, char *t)
{
   /*---(locals)-----------+-----+-----+-*/
   tEDIT      *x           = NULL;
   /*---(assign source)------------------*/
   if (a_part == 'c')   x = &s_cmd;
   else                 x = &s_src;
   /*---(save back)----------------------*/
   if (m != NULL)  *m = x->type;
   if (l != NULL)  strlcpy (l, x->label   , LEN_LABEL);
   if (n != NULL)  *n = x->npos;
   if (a != NULL)  *a = x->apos;
   if (b != NULL)  *b = x->bpos;
   if (c != NULL)  *c = x->cpos;
   if (e != NULL)  *e = x->epos;
   if (f != NULL)  strlcpy (f, x->format  , LEN_LABEL);
   if (t != NULL)  strlcpy (t, x->contents, LEN_RECD);
   /*---(complete)-----------------------*/
   return 0;
}


char         /*-> complete a source mode move --------[ leaf   [gz.E45.001.A0]*/ /*-[02.0000.513.!]-*/ /*-[--.---.---.--]-*/
ysrc_after              (void)
{
   char        rce         =    0;
   char        rc          =    0;
   /*---(prepare)------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_info    ("s_contents", s_cur->contents);
   s_cur->npos     = strlen (s_cur->contents);
   /*---(display debugging)--------------*/
   DEBUG_YSRC   yLOG_value   ("s_npos"   , s_cur->npos);
   DEBUG_YSRC   yLOG_value   ("s_apos"   , s_cur->apos);
   DEBUG_YSRC   yLOG_value   ("s_bpos"   , s_cur->bpos);
   DEBUG_YSRC   yLOG_value   ("s_cpos"   , s_cur->cpos);
   DEBUG_YSRC   yLOG_value   ("s_epos"   , s_cur->epos);
   /*---(check over/underrun)---------*/
   DEBUG_YSRC   yLOG_note    ("correct s_npos over/underruns");
   --rce;  if (s_cur->npos  >= LEN_RECD) {
      s_cur->npos = LEN_RECD - 1;
      s_cur->contents [s_cur->npos] = '\0';
      rc = rce;
   }
   --rce;  if (s_cur->npos < 0) {
      s_cur->npos = 0;
      rc = rce;
   }
   /*---(check min/max)---------------*/
   DEBUG_YSRC   yLOG_note    ("correct min/max limits");
   --rce;  if (s_cur->cpos >=  s_cur->npos)  {  s_cur->cpos = s_cur->npos - 1;  rc = rce; }
   --rce;  if (s_cur->cpos <   0          )  {  s_cur->cpos = 0;                rc = rce; }
   --rce;  if (s_cur->bpos <   0          )     s_cur->bpos = 0;
   DEBUG_YSRC   yLOG_complex ("limits"    , "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(small strings)---------------*/
   if (s_cur->npos == 0) {
      DEBUG_YSRC   yLOG_note    ("check and handle small string");
      s_cur->cpos = s_cur->bpos = s_cur->epos = 0;
   }
   /*---(small strings)---------------*/
   else if (s_cur->npos <= s_cur->apos) {
      DEBUG_YSRC   yLOG_note    ("check and handle small string");
      s_cur->bpos = 0;
      s_cur->epos = s_cur->npos - 1;
   }
   /*---(long strings)----------------*/
   else {
      DEBUG_YSRC   yLOG_note    ("check and handle long string");
      s_cur->epos = s_cur->bpos + s_cur->apos - 1;
      /*---(check off left side)------*/
      DEBUG_YSRC   yLOG_note    ("check off left side of screen");
      if (s_cur->cpos <   s_cur->bpos)  {
         s_cur->bpos = s_cur->cpos;
         s_cur->epos = s_cur->bpos + s_cur->apos - 1;
      }
      /*---(check off right side)-----*/
      DEBUG_YSRC   yLOG_note    ("check off right side of screen");
      if (s_cur->cpos >   s_cur->epos)  {
         s_cur->epos = s_cur->cpos;
         s_cur->bpos = s_cur->epos - s_cur->apos + 1;
      }
      /*---(check scrolling-----------*/
      DEBUG_YSRC   yLOG_note    ("make sure right is locked to end");
      if (s_cur->epos >=  s_cur->npos)  {
         s_cur->epos = s_cur->npos - 1;
         s_cur->bpos = s_cur->epos - s_cur->apos + 1;
      }
   }
   DEBUG_YSRC   yLOG_complex ("post-limit", "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(selection update)---------------*/
   ysrc_select_update (s_cur->cpos);
   DEBUG_YSRC   yLOG_complex ("post-sreg" , "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(update word breaks)-------------*/
   ysrc_word_update ();
   DEBUG_YSRC   yLOG_complex ("post-words", "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   /*---(display debugging)--------------*/
   DEBUG_YSRC   yLOG_value   ("s_npos"   , s_cur->npos);
   DEBUG_YSRC   yLOG_value   ("s_apos"   , s_cur->apos);
   DEBUG_YSRC   yLOG_value   ("s_bpos"   , s_cur->bpos);
   DEBUG_YSRC   yLOG_value   ("s_cpos"   , s_cur->cpos);
   DEBUG_YSRC   yLOG_value   ("s_epos"   , s_cur->epos);
   /*---(complete)--------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       after editing                          ----===*/
/*====================------------------------------------====================*/
static void      o___AFTER___________________o (void) {;}

char         /*-> accept all source changes ----------[ shoot  [gz.210.001.01]*/ /*-[00.0000.102.4]-*/ /*-[--.---.---.--]-*/
ysrc_accept             (void)
{
   char        rc          =    0;
   char        t           [LEN_RECD] = "";
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   switch (yMODE_curr ()) {
   case MODE_SOURCE  :
      DEBUG_YSRC   yLOG_note    ("save source back");
      if (s_saver != NULL && s_cur->npos > 0) {
         strlcpy (t, s_cur->contents, LEN_RECD);
         strldchg (t, G_CHAR_STORAGE, G_KEY_SPACE, LEN_RECD);
         s_saver (t);
      }
      break;
   case MODE_COMMAND :
      DEBUG_YSRC   yLOG_note    ("execute command");
      rc = yVIHUB_yCMD_direct (s_cur->contents);
      strlcpy (s_cur->contents, "", LEN_RECD);
      break;
   case SMOD_HINT    :
      DEBUG_YSRC   yLOG_note    ("execute hint");
      /*> rc = yvikeys_hint_direct (s_cur->contents);                                 <*/
      /*> strlcpy (s_cur->contents, "", LEN_RECD);                                    <*/
      break;
   case MODE_SEARCH  :
      DEBUG_YSRC   yLOG_note    ("execute search");
      rc = yVIHUB_yMARK_execute (s_cur->contents);
      strlcpy (s_cur->contents, "", LEN_RECD);
      break;
   }
   DEBUG_YSRC   yLOG_value   ("rc"        , rc);
   strlcpy (s_cur->original, s_cur->contents, LEN_RECD);
   s_cur->npos  = s_cur->bpos  = s_cur->cpos  = s_cur->epos  = 0;
   ysrc_select_reset (G_SREG_ZERO);
   ysrc_sundo_reset  ();
   ySRC_update (s_src.label, s_src.format, s_src.original);
   yVIHUB_yMAP_refresh ();
   /*> yvikeys_map_reposition  ();                                                    <*/
   DEBUG_YSRC   yLOG_value   ("rc"        , rc);
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*-> reject all source changes ----------[ shoot  [gz.210.001.01]*/ /*-[00.0000.102.4]-*/ /*-[--.---.---.--]-*/
ysrc_reset              (void)
{
   switch (yMODE_curr ()) {
   case MODE_COMMAND :
   case MODE_SEARCH  :
   case SMOD_HINT    :
      strlcpy (s_cur->original, "", LEN_RECD);
      break;
   }
   strlcpy (s_cur->contents, s_cur->original, LEN_RECD);
   s_cur->npos  = s_cur->bpos  = s_cur->cpos  = s_cur->epos  = 0;
   DEBUG_YSRC   yLOG_complex ("reset"     , "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   ysrc_select_reset (G_SREG_ZERO);
   ysrc_sundo_reset ();
   ySRC_update (s_src.label, s_src.format, s_src.original);
   /*> yvikeys_map_reposition  ();                                                    <*/
   DEBUG_YSRC   yLOG_complex ("post-reset", "%3dn %3db %3de %3dc", s_cur->npos, s_cur->bpos, s_cur->epos, s_cur->cpos);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       general accessors                      ----===*/
/*====================------------------------------------====================*/
static void  o___ACCESSORS_______o () { return; }

char*   SOURCE_label            (void) { return s_src.label; }



