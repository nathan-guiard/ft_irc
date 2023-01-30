/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 16:24:38 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/30 15:36:48 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int			g_fd_epoll;
int			g_fd_socket;
vector<int>	g_open_fd;
user_map	g_users;

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

void signal_handling(int signum) {
	(void)signum;
	vector<int>::iterator	it = g_open_fd.begin();
	vector<int>::iterator	ite = g_open_fd.end();
	user_map::iterator		it_map = g_users.begin();
	user_map::iterator		ite_map = g_users.end();

	close(g_fd_epoll);
	close(g_fd_socket);
	for (; it != ite; it++) {
		close(*it);
	}
	for (; it_map != ite_map; it_map++) {
		delete (*it_map).second;
	}

	cout << "Au revoir <3" << endl;
	exit(0);
}

int main(int argc, char **argv) {
	int			port;
	string		password;
	con_data	data;

	if (args_parsing(argc, argv, &port, &password)) {
		return 2;
	}
	signal(SIGINT, signal_handling);
	cout << "Port: " << port << endl;
	cout << "Password: " << password << endl;

	data = init_connection_data(port);

	while (42) {
		// cout << "---" << endl;
		int event_count = epoll_wait(data.fd_epoll, data.events,
										MAX_CONNECTIONS, -1);
		if (event_count == -1)
			closeAndFreeAll("epoll_wait()", 6);
		// cout << event_count << " event(s)" << endl;
		for (int i = 0; i < event_count; i++) {
			// cout << "Event happend on fd " << data.events[i].data.fd << endl;
			
			int user_id = fd_to_id(data.events[i].data.fd);
			
			if (data.events[i].data.fd == data.fd_socket)
				new_connection(data.fd_epoll, data.fd_socket);

			if (data.events[i].events & EPOLLRDHUP)
				deconnection(data, data.events[i].data.fd);
			
			else if (user_id != -1) { // A changer, c'est juste pour check pour l'instant
				vector<string>	command = get_command(data.events[i].data.fd);
				
				if (command.empty() /* -> si la commande a pas finie d'etre ecrite (CTRL-D)*/) {
					// Rien faire ?
				}

				

				vector<string>::iterator	ite = command.end();
				vector<string>::iterator	it = command.begin();
				cout << "\033[0;34m";
				for (; it != ite; it++)
					cout << user_id << " < " << *it << endl;
				cout << "\033[0m";

		
				vector<string>	parsed(10);
				

				// if commande_parsee[0] == NICK
				// 	g_users.at(user_id)->command_NICK(commande_parsee[1])

				//envoyer un message au nouvel user s'il a reussi a se co
				//le message de bienvenue doit etre compose comme ca :
				//" 001 Welcome to the Internet Relay Network <nick>!<user>@<host>"
				
				g_users.at(user_id)->send_to(RPL_WELCOME(string("test"), string("test"), string("localhost")));
			}
		}
	}
}