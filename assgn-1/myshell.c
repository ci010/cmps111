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

typedef struct list {
    int size;
    void** content;
    int limit;
} * List;
List lsnew(int size) {
    List l = malloc(sizeof(struct list));
    l->size = 0;
    l->limit = size;
    l->content = malloc(sizeof(void*) * size);
    return l;
}
void lspush(List ls, void* e) {
    if (ls->size + 1 == ls->limit) {
        ls->content = realloc(ls->content, ls->limit * 2 * sizeof(void*));
        ls->limit *= 2;
    }
    ls->content[ls->size++] = e;
}
void* lsget(List ls, int i) {
    if (i >= ls->size) {
        perror("list");
        exit(1);
    }
    return ls->content[i];
}
void** lsdup(List ls) {
    void** d = malloc((ls->size + 1) * sizeof(void*));
    memcpy(d, ls->content, (ls->size + 1) * sizeof(void*));
    d[ls->size] = NULL;
    return d;
}
void lsclear(List ls) {
    ls->size = 0;
    ls->content[0] = NULL;
}
int lssize(List ls) {
    return ls->size;
}

typedef struct exec_node {
    // definition
    char** argv;
    int argc;
    char* infile;
    char* outfile;
    char forward;
    char background;

} ExecNode;
ExecNode* node_new() {
    ExecNode* node = (ExecNode*)malloc(sizeof(struct exec_node));
    node->argc = 0;
    node->argv = NULL;
    node->background = 0;
    node->infile = NULL;
    node->outfile = NULL;
    node->forward = 0;
    return node;
}
void node_free(ExecNode** nodes) {
    ExecNode* node = *nodes;
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
    node_free(nodes + 1);
    free(node);
}

void prompt() {
    printf("%s > ", cwd);
}
int open_f(char* file, int mode) {
    int fd = open(file, mode, 0666);
    if (fd == -1) {
        perror("myshell: open file:");
        exit(-1);
    }
    return fd;
}
void redirect(int old, int new) {
    if (dup2(old, new) == -1) {
        perror("myshell: redirect: ");
        exit(-1);
    }
}
void updatecwd() {
    if (!getcwd(cwd, sizeof(cwd)))
        perror("myshell: cwd: ");
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

void exec(ExecNode** nodes, int in) {
    ExecNode* cmd = *nodes;
    if (cmd == NULL)
        return;

    int iofd[2];
    if (cmd->forward) {
        if (pipe(iofd) < 0) {
            perror("myshell: pipe: ");
            exit(-1);
        }
    }
    switch (fork()) {
    case -1:
        perror("myshell: fork: ");
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
            perror("myshell: execvp: ");
            exit(1);
        }
        break;
    default:
        wait(NULL);
        close(iofd[1]);
        exec(nodes + 1, iofd[0]);
    }
}

int main() {
    updatecwd();
    List nodes = lsnew(10);
    List args = lsnew(10);
    while (1) {
        prompt();
        char** argv = getline();

        if (builtin_command(argv))
            continue;

        ExecNode* current = NULL;

        for (int i = 0; argv[i] != NULL; i++) {
            const char* arg = argv[i];
            switch (arg[0]) {
            case '|':
                if (current == NULL) {
                    fprintf(stderr, "-myshell: syntax error near unexpected token '|'\n");
                    break;
                }
                current->forward = 1;
            case ';':
                if (current == NULL) // if invalidated, return
                    break;
                current->argc = lssize(args);
                current->argv = (char**)lsdup(args);
                lsclear(args);
                current = NULL;
                break;
            case '<':
                ++i;
                if (terminated(argv[i])) {
                    fprintf(stderr, "-myshell: syntax error near unexpected token '%s'\n", argv[i]);
                    break;
                }
                if (current == NULL) {
                    fprintf(stderr, "-myshell: syntax error near unexpected token '<'\n");
                    break;
                }
                current->infile = argv[i];
                break;
            case '>':
                ++i;
                if (terminated(argv[i])) {
                    fprintf(stderr, "-myshell: syntax error near unexpected token '%s'\n", argv[i]);
                    break;
                }
                if (current == NULL) {
                    fprintf(stderr, "-myshell: syntax error near unexpected token '>'\n");
                    break;
                }
                current->outfile = argv[i];
                break;
            default:
                if (current == NULL) {
                    current = node_new();
                    lspush(nodes, (void*)current);
                }
                lspush(args, (void*)arg);
                break;
            }
        }
        if (current != NULL) {
            current->argc = lssize(args);
            current->argv = (char**)lsdup(args);
            lsclear(args);
            current = NULL;
        }
        ExecNode** nodels = (ExecNode**)lsdup(nodes);
        lsclear(nodes);
        exec(nodels, STDIN_FILENO);
        node_free(nodels);
        free(nodels);
    }
    return 0;
}
