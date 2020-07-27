#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <math.h>
#define Node struct Node
#define List struct List

List {
    Node *head;
};

Node {
    Node *next;
    int num;
    long long int item;
};

long long int check_length(FILE *m, long long int a, long long int b, long long int N, long long int c) {
    if (!((a <= N && a >= 0) && (b <= N && b >= 0)))
        fprintf(m, "bad vertex");
    if (c >= 0 && c <= INT_MAX)
        return 1;
    else {
        fprintf(m, "bad length");
        return 0;
    }

}

long long int* fill (long long int* arr, int vertex, int ribs, FILE* fin, FILE * fout){
    long long int i, j, a, b, c, count = 0;

    for (i = 0; i < vertex; i++){
        for (j = 0; j < vertex; j++){
            if ( i == j)
                *(arr + i*vertex + j) = 0;
            else
                *(arr + i*vertex + j) = LLONG_MAX;
        }
    }

    while (!feof(fin)){
        fscanf(fin, "%lld %lld %lld", &a, &b, &c);
        if (check_length(fout, a, b, vertex, c) != 1) {
            return 1;
        }

        if ( a != b) {
            *(arr + (a - 1)*vertex + (b-1)) = c;
            *(arr + (b - 1)* vertex + (a - 1)) = c;
        }
        count ++;
    }

    if (count < ribs && count != 0) {
        fprintf(fout, "bad number of lines");
        return 1;
    }

    return (arr);
}


int find(List *L, long long int a, List *Min){
    Node *tmp, *time, *pop;
    tmp = L -> head;
    time = (Node*)malloc(sizeof(Node));
    time -> next = NULL;
    while (tmp) {
        if (tmp -> item <= a) {
            time -> num = tmp -> num;
            time -> item = tmp -> item;
            a = tmp -> item;
        }
        tmp = tmp -> next;
    }

    tmp = L -> head;
    pop = L -> head -> next;
    if (tmp -> num == time -> num)
        L -> head = tmp -> next;
    else {
        while (tmp -> next -> num != time -> num) {
            tmp = tmp -> next;
            pop = pop -> next;
        }
        tmp -> next = pop -> next;
    }


    tmp = Min -> head;
    if (Min -> head == NULL)
        Min -> head = time;
    else {
        while (tmp -> next)
            tmp = tmp -> next;
        tmp -> next = time;
    }
    return (time -> num);
}

Node* compare(Node *L, long long int value, long long int a) {
    if ((L -> item > value + a) && value != LLONG_MAX)
        L -> item = value + a;
    return (L);
}

List* create_node(List *L, int numb, long long int value) {
    if (!L -> head) {
        L -> head = (Node*)malloc(sizeof(Node));
        L -> head -> num = numb;
        L -> head -> item = value;
        L -> head -> next = NULL;
    }
    else {
        Node *tmp;
        tmp = L -> head;
        Node *new = (Node*)malloc(sizeof(Node));
        new -> num = numb;
        new -> item = value;
        new -> next = NULL;
        while (tmp -> next)
            tmp = tmp -> next;
        tmp -> next = (Node*)malloc(sizeof(Node));
        tmp -> next = new;
    }
    return(L);
}

int check_n(int a){
    if (a <= 5000 && a >= 0)
        return 1;
}

int check_edg(FILE *m, int a, int b){
    if (a >= 0 && a <= b*(b - 1)/2)
        return 1;
    else
        fprintf(m, "bad number of edges");
}
int check_v(FILE *m, long long int a, long long int b, long long int N){
    if ((a <= N && a > 0) && (b <= N && b > 0))
        return 1;
    else
        fprintf(m, "bad vertex");
}


void in (List *Min, int vertex,int v2, long long int *arr, FILE *m, int v1) {
    int j = 0;

    long long int last = v2;
    for (j = 0; j < vertex; j++) {
        int k = j;
        while (last != v1) {
            j = k;
            while (j <= vertex - 1) {
                if (j == last - 2 && v2 < v1)
                    j ++;
                Node *tmp, *pop;
                tmp = Min -> head;
                pop = Min -> head;
                while (tmp -> num != last)
                    tmp = tmp -> next;

                while (pop -> num != j + 1) {
                    if (pop -> next == NULL) {
                        j ++;
                        pop = Min -> head;
                    }
                    else
                        pop = pop->next;
                }


                if (fabs(tmp -> item - pop -> item) == *(arr + (last - 1) * vertex + j) && *(arr + (last - 1) * vertex + j) != 0) {
                    fprintf(m, "%d ", tmp -> num);
                    j = (int) vertex;
                    last = pop->num;
                } else
                    j ++;

                if (v2 == v1) {
                    fprintf(m, "%d ", v1);
                    j = (int) vertex;
                    last = v1;
                }
            }
        }
    }

    if (last == v1)
        fprintf(m, "%d", v1);
    fprintf(m, "\n");
}

