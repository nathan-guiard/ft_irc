/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 16:24:38 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 12:17:13 by nguiard          ###   ########.fr       */
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
	int	port;
	string password;

	if (args_parsing(argc, argv, &port, &password)) {
		return 2;
	}
	cout << "Port: " << port << endl;
	cout << "Password: " << password << endl;

	//test nathan
	int		fd_epoll = epoll_create1(0); // A besoin d'etre close
	if (fd_epoll < 0) {
		cerr << "epoll_create1(): Error." << endl;
		exit(1);
	}
	int		fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_socket < 0) {
		cerr << "socket(): Error." << endl;
		close(fd_epoll);
		exit(1);
	}

	struct sockaddr_in	s_socket;
	s_socket.sin_addr.s_addr = htonl(INADDR_ANY);
	s_socket.sin_family = AF_INET;
	s_socket.sin_port = htons(port);
	bind(fd_socket, (sockaddr *) &s_socket, sizeof s_socket);

	struct	epoll_event	event_socket;
	event_socket.events = EPOLLIN | EPOLLOUT;
	event_socket.data.fd = fd_socket;
	struct	epoll_event	events[MAX_CONNECTIONS];

	if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_socket, &event_socket)) {
		cerr << "epoll_ctl(): Error." << endl;
		close(fd_epoll);
		close(fd_socket);
		exit(1);
	}

	listen(fd_socket, MAX_CONNECTIONS);

	while (42) {
		int event_count = epoll_wait(fd_epoll, events, MAX_CONNECTIONS, -1);
		if (event_count == -1) {
			cerr << "epoll_wait(): Error." << endl;
			// check d'autres trucs
			exit(1);
		}
		cout << event_count << " event(s)" << endl;
		for (int i = 0; i < event_count; i++) {
			cout << "Event happend on fd " << events[i].data.fd <<endl;

			// if (write(events[i].data.fd, "", 0) == -1)
			// 	perror("write()");

			
			if (events[i].data.fd == fd_socket) {
				if (events[i].events & EPOLLIN)
					add_new_con(fd_epoll, fd_socket);
				if (events[i].events & EPOLLOUT)
					cout << "Someone disconnected." << endl;
			}
			else {
				char buff[16];
				while (read(events[i].data.fd, buff, 16) > 0) {
					write(1, buff, 16);
					if (string(buff).find('\n') != string::npos)
						break;
				}
			}
		}
	}
}

bool	add_new_con(int fd_epoll, int fd_socket) {
	int 				fd_new_con;
	struct sockaddr_in	s_new_con;
	socklen_t			size_s_new_con = sizeof s_new_con;
	struct epoll_event	event_new_con;
	static int			id = 1;

	fd_new_con = accept(fd_socket,
		(sockaddr *) &s_new_con, &size_s_new_con);
	if (fd_new_con < 0) {
		perror("accept()");
		// check d'autres trucs
		exit(1);
	}
	event_new_con.events = EPOLLIN;
	event_new_con.data.fd = fd_new_con;
	epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_new_con, &event_new_con);
	cout << "Client " << id << " added. Fd: " << fd_new_con << endl;
	id++;
	return true;
}