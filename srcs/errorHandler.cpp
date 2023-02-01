/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:24:48 by eleotard          #+#    #+#             */
/*   Updated: 2023/02/01 18:55:04 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	closeFdsExit(const char *message, int errCode)
{
	if (g_fd_epoll >= 0)
		close(g_fd_epoll);
	if (g_fd_socket >= 0)
		close(g_fd_socket);
	perror(message);
	exit(errCode);
}

void	closeAndFreeAll(const char *message, int errCode)
{
	vector<int>::iterator	it = g_open_fd.begin();
	vector<int>::iterator	ite = g_open_fd.end();
	user_map::iterator		it_user = g_users.begin();
	user_map::iterator		ite_user = g_users.end();
	channel_map::iterator	it_chan = g_channels.begin();
	channel_map::iterator	ite_chan = g_channels.end();

	close(g_fd_epoll);
	close(g_fd_socket);
	for (; it != ite; it++) {
		close(*it);
	}
	for (; it_user != ite_user; it_user++) {
		delete (*it_user).second;
	}
	for (; it_chan != ite_chan; it_chan++) {
		delete (*it_chan).second;
	}
	if (string(message) != "")
		perror(message);
	cout << "Au revoir <3" << endl;
	exit(errCode);
}
