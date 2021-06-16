# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <termios.h>
# include <termcap.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
#define READ_END 0
#define WRITE_END 1

# define KEY_UP 183
# define KEY_DOWN 184
# define KEY_RIGHT 185
# define KEY_LEFT 186
# define KEY_ERASE 127
# define ENTER 10
# define CTRL_RETURN 21
# define CTRL_D 4



#define BUFFER_SIZE 15
#define ENVSIZE 50

#define SINGLEQUOTES 5   /// single quotes
#define DOUBLEQUOTES 6
#define NONEQUOTE 7
#define ENVERROR 4
#define EMPTYENV 3
#define SUCCESS 2
#define PARSERROR -1
#define MEMERROR 0

#define APPEND 1 // append to the file
#define OVERWRITE 0  // overwrite in file

#define FOLDER 8
#define NEX 9 // NOT EXCUTABLE

// structres
///// look for fflush and remove it
// typedef struct strlen{
// 	char *line;
// 	int len;/
// } t_strlen;

typedef struct s_history
{
	char				*line;
	struct s_history	*next;
	struct s_history	*prev;
}						t_history;

typedef struct envs{
	char *env_name;
	char *env_value;
	struct envs *next;
}t_envs;

typedef struct txts{
	char *txt;
	struct txts *next;
} t_words;

typedef struct full_env_var
{
	t_envs **exenvs;
	t_words *filledvar;
	t_words *allkeys;
}t_fullvar;

typedef struct cmd{
	char *command;
	t_words *txts;
	t_words *files;
	t_words *ops;
} t_cmd;

typedef struct pipcmd{
	t_cmd *cmd;
	struct pipcmd *next;
} t_pipcmd;

typedef struct completecmd{
	t_pipcmd *splcommand;
	struct completecmd *next;
} t_completecmd;

typedef struct iter{
	int i;
	int j;
	int help;
	int count;
	int error;
	int status;
	int index;
} t_iter;

typedef struct collstrs{
	char *tmp;
	char *line;
	t_iter nums;
} t_collstrs;


typedef struct workingcmds
{
	t_words **cmds;
	struct workingcmds *next;
}t_workingcmds;

///////
typedef struct onecmd{
	int prem; /// if 0 excutable other not
	char *cmd;
	char **args;
	char **files;
	char **ops;
} t_onecmd;

typedef struct pipcommand{
	t_onecmd cmd;
	struct pipcommand *next;
} t_pipcommand;

typedef struct multcmd
{
	t_pipcommand pipcmd;
	struct multcmd *next;
} t_multcmd;
/////////
typedef struct {
	t_envs ***exenvs;
	char *line;
	char *_pwd;
	int envchanged;
	char **envp;
	int exitstatus;
	int fdout;
	int exit;
	int childruning;
	t_history *history;
	t_history *navigate;
	t_history *navigate2;
} g_vars;
typedef struct prstatus
{
    int exitecode;
}t_prstatus;

g_vars glob_vars;

/// random.c
char *cutstring(char *str, int start, int last);
void	update_pwd(t_envs **exenvs);
int is_number(char *num);
void ft_printf(int count, ...);
void free_dstr(char **str);
int ft_atoi(char *str);
char *ft_reverse(char *line);
char* ft_itoa(int value);
void print_pipes(t_words **pipes, int numofcmds);
void print_words(t_words *words, int level, char *name);
void printspaces(int n);
/// minishell.c ///
void bash_loop(char **env, int ret, int i, int d);

/// readline.c
int get_next_line(char **line);
int complete(char **line, char **rest, char **current, int index);
int validate(char **line, char **rest, char **current);
int last_line(char **line, char **rest, char **current);
char *split(char *current, int val, int index);
int error(char **line, char **rest, char **current);
int  ft_strlen(char *str);
char *ft_strjoin(char **rest, char *current) ;
int nlindex(char *str, char c);

