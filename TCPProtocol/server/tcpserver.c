/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

int
main(int argc, char *argv[]) {
    int sock, connected, bytes_sent,bytes_received, true = 1,i;
    char *root_path=(char *)malloc(sizeof(char)*1024);
    char buf[1024];
    root_path=getcwd(buf,1024);
    struct sockaddr_in server_addr, client_addr;
    int sin_size;
    if (argc < 2) {
        printf("Enter PortNo");
        exit(0);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *) &server_addr, sizeof (struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }

    if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
    }

   
	
    while (1) {
	 printf("\nTCPServer Waiting for client on port %s\n", argv[1]);
    	fflush(stdout);
        sin_size = sizeof (struct sockaddr_in);
        connected = accept(sock, (struct sockaddr *) &client_addr, &sin_size);
	
        printf("\n I got a connection from (%s , %d)\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /*printf("\n SEND A WORD(q to quit): ");
        scanf("%s", send_data);
        send(connected, send_data, strlen(send_data), 0);
        if (strcmp(send_data, "q") == 0 || strcmp(send_data, "Q") == 0) {
            break;

        }*/
		while(1)
		{
			char *send_data=(char *)malloc(sizeof(char)*1024);
			char *recv_data=(char *)malloc(sizeof(char)*1024);
			bytes_received=recv(connected,recv_data,1024,0);
			if(bytes_received == -1)
			{
				perror("recv");
				printf("Message Received : %s\n",recv_data);
				exit(1);
			}	
			else if(bytes_received == 0)
			{
				printf("Connedtion closed\n");
				break;
			}
			else
			{
				recv_data[bytes_received]='\0';
				printf("Message Received : %s\n",recv_data);
			}	
		
			if(recv_data[0] == 'N' && recv_data[1] == 'L' && recv_data[2] == 'S' && recv_data[3] == 'T' && (recv_data[4] == 0 || recv_data[4] == 32))
			{
				char *dir_name=(char *)malloc(sizeof(char)*22);
				if(recv_data[4] == 0)
				{
					
				dir_name[0]='.';
				dir_name[1]= '/';
				dir_name[2]= 0;
				}
				else
				{
					i=5;
					dir_name[0]='.';
					dir_name[1]='/';
					while(recv_data[i]!= 0)
					{
						dir_name[i-3]=recv_data[i];
						++i;
					}
					dir_name[i-3]='\0';
				}
				DIR *dir;
				struct dirent *dp;
				dir = opendir(dir_name);
				dp=readdir(dir);
				char *dir_contents=(char *)malloc(sizeof(char)*1024);
				while(dp != NULL)
				{
				if((strcmp(dp->d_name,".")==0) || (strcmp(dp->d_name,"..")==0))
				{
					dp=readdir(dir);
					continue;
				}				
				else
				{
					strcat(dir_contents,dp->d_name);
					strcat(dir_contents,"  ");	
					dp=readdir(dir);
				}
				}
				bytes_sent=send(connected,dir_contents,strlen(dir_contents),0);
			}

			if(recv_data[0] == 'C' && recv_data[1] == 'W' && recv_data[2] == 'D' && (recv_data[3] == 0 || recv_data[3] == 32))
			{
				char *dir_name=(char *)malloc(sizeof(char)*1024);
				char *cwd_name=(char *)malloc(sizeof(char)*1024);
				if(recv_data[3] == 0)
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
					while(recv_data[i]!= 0)
					{
						dir_name[i-2]=recv_data[i];
						++i;
					}
					dir_name[i-2]='\0';
				}
				
				int ret;
				char *buff=(char *)malloc(sizeof(char)*1024);
				if(strcmp(dir_name , "./..")==0)
				{
					cwd_name=getcwd(buff,1024);
					if(strcmp(root_path,cwd_name)==0)
					{
						bytes_sent=send(connected,"No access to go below root",26,0);
					}
					else
					{
						ret = chdir(dir_name);
						cwd_name=getcwd(buff,1024);
						bytes_sent=send(connected,cwd_name,strlen(cwd_name),0);
					}
				}
				else
				{
					ret = chdir(dir_name);
					if(ret == -1)
					{
						bytes_sent=send(connected,"Invalid directory name",22,0);
					}
					else
					{
						cwd_name=getcwd(buff,1024);
						bytes_sent=send(connected,cwd_name,strlen(cwd_name),0);
					}				
				}
				free(cwd_name);
			}
			
			if(recv_data[0] == 'R' && recv_data[1] == 'E' && recv_data[2] == 'T' && recv_data[3] == 'R' &&  recv_data[4] == 32)
			{
				char *file_name=(char *)malloc(sizeof(char)*1024);
				char *file_path=(char *)malloc(sizeof(char)*1024);
					i=5;
					while(recv_data[i]!= 0)
					{
						file_name[i-5]=recv_data[i];
						++i;
					}
					file_name[i-5]='\0';
					char buff[1024];
				        file_path=getcwd(buff,1024);
					strcat(file_path,"/");
					strcat(file_path,file_name);
					printf("hello it is %s\n",file_path);
					char ch;
					FILE *fp;
					char *fch=(char *)malloc(sizeof(char)*3);
					fp=fopen(file_path,"r");
					if(fp == NULL)
					{
						send(connected,"@#",2,0);
						perror("ERROR opening the file\n");
						goto lable1;
					}
					while((fch[0]=fgetc(fp)) != EOF)
					{
						fch[1]=0;
						bytes_sent=send(connected,fch,strlen(fch),0);
					}
					fclose(fp);
					bytes_sent=send(connected,"@$",2,0);
					lable1 :
					printf("get is done\n");
					
			}
			
			if(recv_data[0] == 'S' && recv_data[1] == 'T' && recv_data[2] == 'O' && recv_data[3] == 'R' &&  recv_data[4] == 32)
			{
				char *file_name=(char *)malloc(sizeof(char)*1024);
				i=5;
					while(recv_data[i]!= 0)
					{
						file_name[i-5]=recv_data[i];
						++i;
					}
					file_name[i-5]='\0';
					printf("\nfile name = %s   %d\n",file_name,strlen(file_name));
				FILE *fp;
				fp=fopen(file_name,"w");
				if(fp == NULL)
				{
					perror("Error opening file\n");
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
					if(i!=0)
					{
						fprintf(fp,"%s",recv_data);
					}
					++i;					
					bytes_received = recv(connected, recv_data, 1024, 0);
					recv_data[bytes_received] = '\0';
					
				}
				in :
				fclose(fp);
				printf("I am out of put \n");
				free(recv_data);
				free(file_name);
				free(send_data);
			}

			if(recv_data[0] == 'P' && recv_data[1] == 'W' && recv_data[2] == 'D' &&  recv_data[3] == 0)
			{
				char *pwd_name=(char *)malloc(sizeof(char)*1024);
				char buff[1024];
				pwd_name=getcwd(buff,1024);
				bytes_sent=send(connected,pwd_name,strlen(pwd_name),0);
			}
			/*bytes_sent=send(connected,send_data,strlen(send_data),0);*/
			if(bytes_sent == -1)
			{
				fprintf(stderr, "Error sending message\n");
				break;
			}
			//printf("Message sent is : %s\n",send_data);
		}
        
        close(connected);
    }

    close(sock);
    return 0;
}
