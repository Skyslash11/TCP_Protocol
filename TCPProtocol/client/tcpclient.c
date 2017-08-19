/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*
argv[2]: ipaddress of server
argv[1]: port no of server
 */

int
main(int argc, char *argv[]) {

    int sock, bytes_received, bytes_sent,i;
    struct hostent *host;
    struct sockaddr_in server_addr;
    if (argc < 3) {
        printf("Enter PortNo");
        exit(0);
    }

    host = gethostbyname(argv[2]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(server_addr.sin_zero), 8);
	//printf("Type a word to send(q or Q to quit):");
    if (connect(sock, (struct sockaddr *) &server_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Connect");
        exit(1);
    }
    /*My code starts here*/
    //bytes_received = recv(sock, recv_data, 1024, 0);
    //recv_data[bytes_received] = '\0';
    //printf("\nRecieved data = %s ", recv_data);
	while (1) {
	char *send_data=(char *)malloc(sizeof(char)*1024);
        printf("(ftp) ");
        fgets(send_data,1024,stdin);
	bytes_sent=strlen(send_data);
	send_data[bytes_sent-1]='\0';
	char *recv_data=(char *)malloc(sizeof(char)*1024);
	
	if ((strcmp(send_data, "quit") == 0) )
            break;

        /*else if(send(sock,send_data,strlen(send_data),0) == -1)
	    {
			perror("Failure sending command\n");
			exit(1);
		}*/

        else 
        {
			if(send_data[0] == 'l' && send_data[1] == 's' && (send_data[2] == 0 || send_data[2] == 32))
			{
				
				for(i=bytes_sent+1;i>1;--i)
				{
					send_data[i]=send_data[i-2];
				}
				send_data[0]='N';
				send_data[1]='L';
				send_data[2]='S';
				send_data[3]='T';
				send(sock,send_data,strlen(send_data),0);
				bytes_received = recv(sock, recv_data, 1024, 0);
            			recv_data[bytes_received] = '\0';
            			printf("%s\n", recv_data);
			}
			
			else if(send_data[0] == 'c' && send_data[1] == 'd' && (send_data[2] == 0 || send_data[2] == 32))
			{
				
				for(i=bytes_sent;i>0;--i)
				{
					send_data[i]=send_data[i-1];
				}
				send_data[0]='C';
				send_data[1]='W';
				send_data[2]='D';
				send(sock,send_data,strlen(send_data),0);
				bytes_received = recv(sock, recv_data, 1024, 0);
            			recv_data[bytes_received] = '\0';
           			 printf("Server pwd : %s\n", recv_data);
			}
	
			else if(send_data[0] == 'l' && send_data[1] == 'c' && send_data[2] == 'd' && (send_data[3] == 0 || send_data[3] == 32))
			{
				char *dir_name=(char *)malloc(sizeof(char)*1024);
				char *cwd_name=(char *)malloc(sizeof(char)*1024);
				if(send_data[3] == 0)
				{
					
				dir_name[0]='.';
				dir_name[1]= '/';
				dir_name[2]= 0;
				}
				else
				{
					i=4;
					dir_name[0]='.';
					dir_name[1]='/';
					while(send_data[i]!= 0)
					{
						dir_name[i-2]=send_data[i];
						++i;
					}
					dir_name[i-2]='\0';
				}
				int ret;
				char *buff=(char *)malloc(sizeof(char)*1024);
				ret = chdir(dir_name);
				if(ret == -1)
				{
					printf("Invalid directory name\n");
				}
				else
				{
					cwd_name=getcwd(buff,1024);
					printf("Client pwd : %s\n",cwd_name);
				}
				
			}

			else if(send_data[0] == 'g' && send_data[1] == 'e' && send_data[2] == 't' && (send_data[3] == 0 || send_data[3] == 32))
			{
				char *file_name=(char *)malloc(sizeof(char)*1024);
				
				for(i=bytes_sent;i>0;--i)
				{
					send_data[i]=send_data[i-1];
				}
				send_data[0]='R';
				send_data[1]='E';
				send_data[2]='T';
				send_data[3]='R';
				send(sock,send_data,strlen(send_data),0);
				i=5;
					while(send_data[i]!= 0)
					{
						file_name[i-5]=send_data[i];
						printf("%c %s\n",send_data[i],send_data);
						++i;
					}
					file_name[i-5]='\0';
				//printf("\nfile name = %s   %d\n",file_name,strlen(file_name));
				FILE *fp;
				fp=fopen(file_name,"w");
				if(fp == NULL)
				{
					printf("Error opening file\n");
					exit(1);
				}
				
				i=0;
				while (strcmp(recv_data , "@$") != 0)
				{
					if(strcmp(recv_data , "@#")==0)
					{
						perror("No such file in the server\n");
						//fclose(fp);
						//free(recv_data);
						goto in;
					}
					//printf("%s",recv_data);
					if(i==1)
					{
						
					}
					if(i!=0)
					{
						fprintf(fp,"%s",recv_data);
					}
					++i;					
					bytes_received = recv(sock, recv_data, 1024, 0);
					recv_data[bytes_received] = '\0';
					
				}
				in :
				fclose(fp);
				free(recv_data);
				free(file_name);
				free(send_data);
			}
			
			else if(send_data[0] == 'p' && send_data[1] == 'u' && send_data[2] == 't' && (send_data[3] == 0 || send_data[3] == 32))
			{
				
				char *file_name=(char *)malloc(sizeof(char)*1024);
				char *file_path=(char *)malloc(sizeof(char)*1024);
				for(i=bytes_sent;i>0;--i)
				{
					send_data[i]=send_data[i-1];
				}
				send_data[0]='S';
				send_data[1]='T';
				send_data[2]='O';
				send_data[3]='R';
				send(sock,send_data,strlen(send_data),0);
				i=5;
					while(send_data[i]!= 0)
					{
						file_name[i-5]=send_data[i];
						++i;
					}
					file_name[i-5]='\0';
					char buff[1024];
				        file_path=getcwd(buff,1024);
					strcat(file_path,"/");
					strcat(file_path,file_name);
					char ch;
					FILE *fp;
					char *fch=(char *)malloc(sizeof(char)*3);
					fp=fopen(file_path,"r");
					if(fp == NULL)
					{
						send(sock,"@#",2,0);
						perror("ERROR opening the file\n");
						goto lable1;
					}
					while((fch[0]=fgetc(fp)) != EOF)
					{
						fch[1]=0;
						bytes_sent=send(sock,fch,strlen(fch),0);
					}
					fclose(fp);
					bytes_sent=send(sock,"@$",2,0);
					lable1 :
					printf("");
			}
			
			else if(send_data[0] == 'p' && send_data[1] == 'w' && send_data[2] == 'd' && send_data[3] == 0)
			{
				send_data[0]='P';
				send_data[1]='W';
				send_data[2]='D';
				send(sock,send_data,strlen(send_data),0);
				bytes_received = recv(sock, recv_data, 1024, 0);
				recv_data[bytes_received] = '\0';
				printf("%s\n",recv_data);
			}
			else
			{
				printf("Invalid command\n");
			}
		}    
    }

    close(sock);
    
    return 0;
}
