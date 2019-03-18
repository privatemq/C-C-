#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>

void jiemian(int fd)
{
	char user[20];
	char pass[20];
	char num[2];

	system("clear");
	memset(user, 0, sizeof(user));
	memset(pass, 0, sizeof(pass));
	memset(num, 0, sizeof(num));
	printf("1-注册账号\n");
	printf("2-登录账号\n");
	printf("3-添加好友\n");
	printf("4-联络好友\n");
	printf("5-退出系统\n");
	printf("-------------------------------------\n");
	//	send(fd, &num, sizeof(num), 0);
	scanf("%s", num);

	switch (atoi(num))
	{
		send(fd, num, sizeof(num), 0);
	case 1:

		printf("请输入账号：\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		printf("请输入密码：\n");
		scanf("%s", pass);
		send(fd, pass, sizeof(pass), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '1') == 0)
			{
				printf("注册成功\n");
			}
			else
			{
				printf("注册失败\n");
			}
		}
		//注册
		//write_user(user_A，user_B);
		break;
	case 2:
		printf("请输入账号：\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		printf("请输入密码：\n");
		scanf("%s", pass);
		send(fd, pass, sizeof(pass), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '1') == 0)
			{
				printf("登录成功\n");
			}
			else
			{
				printf("登录失败\n");
			}
		}
		break;
	case 3:
		printf("请输入好友账号：\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '1') == 0)
			{
				printf("添加成功\n");
			}
			else
			{
				printf("添加失败\n");
			}
		}
		break;
	case 4:
		printf("请输入好友账号：\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '0') == 0)
			{
				printf("好友不存在\n");
			}
			else if (strcmp(num, '1') == 0)
			{
				printf("好友不在线\n");
			}
		}
		break;
	case 5:

		send(fd, "exit\n", 5, 0);

		return;
	default:
		printf("输入有误！\n");
	}

}

int main()
{
	int sockfd, numbytes;
	char buff[100];
	struct sockaddr_in their_addr;
	int i = 0;

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(9999);
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");   //本次设置的是本地连接
	bzero(&(their_addr.sin_zero), 0);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error!\n");
		exit(1);
	}

	// 使用connect连接服务器，their_addr获取服务器信息
	if (connect(sockfd, (struct sockaddr*)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect error!\n");
		exit(1);
	}

	jiemian(sockfd);

	/* while(1)
	{
	//连接成功后
	memset(buff,0,sizeof(buff));
	printf("clinet----:");
	scanf("%s",buff);
	//客户端开始写入数据，*****此处buff需要和服务器中接收
	if(send(sockfd,buff,sizeof(buff),0) == -1)
	{
	perror("send error \n");
	exit(1);
	}
	}
	*/
	close(sockfd);
	return 0;
}