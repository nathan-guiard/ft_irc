/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:00:21 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/06 11:07:06 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static string	read_connection_data(int fd_user);

/**
 * @brief 	get every command that the user sended.
 * 			stocks the command if no \r\n were given
 * 			(like get_next_line)
 * 
 * @param	fd_user fd of the given user	
 * @return	Every command that the user sent. Empty if none.
 */
vector<string>	get_command(int fd_user, int id_user) {
	static map<int, string>	command;
	// static string	command;
	string			line;
	vector<string>	res;
	bool			has_no_newline;
	size_t			index = string::npos - 1;
	bool			whole_string_splitted = false;

	try {
		command.at(id_user);
	} catch (exception const &e) {
		command.insert(make_pair(id_user, string()));
	}
	command[id_user].append(read_connection_data(fd_user));
	has_no_newline = command[id_user].find("\r\n") == string::npos;
	
	if (has_no_newline)
		return res;

	while (!whole_string_splitted) {
		bool should_not_push_back;
		command[id_user] = string(command[id_user], index + 2);
		index = command[id_user].find("\r\n");
		line = string(command[id_user], 0, index);

		should_not_push_back = line.empty() || line.size() == 0
								|| command[id_user].find("\r\n") == string::npos;
		if (should_not_push_back) {
			size_t index_nl = command[id_user].find("\r\n");

			if (index_nl != string::npos) {
				command[id_user].erase(index_nl, 2);
			}
			break;
		}
		res.push_back(line);
		whole_string_splitted = line.empty() || line.size() == 0;
	}

	return res;
}

static string	read_connection_data(int fd_user) {
	char				buff[READ_SIZE + 1];
	int					bytes_read;
	string				res;	

	
	bzero(buff, READ_SIZE + 1);
	bytes_read = read(fd_user, buff, READ_SIZE);
	buff[bytes_read] = 0;
	while (bytes_read > 0) {
		res.append(buff);
		if (bytes_read < READ_SIZE || strstr(buff, "\r\n") == buff + 14)
			break;
		bytes_read = read(fd_user, buff, READ_SIZE);
		buff[bytes_read] = 0;
	}
	return res;
}