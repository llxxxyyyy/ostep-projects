#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>

char *path[20];

size_t erro_massage(){
    static const char error_message[30] = "An error has occurred\n";
    return write(STDERR_FILENO, error_message, strlen(error_message));
}



int main(){
    char *line = NULL;
    char *token[20];
    char *redir_file;
    size_t inputsiz=0;

    while (1)
    {
        printf("wish>");

        int res=getline(&line,&inputsiz,stdin);

        if (res==1)
        {
            continue;
        }
        else if(res==-1)
        {
            exit(0);
        }
        else{
            //get toknes from input line
            int length = 0;
            const char space=' ';
            while (1)
            {
                token[length]=strsep(&line,&space);
                if (token[length]==NULL)
                {
                    break;
                }
                length++;
            }
            
            if (strcmp(token[0],"cd")==0)
            {
                if (length>2)
                {
                    erro_massage();
                }
                //change directory 
                if(chdir(token[1])!=0){
                    erro_massage();
                    exit(1);
                }
                continue;
            }
            if (strcmp(token[0],"exit")==0)
            {
                exit(0);
            }
            if (strcmp(token[0],"path")==0)
            {

                //update path
                int p=0;
                while (1)
                {
                    if (token[p]==NULL)
                    {
                        path[p]=NULL;
                        break;
                    }
                    else{
                        path[p]=(char *)malloc(strlen(token[p]+1));
                        strcpy(path[p],token[p]);
                    }
                    p++;
                }
                //print path
                for ( int j = 0; j <= p; j++)
                {
                    if (token[j]==NULL)
                    {
                        printf("\n");
                        break;
                    }
                    else{
                        printf("%s",token[j]);
                    }
                    j++;
                }
                
                continue;             
            }
            
            int redir_index;
            int type=1;
            while (1)
            {
                if (token[redir_index]==NULL)
                {
                    redir_index=-1;
                    type=0;
                    break;
                }
                /*if (strcmp(token[redir_index],"<")==0)
                {
                    in_or_out=1;
                }
                if (strcmp(token[redir_index],">")==0)
                {
                    in_or_out=2;
                }*/
                
                redir_index++;
            }

            if (length-2 > redir_index)
            {
                if (type!=0)
                {
                    erro_massage();
                    exit(1);
                }
            }
            else
            {
                redir_file=token[redir_index+1];
                free(token[redir_index]);    
            }
            //change
            int pid_ind;
            
            if (pid_ind==-1)
            {
                erro_massage();
                exit(1);
            }
            
            //
            int pid_child=fork();

            if (pid_child==-1)
            {
                erro_massage();
                exit(1);
            }
            else if (pid_child==0)
            {
                if (redir_file == NULL)
                {
                    erro_massage();
                    exit(1);
                }
                
                close(STDOUT_FILENO);
                open(redir_file,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                
                int exe=execv(token[0],token);
                if (exe==-1)
                {
                    erro_massage();
                    exit(1);
                }
                
            }
            else
            {
                wait(NULL);
            }

        }

    }
    
}

