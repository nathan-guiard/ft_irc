/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:50:24 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/06 18:13:25 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	exec_commands(int user_id, int user_fd,
						const string &password, con_data &data) {
	vector<string>	command = get_command(user_fd, user_id);
	
	if (command.empty()) {
		return;
	}
	
	vector<string>::iterator	ite = command.end();
	vector<string>::iterator	it = command.begin();
	User						*bob = g_users.at(user_id);

	cout << "\033[0;34m";
	for (; it != ite; it++) {
		cout << user_id << " < " << *it << endl;
		vector<string> splitted_command = command_parsing(*it);

		if (splitted_command[0] == "PASS")
		{
			if (bob->command_PASS(splitted_command, password) == false)
				deconnection(data, user_fd);
		}
		else if (splitted_command[0] == "NICK")
			bob->command_NICK(splitted_command);
		else if (splitted_command[0] == "USER")
			bob->command_USER(splitted_command);
		else if (splitted_command[0] == "PING")
			bob->command_PING(splitted_command);
		else if (splitted_command[0] == "JOIN")
			bob->command_JOIN(splitted_command);
		else if (splitted_command[0] == "PART")
			bob->command_PART(splitted_command);
		else if (splitted_command[0] == "PRIVMSG")
			bob->command_PRIVMSG(splitted_command);
		else if (splitted_command[0] == "KICK")
			bob->command_KICK(splitted_command);
		else if (splitted_command[0] == "TOPIC")
			bob->command_TOPIC(splitted_command);
		else if (splitted_command[0] == "KILL")
		{
			int fd = bob->command_KILL(splitted_command);
			if (fd)
				deconnection(data, fd);
		}
		else if (splitted_command[0] == "OPER")
			bob->command_OPER(splitted_command);
		else if (splitted_command[0] == "QUIT")
		{
			if (bob->command_QUIT(splitted_command) == true)
				deconnection(data, user_fd);
		}
		check_dead_channels();
	}
	cout << "\033[0m";
}