List *twice(List *Min, int vertex, int v2, long long int *arr, FILE *m, int v1) {
    int count = 0, i;
    for (i = vertex + 1; i > 0; i --) {
        Node *tmp = Min -> head;
        long long int imp, imp_item;
        while (tmp) {
            if (tmp -> num == i) {
                imp = i;
                imp_item = tmp -> item;
                tmp = NULL;
            } else
                tmp = tmp -> next;
        }

        Node *pop;
        pop = Min -> head;
        while (pop) {
            if (pop ->  item == imp_item && v1 != v2 && pop -> num != imp && pop -> num != v2 && pop -> num != v1) {
                count++;
                int k= pop -> num;
                pop -> num = -10;
                in (Min, vertex, v2, arr, m, v1);
                pop -> num = k;
            }

            pop = pop -> next;
        }
    }
    if (count == 0)
        in (Min, vertex, v2, arr, m, v1);
}

int main() {

    FILE *fin, *fout;
    fin = fopen("in.txt", "r");
    fout = fopen("out.txt", "w");
    if (!fin){
        puts("error io");
        return 1;
    }

    long long int *arr;
    int vertex, ribs, v1, v2, i, j;
    List *L, *Min;

    L = (List*)malloc(sizeof(Node));
    Min = (List*)malloc(sizeof(Node));
    L -> head = NULL;
    Min -> head = NULL;

    fscanf(fin, "%d", &vertex);
    if (check_n(vertex) != 1) {
        fprintf(fout, "bad number of vertices");
        fclose(fout);
        return 1;
    }

    fscanf(fin, "%d %d", &v1, &v2);
    if (check_v(fout, v1, v2, vertex) != 1)
        return 1;

    fscanf(fin, "%d\n", &ribs);
    if (check_edg(fout, ribs, vertex) != 1)
        return 1;

    arr = (long long int*)malloc((size_t) ((vertex * vertex) * sizeof(long long int)));
    arr = fill(arr, vertex, ribs, fin, fout);
    if ((int) arr == 1)
        return 1;


    i = v1 - 1;

    for (j = 0; j < vertex; j ++) {
        L = create_node(L, j + 1, *(arr + i*vertex + j));
    }


    long long int min = LLONG_MAX, K;
    int ind, count = 0;

    while (count != vertex) {
        ind = find(L, min, Min);
        Node *tmp;
        tmp = Min -> head;
        while(tmp -> next)
            tmp = tmp -> next;
        K = tmp -> item;
        i = ind - 1;
        tmp = L -> head;
        while(tmp) {
            tmp = compare(tmp, *(arr + i * vertex + tmp -> num - 1), K);
            tmp = tmp -> next;
        }
        count++;
    }
    fclose(fin);

    Node *tmp = Min -> head;
    count = 1;
    while (count != vertex + 1) {

        while (tmp -> num != count)
            tmp = tmp -> next;

        if (tmp -> item > INT_MAX && tmp -> item < LLONG_MAX)
            fprintf(fout, "INT_MAX+ ");
        else
          if (tmp -> item <= INT_MAX)
            fprintf(fout, "%lld ", tmp -> item);
          else
            fprintf(fout, "oo");
        count++;
        tmp = Min -> head;
    }
    fprintf(fout, "\n");

    tmp = Min -> head;
    count = 0;
    int k;

    while (tmp && k != 0) {
        if (tmp -> num == v2) {
            if (tmp->item == LLONG_MAX) {
                fprintf(fout, "no path");
                return 1;
            }
            k = 0;
        }
        else {
            if (tmp -> item == INT_MAX)
                count ++;
            if (count >= 2) {
                fprintf(fout, "overflow");
                return 1;
            }
        }

        tmp = tmp -> next;
    }

    twice(Min, vertex, v2, arr, fout, v1);
    fclose(fout);
    free(Min);
    free(L);
}