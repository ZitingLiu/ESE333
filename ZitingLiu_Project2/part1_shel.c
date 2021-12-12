#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct node
{
	struct node* next;
	char * command;
};

struct node* split(const char* in){
	//printf("input line it %s\n",in);
	struct node* temp;
	temp = malloc(sizeof(struct node));
	temp->next=malloc(sizeof(struct node));
	temp->command=malloc( sizeof(*temp->command) * 256 );
	struct node* head;
	head = malloc(sizeof(struct node));
	head->next=malloc(sizeof(struct node));
	head->command=malloc( sizeof(*head->command) * 256 );
	temp=head;
	int hasSomething=0;
	int index=0;
	char *newcommand=malloc(100);
	for(int i=0;i<100;i++){
		
		if(in[i]==' '){
			if(hasSomething){
				
				strcpy(temp->command,newcommand);
				//printf("found element %s \n", temp->command);
				//printf("new command is %s \n",newcommand);
				temp=temp->next;
				
				temp->next=malloc(sizeof(struct node));
				temp->command=malloc( sizeof(*temp->command) * 256 );
				free(newcommand);
				char *newcommand=malloc(100);
				index=0;
				hasSomething=0;
			}		
		}else if(in[i]=='\n'){
			if(hasSomething){
				//temp->command=malloc( sizeof(*temp->command) * 256 );
				strcpy(temp->command,newcommand);
			}

		}else{
			newcommand[index]=in[i];
			hasSomething=1;
			index+=1;
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
	int i,j;
	int pfd[2];
	pid_t procid;
	i = pipe(pfd); // create pipe
	if (i!=0) { printf("pipe creation failed\n"); exit(1); }
	while(1){
		char *buff=malloc( sizeof(*buff) * 256 );
		fgets(buff ,100,stdin);
		fputs(buff ,stdout);
		procid=fork();
		if(procid==0){                                   //if this is child process
			struct node* current;
			current = malloc(sizeof(struct node));
			current->next=malloc(sizeof(struct node));
			//current->command=(char*)malloc(100);
			current=split(buff);
			
			struct node* head;
			head = malloc(sizeof(struct node));
			head->command ="0";
			head->next = malloc(sizeof(struct node));
			//fputs("commands are \n",stdout);
			int counter=0;
			while(current != NULL)
			{	
				//printf("element:");
				printf("%d found element %s \n", counter,current->command);
				//fputs(current->command,stdout);
				counter++;
				head = current;
				current = current->next;
				free(head);
				
				
			}
			free(current);

			free(buff);
			exit(0);

		}
		//char *cmd="mkdir";
		//char *argv[3];
		//argv[0]="mkdir";
		//argv[1]="tesst";
		//argv[2]=NULL;
		//char temp[]=command;
		//fputs(c ,stdout);
		//execvp(cmd,argv);
		//printf("%s\n",c);

		
	}

}