#include <stdio.h>
#include <malloc.h>
#define Stack struct stack
#define Node struct node

Stack{
    Node *head;
};

Node {
    Node *next;
    int v1;
};

int check_v(int a, FILE *m){
    if (a <= 1000 && a >= 0)
        return 1;
    else {
        fprintf(m, "bad number of vertices");
        fclose(m);
        return 0;
    }
}

int check_free(int vertex, int ribs) {
    if (vertex == 1 && ribs == 0)
        return 1;
}

int check_edg(FILE *m, int a, int b){
    if (a >= 0 && a <= b*(b - 1)/2)
        return 1;
    else {
        fprintf(m, "bad number of edges");
        fclose (m);
    }
}

int check(int vertex, int ribs, FILE *fout) {
    if (check_v(vertex, fout) != 1)
        return 1;
    if (check_edg(fout, ribs, vertex) != 1)
        return 1;
    if (check_free(vertex, ribs) == 1)
        return 1;
    }

Node *element(int b){
    Node *new;
    new  = (Node *)malloc(sizeof(Node));
    new -> v1 = b;
    new -> next = NULL;
    return (new);
}

int check_bad(FILE *m, int a, int b, int N) {
    if (!((a <= N && a >= 0) && (b <= N && b >= 0))){
        fprintf(m, "bad vertex");
        fclose(m);
        return 1;
    }
    else
        return 0;
}

int check_lines(int a, int ribs, FILE *m){
    if (a == 0 && ribs == 1) {
        fprintf(m, "bad number of lines");
        fclose(m);
        return 0;
    }
    else
        return 1;
}

int numb_one_more(int count, int ribs, int vertex, int a, FILE *m){
    if ((count < ribs && count != 0) || (a == 0 && vertex == 2)) {
        fprintf(m, "bad number of lines");
        fclose (m);
        return 1;
    }
    else
        return 0;
}

Stack **fill_matrix(FILE *fin, Stack **arr, FILE *fout, int ribs, int vertex){
    int count = 0, a, b;
    while (!feof(fin)){
        fscanf (fin, "%d %d\n", &a, &b);

        if (check_lines(a, ribs, fout) != 1)
            return 1;
        if (check_bad(fout, a, b, vertex) == 1)
            return 1;

        if (!((*(arr + a - 1)) -> head))
            (*(arr + a - 1)) -> head  = element(b - 1);
        else {
            Node *tmp = (*(arr + a - 1)) -> head;
            while(tmp -> next)
                tmp = tmp -> next;
            tmp -> next = element(b - 1);
        }
        count ++;
    }
    if (numb_one_more(count, ribs, vertex, a, fout) == 1) {
        free(arr);
        return 1;
    }
    return arr;
}

Stack *push(Stack *S, int v1){
    Node *new_node;
    new_node = (Node*)malloc(sizeof(Node));
    new_node -> v1 = v1;
    if (!S -> head) {
        S -> head = new_node;
        new_node -> next = NULL;
        return S;
    }
    new_node -> next = S -> head;
    S -> head = new_node;
    return(S);
}

Stack *DFS(Stack **arr, int vertex, int *color, Node *tmp, Stack * import){
    if ( *(color + (tmp -> v1)) == 1)
        return import;
    if (*(color + (tmp -> v1)) == 2)
        return import;

    if (tmp -> next) {
        *(color + (tmp->v1)) = 1;
        import = DFS(arr, vertex, color, tmp -> next, import);
    }

    if ((*(arr + (tmp -> v1))) -> head) {
        int k = tmp -> v1;
        *(color + (tmp -> v1)) = 1;
        tmp = (*(arr + (tmp -> v1))) -> head;
        import = DFS(arr, vertex, color, tmp, import);
        *(color + (tmp -> v1)) = 2;
        import =  push(import, k + 1);
    }
    else {
        *(color + (tmp->v1)) = 2;
        import =  push(import, tmp -> v1 + 1);
    }
    return (import);
}

int *fill_color(int *color, int vertex){
    int i;
    for (i = 0; i < vertex; i++)
        *(color + i) = 0;
    return(color);
}

void out(Stack *import, FILE *fout){
    while(import -> head) {
        fprintf(fout, "%d ", import->head->v1);
        import->head = import->head->next;
    }
    fclose(fout);
}

int imp(int vertex, int count, FILE *m){
    if (count != vertex){
        fprintf(m, "impossible to sort");
        fclose(m);
        return 1;
    }
    else
        return 0;
}

Stack **fill_pointers(int vertex, Stack** arr){
    int i;
    for (i = 0; i < vertex; i++){
        Stack *p;
        p = (Stack*)malloc(sizeof(Stack));
        p -> head = NULL;
        *(arr + i) = p;
    }
    return arr;
}

int check_cycle(Node *tmp, int vertex, FILE *fout){
    int count = 0;
    while(tmp){
        count ++;
        tmp = tmp -> next;
    }
    if (imp(vertex, count, fout) == 1)
        return 1;
}

Stack *cyclic_processing(int vertex, int *color, Stack **arr, Stack *import){
    int i;
    do { i = 0;
        while (*(color + i) != 0)
            i ++;
        if (i <= vertex - 1) {
            Node *tmp = (*(arr + i))->head;
            if (!(tmp)) {
                import = push(import, i + 1);
                *(color + i) = 2;
            }
            else {
                import = DFS(arr, vertex, color, tmp, import);
                *(color + i) = 2;
                import = push(import, i + 1);
            }
        }
    }while(i <= vertex - 1);
    return import;
}

void clear(Stack *import, int *color, Stack **arr){
    free(arr);
    free(import);
    free(color);
}

int main() {
    int vertex, ribs;
    FILE *fin, *fout;
    fin = fopen("in.txt", "r");
    if (!fin){
        puts ("array io");
        return 0;
    }
    fout = fopen("out.txt", "w");
    fscanf(fin, "%d", &vertex);
    fscanf(fin, "%d\n", &ribs);
    if (check(vertex, ribs, fout) == 1)
        return 1;
    Stack **arr = (Stack**)malloc(vertex*sizeof(Stack*));
       arr = fill_pointers(vertex, arr);
    Stack *import = (Stack*)malloc(sizeof(Stack*));
        import -> head = NULL;

    if ((int)fill_matrix(fin, arr, fout, ribs, vertex) == 1)
        return 1;
    else
        arr = fill_matrix(fin, arr, fout, ribs, vertex);
    int *color = (int*)malloc(vertex*sizeof(int));
    color = fill_color(color, vertex);
    import = cyclic_processing(vertex, color, arr, import);
    Node *tmp = import -> head;
    if (check_cycle(tmp, vertex, fout)== 1) {
        clear(import, color, arr);
        return 1;
    }
    out(import, fout);
    clear(import, color, arr);
    return 0;
}