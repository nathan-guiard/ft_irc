/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 16:24:38 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 15:02:24 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

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

int main(int argc, char **argv) {
	int			port;
	string		password;
	con_data	data;
	client_map	clients;

	if (args_parsing(argc, argv, &port, &password)) {
		return 2;
	}
	cout << "Port: " << port << endl;
	cout << "Password: " << password << endl;

	data = init_connection_data(port);

	while (42) {
		// cout << "---" << endl;
		int event_count = epoll_wait(data.fd_epoll, data.events,
										MAX_CONNECTIONS, -1);
		if (event_count == -1) {
			cerr << "epoll_wait(): Error." << endl;
			// check d'autres trucs
			exit(1);
		}
		// cout << event_count << " event(s)" << endl;
		for (int i = 0; i < event_count; i++) {
			// cout << "Event happend on fd " << data.events[i].data.fd << endl;
			
			int client_id = fd_to_id(clients, data.events[i].data.fd);
			
			if (data.events[i].data.fd == data.fd_socket)
				new_connection(data.fd_epoll, data.fd_socket, &clients);

			if (data.events[i].events & EPOLLRDHUP)
				deconnection(data, data.events[i].data.fd, &clients);
			

			else if (client_id != -1) { // A changer, c'est juste pour check pour l'instant
				char buff[16];
				bzero(buff, 16);
				cout << "\033[1mClient " << client_id << ": \033[0m" << flush;
				while (read(data.events[i].data.fd, buff, 16) > 0) {
					write(1, buff, 16);
					if (string(buff).find('\n') != string::npos)
						break;
				}
			}
		}
	}
}