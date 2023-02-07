/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 13:58:42 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/07 18:07:52 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

vector<int>	g_fds;

bool args_parsing(int argc, char **argv, int *port, string *password) {
	if (argc != 3) {
		cerr << "ircserv needs 2 arguments." << endl;
		cerr << "Usage: irceserv <PORT> <PASSWORD>" << endl;
		return true;
	}
	for (int i = 0; argv[1][i]; i++) {
		if (!isdigit(argv[1][i])) {
			cerr << "Port needs to be an integer." << endl;
			return true;
		}
	}
	*port = atoi(argv[1]);
	*password = argv[2];
	return false;
}

short SocketCreate(void)
{
    short hSocket;
    printf("Create the socket\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}

int SocketConnect(int hSocket, int port)
{
    int iRetval=-1;
    struct sockaddr_in remote;
    remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote.sin_family = AF_INET;
    remote.sin_port = htons(port);
    iRetval = connect(hSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
    return iRetval;
}

int SocketSend(int hSocket,char* Rqst,short lenRqst)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if(setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = send(hSocket, Rqst, lenRqst, 0);
    return shortRetval;
}

int SocketReceive(int hSocket,char* Rsp,short RvcSize)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = recv(hSocket, Rsp, RvcSize, 0);
    return shortRetval;
}

int main(int argc, char **argv) {
	int		port, hSocket, read_size;
	string	password;
	char server_reply[200];
	string auth = "PASS test\r\nUSER ircbot ircbot localhost bot\r\nNICK ircbot\r\n";
	string reply;
	map<string, Client *>	clients;

	if (args_parsing(argc, argv, &port, &password))
		return 2;

	hSocket = SocketCreate();
	if(hSocket == -1)
	{
		printf("Could not create socket\n");
		return 1;
	}
	if (SocketConnect(hSocket, port) < 0)
	{
		perror("Connect failed.\n");
		return 1;
	}

	SocketSend(hSocket, (char *)auth.c_str(), auth.length());
	read_size = SocketReceive(hSocket, server_reply, 200);
	reply.append(server_reply);
	if (reply.find("433") == 0) {
		cerr << "Server already has an IRC bot." << endl;
		return 1;
	}
	reply.clear();

	while (1) {
		int level;
		string to_send, answer;

		read_size = SocketReceive(hSocket, server_reply, 200);
		reply.append(server_reply);
		while (read_size == 200) {
			read_size = SocketReceive(hSocket, server_reply, 200);
			reply.append(server_reply);
		}

		sleep(1);
		
		string nick;
		if (reply[0] == ':' && reply.find("PRIVMSG") == string::npos)
			continue;

		reply = string(reply, 0, reply.length() - 2);

		if (reply[0] == ':')
			nick = string(reply, 1, reply.find("!") - 1);

		cout << "|" << reply << "|" << nick << "|" << endl;

		if (reply.find_last_of(":") == string::npos)
			continue;
		answer = string(reply, reply.find_last_of(":"), reply.length() - 1);
		
		Client *current = NULL;
		
		try {
			current = clients.at(nick);
		} catch (exception const &e) {
			current = new Client();
			clients.insert(make_pair(nick, current));
			to_send = string("Salut ") + nick + "! Bienvenue au super quizz."
				+ " Reponds a 5 questions pour avoir un super cadeau!"
				+ " Quelle est la capitale de france?";
		}

		level = current->level();

		if (level == -1) {
			to_send = "Tu as deja perdu, tu ne peux pas reessayer...";
		} else if (level == 0) {
			if (answer.find("Paris") == 0) {
				current->passed(level);
				to_send = "Good job. What is feur?";
			} else {
				current->failed();
				to_send = "This was not the good answer... Too bad.";
			}
		}

		to_send = "PRIVMSG " + nick + " :" + to_send + "\r\n";

		SocketSend(hSocket, (char *)to_send.c_str(), to_send.length());
		cout << reply << endl;
		reply.clear();
	}
	return 0;
}