#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#define BUFFER_SIZE 100

typedef struct {
	int Is_Being; //0未上线，1上线
	char Is_Friend[10][20]; //好友列表。目前设置为10
}zhuangtai;

typedef struct {
	char user_ID[20]; //账号
	char password[20];  //密码
	zhuangtai zt;
}user;

// 注册账号
int write_user(char *userID, char *password)
{
	FILE *file_user, *file_password;
	file_user = fopen("user_ID.txt", "w+");
	file_password = fopen("password.txt", "w+");
	int i = 0;
	if (file_user != NULL && file_password != NULL)
	{
		fwrite(userID, sizeof(*userID), 1, file_user);
		fwrite("\t0", sizeof("\t0"), 1, file_user);
		fputc('\n', file_user);

		fwrite(password, sizeof(*password), 1, file_password);
		fputc('\n', file_password);
		i++;
	}

	fclose(file_user);
	fclose(file_password);
	return i;
}

// 读取存放账号的文件，如果账号不存在，则返回0，密码不存在返回1，没问题返回2
int read_user(char *userID, char *password, int is_Login)
{
	FILE *file_user, *file_password;
	char buff[20];
	int istrue = 0;
	int hang = 0;
	file_user = fopen("user_ID.txt", "a");
	file_password = fopen("password.txt", "r");
	if (file_user != NULL)
	{
		while (fscanf(file_user, "%[^\n]", buff) != EOF)
		{
			hang++;
			fgetc(file_user);
			if (strcmp(buff, userID) == 0)
			{
				if (is_Login == 1)
				{
					if (buff[strlen(buff) - 1] == '1')
					{
						return 3;
					}
					return 4;
				}

				istrue++;
				break;
			}
			memset(buff, 0, sizeof(buff));
		}

		if (file_password != NULL && istrue > 0)
		{
			memset(buff, 0, sizeof(buff));
			while (fscanf(file_password, "%[^\n]", buff) != EOF)
			{
				hang--;
				fgetc(file_password);
				if (strcmp(buff, password) == 0 && hang == 0)
				{
					istrue++;
					break;
				}
				memset(buff, 0, sizeof(buff));
			}
		}
	}

	fclose(file_user);
	fclose(file_password);
	return istrue;
}

//是否是朋友
int IsFriend(user *user_A, char *user_B)
{
	FILE *file;
	char buff[20];
	file = fopen("Friend.txt", "r");
	if (file != NULL)
	{
		while (fscanf(file, "%[^\n]", buff) != EOF)
		{
			fgetc(file);
			if ((strcmp(buff, user_A->user_ID) == 0) && (strcmp(buff, user_B) == 0))
			{
				fclose(file);
				return 1;
			}
			memset(buff, 0, sizeof(buff));
		}
		fclose(file);
		return 0;
	}
}

// 1 发送消息给 2
char A_send_B(user *user_A, char *user_B, char *buff, int sockfd_B)
{
	char a[100];
	memset(a, 0, sizeof(a));
	//判断2是否为好友
	if (IsFriend(user_A, user_B) == 0)
	{
		return '0';
	}

	//判断2是否在线
	if (read_user(user_B, NULL, 1) == 3)
	{
		return '1';
	}

	//开始通信
	//memset(buff, 0, sizeof(buff));
	//printf("请输入:");
	//scanf("%s",buff);
	strcpy(a, buff);
	strcat(a, ":");
	strcat(a, "\0");
	if (send(sockfd_B, a, sizeof(a), 0) < 0)
	{
		//perror("%s 发送给 %s的信息失败了！\n", user_A->user_ID, user_B->user_ID);
		exit(1);
	}

	if (send(sockfd_B, a, sizeof(a), 0) < 0)
	{
		printf("%s 发送给 %s的信息失败了！\n", user_A->user_ID, user_B);
		exit(1);
	}

	return '2';
}

int addfriend(user *use, char *friend)
{
	int i;
	for (i = 0; i < 10; i++)
	{
		if (strlen(use->zt.Is_Friend[i]) <= 0)
		{
			strcat(use->zt.Is_Friend[i], friend);
			return 1;
		}

	}
	return 0;
}

