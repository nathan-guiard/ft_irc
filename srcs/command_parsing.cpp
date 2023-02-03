/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 16:08:04 by nguiard           #+#    #+#             */
/*   Updated: 2023/02/03 16:58:36 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

vector<string>	split(const string &s);

vector<string>	command_parsing(const string &s) {
	int i = 0;
	vector<string>	res = split(s);

	if (res.empty())
		return res;
	while (!res[i].empty()) {
		if (res[i][0] == ':') {
			for (size_t j = i + 1; !res[j].empty(); j++) {
				res[i] += " ";
				res[i] += res[j];
				res[j] = string();
			}
			res[i].erase(0, 1);
			break;
		}
		i++;
	}
	transform(res[0].begin(), res[0].end(), res[0].begin(), ::toupper);
	return res;
}

vector<string>	split(const string &s) {
	vector<string>	res(10);
	string			str = string(s);
	string 			delimiter = " ";
	string 			token;
	size_t 			pos = 0;
	int				i = 0;

	if (s.empty())
		return res;
	while ((str.find_last_of(' ') == str.length() - 1))
		str.erase(str.length() - 1, 1);
	str.push_back(' ');
	
	while ((pos = str.find(delimiter)) != string::npos || token.empty()) {
		token = str.substr(0, pos);
		if (i < 10) {
			res[i] = token;
			i++;	
		}
		else
			res.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	res.push_back(string());
	return res;
}