/// workonline.c
int workon_line(char *line, t_completecmd **complete, int numofcmds, int help);
int	workon_line_part2(t_workingcmds **wcmd, t_words **commands, int numofcmds, t_completecmd **complete);
int	workon_line_part1(char *line, t_words **commands, t_workingcmds **wcmd, int *numofcmds);
void rest(t_workingcmds **wcmd, t_words **commands);
int fill_commands(t_words **commands, char *line);
void rest_txt_next(char **str, t_words **next);
int calcfirst(char *line, char c, int *last, int *is_str);
int	h1_calcfirst(int i);
int backslash(char *line, int index);
int help_short_calcfirst(int *i, int *dq, int *sq, int is_first);
int countnumberofcmds(t_words *commands);
int simplecheck(char *line);
int check_errors(int ern, t_words **words);
void free_words(t_words **words);
void free_pipes(int **pipes, int count);
void free_wcmd(t_workingcmds **wcmd, int numofcmds);
void help_short_count(char *help, int *i);
int fill_string_cmdtxt(t_words **commands, int len, char *line);
int fill_wcmd(t_workingcmds **wcmd, t_words *commands, int numofcmds);
int fill_pips(t_words **pip, char *line);
int help_short_fill_wcmd(t_workingcmds **wcmd, int numofcmds, t_words **pip);
int help_fill_pipes(t_words **pip, char *line, int len);
int fill_completecmd(t_completecmd **compcmd, t_words **pips, int pipindex);
int fill_pipcmd(t_pipcmd **pipcmd, t_words *pip);
int splitlinetowords(char *str, t_cmd **command);
int fill_words(t_words **words, char *str);
int	h1_fill_words(t_words **words, char *str, int *index, int help);
int valditadsq(char *str);
int  validchracter(char c);
int splitby(char *str, int *index);
int fill_command(t_cmd **command, char *str, int where);
int fill_cmd_objs(t_words **txts, char *str);
int	h1_fill_cmd_objs(t_words **txts, char *str);
int istxt(char c);
int opvalid(char *str);
int ft_cmpstr(char *s1, char *s2);
int ft_strcmp(char *s1, char *s2);
int reset_command(t_cmd **command);
int fill_cmdstruct(t_words *words, t_cmd **command);
int	h1_fill_cmdstruct(t_words **words, t_cmd **command, char *firstword);
int	h2_fill_cmdstruct(t_words **words, t_cmd **command, char *firstword, int ret);
int is_empty(char *line);
int check_newlines(char *line);
int	h1_check_newlines(char **line, char *backup, int *i);
char *ft_strdup(char *str);
void free_cmdstr(t_cmd **cmd);
void free_pipcmd(t_pipcmd **pipcmd);
void free_comp(t_completecmd **cmp);
// filter complete
int filter_complete(t_completecmd **completecmd);
int filter_check_envvar(t_words *txts);
int filter_pipcmd1(t_pipcmd **pipcmd);
int filter_cmd(t_cmd **cmd);
int modify_str(char **str);
int addtowords(t_words **words, char *str, int start, int end);
void addtmptowords(t_words **words, t_words **word);
int modify_ln(t_words **words);
char *cleanWord(t_words *words, int size);
void free_w(t_words **words);
void free_onecmd(t_onecmd *cmd);
void free_laststr(t_pipcommand **pcmd);
//// gather_env
int fill_envtable(t_fullvar **fullvar, char **env);
int rest_envt(t_envs ***envtable);
int ft_exist(t_words *words, char *str);
int hash_env_name(char *env_name);
int add_toenvtable(t_fullvar **vars, char *line);
int add_node_to(t_envs **head, t_envs **current);
void free_envlist(t_envs **envlist);
void free_env(t_envs ***envtable);
void free_one_env(t_envs **oneenv);
void print_tables(t_envs **table);
void print_one_oft(t_envs *ln);
int check_envvar(char *line, int eq_pos);
int ft_isalpha(char c);
int ft_isdigit(char c);
t_envs *make_node_env(int *ern, char *line, t_fullvar **vars);
int	help_make_node_env(t_fullvar **vars, t_envs **curenv, char *line, int *ern);
void help2_make_node_env(t_fullvar **vars, t_envs **cuenv, char *line);
int	free_error(int *ern, t_envs *cuenv, int is_free);
int	help_norm_make_node(t_envs **cuenv, t_envs **or, int *eq_pos, char *line);
t_envs *get_env(int *found, char *env_name, t_envs **table);
t_envs *look_inln(int *found,  char *env_name, t_envs *lnenv);
int delete_exactfromln(t_envs **list, char *key, int *found);
int delete_env(t_envs ***table, char *env_name, int *founded);
void free_env_node(t_envs *n);
int add_envvar_to_table(char *line, t_fullvar **variables);
int add_to_words_str(t_words **hid_var, char *line);


int get_status(int st);

void print_cmd(t_cmd *cmd, int level);
void print_oldwords(t_words *word, int level, char *name);
void print_pipcmd(t_pipcmd *pipcmd);
void print_completecmd(t_completecmd *complete);

t_words **expand_txtsh1(t_words *words, int *l);
t_words *local_wordsh1(int *index, int start,char *line);
t_words *get_last_wordstruct(t_words *words);
// t_words *collect_strs(t_words *keys, t_envs **exenv, t_strlen info, int order);
t_words *split_by_spaces(char *line, int status);
t_words *first_case(char *line);
t_words *second_case(char *line);
t_words  *third_case(char *line);

