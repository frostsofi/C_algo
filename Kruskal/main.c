#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#define List struct list
#define Node struct node
#define List_t struct list_t
#define Tree struct tree

List{
    Node *head;
};

Node{
    int v1;
    int v2;
    int item;
    Node *next;
};

List_t{
    List_t *next;
    int v1;
    Tree *t;
};

Tree{
    int v1;
    int v2;
    int item;
    Tree *left;
    Tree *right;
};

int check_n(int a, FILE *m){
    if (a <= 5000 && a >= 0)
        return 1;
    else {
        fprintf(m, "bad number of vertices");
        fclose(m);
        return 0;
    }
}

int check_length(FILE *m, int a, int b, int c, int N) {

    if (!((a <= N && a >= 0) && (b <= N && b >= 0))) {
        fprintf(m, "bad vertex");
        return 0;
    }

    if (c >= 0 && c <= INT_MAX)
        return 1;
    else {
        fprintf(m, "bad length");
        return 0;
    }
}

int check_edg(FILE *m, int a, int b){
    if (a >= 0 && a <= b*(b - 1)/2)
        return 1;
    else
        fprintf(m, "bad number of edges");
}

int check_numb(int count, FILE *m, int ribes){
    if (count < ribes) {
        fprintf(m, "bad number of lines");
        fclose(m);
        return 0;
    }
}

int check_numb_2(int a, int ribs, FILE *m){

    if (a == 0 && ribs == 1) {
        fprintf(m, "bad number of lines");
        fclose(m);
        return 0;
    }
    else
        return 1;
}

int false(FILE *m, List *L, List_t *F){
    fprintf(m, "no spanning tree");
    fclose(m);
    free(L);
    free(F);
    return 1;
}

int check_one(int vertex, int ribs) {
    if (vertex == 1 && ribs == 0)
        return 1;
}

int spanning(int ribs, int vertex, FILE *m){
    if ((ribs == 0 && vertex == 0) || (ribs == 0)){
        fprintf(m, "no spanning tree");
        fclose(m);
        return 1;
    }
    else
        return 0;
}

int check(int vertex, int ribs, FILE *fout) {
    if (check_n(vertex, fout) != 1) {
        fclose(fout);
        return 1;
    }
    if (check_one(vertex, ribs) == 1) {
        fclose(fout);
        return 1;
    }
    if (check_edg(fout, ribs, vertex) != 1) {
        fclose(fout);
        return 1;
    }
    if (spanning(ribs, vertex, fout) == 1) {
        fclose(fout);
        return 1;
    }
}

void in_order(Tree *t, FILE *m){
    if (t -> left)
        in_order(t->left, m);
    fprintf(m, "%d %d", t -> v1, t -> v2);
    fprintf(m, "\n");

    if (t -> right)
        in_order(t -> right, m);
}

List_t *pop(List_t *useless, List_t *f){
    List_t *tmp = f;
    List_t *cur = f -> next -> next;

    if (f -> v1 == useless -> v1) {
        f = f -> next;
        return (f);
    }

    if (!(cur) && tmp -> next -> v1 == useless -> v1) {
        tmp -> next = NULL;
        return (f);
    }

    if (f  -> next -> next -> v1 == useless -> v1 && !useless -> next) {
        tmp -> next -> next = NULL;
        return (f);
    }

    while (tmp -> next -> v1 != useless -> v1) {
        tmp = tmp -> next;
        cur = cur -> next;
    }
    tmp -> next = cur;
    return(f);
}

Tree *insert(Tree *tmp, Tree *new){
    if (!tmp) {
        tmp = new;
        return (tmp);
    }

    if (new -> item < tmp -> item)
        if (tmp -> left)
            insert(tmp -> left, new);
        else
            tmp -> left = new;
    else
    if (tmp -> right)
        insert(tmp -> right, new);
    else
        tmp -> right = new;

    return tmp;
}

List_t *unification(List_t *f, int one, int two){
    List_t *cur, *tmp = f;
    cur = f;

    while (cur && cur -> v1 != one)
        cur = cur -> next;

    while (tmp && tmp -> v1 != two)
        tmp = tmp -> next;

    tmp -> t = insert(tmp -> t, cur -> t);

    cur -> t = NULL;
    f = pop(cur, f);
    return(f);
}

void put_list(List_t *f, Tree *new, int one){
    List_t *tmp = f;

    while(tmp) {
        if (tmp -> v1 == one)
            tmp -> t = insert(tmp -> t, new);
        tmp = tmp -> next;
    }
}

