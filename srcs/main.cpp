/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 16:24:38 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/31 15:13:59 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int			g_fd_epoll;
int			g_fd_socket;
vector<int>	g_open_fd;
user_map	g_users;
channel_map	g_channels;

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
	closeAndFreeAll("", 0);
}

int main(int argc, char **argv) {
	int			port;
	string		password;
	con_data	data;

	if (args_parsing(argc, argv, &port, &password)) {
		return 2;
	}
	signal(SIGINT, signal_handling);
	cout << "Port: " << port << endl << "Password: " << password << endl;
	data = init_connection_data(port);

	while (42) {
		int event_count = epoll_wait(data.fd_epoll, data.events,
										MAX_CONNECTIONS, -1);
		if (event_count == -1)
			closeAndFreeAll("epoll_wait()", 6);
		for (int i = 0; i < event_count; i++) {
			int user_id = fd_to_id(data.events[i].data.fd);
			
			if (data.events[i].data.fd == data.fd_socket)
				new_connection(data.fd_epoll, data.fd_socket);

			if (data.events[i].events & EPOLLRDHUP)
				deconnection(data, data.events[i].data.fd);
			
			else if (user_id != -1) {
				exec_commands(user_id, data.events[i].data.fd, password, data);
			}
		}
	}
}