//服务器读写方法
void listen_data(int fd, int id)
{
	char buffer[BUFFER_SIZE];
	char use[20];
	char pass[20];
	char num[2];
	user *myself = (user *)malloc(sizeof(user));

	memset(myself, 0, sizeof(user));

	while (1)     //一直处于监听客户端信息状态,知道客户端断开或客户端发出退出信息
	{
		memset(buffer, 0, BUFFER_SIZE);
		memset(use, 0, sizeof(user));
		memset(pass, 0, sizeof(pass));
		memset(num, 0, sizeof(num));

		int len = recv(fd, num, sizeof(num), 0);
		switch (atoi(num))
		{
		case 1:
			memset(num, 0, sizeof(num));
			recv(fd, use, sizeof(use), 0);
			recv(fd, pass, sizeof(pass), 0);
			if (write_user(use, pass) == 1)
			{
				strcpy(num, "1");
				send(fd, num, sizeof(num), 0);
			}
			break;
		case 2:
			memset(num, 0, sizeof(num));
			recv(fd, use, sizeof(use), 0);
			recv(fd, pass, sizeof(pass), 0);
			int q = read_user(use, pass, 0);
			if (q == 2)
			{
				strcpy(num, "2");
				strcat(myself->user_ID, use);
				strcat(myself->password, pass);
				myself->zt.Is_Being = 1;
			}
			else if (q == 1)
			{
				strcpy(num, "1");
			}

			send(fd, num, sizeof(num), 0);
			break;
		case 3:
			memset(num, 0, sizeof(num));
			recv(fd, use, sizeof(use), 0);
			if (addfriend(myself, use) > 0)
			{
				strcpy(num, "1");
			}
			send(fd, num, sizeof(num), 0);
			break;
		case 4:
			memset(num, 0, sizeof(num));
			//接受好友名称
			recv(fd, use, sizeof(use), 0);
			//接受发送内容
			recv(fd, buffer, sizeof(buffer), 0);
			num[0] = A_send_B(myself, use, buffer, fd);
			send(fd, num, sizeof(num), 0);
			break;
		case 5:
			len = recv(fd, use, sizeof(use), 0);

			if (len > 0)   //如果接受到数据，则判断是否为退出信息
			{
				if (strcmp(buffer, "exit\n") == 0)
				{
					printf("id %s exited.\n", myself->user_ID);
					break;
				}

				printf("ID_%d:%s\n", id, buffer);   //输出第N 个用户，输出的信息
			}
			else    //接收数据小于0，或等于0 则退出
			{
				printf("clinet %d close!\n", id);
				break;
			}
		}



		//如果服务端需要发送信息，此处添加发送信息
		//memset(buffer,0,BUFFER_SIZE);
		//scanf("%s",buffer);
		//send(fd,buffer,BUFFER_SIZE,0);
	}
	close(fd);   //关闭此客户端的连接的socket
}

//开启socked
int begin_socked()
{
	//	int socked;
	struct sockaddr_in my_addr;   //本地连接socked
	int sockfd;

	memset(&my_addr, 0, sizeof(my_addr)); //数据初始化--清零
										  //记录本地信息
	my_addr.sin_family = AF_INET;  //IPV4
	my_addr.sin_port = htons(9999);  //绑定端口9999，并将其转化为网络字节
	my_addr.sin_addr.s_addr = INADDR_ANY;   //指定接收IP为任意（如果指定IP，则只接收指定IP）
	bzero(&(my_addr.sin_zero), 0);   //最后位置赋0，补位置

									 //设置socked
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error!\n");
		exit(1);
	}

	//绑定socked
	if ((bind(sockfd, (struct sockaddr_in*)&my_addr, sizeof(my_addr))) < 0)
	{
		perror("bind error!\n");
		exit(1);
	}

	//开启监听,可连接客户端最大为10个
	if (listen(sockfd, 10) == -1)
	{
		perror("listen error!\n");
		exit(1);
	}

	return sockfd;
}

void chuli(int sockfd)
{
	int new_fd;
	struct sockaddr_in their_addr;
	int sin_size;
	int i = 0;
	//服务端一直运行，等待客户端连接
	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);

		//等待客户端连接，连接后their_addr接收客户端的IP等信息，没有客户端连接，则一直等待
		if ((new_fd = accept(sockfd, (struct sockaddr_in*)(&their_addr), &sin_size)) < 0)
		{
			perror("accept error!\n");
			exit(1);
		}
		//连接成功后，连接客户端数+1
		i++;

		//开启进程运行客户端
		pid_t childid;
		childid = fork();   //fork()函数有两个返回值，0为子进程，-1为错。子进程运行客户端
		if (childid == 0)
		{
			close(sockfd);  //子进程中不再需要sockfd去监听，这里释放它，只需要new_fd即可
			listen_data(new_fd, i);
			exit(0);
		}

		//父进程继续执行while，在accept（）等待客户端。父进程的socked此时还在运行，没有关闭
		//此处没有设置父进程退出的代码，因为假设服务器一直运行，如果需要服务器自动退出，可设置服务器
		//等待连接的时间，如果一定时间没有客户端连接，可以退出等待，结束
	}
	//所有客户端
	close(sockfd);
	printf("server-------closed.\n");
}

int main()
{
	int socked, new_fd;
	struct sockaddr_in their_addr;   //客户端连接socked
	int numbytes, sin_size;
	char buffer[BUFFER_SIZE];
	static int i = 0;  //记录连接客户端数目，可以使用数组，结构体等数据类型记录客户端信息（IP，端口等）

	user *use;
	use = (user *)malloc(sizeof(user));

	socked = begin_socked();
	chuli(socked);
	return 0;
}