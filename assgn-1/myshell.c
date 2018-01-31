#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define terminated(arg) arg == NULL || arg[0] == ';' || arg[0] == '\n'

extern char** getline(void);

char cwd[1024];

typedef struct ls {
    int size;
    const char** content;
    int limit;
} * ArgList;
ArgList ls_new(int size) {
    ArgList l = malloc(sizeof(struct ls));
    l->size = 0;
    l->limit = size;
    l->content = malloc(size * sizeof(char*));
    return l;
}
void ls_clear(ArgList ls) {
    ls->size = 0;
    ls->content[0] = NULL;
}
void ls_push(ArgList ls, const char* word) {
    if (ls->size + 1 == ls->limit) {
        ls->content = realloc(ls->content, ls->limit * 2 * sizeof(char*));
        ls->limit *= 2;
    }
    ls->content[ls->size++] = word;
}
char** ls_dup(ArgList ls) {
    char** d = malloc((ls->size + 1) * sizeof(char*));
    memcpy(d, ls->content, (ls->size) * sizeof(char*));
    d[ls->size] = NULL;
    return d;
}

typedef struct exec_node {
    // definition
    char** argv;
    int argc;
    char* infile;
    char* outfile;
    struct exec_node* next;
    char forward;
    char background;

} ExecNode;
ExecNode* node_new() {
    ExecNode* node = (ExecNode*)malloc(sizeof(struct exec_node));
    node->argc = 0;
    node->argv = NULL;
    node->next = NULL;
    node->background = 0;
    node->infile = NULL;
    node->outfile = NULL;
    node->forward = 0;
    return node;
}
void node_free(ExecNode* node) {
    if (node == NULL)
        return;
    if (node->argv) {
        for (int i = 0; i < node->argc; ++i)
            free(node->argv[i]);
        free(node->argv);
    }
    if (node->infile)
        free(node->infile);
    if (node->outfile)
        free(node->outfile);
    if (node->next)
        node_free(node->next);
    free(node);
}
void node_debug(ExecNode* n) {
    if (n == NULL) {
        printf("Node: NULL\n");
        return;
    }
    printf("===========debug===========\n");
    printf("Command:");
    for (int i = 0; i < n->argc; ++i) {
        printf(" %s", n->argv[i]);
    }
    printf("\n");
    if (n->infile)
        printf("infile: %s\n", n->infile);
    if (n->outfile)
        printf("outfile: %s\n", n->outfile);
    if (n->next) {
        node_debug(n->next);
    }
    printf("===========================\n");
}

typedef struct exec_builder {
    ExecNode head;
    ExecNode* tail;

    ExecNode* current;
} * ExecBuilder;
ExecBuilder builder_new() {
    ExecBuilder b = malloc(sizeof(struct exec_builder));
    return b;
}
void builder_prepare(ExecBuilder b) {
    b->head.next = NULL;
    b->tail = &b->head;
    b->current = NULL;
}
void builder_begin(ExecBuilder builder) {
    if (builder->current == NULL) {
        builder->current = node_new();
        builder->tail->next = builder->current;
        builder->tail = builder->current;
    }
}
void builder_end(ExecBuilder builder, ArgList list) {
    if (builder->current == NULL) // if invalidated, return
        return;
    builder->current->argc = list->size;
    builder->current->argv = ls_dup(list);
    ls_clear(list);
    builder->current = NULL; // invalidate
}
void builder_forward(ExecBuilder builder) {
    if (builder->current == NULL) {
        fprintf(stderr, "-myshell: syntax error near unexpected token '|'\n");
        return;
    }
    builder->current->forward = 1;
}
void builder_infile(ExecBuilder builder, char* file) {
    if (terminated(file)) {
        fprintf(stderr, "-myshell: syntax error near unexpected token '%s'\n", file);
        return;
    }
    if (builder->current == NULL) {
        fprintf(stderr, "-myshell: syntax error near unexpected token '<'\n");
        return;
    }
    builder->current->infile = file;
}
void builder_outfile(ExecBuilder builder, char* file) {
    if (terminated(file)) {
        fprintf(stderr, "-myshell: syntax error near unexpected token '%s'\n", file);
        return;
    }
    if (builder->current == NULL) {
        fprintf(stderr, "-myshell: syntax error near unexpected token '>'\n");
        return;
    }
    builder->current->outfile = file;
}

void prompt() {
    printf("%s > ", cwd);
}

int open_f(char* file, int mode) {
    int fd = open(file, mode, 0666);
    if (fd == -1) {
        perror("myshell");
        exit(-1);
    }
    return fd;
}
void redirect(int old, int new) {
    if (dup2(old, new) == -1) {
        fprintf(stderr, "Cannot redirect %d -> %d\n", old, new);
        perror("myshell");
        exit(-1);
    }
}

void updatecwd() {
    if (!getcwd(cwd, sizeof(cwd)))
        perror("myshell");
}

int builtin_command(char** argv) {
    if (argv[0] == NULL)
        return 1;
    if (strcmp(argv[0], "exit") == 0)
        exit(0);
    if (strcmp(argv[0], "cd") == 0) {
        if (argv[1] != NULL) {
            if (argv[2] != NULL) {
                fprintf(stderr, "myshell: cd: to many arguments\n");
                return 1;
            }
            if (chdir(argv[1]) != 0) {
                perror("myshell");
                return 1;
            }
            updatecwd();
        }

        return 1;
    }
    return 0;
}

void exec(ExecNode* cmd, int in) {
    if (cmd == NULL)
        return;

    int iofd[2];
    if (cmd->forward) {
        if (pipe(iofd) < 0) {
            perror("myshell");
            exit(-1);
        }
    }
    switch (fork()) {
    case -1:
        perror("myshell");
        exit(-1);
    case 0:
        if (cmd->infile != NULL) {
            int fd = open_f(cmd->infile, O_RDONLY);
            redirect(fd, STDIN_FILENO);
            close(fd);
        } else if (in) {
            redirect(in, STDIN_FILENO);
            close(in);
        }

        if (cmd->outfile != NULL) {
            int fd = open_f(cmd->outfile, O_CREAT | O_WRONLY);
            redirect(fd, STDOUT_FILENO);
            close(fd);
        } else if (cmd->forward) {
            close(iofd[0]);
            redirect(iofd[1], STDOUT_FILENO);
            close(iofd[1]);
        }

        if (execvp(*(cmd->argv), cmd->argv) < 0) {
            perror("myshell");
            exit(1);
        }
        break;
    default:
        wait(NULL);
        close(iofd[1]);
        exec(cmd->next, cmd->forward ? iofd[0] : STDIN_FILENO);
    }
}

int main() {
    updatecwd();
    ExecBuilder builder = builder_new();
    ArgList argls = ls_new(10);

    while (1) {
        prompt();
        char** argv = getline();

        if (builtin_command(argv))
            continue;

        builder_prepare(builder);
        for (int i = 0; argv[i] != NULL; i++) {
            const char* arg = argv[i];
            switch (arg[0]) {
            case '|':
                builder_forward(builder);
                builder_end(builder, argls);
                break;
            case ';':
                builder_end(builder, argls);
                break;
            case '<':
                builder_infile(builder, argv[++i]);
                break;
            case '>':
                builder_outfile(builder, argv[++i]);
                break;
            default:
                builder_begin(builder);
                ls_push(argls, arg);
                break;
            }
        }
        builder_end(builder, argls);
        exec(builder->head.next, STDIN_FILENO);
        node_free(builder->head.next);
    }
    return 0;
}
