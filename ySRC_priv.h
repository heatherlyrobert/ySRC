/*============================----beg-of-source---============================*/
#ifndef ySRC_priv
#define ySRC_priv yes



/*===[[ BEG_HEADER ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

/*===[[ ONE_LINERS ]]=========================================================*/

#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "us (user control)"
#define     P_SUBJECT   "source input"
#define     P_PURPOSE   ""

#define     P_NAMESAKE  "angelia-minyma (the message)"
#define     P_HERITAGE  ""
#define     P_IMAGERY   ""
#define     P_REASON    ""

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  "ySRC"
#define     P_FULLNAME  "/usr/local/lib64/libySRC"
#define     P_SUFFIX    ""
#define     P_CONTENT   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"
#define     P_DEPENDS   "none"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   ""

#define     P_VERMAJOR  "2.--, clean, improve, and expand"
#define     P_VERMINOR  "2.0-, separated into independent library"
#define     P_VERNUM    "2.0k"
#define     P_VERTXT    "all base sreg functions tested, but not mode stuff yet"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*===[[ END_HEADER ]]=========================================================*/

/*---(ansi-c standard)-------------------*/
#include    <stdio.h>             /* clibc  standard input/output             */
#include    <stdlib.h>            /* clibc  standard general purpose          */
#include    <string.h>            /* clibc  standard string handling          */
/*---(custom)----------------------------*/
#include    <yURG.h>              /* heatherly urgent processing              */
#include    <yLOG.h>              /* heatherly program logging                */
#include    <ySTR.h>              /* heatherly string processing              */
#include    <yVIKEYS_solo.h>      /* heatherly vi/vim keys processing         */
#include    <yMODE.h>             /* heatherly yVIKEYS mode tracking          */
#include    <yKEYS.h>             /* heatherly yVIKEYS key handling           */



extern char    (*s_saver) (char *a_contents);
extern uchar    *g_stub;
extern uchar     g_char;
extern uchar     g_goto;



#define     S_SREG_NOT '-'
#define     S_SREG_YES 'y'

/*===[[ ySRC_sundo.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#define     S_SUNDO_MAX      1000
typedef struct cSUNDO  tSUNDO;
struct cSUNDO {
   int         seq;
   char        major;
   char        minor;
   int         cpos;
   uchar       before;
   uchar       after;
};
extern tSUNDO g_sundos  [S_SUNDO_MAX];
extern int    g_nsundo;
extern int    g_csundo;
extern int    g_nseq;


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#define     UPDATE_BEFORE_CHANGES   ysrc_before_change ()
#define     UPDATE_AFTER_CHANGES    ysrc_after_change ()
/*---(program)--------------*/
char*       ySRC_version            (void);
char        ySRC_init               (void);
char        ySRC_wrap               (void);
/*---(ends)-----------------*/
char        ysrc_reset              (void);
char        ySRC_update             (char *a_label, char *a_contents);
char        ySRC_start              (char *a_prefix);
char        ysrc_before_change      (void);
char        ysrc_after_change       (void);
char        ysrc_accept             (void);
char        ysrc_reject             (void);
/*---(unittest)-------------*/
char        ysrc__unit_quiet        (void);
char        ysrc__unit_loud         (void);
char        ysrc__unit_end          (void);
char*       ySRC__unit              (char *a_question, int n);



/*===[[ ySRC_source.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
typedef  struct cEDIT   tEDIT;
struct cEDIT {
   char        type;                      /* type of input                    */
   char        label       [LEN_LABEL];   /* source label                     */
   char        original    [LEN_RECD];    /* pre-edit content                 */
   char        contents    [LEN_RECD];    /* working content                  */
   short       wide;                      /* full display space width         */
   short       apos;                      /* available space for display      */
   short       npos;                      /* length of edited content         */
   short       cpos;                      /* current character position       */
   short       bpos;                      /* beginning of visiable part       */
   short       epos;                      /* end of visiable part             */
   char        words       [LEN_RECD];    /* working space for wbe keys       */
};
extern tEDIT   s_src;
extern tEDIT   s_cmd;
extern tEDIT  *s_cur;

#define      EDIT_NORM        '-'
#define      EDIT_CMDS        'c'
#define      EDIT_SRCH        's'

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(workers)--------------*/
char        ysrc_replace_one        (uchar a_key);
char        ysrc_delete_one         (void);
char        ysrc_backspace_one      (void);
char        ysrc_insert_one         (uchar a_key);
char        ysrc_insert_and_move    (uchar a_key);
char        ysrc_append_one         (uchar a_key);
/*---(selection)------------*/
char        ysrc_clear_select       (void);
char        ysrc_delete_select      (void);
char        ysrc_multi_pure         (uchar a_major, uchar a_minor);
char        ysrc_copy               (void);
char        ysrc_replace            (void);
char        ysrc_paste              (char a_dir);
char        ysrc_swap_all           (char *a_new);
/*---(accessors)------------*/
int         ysrc_cpos               (void);
int         ysrc_npos               (void);
char       *ysrc_contents           (void);



