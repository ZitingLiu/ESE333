#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct node
{
	struct node* next;
	char* command;
};

struct node* split(const char* in) {
	//printf("input line it %s\n",in);
	char* quo = "\"";
	struct node* temp;
	//temp = malloc(sizeof(struct node));
	//temp->next=malloc(sizeof(struct node));
	//temp->command=malloc( sizeof(*temp->command) * 256 );
	struct node* head;
	head = malloc(sizeof(struct node));
	head->next = malloc(sizeof(struct node));
	head->command = malloc(sizeof(*head->command) * 256);
	temp = head;
	int hasSomething = 0;
	int index = 0;
	char* newcommand = malloc(100);
	int open = 0;
	for (int i = 0; i < 100; i++) {

		if (in[i] == ' ' && open == 0) {
			if (hasSomething && in[i + 1] != '\n' && in[i + 1] != ' ') {

				strcpy(temp->command, newcommand);
				//printf("found element %s \n", temp->command);
				//printf("new command is %s \n",newcommand);
				temp = temp->next;

				temp->next = malloc(sizeof(struct node));
				temp->command = malloc(sizeof(*temp->command) * 256);
				free(newcommand);
				//newcommand=NULL;
				char* newcommand = malloc(100);
				index = 0;
				hasSomething = 0;
			}
		}
		else if (in[i] == '\n') {
			if (hasSomething) {
				//temp->command=malloc( sizeof(*temp->command) * 256 );
				//temp->command=NULL;
				//printf("new command is %s \n",newcommand);
				strcpy(temp->command, newcommand);
			}

		}
		else {
			if (in[i] == '\"') {
				if (open == 0) {
					open = 1;
				}
				else {
					open = 0;
				}
			}
			newcommand[index] = NULL;
			newcommand[index] = in[i];
			//printf("new command is %s \n",newcommand);
			hasSomething = 1;
			index += 1;
		}
	}
	//////////////////////


	//printf("new command is %s \n",newcommand);
	//for(int j=0;j<100;j++) newcommand[j]=NULL;

	free(newcommand);
	return head;
}

int main()
{
	int i, j;
	int pfd[2];
	pid_t procid[2];
	i = pipe(pfd); // create pipe
	if (i != 0) { printf("pipe creation failed\n"); exit(1); }
	while (1) {
		char* cmd1 = malloc(sizeof(*cmd1) * 256);
		char* cmd2 = malloc(sizeof(*cmd2) * 256);
		char* argv1[100];
		char* argv2[100];
		int two = 0;
		int ind1 = 0;
		int ind2 = 0;
		i = pipe(pfd);
		if (i != 0) { printf("pipe creation failed\n"); exit(1); }

		char* buff = malloc(sizeof(*buff) * 256);
		fgets(buff, 100, stdin);
		fputs(buff, stdout);
		struct node* current;
		current = malloc(sizeof(struct node));
		current->next = malloc(sizeof(struct node));
		current = split(buff);

		struct node* head;
		head = malloc(sizeof(struct node));
		head->command = "0";
		head->next = malloc(sizeof(struct node));
		int counter = 0;
		int para = -1, back = -1, rediR = -1, rediL = -1;
		while (current != NULL && current->command != NULL)
		{
			char* c = current->command;
			char* parallel = "|";
			char* back = "&";
			char* rediR = "<";
			char* rediL = ">";
			para = strcmp(c, parallel);
			if (para == 0 || back == 0 || rediR == 0 || rediL == 0) {
				two = 1;
			}
			else {
				if (two == 0) {
					if (ind1 == 0) {
						cmd1 = c;
						argv1[0] = c;
						ind1++;
					}
					else {
						argv1[ind1] = c;
						ind1++;
					}

				}
				else {
					if (ind2 == 0) {
						cmd2 = c;
						argv2[0] = c;
						ind2++;
					}
					else {
						argv2[ind2] = c;
						ind2++;
					}
				}
			}
			counter++;
			head = current;
			current = current->next;
			free(head);
			//printf("gothere\n");

		}
		free(current);
		free(buff);

		//int myread;
		//close(pfd[1]);
		//read(pfd[0],&myread,sizeof(int));
		//printf("number I got was %d \n",myread);
		//printf("got here");
		if (two == 0) {
			printf("command : %s \n", cmd1);
			printf("%s :", cmd1);
			for (int iter = 1; iter < ind1; iter++) {
				printf("%s ", argv1[iter]);
			}
			printf("\n");
		}
		else {
			printf("command: %s  %s \n", cmd1, cmd2);
			printf("%s :", cmd1);
			for (int iter = 1; iter < ind1; iter++) {
				printf("%s ", argv1[iter]);
			}
			printf("\n");
			printf("%s :", cmd2);
			for (int iter = 1; iter < ind2; iter++) {
				printf("%s ", argv2[iter]);
			}
			printf("\n");
			//printf("two commands\n");
		}

		if (two == 1) {
			//procid[1]=fork();
		}
		//if(procid[1]==0){
			//printf("I am grand child\n");
		//}else{
			//printf("I am child\n");
		//}
		procid[0] = fork();
		if (procid[0] == 0) {
			//execvp(cmd1,argv1);
			exit(0);

		}
		else {
			wait();
			//printf("I am main\n");
			//char *cmd="mkdir";
			//char *argv[100];
			//argv[0]="mkdir";
			//argv[1]="text";
			//argv[2]=NULL;
			//char temp[]=command;
			//fputs(c ,stdout);
			//execvp(cmd,argv);

		}



	}

}
