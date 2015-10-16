#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>

#define MAX_PAYLOAD 1024 /*��Ϣ�����Ϊ1024�ֽ�*/

int main(int argc, char* argv [])
{
	struct sockaddr_nl dest_addr;
	struct nlmsghdr *nlh = NULL;
	struct iovec iov;
	int sock_fd = -1;
	struct msghdr msg;
	printf("%d",NETLINK_TEST);
        
	if (-1 == (sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST))) { //�����׽���
		perror("can't create netlink socket!");
		return 1;
	}
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0; /*���ǵ���Ϣ�Ƿ����ں˵�*/
	dest_addr.nl_groups = 0; /*�ڱ�ʾ���в�����ʹ�ø�ֵ�����*/
        
	//���׽��ֺ�Netlink��ַ�ṹ����а�
	if (-1 == bind(sock_fd, (struct sockaddr*)&dest_addr, sizeof(dest_addr))) {
		perror("can't bind sockfd with sockaddr_nl!");
		return 1;
	}

	if (NULL == (nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD)))) {
		perror("alloc mem failed!");
		return 1;
	}

	memset(nlh, 0, MAX_PAYLOAD);
	/* ���Netlink��Ϣͷ�� */
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = 0;
	nlh->nlmsg_type = NLMSG_NOOP; //ָ�����ǵ�Netlink����Ϣ������һ������Ϣ,������˵����ͷ���ĸ�����Ϣ
	nlh->nlmsg_flags = 0;

    /*����Netlink����Ϣ���ݣ�������������������ĵ�һ������*/
	strcpy(NLMSG_DATA(nlh), argv[1]);

    /*�����ģ�壬��ʱ���þ���ΪʲôҪ�����á���ʱ����ϸ����socketʱ��˵*/
	memset(&iov, 0, sizeof(iov));
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	sendmsg(sock_fd, &msg, 0); //ͨ��Netlink socket���ں˷�����Ϣ

	    /* �ر�netlink�׽��� */
	close(sock_fd);
	free(nlh);
	return 0;
}
