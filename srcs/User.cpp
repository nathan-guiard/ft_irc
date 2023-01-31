/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 19:05:37 by nguiard           #+#    #+#             */
/*   Updated: 2023/01/24 14:21:07 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(): _id(-1), _fd(-1), _nick(), _user(), _realname() {
	cerr << "Don't use this constructor" << endl;
}

User::User(int id, int fd): _id(id), _fd(fd), _nick(), _user(), _realname(),
	_has_pass(false), _has_nick(false), _has_user(false), _is_identified(false) {}

User::User(const User &copy) {
	*this = copy;
}

string	User::get_user()	const {
	return _user;
}

string	User::get_nick()	const {
	return _nick;
}

string	User::get_real()	const {
	return _realname;
}

int	User::get_id()	const {
	return _id;
}

int	User::get_fd()	const {
	return _fd;
}

User &User::operator = (const User &copy) {
	this->_fd = copy._fd;
	this->_id = copy._id;
	this->_nick = copy._nick;
	this->_realname = copy._realname;
	this->_user = copy._user;
	return *this;
}

/**
 * @brief Execute la commande NICK
 * 
 * @param new_nick	nouveau nickname
 * @param c_map 	set de tous les users deja existants
 * @return true 	le nickname a ete change
 * @return false	le nickname n'a pas ete change (erreur)
 */
bool	User::command_NICK(vector<string> const& tab)
{
	user_map::const_iterator	it = g_users.begin();
	user_map::const_iterator	ite = g_users.end();

	if (!_has_pass)
		return false;

	if (tab[1].empty()) {
		send_to(ERR_NONICKNAMEGIVEN);
		return false;
	}

	// Check si la string new_nick est bien formattee
	// sinon ERR_ERRONEUSNICKNAME et return false

	for (; it != ite; it++) {
		bool	is_the_same = (*it).second->get_nick() == tab[1];
		bool	is_not_me	= (*it).second->get_nick() != _nick;

		if (is_the_same && is_not_me) {
			send_to(ERR_NICKNAMEISUSE(tab[1]));
			return false;
		}
	}
	_nick = tab[1];
	_has_nick = true;
	if (_has_user && _has_nick && !_is_identified)
	{
		_is_identified = true;
		send_to(RPL_WELCOME(_nick, _user, string("localhost")));
	}
	return true;
}

/**
 * @brief Execute la commande USER
 * 
 * @param tab[1]		nouveau username
 * @param new_realname 	nouveau realname
 * @return true 		le username et le realname on ete enregistres
 * @return false		le username et le realname n'ont pas ete change (erreur)
 */
bool User::command_USER(vector<string> const& tab)
{
	bool	not_enough_params = tab[4].empty();
	bool	already_registered = _has_user;

	if (!_has_pass) {
		return false;
	}

	if (not_enough_params) {
		send_to(ERR_NEEDMOREPARAMS(string("USER")));
		return false;
	}

	// il faut check si on peut avoir des noms vides
	if (already_registered) {
		send_to(ERR_ALREADYREGISTERED);
		return false;
	}

	_user = tab[1];
	_realname = tab[4];
	_has_user = true;
	if (_has_user && _has_nick && !_is_identified)
	{
		_is_identified = true;
		send_to(RPL_WELCOME(_nick, _user, string("localhost")));
	}
	return true;
}

bool	User::send_to(string text) {

	if (write(_fd, text.c_str(), text.length()) < 1)
		return false;

	cout << "\033[32m" << _id << " > " << text << "\033[0m" << endl;
	return true;
}

bool	User::command_PASS(vector<string> const& tab, string const& password)
{
	if (_has_pass)
	{
		send_to(ERR_ALREADYREGISTERED);
		return true;
	}
	if (tab[1] == password)
		_has_pass = true;
	else
	{
		send_to(ERR_PASSWDMISMATCH);
		return false;
	}
	return true;
}

bool	User::identified()	const {
	return _is_identified;
}

//check pass
//check nick
