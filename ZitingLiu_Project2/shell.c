#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct node
{
	struct node* next;
	char * command;
};

struct node* split(const char* in){
	//printf("input line it %s\n",in);
	char* quo="\"";
	struct node* temp;
	//temp = malloc(sizeof(struct node));
	//temp->next=malloc(sizeof(struct node));
	//temp->command=malloc( sizeof(*temp->command) * 256 );
	struct node* head;
	head = malloc(sizeof(struct node));
	head->next=malloc(sizeof(struct node));
	head->command=malloc( sizeof(char) * 256 );
	temp=head;
	int hasSomething=0;
	int index=0;
	char *newcommand=malloc(100);
	int open=0;
	for(int i=0;i<100;i++){
		
		if(in[i]==' '&&open==0){
			if(hasSomething && in[i+1]!='\n'&& in[i+1]!=' '){
				newcommand[index]='\0';
				strcpy(temp->command,newcommand);
				//printf("found element %s \n", temp->command);
				//printf("new command is %s \n",newcommand);
				temp=temp->next;
				
				temp->next=malloc(sizeof(struct node));
				temp->command=malloc( sizeof(char) * 256 );
				free(newcommand);
				//newcommand=NULL;
				char *newcommand=malloc(100);
				index=0;
				hasSomething=0;
			}		
		}else if(in[i]=='\n'){
			if(hasSomething){
				//temp->command=malloc( sizeof(*temp->command) * 256 );
				//temp->command=NULL;
				//printf("new command is %s \n",newcommand);
				newcommand[index]='\0';
				strcpy(temp->command,newcommand);
			}

		}else{
			if(in[i]=='\"'){
				if(open==0){
					open=1;
				}else{
					open=0;
				}
			}
			newcommand[index]='\0';
			newcommand[index]=in[i];
			//printf("new command is %s \n",newcommand);
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
	pid_t procid[3];
	
	//if (i!=0) { printf("pipe creation failed\n"); exit(1); }
	while(1){
		char cwd[100];
		getcwd(cwd,sizeof(cwd));
		printf("\n%s>> ", cwd);
		char *cmd1 = malloc( sizeof(char) * 256 );
		char *cmd2 = malloc( sizeof(char) * 256 );
		char *argv1[100];
		char *argv2[100];
		int two=0;
		int parallelProcesses=0;
		int backProcess=0;
		int redirectR=0;
		int redirectL=0;
		int ind1=0;
		int ind2=0;
		i = pipe(pfd); // create pipe
		if (i!=0) { printf("pipe creation failed\n"); exit(1); }

		//char * cheatspace=" ";
		char *buff=malloc( sizeof(*buff) * 256 );
		fgets(buff ,100,stdin);
		fputs(buff ,stdout);
		//strcat(buff,cheatspace);
		procid[0]=fork();
		if(procid[0]==0){ 
			                               //if this is child process
			struct node* current;
			current = malloc(sizeof(struct node));
			current->next=malloc(sizeof(struct node));
			current=split(buff);
			
			struct node* head;
			head = malloc(sizeof(struct node));
			head->command ="0";
			head->next = malloc(sizeof(struct node));
			int counter=0;
			int para=-1,back=-1,rediR=-1,rediL=-1,changeDir=-1;
			while(current != NULL&&current->command!=NULL)
			{	
				char* c=current->command;
				char* parallel="|";
				char* ba="&";
				char* reR=">";
				char* reL="<";
				para=strcmp(c,parallel);
				back=strcmp(c,ba);
				rediR=strcmp(c,reR);
				rediL=strcmp(c,reL);
				if(para==0 ){
					two=1;
					parallelProcesses=1;
				}else if(back==0){
					two=1;
					backProcess=1;
				}else if(rediR==0){
					two=1;
					redirectR=1;
				}else if(rediL==0){
					two=1;
					redirectL=1;
				}else{
					if(two==0){
						if(ind1==0){
							cmd1=c;
							argv1[0]=c;
							ind1++;
						}else{
							argv1[ind1]=c;
							ind1++;
						}

					}else{
						if(ind2==0){
							cmd2=c;
							argv2[0]=c;
							ind2++;
						}else{
							argv2[ind2]=c;
							ind2++;
						}
					}
				}
				argv1[ind1]=NULL;
				argv2[ind2]=NULL;
				counter++;
				head = current;
				current = current->next;
				free(head);
				
			}
			free(current);
			free(buff);
			
			//////////////////////////////////////////////////////////////////
			//printf("got here");
			if(strcmp(cmd1,"cd")==0){
				printf("command: cd\n");
				char newdir[100];
				strcpy(newdir,argv1[1]);
				strcat(cwd,newdir);
				chdir(newdir);
				//exit(0);
			}
			else if(two==0){
				printf("command : %s \n",cmd1);
				printf("%s :", cmd1);
				for(int iter=1;iter<ind1;iter++){
					printf("%s ",argv1[iter]);
				}
				printf("\n");
				execvp(cmd1,argv1);
			}else if(parallelProcesses==1){
				printf("command: %s  %s \n",cmd1,cmd2);
				printf("%s :",cmd1);
				for(int iter=1;iter<ind1;iter++){
					printf("%s ",argv1[iter]);
				}
				printf("\n");
				printf("%s :",cmd2);
				for(int iter=1;iter<ind2;iter++){
					printf("%s ",argv2[iter]);
				}
				printf("\n");
				fflush(stdout);
				argv1[ind1]=NULL;
				argv2[ind2]=NULL;
				//printf("this\n");
				procid[1]=fork();
				if(procid[1]==0){
					int saved=dup(1);
					
					dup2(pfd[1],1);
					close(pfd[0]);//close read
					close(pfd[1]);
					execvp(cmd1,argv1);
					perror("execvp");
       				exit(1);
					//puts("finished");
					//dup2(stdout,1);
				}
				
				procid[2]=fork();
				if(procid[2]==0){
					dup2(pfd[0],STDIN_FILENO);
					close(pfd[1]);
					close(pfd[0]);
					execvp(cmd2,argv2);
				}
				close(pfd[1]);
				close(pfd[0]);
				waitpid(procid[1],NULL,0);
				waitpid(procid[2],NULL,0);
				exit(1);
				exit(0);
			}
			else if(backProcess==1){
				printf("command : %s \n",cmd1);
				procid[1]=fork();
				if(procid[1]==0){
					printf("%s :", cmd1);
					for(int iter=1;iter<ind1;iter++){
						printf("%s ",argv1[iter]);
					}
					printf("\n");
					execvp(cmd1,argv1);
				}
				waitpid(procid[1],NULL,WNOHANG);
				exit(0);
			}
			else if(redirectL==1){
				printf("command : %s\n",cmd1);
				int file = open(cmd2, O_RDONLY);
				dup2(file,0);
				execvp(cmd1,argv1);
				
			}
			else if(redirectR==1){
				
				printf("command : %s \n",cmd1);
				printf("%s :", cmd1);
				for(int iter=1;iter<ind1;iter++){
					printf("%s ",argv1[iter]);
				}

				printf("\n");
				int file=open(cmd2,O_WRONLY | O_CREAT, 0777);
				int saved=dup(1);
				dup2(file,1);
				//printf("test\n");
				execvp(cmd1,argv1);
				perror("execvp");
				close(file);
       			exit(1);
			}else{
				
				//printf("two commands\n");
			}	

			if(two==1){
				//procid[1]=fork();
			}
			//if(procid[1]==0){
				//printf("I am grand child\n");
			//}else{
				//printf("I am child\n");
			//}
			//printf("exe\n");
			
			

			//exit(0);

		}else{
			close(pfd[1]);
			close(pfd[0]);
			waitpid(procid[0],NULL,0);
			//fputs("finish\n",stdout);
		}
		

		
	}

}