t_pipcommand *expand_current_command(t_completecmd *complet, t_fullvar *envs);
int expand_full_pipcmd(t_pipcmd **pipcmd, t_envs **exenvs);
int expand_one_cmdstrct(t_cmd **cmd, t_envs **exenvs);
int expand_commandtxt(t_cmd **cmd, t_envs **exenvs);
int expand_txts(t_words **txts, t_envs **exenvs);
int expand_txtsh2(int len, t_words **all);
int backs_filter_str(char **str, t_envs **exenvs, t_words **newwords);
int work_on_words(t_words **mod_words, t_words *words, t_envs **exenvs, int order);
int filter_string(t_words **words, t_words *w, t_envs **exenvs, int order);
int get_var_name(char *line, char **key);
int is_special(char c);
int loop_in_filter_stringh1(int *index, char *line, t_words **keys, t_envs **exenvs); /// return variable size
// t_strlen loop_in_filter_string(char *line, t_envs **exenv, t_words **keys);
int fill_all_var(char *tmp, char *value, int index);
int fill_first(char *tmp, int index, char *value);
int mk_and_add_to_words(t_words **words, char *line);
int fill_from_words(char *tmp, int index, t_words *words);
void	ft_remove_q_tmp(char **tmp, int start);
int fill_normal(char *tmp, int index, char *value);
int first_one(t_words **help, char *line);
int get_words(char *line, t_words **help);
int last_word(t_words **nw, char *line);
int modify_prev(char *prv, char *cur);
int nonequt(char c);
void collect_strs_h1(t_collstrs *vars, t_words **keys, t_envs **exenv, int order); //// returns status;;
// void  help_fill_collstrs(t_strlen info, t_collstrs *vars);
void add_word_tofront(t_words **words, t_words **cuw);
void add_words(t_words **orgin, t_words **forien);

char *get_last_word(t_words *words);
char *get_word(char *line, int *next);


t_pipcommand *get_cmd_struct(t_pipcmd *cmd, t_envs **exenvs);
int fill_onepipcmd(t_pipcommand *pipcmd, t_pipcmd *pip, t_envs **exenvs);
int fill_one_cmd(t_onecmd *fcmd, t_cmd *pcmd, t_envs **exenvs);
void	help_fill_one_cmd(t_onecmd *fcmd, t_cmd *pcmd);
char **transfrm_ln_arr(t_words *words, char *cmd, int iscmd);
int how_many_words(t_words *words);

void print_p(t_pipcommand *pip);
void print_c(t_onecmd *cmd);
void print_arr(char **str);

char *get_command(char *cmd, char *PATH, int *prem, char *lcmd);
int	help_get_command(char *cmd, int ret, char **command);
int	help2_get_command(char *lcmd, char **command, char *path);
int is_comcmd(char *cmd, char *lcmd); // if 1 means not look in PATH 0 look in PATH
char *get_using_path(char *cmd, char *PATH, struct stat *fst);
char *get_one_path(char *PATH, int *index);
int is_excutable(struct stat fst);
int get_cmd_state(char *cmd);
char *lower_str(char *str);
char lower_char(char c);
/// EXCUTION ///
int fullexcute(t_completecmd **complete, t_fullvar **variables);
int excute_one_cmd(t_pipcommand *pcmd, t_fullvar **variables);
int get_num_subcmds(t_pipcommand *pcmd);
void default_fds(int *stdo, int *stdi);
void alloc_pipes(int ***pipes, int count);
int exec_multi_pipe(t_pipcommand *pcmd, int **pipe, t_fullvar **variables, t_iter nums);
char  **creat_w_files(char **files, char **ops, int *error, int *append); // returns last file
void close_pipes(int **pipes, int inex, int pipsize);
void close_write_rest(int **pipes, int index, int pipsize);
void close_read_rest(int **pipes, int index, int pipsize);
int decide_in_out(int **pipes, char **files, char **ops, t_iter nums);
int	help_decide_in_out(char **fs);
int run_command(t_onecmd cmd);
int close_in_parent(int **pipe, int pindex);
int ex_mu_p_cmd(t_pipcommand *pcmd, int **pipe, t_fullvar **env_var, t_iter nums);
t_onecmd help_short_ex_mu(t_pipcommand *pcmd, int **pipe, t_iter nums);

int exc_one_cmd(t_onecmd cmd, int **pipe, t_iter nums, t_fullvar **env_var);
int prm_check(t_onecmd cmd);
void	help_prem_check(char *cmd, int *ret);
int run_exact_cmd(t_onecmd cmd, t_fullvar **env_var);
int run_cmd(t_onecmd cmd, t_fullvar **env_var);

