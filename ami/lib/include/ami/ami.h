#ifndef _AMI_H_
#define _AMI_H_

#define _GNU_SOURCE
#include <search.h>

#include "action.h"
#include "flow.h"

#include "khash.h"
#include "kvec.h"

#define MAX_VARIABLES 1

KHASH_MAP_INIT_STR(strhash, char *)

struct _ami_kvec_t {
  size_t n;
  size_t m;
  char **a;
};
typedef struct _ami_kvec_t ami_kvec_t;

struct _ami_actions_kvec_t {
  size_t n;
  size_t m;
  ami_action_t **a;
};
typedef struct _ami_actions_kvec_t ami_actions_kvec_t;

enum _ami_error_t {
	NO_ERROR,
        NO_VERSION,
};
typedef enum _ami_error_t ami_error_t;

struct _ami_replace_t {
  char *field;
  char *from;
  char *to;
};
typedef struct _ami_replace_t ami_replace_t;

typedef void (*print_message_cb)(char *message);
typedef void (*sleep_cb)(int msec);
typedef void (*ami_action_cb)(ami_action_t *action);

struct _ami_ast_t {
  int var_value_from_function;
  int static_var;
  int parsing_function;
  int in_action;
  int action_block_id;
  int repeat_block_id;
  int opened_sections;
  char *current_variable_value;
  char *current_field_value;
  ami_kvec_t func_arguments;
  char *repeat_index_as;
  int repeat; // Everytime we capture an action it is a repeat action of at least 1 :)
  /* ami_actions_kvec_t repeat_actions; */
  ami_flow_kvec_t repeat_flow;
  ami_flow_t *current_flow;
  ami_kvec_t replace_key;
  ami_kvec_t replace_val;
  char *action_name;
  char *action_exec;
};
typedef struct _ami_ast_t ami_ast_t;

struct _ami_t {
  ami_ast_t *_ast;
  ami_error_t error;
  int debug;
  int version;
  int revision;
  char *author;
  char *shortdesc;
  char *tag;
  ami_kvec_t references;
  khash_t(strhash) *global_variables;
  khash_t(strhash) *local_variables;
  ami_actions_kvec_t actions;
  sleep_cb sleepcb;
  print_message_cb printmessagecb;
  ami_action_cb action_cb;
};
typedef struct _ami_t ami_t;

typedef int (*foreach_action_cb)(ami_t *ami, ami_action_t *action, void *user_data);

ami_t *ami_new(void);
int ami_parse_file(ami_t *ami, char *file);
void ami_set_message_callback(ami_t *ami, print_message_cb message_cb);
void ami_set_sleep_callback(ami_t *ami, sleep_cb sleep_cb);
int ami_loop(ami_t *ami, foreach_action_cb action_cb, void *user_data);
void ami_close(ami_t *ami);
char *ami_error_to_string(ami_t *ami);
int ami_set_global_variable(ami_t *ami, char *key, char *val);
const char *ami_get_global_variable(ami_t *ami, char *key);
void ami_debug(ami_t *ami);

int ami_set_local_variable(ami_t *ami, char *key, char *val);
const char *ami_get_local_variable(ami_t *ami, char *key);
void ami_erase_local_variables(ami_t *ami);
int ami_nast_repeat_flow_reset(ami_t *ami);
void ami_set_action_callback(ami_t *ami, ami_action_cb action_cb);

#endif // _AMI_H_
