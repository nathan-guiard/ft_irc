/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 16:08:04 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/30 16:31:24 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

vector<string>	command_parsing(const string &s) {
	string			str = string(s);
	string 			delimiter = " ";
	size_t 			pos = 0;
	string 			token;
	vector<string>	res;

	if (s.empty())
		return res;
	while ((pos = str.find(delimiter)) != string::npos || token.empty()) {
		token = str.substr(0, pos);
		res.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	
	vector<string>::iterator	it = res.begin();
	vector<string>::iterator	ite = res.end();

	for (; it != ite; it++) {
		cout << *it << endl;
	}
	return res;
}