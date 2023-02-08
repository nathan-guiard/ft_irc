/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 13:58:42 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/08 09:00:45 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

vector<int>				g_fds;
map<string, Client *>	clients;
string					reply;
string					auth = "PASS test\r\nUSER ircbot ircbot localhost bot\r\nNICK ircbot\r\n";

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

int socket_create(void)
{
	int fd_socket;
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	return fd_socket;
}

int socket_connect(int fd_socket, int port)
{
	int ret_connect=-1;
	struct sockaddr_in remote;
	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	ret_connect = connect(fd_socket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
	return ret_connect;
}

int socket_send(int fd_socket,char* to_send,int len)
{
	int send_ret = -1;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if(setsockopt(fd_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof tv) < 0) {
		return -1;
	}
	send_ret = send(fd_socket, to_send, len, 0);
	return send_ret;
}

int SocketReceive(int fd_socket, char* buff, int size)
{
	int send_ret = -1;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if (setsockopt(fd_socket, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv)) < 0) {
		return -1;
	}
	send_ret = recv(fd_socket, buff, size, 0);
	return send_ret;
}

void	sig_handling(int sig) {
	(void)sig;

	map<string, Client *>::iterator it = clients.begin();
	map<string, Client *>::iterator ite = clients.end();

	for (; it != ite; it++) {
		delete (*it).second;
	}

	for (size_t i = 0; i < g_fds.size(); i++) {
		close(g_fds[i]);
	}
	exit(0);
}

int main(int argc, char **argv) {
	int		port, fd_socket, read_size;
	string	password;
	char	server_reply[200];

	if (args_parsing(argc, argv, &port, &password))
		return 2;

	fd_socket = socket_create();
	if (fd_socket == -1)
		return 1;
	if (socket_connect(fd_socket, port) < 0) {
		perror("connect()");
		return 1;
	}

	signal(SIGINT, sig_handling);

	socket_send(fd_socket, (char *)auth.c_str(), auth.length());
	read_size = SocketReceive(fd_socket, server_reply, 200);
	reply.append(server_reply);
	if (reply.find("433") == 0) {
		cerr << "Server already has an IRC bot." << endl;
		return 1;
	}
	reply.clear();

	while (1) {
		int level;
		string to_send, answer;

		bzero(server_reply, 200);
		read_size = SocketReceive(fd_socket, server_reply, 200);
		reply.append(server_reply);
		while (read_size == 200) {
			bzero(server_reply, 200);
			read_size = SocketReceive(fd_socket, server_reply, 200);
			reply.append(server_reply);
		}
		
		string nick;
		if (reply.find("PRIVMSG") == string::npos)
			continue;

		reply = string(reply, 0, reply.length() - 2);

		if (reply[0] == ':')
			nick = string(reply, 1, reply.find("!") - 1);

		if (!(reply.find_last_of(":") == string::npos || reply.find("NICK") != string::npos))
		{
			answer = string(reply, reply.find_last_of(":") + 1, reply.length() - 1);
			
			cout << "\033[36m" + nick + " < " + answer << endl;

			Client *current = NULL;
			
			try {
				current = clients.at(nick);
				level = current->level();

				if (answer == "Quoi?" || answer == "Quoi" || answer == "quoi" || answer == "quoi?")
					to_send = "feur";
				else if (level == -1) {
					to_send = "Tu as deja perdu, tu ne peux pas reessayer...";
				} else if (level == 0) {
					if (answer.find("Paris") == 0) {
						current->passed(level);
						to_send = "Bonne reponse, Combien font 783 * 1276 / 4995 (arrondi)?";
					} else {
						current->failed();
						to_send = "C'est une mauvaise reponse, pas de chance.";
					}
				} else if (level == 1) {
					if (answer.find("200") == 0) {
						current->passed(level);
						to_send = "Bonne reponse, Quel est le meilleur jeu sorti en 2019 ?";
					} else {
						current->failed();
						to_send = "C'est une mauvaise reponse, pas de chance.";
					}
				} else if (level == 2) {
					if (answer.find("Outer Wilds") == 0) {
						current->passed(level);
						to_send = "Bonne reponse, Quelles sont les 4 premieres decimalees de pi?";
					} else {
						current->failed();
						to_send = "C'est une mauvaise reponse, pas de chance.";
					}
				} else if (level == 3) {
					if (answer.find("1415") == 0 || answer.find("3.1415") == 0) {
						current->passed(level);
						to_send = "Bonne reponse, Quel est le sens de la vie?";
					} else {
						current->failed();
						to_send = "C'est une mauvaise reponse, pas de chance.";
					}
				} else if (level == 4) {
					if (answer.find("42") == 0) {
						current->passed(level);
						to_send = string("Bravo! Tu as gagne le mot de passe OPER.")
						+ " With great power comes great responsibility! Fais y attention:"
						+ " JesuisBelle_42";
					} else {
						current->failed();
						to_send = "C'est une mauvaise reponse, pas de chance.";
					}
				} else {
					to_send = "Tu as deja fini le quizz.";
				}
			} catch (exception const &e) {
				current = new Client();
				clients.insert(make_pair(nick, current));
				to_send = string("Salut ") + nick + "! Bienvenue au super quizz."
					+ " Reponds a 5 questions pour avoir un super cadeau!"
					+ " Quelle est la capitale de france?";
			}

			cout << "\033[31m" << nick << " > " << to_send  << "\033[0m" << endl;

			to_send = "PRIVMSG " + nick + " :" + to_send + "\r\n";

			socket_send(fd_socket, (char *)to_send.c_str(), to_send.length());
		}
		reply.clear();
	}
	return 0;
}