int cre_write_files(char ***fs, char *file, char *op, int *append);
void init_in_creat_wf(char ***fs, int *i, int *error);
int run_sim_cmd(t_onecmd cmd, t_fullvar **env_var);
int run_sim_ifcmd(t_onecmd cmd, t_fullvar **env_var);
int	help_run_sim_ifcmd(t_onecmd cmd, int *def);
int handl_red(t_onecmd cmd);
void help_handl_red(char **fs, int append);
int actual_exec_one(t_onecmd cmd);
int builtin(char *cmd, char *origin);
char **update_env_var(t_envs **exenvs);
int get_hasht_size(t_envs **exenvs);
///// export 
char *get_key(char *line);
 int exc_one_built(t_onecmd cmd, t_fullvar **env_var);
void export_print(t_fullvar *vars);
int ft_export(char **args, t_fullvar **vars);
void update_exit_status(t_envs **exenvs);
int sub_export(t_fullvar **vars, char *line);
int	help1_sub_export(t_fullvar **vars, char *line, char **key, char **tmp);
int	help2_sub_export(t_fullvar **vars, char *key, char **tmp1, t_envs **cuvar1);
void	help3_sub_export(char *line, char *key, char **tmp1, t_envs **cuvar);
int check_exvar(char *line);
//// echo
int     _echo(char **args);
int cd(char **paths, t_fullvar **vars);
int	help_cd(char **paths, t_fullvar **vars, char *home);
void set_pwd(t_fullvar **vars);
//// print_export 
t_words *sortd_merg(t_words *w1, t_words *w2);
int	help_sortd_merg(t_words **w1, t_words **w2, t_words **fin);
int sort_words(t_words **words);
void red_in_decide_files(char **fs, int **pipe, int append, t_iter nums);
void	overwrite_or_append(int append, char **fs);
void red_in_decide_no_files(int **pipe, t_iter nums);
void real_work_sort(t_words **words);
void fill_rest(t_words **fin, t_words *w);
int run_built_in(t_onecmd cmd, t_fullvar **vars);
int	help_run_built_in(t_onecmd cmd, t_fullvar **vars, char *lcmd);
void split_words(t_words *words, t_words **a, t_words **b);
void fill_unchaged(t_words **word, char *line);
int var_founded(t_words **words, int kl, int counter);
int var_exist(char *line, int *len);
int ch_was_var(t_words **words);
/////// unset
int ft_unset(char **args, t_fullvar **vars);
int check_unsetvar(char *line);
int unset_one_var(char *key, t_fullvar **vars);
void delete_exact_word(t_words **words, char *word);
/// exit
int ft_exit(char **args);
void	help_ft_exit(int status, char **args);
// ENV
int ft_env (t_fullvar *vars);
int		pwd();

///////////////////////////////////////////////////////////////////////////////////// TESTS NEED TO REMOVE
void handl_sig(int signum);
int get_status(int st);
/////////////////////////////////////////////////////////////////////////////////////
// 62 68  0x0000000100103330
//
//
////////////////////  newexpan.c
void get_full_expanded_line(t_cmd *cmd, t_envs **exenvs);
int	help1_get_full_expanded(t_cmd *cmd, t_envs **exenvs, char **cline, int *exp);
void	help2_get_full_exp(char *line, char **cmd, char **cline);
int is_export(char *cmd);
char *expand_one_word(char *str, t_envs **exenvs);
t_words *split_by_qout(char *str);
char *get_quot_word(char *str, int *index);
int  index_of_dq(char*str, int start);
char *get_squto_word(char *str, int *index);
char *get_non_sdquot(char *str, int *index);
int get_len_ofstrs_in_words(t_words *words);
char *get_line_from_words(t_words *words);
char *get_line_from_words_with_spaces(t_words *words);
void var_expan(t_words *words, t_envs **exenvs);
char *get_words_expaned(char *str, t_envs **expanded);
char *double_qout_part(char *str, t_envs **exenvs);
int get_len_double_qout(char *str, t_envs **exenvs);
void	sim_h1_get_len(int *tot, int *i);
int fill_part_of_str(char *tmp, int *index, char *str, t_envs **exenvs);
char *last_pars(char *line, t_words **txts, int exp);
int	help1_last_pars(char *line, int i);
int get_next_dqpos(char *line);
int get_next_sqpos(char *line);
int get_next_nq(char *line);
int skip_spaces(char *line, int i);
void delete_backslachs(t_words **txts, int exp);
char *remove_back_from_one(char *line, int exp);
char *finl_cost_back(char *line, int exp);
char *double_quot_comp(char *line, int exp);
char *none_qout_comp(char *line, int exp);
int is_special_in_double(char c);
int is_special_in_none(char c);
////////
void file_creation(char **files, char **ops);
////// sig
int level_of_bash(t_envs **exenvs);

int	get_char(void);
int prompt();
int	push_to_history(t_history **h, char *line);
int check_history(char *line);

void	ctrl_return();
void	key_erase();
int key_down();
int	key_up();
void	key_enter(t_completecmd **complete, t_fullvar **variables);
