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
	printf("1-ע���˺�\n");
	printf("2-��¼�˺�\n");
	printf("3-��Ӻ���\n");
	printf("4-�������\n");
	printf("5-�˳�ϵͳ\n");
	printf("-------------------------------------\n");
	//	send(fd, &num, sizeof(num), 0);
	scanf("%s", num);

	switch (atoi(num))
	{
		send(fd, num, sizeof(num), 0);
	case 1:

		printf("�������˺ţ�\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		printf("���������룺\n");
		scanf("%s", pass);
		send(fd, pass, sizeof(pass), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '1') == 0)
			{
				printf("ע��ɹ�\n");
			}
			else
			{
				printf("ע��ʧ��\n");
			}
		}
		//ע��
		//write_user(user_A��user_B);
		break;
	case 2:
		printf("�������˺ţ�\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		printf("���������룺\n");
		scanf("%s", pass);
		send(fd, pass, sizeof(pass), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '1') == 0)
			{
				printf("��¼�ɹ�\n");
			}
			else
			{
				printf("��¼ʧ��\n");
			}
		}
		break;
	case 3:
		printf("����������˺ţ�\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '1') == 0)
			{
				printf("��ӳɹ�\n");
			}
			else
			{
				printf("���ʧ��\n");
			}
		}
		break;
	case 4:
		printf("����������˺ţ�\n");
		scanf("%s", user);
		send(fd, user, sizeof(user), 0);
		if (recv(fd, num, sizeof(num), 0) > 0)
		{
			if (strcmp(num, '0') == 0)
			{
				printf("���Ѳ�����\n");
			}
			else if (strcmp(num, '1') == 0)
			{
				printf("���Ѳ�����\n");
			}
		}
		break;
	case 5:

		send(fd, "exit\n", 5, 0);

		return;
	default:
		printf("��������\n");
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
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");   //�������õ��Ǳ�������
	bzero(&(their_addr.sin_zero), 0);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error!\n");
		exit(1);
	}

	// ʹ��connect���ӷ�������their_addr��ȡ��������Ϣ
	if (connect(sockfd, (struct sockaddr*)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect error!\n");
		exit(1);
	}

	jiemian(sockfd);

	/* while(1)
	{
	//���ӳɹ���
	memset(buff,0,sizeof(buff));
	printf("clinet----:");
	scanf("%s",buff);
	//�ͻ��˿�ʼд�����ݣ�*****�˴�buff��Ҫ�ͷ������н���
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