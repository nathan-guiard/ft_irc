/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:24:48 by eleotard          #+#    #+#             */
/*   Updated: 2023/01/26 18:05:54 by eleotard         ###   ########.fr       */
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
	perror(message);
	cout << "Au revoir <3" << endl;
	exit(errCode);
}
