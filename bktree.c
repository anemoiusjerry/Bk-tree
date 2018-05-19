#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 10
#define N 100
#define TOL 2

typedef struct node {
	char *word;
	int next[2*MAXLEN];
} Node;

int min(int sub, int del, int ins);
int leven_dist(char *word1, char *word2);
void add(Node root, Node curr);
char **getSimilarWords(Node root, char *s);


int ptr;
Node tree[N];


int main(int argc, char const *argv[]) {
	int i, sz = 10;

	Node RT = malloc(sizeof(Node));
	for (i=0; i<2*MAXLEN; i++) {
		RT->next[i] = 0;
		//printf("%d\n",i);
	}

	// dictionary words
  char *dictionary[] = {"hell","help","shel","smell","fell","felt","oops",
												 "pop","oouch","halt"};
	ptr = 0;

  // adding dict words on to tree
  for(i=0; i<sz; i++) {
    Node tmp = malloc(sizeof(Node));
		tmp->word = dictionary[i];
		//printf("=========\n");
		for (i=0; i<2*MAXLEN; i++) {
			tmp->next[i] = 0;
			//printf("%d\n",i);
		}

		printf("=========\n");
    add(RT, tmp);
  }


	char* w1 = "ops";
	char **match = getSimilarWords(RT,w1);
	i=0;
	while (strcmp(match[i], "EOF") != 0) {
		printf("%s\n", match[i]);
		i++;
	}
	return 0;
}




/* Find minimum of 3 numbers */
int min(int sub, int del, int ins) {
	int min = sub;
	/* Test whether next 2 numbers are greater than the first, if so update min
	 * accordingly */
	if (min > del) {
		min = del;
	}
	if (min > ins) {
		min = ins;
	}
	return min;
}

int leven_dist(char *word1, char *word2) {
	int n = strlen(word1);
	int m = strlen(word2);
	int i, j, sub_cost, sub, del, ins;
	/* Create (n+1)x(m+1) array with leven[i, j] being the edit distance between
	 * first i chars of word1 and first j chars of word 2. */
	int **leven = malloc((n+1) * sizeof(int *));
	for (i=0; i<n+1; i++) {
		leven[i] = malloc((m+1) * sizeof(int));
	}

	/* Fill in known values (the distance between string and null) */
	for (i=0; i<=n; i++) {
		leven[i][0] = i;
	}
	for (j=1; j<=m; j++) {
		leven[0][j] = j;
	}

	/* Fill in the array with edit distances */
	for (i=1; i<=n; i++) {
		for (j=1; j<=m; j++) {
			if (word1[i-1] == word2[j-1]) {
				sub_cost = 0;
			}
			else {
				sub_cost = 1;
			}
			sub = leven[i-1][j-1] + sub_cost;
			del = leven[i-1][j] + 1;
			ins = leven[i][j-1] + 1;

			leven[i][j] = min(sub, del, ins);
		}
	}
	return leven[n][m];
}


// adds curr Node to the tree
void add(Node root, Node curr) {
	printf("%s\n", root.word);
  /* if it is the first Node */
  if (strcmp(root.word, "")) {
			printf("%s\n", root.word);
    /* then make it the root Node */
    root.word = curr.word;
    return;
  }

  // get edit distance from the Root
  int dist = leven_dist(root.word, curr.word);
	/* if no Node exists at this dist from root, make it child */
  if (strcmp(tree[root.next[dist]].word, "")) {

    // incrementing the pointer for curr Node
    ptr++;

    // adding curr Node to the tree
    tree[ptr] = curr;

    // curr as child of root Node
    root.next[dist] = ptr;
  }
  else {
    // recursively find the parent for curr Node
    add(tree[root.next[dist]], curr);
  }
}


char **getSimilarWords(Node root, char *s) {
	static char *ret[N+1];
	int num = 0;

  if (strcmp(root.word, NULL))
     return ret;

  // calculating edit distance of s from root
  int dist = leven_dist(root.word, s);

  // if dist is less than tolerance value
  // add it to similar words
  if (dist <= TOL) {
    ret[num] = root.word;
		num++;
	}

  // iterate over the string having tolerance
  // in range (dist-TOL , dist+TOL)
  int start = dist - TOL;
  if (start < 0)
     start = 1;

  while (start < dist + TOL) {
    getSimilarWords(tree[root.next[start]], s);
    start++;
  }
	ret[num+1] = "EOF";
	return ret;
}
