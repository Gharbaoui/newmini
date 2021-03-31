#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
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

// structres
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
	t_words *emptyvar;
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

typedef struct workingcmds
{
	t_words **cmds;
	struct workingcmds *next;
}t_workingcmds;

/// random.c
char *cutstring(char *str, int start, int last);
void print_pipes(t_words **pipes, int numofcmds);
void print_words(t_words *words, int level, char *name);
void printspaces(int n);
/// minishell.c ///
void bash_loop(char **env);

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
void rest(t_workingcmds **wcmd, t_words **commands);
int fill_commands(t_words **commands, char *line);
void rest_txt_next(char **str, t_words **next);
int calcfirst(char *line, char c, int *last, int *is_str);
int backslash(char *line, int index);
int help_short_calcfirst(int *i, int *dq, int *sq, int is_first);
int countnumberofcmds(t_words *commands);
int simplecheck(char *line);
int check_errors(int ern, t_words **words);
void free_words(t_words **words);
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
int valditadsq(char *str);
int  validchracter(char c);
int splitby(char *str, int *index);
int fill_command(t_cmd **command, char *str, int where);
int fill_cmd_objs(t_words **txts, char *str);
int istxt(char c);
int opvalid(char *str);
int ft_cmpstr(char *s1, char *s2);
int reset_command(t_cmd **command);
int fill_cmdstruct(t_words *words, t_cmd **command);
int is_empty(char *line);
int check_newlines(char *line);
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
//// gather_env
int fill_envtable(t_fullvar **fullvar, char **env);
int rest_envt(t_envs ***envtable);
int hash_env_name(char *env_name);
int add_toenvtable(t_envs ***envtable, char *line, t_words **allkeyes);
int add_node_to(t_envs **head, t_envs **current);
void free_envlist(t_envs **envlist);
void free_env(t_envs ***envtable);
void free_one_env(t_envs **oneenv);
void print_tables(t_envs **table);
void print_one_oft(t_envs *ln);
int check_envvar(char *line, int eq_pos);
int ft_isalpha(char c);
int ft_isdigit(char c);
t_envs *make_node_env(int *ern, char *line, t_words **allkeys);
t_envs *get_env(int *found, char *env_name, t_envs **table);
t_envs *look_inln(int *found,  char *env_name, t_envs *lnenv);
int delete_exactfromln(t_envs **list, char *key, int *found);
int delete_env(t_envs ***table, char *env_name, int *founded);
int add_envvar_to_table(char *line, t_fullvar **variables);
int add_to_words_str(t_words **hid_var, char *line);



void print_cmd(t_cmd *cmd, int level);
void print_oldwords(t_words *word, int level, char *name);
void print_pipcmd(t_pipcmd *pipcmd);
void print_completecmd(t_completecmd *complete);
/// configurcmdwithenvvar ////
int expand_current_command(t_completecmd **complet, t_fullvar *envs);
int expand_full_pipcmd(t_pipcmd **pipcmd, t_envs **exenvs);
int expand_one_cmdstrct(t_cmd **cmd, t_envs **exenvs);
int backs_filter_str(char **str, t_envs **exenvs);
int local_words(t_words **words, char *line);
void  add_word_tofront(t_words **words, t_words **cuw);
int work_on_words(t_words **mod_words, t_words *words, t_envs **exenvs);
int filter_string(t_words **words, char *line, t_envs **exenvs);
int get_var_name(char *line, char **key);
int is_special(char c);
int collect_strs(t_words **words, char *line, int size);

// 62 68  0x0000000100103330