/*===[[ ySRC_sundo.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(program)--------------*/
char        ysrc_sundo_trim         (int a_start);
char        ysrc_sundo_reset        (void);
char        ysrc_sundo_init         (void);
/*---(grouping)-------------*/
char        ysrc_sundo_beg          (void);
char        ysrc_sundo_chain        (void);
char        ysrc_sundo_end          (void);
/*---(record)---------------*/
char        ysrc_sundo_add          (char a_major, char a_minor, int a_pos, char a_before, char a_after);
char        ysrc_sundo_single       (char a_minor, int a_pos, char a_before, char a_after);
/*---(undo)-----------------*/
char        ysrc_sundo__undo        (int *a_pos);
char        ysrc_sundo_undo         (int *a_pos);
/*---(redo)-----------------*/
char        ysrc_sundo__redo        (int *a_pos);
char        ysrc_sundo_redo         (int *a_pos);
/*---(reporting)------------*/
char        ysrc_sundo_status       (char *a_list);
/*---(done)-----------------*/



/*===[[ ySRC_input.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_add_one            (uchar a_mode, uchar a_minor);
char        ysrc_input_umode        (uchar a_major, uchar a_minor);



/*===[[ ySRC_sreg.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#define        S_SREG_MAX      100
typedef struct  cSREG  tSREG;
struct cSREG {
   /*---(active)------------*/
   char        active;                      /* register in use                */
   char        source;                      /* user, file, import             */
   /*---(source)------------*/
   char       *label;                       /* original cell                  */
   short       beg;                         /* original starting point        */
   short       end;                         /* original ending point          */
   short       root;
   /*---(contents)----------*/
   short       len;                         /* length of text                 */
   char       *data;                        /* text                           */
   /*---(done)--------------*/
};
extern char   G_SREG_LIST     [S_SREG_MAX];
extern tSREG  g_sreg;
extern tSREG  g_sregs  [S_SREG_MAX];
extern uchar  g_nsreg;
extern uchar  g_csreg;
extern uchar  g_wsreg;
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        ysrc_sreg__wipedata     (tSREG *a_dst);
char        ysrc_sreg__wipeall      (char a_scope, tSREG *a_dst);
/*---(program)--------------*/
char        ysrc_sreg_purge         (char a_scope);
char        ysrc_sreg_init          (void);
/*---(utility)--------------*/
char        ysrc_sreg_index         (uchar a_abbr);
char        ysrc_sreg_setreg        (uchar a_abbr);
char        ysrc_sreg_setwork       (uchar a_abbr);
char        ysrc_sreg_clear         (uchar a_abbr);
char        ysrc_sreg_push          (uchar a_abbr, char *a_data);
char        ysrc_sreg_pop           (uchar a_abbr, char *a_data);
char        ysrc_sreg_fetch         (short *a_len, char *a_data);
char        ysrc_sreg_save          (void);
/*---(status)---------------*/
char        ysrc_sreg__line         (uchar a_abbr, char *a_entry);
char        ysrc_sreg_status        (char *a_entry);
char        ysrc_sreg_info          (int a_index, char *a_entry);
/*---(mode)-----------------*/
char        ysrc_sreg__export       (char a_id);
char        ysrc_sreg__import       (char a_id);
char        ysrc_sreg__copy         (char a_id, char a_src);
char        ysrc_sreg__save         (char a_id, char *a_string);
char        ysrc_sreg_direct        (char *a_string);
char        ysrc_sreg_smode         (uchar a_major, uchar a_minor);
int         ysrc_sreg_dump          (void *a_file);
/*---(done)-----------------*/



/*===[[ ySRC_input.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_select_reset       (short a_pos);
char        ysrc_select_update      (short a_pos);
short       ysrc_select_reverse     (void);
char        ysrc_select_exact       (short a_beg, short a_end, short a_root);
char        ysrc_select_curr        (short *a_beg, short *a_end, short *a_root);
char        ysrc_select_get         (short *a_beg, short *a_end, short *a_root, short *a_len, char *a_data);
char        ysrc_select_all         (void);
char        ysrc_select_getlive     (void);
char        ysrc_select_islive      (void);
char        ysrc_select_makelive    (void);



/*===[[ ySRC_input.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
uchar       ysrc_word__type         (char a_type, char a_curr);
char        ysrc_word__curr         (char a_type, char a_save, char a_mode);
char        ysrc_word__prev         (char a_type, char a_save, char a_mode);
char        ysrc_word__update       (short a_pos, char a_curr);
char        ysrc_word_update        (void);
char        ysrc_word_next          (char a_minor, short *a_cur, short *a_len);
char        ysrc_word_end           (char a_minor, short *a_cur, short *a_len);
char        ysrc_word_prev          (char a_minor, short *a_cur, short *a_len);
char        ysrc_word_hmode         (uchar a_major, uchar a_minor);


/*===[[ ySRC_move.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(absolute)-------------*/
char        ysrc_move_simple        (uchar a_major, uchar a_minor);
char        ysrc_move_char_next     (uchar a_ch);
char        ysrc_move_char_prev     (uchar a_ch);
/*---(screen)---------------*/
char        ysrc_move_goto          (uchar a_major, uchar a_minor);
char        ysrc_move_scroll        (uchar a_major, uchar a_minor);
/*---(done)-----------------*/



/*===[[ ySRC_replace.c ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_source_mode        (uchar a_major, uchar a_minor);



/*===[[ ySRC_replace.c ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_replace_umode      (int a_major, int a_minor);

#endif