int way(Tree *f, int v1, int v2, int one){
    Tree *cur = f;

    if (!f)
        if (v1 == v2)
            return v1;
        else
            return one;

    if (cur -> v1 == v2 || cur -> v2 == v2)
        return (v1);

    if (cur -> left)
        return (way(cur -> left, v1, v2, one));
    else
    if (cur -> right)
        return (way(cur -> right, v1, v2, one));

    return one;
}

Tree *create_list(Node *L){
    Tree *new_node = (Tree*)malloc(sizeof(Tree));
    new_node -> right = NULL;
    new_node -> left = NULL;
    new_node -> v1 =  L -> v1;
    new_node -> v2 = L -> v2;
    new_node -> item = L -> item;
}

List_t *check_graph(Node *L, List_t *f, int vertex){
    List_t *tmp = f;
    int one = 0, two = 0;

    while(tmp){
        one = way(tmp -> t, tmp -> v1, L -> v2, one);
        two = way(tmp -> t, tmp -> v1, L -> v1, two);
        tmp = tmp -> next;
    }

    if (one != two && (one <= vertex && one > 0) && (two > 0 && two <= vertex)) {
        Tree *new = create_list(L);
        put_list(f, new, one);
        f = unification(f, one, two);
        return(f);
    }

    return (f);
}

List_t *create_node_t(List_t *f, int v1){
    if (!f) {
        f = (List_t*)malloc(sizeof(List_t));
        f -> v1 = v1;
        f -> t = NULL;
        f -> next = NULL;
        return(f);
    }
    else{
        List_t *tmp;
        tmp = f;
        while (tmp -> next)
            tmp = tmp -> next;
        tmp -> next = (List_t*)malloc(sizeof(List_t));
        tmp -> next -> v1 = v1;
        tmp -> next -> t = NULL;
        tmp -> next -> next = NULL;
        return(f);
    }
}

List_t *create_forest(List_t *f, int vertex) {
    f = NULL;
    int i;
    for (i = 1; i <= vertex; i++)
        f = create_node_t(f, i);

    return(f);
}

Node *element(Node *new, int v1, int v2, int value){
    new = (Node *) malloc(sizeof(Node));
    new -> v1 = v1;
    new -> v2 = v2;
    new -> item = value;
    new -> next = NULL;
    return(new);
}

List *create_node(List *L, int v1, int v2, int value) {
    Node *tmp, *new;
    new = element(new, v1, v2, value);

    if (!L -> head) {
        L -> head = new;
        return L;
    }
    else
        tmp = L -> head;

    if (!tmp -> next || L -> head -> item > new -> item)
        if (tmp -> item > new -> item) {
            new -> next = L -> head;
            L -> head = new;
            return L;
        }
        else {
            if (L -> head -> next)
                new -> next = L -> head -> next;
            L -> head -> next = new;
            return L;
        }

    while (tmp -> next && tmp -> next -> item <= new -> item)
        tmp = tmp -> next;
    new -> next = tmp -> next;
    tmp -> next = new;
    return(L);
}

void clear(FILE *m, FILE *n, List *L){
    free(L);
    fclose(m);
    fclose(n);
}

int sort(List *L, int vertex, int ribs, FILE *m, FILE *n){
    int a, b, c, count = 0;
    while (!feof(m)) {
        fscanf(m, "%d %d %d\n", &a, &b, &c);
        count++;

        if (check_numb_2(a, ribs, n) == 0) {
            clear(n, m, L);
            return 1;
        }
        if (check_length(n, a, b, c, vertex) == 0) {
            clear(n, m, L);
            return 1;
        }
        create_node(L, a, b, c);
    }

    if (check_numb(count, n, ribs) == 0) {
        clear(n, m, L);
        return 1;
    }

    return 0;
}

int main() {
    FILE *fin, *fout;
    fin = fopen("in.txt", "r");
    fout = fopen("out.txt", "w");
    if (!fin){
        puts("error io");
        return 1;
    }

    int vertex, ribs, count = 0;
    fscanf(fin, "%d\n %d\n", &vertex, &ribs);
    if (check(vertex, ribs, fout) == 1)
        return 1;

    List *import = (List*)malloc(sizeof(Node));
    import -> head = NULL;
    if (sort(import, vertex, ribs, fin, fout) == 1)
        return 1;
    fclose(fin);

    List_t *forest, *cur;
    forest = create_forest(forest, vertex);
    Node *tmp = import -> head;
    while (tmp) {
        forest = check_graph(tmp, forest, vertex);
        tmp = tmp -> next;
    }

    cur = forest;
    while(cur) {
        if (cur -> t == NULL)
            count ++;
        cur = cur -> next;
    }
    if (count != 0 || forest == NULL) {
        false(fout, import, forest);
        return 1;
    }

    in_order(forest -> t, fout);
    free(forest);
    free(import);
    return 0;
}