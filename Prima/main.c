#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#define List struct list
#define Node struct node

List{
    Node *head;
};

Node{
    int v1;
    int v2;
    long long int item;
    Node *next;
};

int check_v(int a, FILE *m){
    if (a <= 5000 && a >= 0)
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

int spanning(int ribs, FILE *m){
    if (ribs == 0){
        fprintf(m, "no spanning tree");
        fclose(m);
        return 1;
    }
    else
        return 0;
}

int check_s(int vertex, int ribs, int count, FILE *m){
    if (vertex > ribs + 1 || count == ribs) {
        fprintf(m, "no spanning tree");
        fclose(m);
        return 1;
    }
    else
        return 0;
}

int span_one_more(Node *cur, FILE *m){
    if (cur -> item == -1 || cur -> v1 == cur -> v2) {
        fprintf(m, "no spanning tree");
        fclose(m);
        return 1;
    }
}

int check(int vertex, int ribs, FILE *fout) {
    if (check_v(vertex, fout) != 1)
        return 1;
    if (check_edg(fout, ribs, vertex) != 1)
        return 1;
    if (check_free(vertex, ribs) == 1)
        return 1;
    if (spanning(ribs, fout) == 1)
        return 1;
    }


int length_one_more(int count, int ribs, int vertex, int a, FILE *m){
    if ((count < ribs && count != 0) || (a == 0 && vertex == 2)) {
        fprintf(m, "bad number of lines");
        fclose (m);
        return 1;
    }
}

int check_length(FILE *m, int a, int b, long long int c, int N) {

    if (!((a <= N && a >= 0) && (b <= N && b >= 0))){
        fprintf(m, "bad vertex");
        fclose(m);
        return 1;
    }
    else {
        if (c >= 0 && c <= INT_MAX)
            return 0;
        else {
            fprintf(m, "bad length");
            fclose(m);
            return 1;
        }
    }
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

Node *element(int v1, int v2, long long int value){
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> next = NULL;
    new -> v1 = v1;
    new -> v2 = v2;
    new -> item = value;
    return(new);
}

List *create_node(List *L, Node *new) {
    Node *tmp;

    if (!L -> head) {
       L -> head = new;
       return L;
    }
    else
       tmp = L -> head;

    while (tmp -> next)
       tmp = tmp -> next;
    tmp -> next = new;
    return(L);
}

long long int* fill (long long int* arr, int vertex, int ribs, FILE* fin, FILE * fout){
    int i, j, a, b, count = 0;
    long long int c;
    for (i = 0; i < vertex; i++){
        for (j = 0; j < vertex; j++){
            if ( i == j)
                *(arr + i*vertex + j) = 0;
            else
                *(arr + i*vertex + j) = -1;
        }
    }

    while (!feof(fin)){
        fscanf(fin, "%d %d %lld", &a, &b, &c);
        check_lines(a, ribs, fout);
        if (check_length(fout, a, b, c, vertex) == 1) {
            return 1;
        }
        if ( a != b) {
            *(arr + (a - 1)*vertex + (b - 1)) = c;
            *(arr + (b - 1)* vertex + (a - 1)) = c;
        }
        count ++;
    }

    if (length_one_more(count, ribs, vertex, a, fout) == 1)
        return 1;
    return (arr);
}


Node *find_min(int k,long long int *arr, int vertex){
    int i, v1, v2;
    long long int min = LLONG_MAX;
    for (i = 0; i < vertex; i ++)
        if (*(arr + (k * vertex + i)) < min && *(arr + (k * vertex + i)) > 0) {
            min = *(arr + (k * vertex + i));
            v1 = i;
            v2 = k;
        }

    return (element(v1 + 1, v2 + 1, min));
}

int check_circle(List *L, Node *new) {
     Node *tmp = L -> head;
     int count = 0, count_2 = 0;

    while (tmp) {
      if (tmp -> v1 == new -> v1 || tmp -> v2 == new -> v1)
         count ++;
      if (tmp -> v1 == new -> v2 || tmp -> v2 == new -> v2)
         count_2 ++;
    tmp = tmp->next;
    }

    if (count != 0 && count_2 != 0)
         return 1;
    else
         return 0;
}

List *start_import(int *arr, List *import, int vertex){
    import->head = find_min(0, arr, vertex);
    return (import);
}

Node *needed_element(Node *tmp, int *arr, int vertex, long long int min, Node *new){                                      //just find the smallest element? but do this very difficult
    while (tmp){                                                                                                         //cause we should print it in special way
        if (tmp -> v2 < tmp -> v1) {
            if (find_min(tmp -> v2 - 1, arr, vertex)-> item <= min) {
                new = find_min(tmp->v2 - 1, arr, vertex);
                min = new -> item;
            }
            if ((find_min(tmp -> v1 - 1, arr, vertex)) -> item < min || min == LLONG_MAX) {
                new = find_min(tmp -> v1 - 1, arr, vertex);
                min = new -> item;
            }
        }
        else {
            if (find_min(tmp -> v1 - 1, arr, vertex) -> item <= min) {
                new = find_min(tmp -> v1 - 1, arr, vertex);
                min = new -> item;
            }

            if ((find_min(tmp -> v2 - 1, arr, vertex)) -> item < min || min == LLONG_MAX) {
                new = find_min(tmp -> v2 - 1, arr, vertex);
                min = new -> item;
            }
        }
        tmp = tmp->next;
    }

    return new;
}

List *wire(List *import, long long int *arr, int vertex) {                                                    //it find the smallest element in graph, handle all graph
    long long int min = LLONG_MAX;                                                                            //from down to up
    Node *tmp = import -> head, *new = NULL;
    if (!import -> head) {
        import = start_import(arr, import, vertex);
        (*(arr + (import -> head -> v1 - 1) * vertex + import -> head -> v2 - 1) = - 1);
        (*(arr + (import -> head -> v2 - 1) * vertex + import -> head -> v1 - 1) = - 1);
        return(import);
    }

    new = needed_element(tmp, arr, vertex, min, new);
    (*(arr + (new -> v1 - 1)* vertex + new -> v2 - 1) = -1);                                                 //we don't need this elements in the table anymore
    (*(arr + (new -> v2 - 1)* vertex + new -> v1 - 1) = -1);

    if (check_circle(import, new) == 0)                                                                      //check that this element doesnt't make cycle
        create_node(import, new);
    else{
        if(import -> head -> item == new -> item){
            new -> item = -1;
            create_node(import, new);
        }
    }
    return(import);
}

List *shaping(int count, int vertex, int *arr, List *import, FILE *m){                                       //start to put list into loop every time, how it returns,
    while (count != vertex - 1) {                                                                             //it finishes only when algorithm  will be handle all graph
        count = 1;
        import = wire(import, arr, vertex);
        Node *cur = import -> head;
        while (cur -> next){
            cur = cur -> next;
            count ++;
        }
        span_one_more(cur, m);
    }
    return(import);
}

void out(List *import, FILE *m){
    while(import -> head) {
        fprintf(m, "%d %d\n", import->head->v2, import->head->v1);
        import -> head = import -> head -> next;
    }
}

int main() {
    FILE *fin, *fout;
    fin = fopen("in.txt", "r");
    fout = fopen("out.txt", "w");
    if (!fin){
        puts("error io");
        return 1;
    }

    int vertex, ribs, count = 1;
    fscanf(fin, "%d\n %d\n", &vertex, &ribs);
    if (check(vertex, ribs, fout) == 1)
        return 1;

    List *import = (List*)malloc(sizeof(Node));                                                              //this list for needed elements
    import -> head = NULL;
    long long int *arr = NULL;
    arr = (long long int*)malloc(vertex*vertex*sizeof(long long int));                                       //this is arr for graph adjacency table
    arr = fill(arr, vertex, ribs, fin, fout);
    if ((int) arr == 1)
        return 1;
    if (check_s(vertex, ribs, count, fout) == 1)
        return 1;
    import = shaping(count, vertex, arr, import, fout);                                                      //begin to do Prima
    out(import, fout);                                                                                       //print my list with needed tree
    free(import);
    free(arr);
    return 0;
}