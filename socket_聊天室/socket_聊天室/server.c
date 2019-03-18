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
	int Is_Being; //0δ���ߣ�1����
	char Is_Friend[10][20]; //�����б�Ŀǰ����Ϊ10
}zhuangtai;

typedef struct {
	char user_ID[20]; //�˺�
	char password[20];  //����
	zhuangtai zt;
}user;

// ע���˺�
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

// ��ȡ����˺ŵ��ļ�������˺Ų����ڣ��򷵻�0�����벻���ڷ���1��û���ⷵ��2
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

//�Ƿ�������
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

// 1 ������Ϣ�� 2
char A_send_B(user *user_A, char *user_B, char *buff, int sockfd_B)
{
	char a[100];
	memset(a, 0, sizeof(a));
	//�ж�2�Ƿ�Ϊ����
	if (IsFriend(user_A, user_B) == 0)
	{
		return '0';
	}

	//�ж�2�Ƿ�����
	if (read_user(user_B, NULL, 1) == 3)
	{
		return '1';
	}

	//��ʼͨ��
	//memset(buff, 0, sizeof(buff));
	//printf("������:");
	//scanf("%s",buff);
	strcpy(a, buff);
	strcat(a, ":");
	strcat(a, "\0");
	if (send(sockfd_B, a, sizeof(a), 0) < 0)
	{
		//perror("%s ���͸� %s����Ϣʧ���ˣ�\n", user_A->user_ID, user_B->user_ID);
		exit(1);
	}

	if (send(sockfd_B, a, sizeof(a), 0) < 0)
	{
		printf("%s ���͸� %s����Ϣʧ���ˣ�\n", user_A->user_ID, user_B);
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

//��������д����
void listen_data(int fd, int id)
{
	char buffer[BUFFER_SIZE];
	char use[20];
	char pass[20];
	char num[2];
	user *myself = (user *)malloc(sizeof(user));

	memset(myself, 0, sizeof(user));

	while (1)     //һֱ���ڼ����ͻ�����Ϣ״̬,֪���ͻ��˶Ͽ���ͻ��˷����˳���Ϣ
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
			//���ܺ�������
			recv(fd, use, sizeof(use), 0);
			//���ܷ�������
			recv(fd, buffer, sizeof(buffer), 0);
			num[0] = A_send_B(myself, use, buffer, fd);
			send(fd, num, sizeof(num), 0);
			break;
		case 5:
			len = recv(fd, use, sizeof(use), 0);

			if (len > 0)   //������ܵ����ݣ����ж��Ƿ�Ϊ�˳���Ϣ
			{
				if (strcmp(buffer, "exit\n") == 0)
				{
					printf("id %s exited.\n", myself->user_ID);
					break;
				}

				printf("ID_%d:%s\n", id, buffer);   //�����N ���û����������Ϣ
			}
			else    //��������С��0�������0 ���˳�
			{
				printf("clinet %d close!\n", id);
				break;
			}
		}



		//����������Ҫ������Ϣ���˴���ӷ�����Ϣ
		//memset(buffer,0,BUFFER_SIZE);
		//scanf("%s",buffer);
		//send(fd,buffer,BUFFER_SIZE,0);
	}
	close(fd);   //�رմ˿ͻ��˵����ӵ�socket
}

//����socked
int begin_socked()
{
	//	int socked;
	struct sockaddr_in my_addr;   //��������socked
	int sockfd;

	memset(&my_addr, 0, sizeof(my_addr)); //���ݳ�ʼ��--����
										  //��¼������Ϣ
	my_addr.sin_family = AF_INET;  //IPV4
	my_addr.sin_port = htons(9999);  //�󶨶˿�9999��������ת��Ϊ�����ֽ�
	my_addr.sin_addr.s_addr = INADDR_ANY;   //ָ������IPΪ���⣨���ָ��IP����ֻ����ָ��IP��
	bzero(&(my_addr.sin_zero), 0);   //���λ�ø�0����λ��

									 //����socked
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error!\n");
		exit(1);
	}

	//��socked
	if ((bind(sockfd, (struct sockaddr_in*)&my_addr, sizeof(my_addr))) < 0)
	{
		perror("bind error!\n");
		exit(1);
	}

	//��������,�����ӿͻ������Ϊ10��
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
	//�����һֱ���У��ȴ��ͻ�������
	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);

		//�ȴ��ͻ������ӣ����Ӻ�their_addr���տͻ��˵�IP����Ϣ��û�пͻ������ӣ���һֱ�ȴ�
		if ((new_fd = accept(sockfd, (struct sockaddr_in*)(&their_addr), &sin_size)) < 0)
		{
			perror("accept error!\n");
			exit(1);
		}
		//���ӳɹ������ӿͻ�����+1
		i++;

		//�����������пͻ���
		pid_t childid;
		childid = fork();   //fork()��������������ֵ��0Ϊ�ӽ��̣�-1Ϊ���ӽ������пͻ���
		if (childid == 0)
		{
			close(sockfd);  //�ӽ����в�����Ҫsockfdȥ�����������ͷ�����ֻ��Ҫnew_fd����
			listen_data(new_fd, i);
			exit(0);
		}

		//�����̼���ִ��while����accept�����ȴ��ͻ��ˡ������̵�socked��ʱ�������У�û�йر�
		//�˴�û�����ø������˳��Ĵ��룬��Ϊ���������һֱ���У������Ҫ�������Զ��˳��������÷�����
		//�ȴ����ӵ�ʱ�䣬���һ��ʱ��û�пͻ������ӣ������˳��ȴ�������
	}
	//���пͻ���
	close(sockfd);
	printf("server-------closed.\n");
}

int main()
{
	int socked, new_fd;
	struct sockaddr_in their_addr;   //�ͻ�������socked
	int numbytes, sin_size;
	char buffer[BUFFER_SIZE];
	static int i = 0;  //��¼���ӿͻ�����Ŀ������ʹ�����飬�ṹ����������ͼ�¼�ͻ�����Ϣ��IP���˿ڵȣ�

	user *use;
	use = (user *)malloc(sizeof(user));

	socked = begin_socked();
	chuli(socked);
	return 0;
}