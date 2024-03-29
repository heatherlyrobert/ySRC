/*============================----beg-of-source---============================*/
#ifndef ySRC
#define ySRC yes


typedef  unsigned char        uchar;


/*===[[ ySRC_base.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char*       ySRC_version            (void);
char        ySRC_init               (void);
char        ySRC_init_after         (void);
char        ySRC_config             (void *a_saver, void *a_sreg);
char        ySRC_wrap               (void);

char        ySRC_source             (char *a_label, char *a_format, char *a_contents);
char        ySRC_update             (char *a_label, char *a_format, char *a_contents);
char        ySRC_start              (char *a_prefix);

char        ySRC_mode               (uchar a_major, uchar a_minor);

char        ySRC_size               (short a_formula, short a_command);
char        ySRC_contents           (char a_part, char *m, char *l, short *n, short *a, short *b, short *c, short *e, char *f, char *t);
char        ySRC_push               (uchar a_sreg, char *a_data);



char        ySRC_select_islive      (void);
char        ySRC_select_status      (char a_size, short a_wide, char *a_list);
char        ySRC_sreg_status        (char a_size, short a_wide, char *a_list);
char        ySRC_sreg_saver         (void *a_saver);

char*       ySRC__unit              (char *a_question, int n);

#endif
/*============================----end-of-source---============================*/
