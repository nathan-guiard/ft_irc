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

User::User(): _id(-1), _fd(-1), _nick(), _user(), _realname(), _channels() {
	cerr << "Don't use this constructor" << endl;
}

User::User(int id, int fd): _id(id), _fd(fd), _nick(), _user(), _realname(),
	_has_pass(false), _has_nick(false), _has_user(false), _is_identified(false),
	_is_op(false), _channels() {}

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

bool	User::get_is_op()	const {
	return _is_op;
}

void	User::setIsOp(bool op) {
	_is_op = op;
}

User &User::operator = (const User &copy) {
	this->_fd = copy._fd;
	this->_id = copy._id;
	this->_nick = copy._nick;
	this->_realname = copy._realname;
	this->_user = copy._user;
	return *this;
}

// a faire -> renvoyer :old NICK new a tous les users de tous les
//				channels ou il est
/**
 * @brief	Execute la commande NICK
 * 
 * @param	tab		le vecteur de paramettres de la commande
 * @return	true 	le nickname a ete change
 * @return	false	le nickname n'a pas ete change (erreur)
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

	if(tab[1][0] == '#') {
		send_to(ERR_ERRONEUSNICKNAME(tab[1]));
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
	it = g_users.begin();
	ite = g_users.end();
	if (_has_nick) {
		for (; it != ite; it++) {
			if ((*it).second->identified())
				(*it).second->send_to(NICK(_nick, tab[1]));
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
 * @brief	Execute la commande USER
 * 
 * @param	tab		le vecteur de paramettres de la commande
 * @return	true 	le username et le realname on ete enregistres
 * @return	false	le username et le realname n'ont pas ete change (erreur)
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


/**
 * @brief	Execute la commande PASS
 * 
 * @param tab		Le vecteur d'arguments de la commande
 * @param password	Le mot de passe du serveur
 * @return true		L'utilisateur ne doit pas etre deconnecte
 * @return false	L'utilisateur doit etre deconnecte (mauvais password)
 */
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

/**
 * @brief	Execute la commande PING
 * 
 * @param	tab		Le vecteur d'arguments de la commande
 * @return	true	Le pong a ete envoye
 * @return	false	Le pong n'as pas ete envoye (erreur)
 */
bool	User::command_PING(vector<string> const &tab) {
	if (tab[1].empty()) {
		send_to(ERR_NEEDMOREPARAMS(string("PING")));
		return false;
	}
	send_to(string("PONG :") + tab[1] + string("\r\n"));
	return true;
}

bool	User::command_JOIN(vector<string> const &tab) {
	if (!_is_identified)
		return false;
	
	if (tab[1].empty()) {
		send_to(ERR_NEEDMOREPARAMS(string("JOIN")));
		return false;
	}

	if (tab[1][0] != '#') {
		send_to(ERR_BADCHANMASK(tab[1]));
		return false;
	}

	try {
		Channel *chan = g_channels.at(tab[1]);
		cout << "Pas de nouveaux chan" << endl;
		chan->add_user(this, false);
	}
	catch (exception const &e) {
		Channel *new_chan = new Channel(tab[1]);
		g_channels.insert(make_pair<string, Channel *>(tab[1], new_chan));
		cout << "creation nouveaux chan" << endl;
		new_chan->add_user(this, true);
	}
	return true;
}

bool	User::identified()	const {
	return _is_identified;
}

bool	User::send_to(string text) const {

	if (write(_fd, text.c_str(), text.length()) < 1)
		return false;

	cout << "\033[32m" << _id << " > " << text << "\033[0m";
	return true;
}

bool	User::command_PRIVMSG(vector<string> const& tab) {
	if (!_is_identified)
		return false;
	
	if (tab[2].empty()) {
		send_to(ERR_NEEDMOREPARAMS(string("PRIVMSG")));
		return false;
	}

	if (tab[1][0] == '#')
	{
		try {
			Channel *chan = g_channels.at(tab[1]);
			string s = tab[2];
			for (size_t j = 3; j < tab.size(); j++) {
				if (!tab[j].empty())
				{
					s += " ";
					s += tab[j];
				}
			}
			chan->broadcast(PRIVMSG(_nick, _user, string("localhost"), 
				tab[1], s), this);
		}
		catch (exception const& e)
		{
			send_to(ERR_NOSUCHNICK(tab[1]));
			return false;
		}
	}
	else {
		try {
			user_map::iterator	it = g_users.begin();
			user_map::iterator	ite = g_users.end();
			int					user_id;

			for (; it != ite; it++) {
				if ((*it).second->get_nick() == tab[1])
					user_id = (*it).second->get_id();
			}
			User *user = g_users.at(user_id);
			string s = tab[2];
			for (size_t j = 3; j < tab.size(); j++) {
				if (!tab[j].empty())
				{
					s += " ";
					s += tab[j];
				}
			}
			user->send_to(PRIVMSG(_nick, _user, string("localhost"), 
				tab[1], s));
		}
		catch (exception const& e)
		{
			send_to(ERR_NOSUCHNICK(tab[1]));
			return false;
		}

	}
	return true;
}

bool User::command_QUIT(vector<string> const& tab) {
	user_map::iterator	it = g_users.begin();
	user_map::iterator	ite = g_users.end();
	
	if (!_is_identified)
		return false;

	string s = tab[1];
	for (size_t j = 2; j < tab.size(); j++) {
		if (!tab[j].empty())
		{
			s += " ";
			s += tab[j];
		}
	}
	bool has_a_reason = !s.empty();
	for (; it != ite; it++) {
		if ((*it).second != this)
		{
			if (has_a_reason)
				(*it).second->send_to(QUIT_REASON(_nick, _user, string("localhost"), s));
			else
				(*it).second->send_to(QUIT(_nick, _user, string("localhost")));
		}
	}

	channel_map::iterator	it2 = g_channels.begin();
	channel_map::iterator	ite2 = g_channels.end();
	Channel					*chan;
	for (; it2 != ite2; it2++){
		chan = (*it2).second;
		if (chan->has_user(this))
		{
			if (has_a_reason)
				chan->broadcast(PART_REASON(_nick, _user, string("localhost"), tab[1], s), NULL);
			else
				chan->broadcast(PART(_nick, _user, string("localhost"), tab[1]), NULL);
			chan->rm_user(this);
		}
	}
	return true;
}

bool User::command_PART(vector<string> const& tab) {
	bool				has_a_reason;
	
	if (!_is_identified)
		return false;

	if (tab[1].empty()) {
		send_to(ERR_NEEDMOREPARAMS(string("PART")));
		return false;
	}

	string s = tab[2];
	for (size_t j = 3; j < tab.size(); j++) {
		if (!tab[j].empty())
		{
			s += " ";
			s += tab[j];
		}
	}

	channel_map::iterator	it = g_channels.begin();
	channel_map::iterator	ite = g_channels.end();
	Channel					*chan;
	has_a_reason = !s.empty();

	for (; it != ite; it++) {
		if (tab[1] == (*it).first)
		{
			chan = (*it).second;
			if (chan->has_user(this))
			{
				if (has_a_reason)
					chan->broadcast(PART_REASON(_nick, _user,
						string("localhost"), tab[1], s), NULL);
				else
					chan->broadcast(PART(_nick, _user,
						string("localhost"), tab[1]), NULL);
				chan->rm_user(this);
				// fias un truc
			}
			else
			{
				// ERREUR ?
				return false;
			}
		}
	}
	return true;
}

bool User::command_KICK(vector<string> const &tab) {
	string reason;

	if (tab[3].empty())
		reason = "You have been kicked by an operator.";
	else {
		reason = tab[3];
		for (size_t j = 4; tab[j].length(); j++) {
				reason += " ";
				reason += tab[j];
		}
	}
	cerr << reason << endl;

	if (!_is_identified) {
		return false;
	}

	if (tab[2].empty()) {
		send_to(ERR_NEEDMOREPARAMS(string("KICK")));
		return false;
	}

	Channel *chan = NULL;
	
	try {
		chan = g_channels.at(tab[1]);
	} catch (exception const &e) {
		send_to(ERR_NOSUCHNICK(tab[1]));
		return false;
	}

	//verifier que le user est dnas le channel et qu'il est operateur
	if (!chan->has_user(this)) {
		send_to(ERR_NOTONCHANNEL(chan->get_name()));
		return false;
	}
	else if (!chan->is_op(this)) {
		send_to(ERR_CHANOPRIVSNEEDED(chan->get_name()));
		return false;
	}
	User *to_ban = NULL;
	try {
		to_ban = g_users.at(nick_to_id(tab[2]));
		if (!chan->has_user(to_ban))
			throw exception();
	} catch (exception const &e) {
		send_to(ERR_USERNOTINCHANNEL(tab[2], chan->get_name()));
		return false;
	}

	chan->broadcast(KICK(_nick, _user, string("localhost"),
						chan->get_name(), tab[2], reason), NULL);

	chan->rm_user(to_ban);

	return true;
}

int	User::command_KILL(vector<string> const &tab) {
	(void)tab;
	if (!_is_identified) {
		return 0;
	}
	if (tab[1].empty()) {
		send_to(ERR_NEEDMOREPARAMS(string("KILL")));
		return 0;
	}
	if (_is_op == false) {
		send_to(ERR_NOPRIVILEGES);
		return 0;
	}
	User *to_kill = NULL;
	try {
		to_kill = g_users.at(nick_to_id(tab[1]));
	} catch (exception const &e) {
		send_to(ERR_NOSUCHNICK(tab[1]));
		return 0;
	}

	user_map::iterator	it = g_users.begin();
	user_map::iterator	ite = g_users.end();
	
	if (!_is_identified)
		return 0;

	string s = tab[1];
	for (size_t j = 2; j < tab.size(); j++) {
		if (!tab[j].empty())
		{
			s += " ";
			s += tab[j];
		}
	}
	bool has_a_reason = !s.empty();
	for (; it != ite; it++) {
		if ((*it).second != this)
		{
			if (has_a_reason)
				(*it).second->send_to(KILL_REASON(_nick, to_kill->get_nick(), s));
			else
				(*it).second->send_to(KILL(_nick, to_kill->get_nick()));
		}
	}

	channel_map::iterator	it2 = g_channels.begin();
	channel_map::iterator	ite2 = g_channels.end();
	Channel					*chan;
	for (; it2 != ite2; it2++) {
		chan = (*it2).second;
		if (chan->has_user(to_kill))
			chan->rm_user(to_kill);
	}

	return to_kill->get_fd();
}

bool	User::command_OPER(vector<string> const &tab) {
	if (!_is_identified) {
		return false;
	}
	if (tab[2].empty()) {
		send_to(ERR_NEEDMOREPARAMS(string("OPER")));
		return false;
	}
	if (tab[2] != PASSWORD) {
		send_to(ERR_PASSWDMISMATCH);
		return false;
	}
	send_to(RPL_YOUREOPER(_nick));
	_is_op = true;
	return true;
}

bool	User::command_MODE(vector<string> const &tab) {
	int	curr_arg = 2;
	Channel *chan = NULL;

	if (tab[1].empty())
		return false;
	if (tab[1] != _nick) {
		if (tab[1][0] == '#') {
			try {
				chan = g_channels.at(tab[1]);
			} catch (exception const &e) {
				send_to(ERR_NOSUCHCHANNEL(tab[1]));
				return false;
			}
		}
		else {
			send_to(ERR_USERSDONTMATCH(_nick));
			return false;
		}
	}
	if (tab[2].empty() && !chan) {
		// send_to(RPL_UMODEIS()); -> ?
		return true;
	}

	if (!chan) {
		return true; // ??
	}

	if (!_is_op && !chan->is_op(this)) {
		send_to(ERR_CHANOPRIVSNEEDED(chan->get_name()));
		return false;
	}

	for (int i = 2; !tab[i].empty(); i++) {
		bool	plus;
		bool	args = true;

		curr_arg = _next_arg_mode(tab, curr_arg);
		tab[i][0] == '+' ? plus = true : plus = false;

		if (curr_arg == -1)
			args = false;

		if (curr_arg == i)
			continue;
		if (tab[i].size() == 1)
			continue;
		
		for (size_t j = 1; tab[i].size() > j; j++) {
			if (tab[i][j] == 'b' && args) {
				if (plus)
					chan->ban(tab[curr_arg]);
				else
					chan->unban(tab[curr_arg]);
				curr_arg = _next_arg_mode(tab, curr_arg);
			}
			if (tab[i][j] == 'l') {
				if (plus)
				{
					chan->set_limit(atoi(tab[curr_arg].c_str()));
					curr_arg = _next_arg_mode(tab, curr_arg);
				}
				else
					chan->set_limit(-1);
			}
			if (tab[i][j] == 'i') {
				chan->set_invite_only(plus);
			}
			if (tab[i][j] == 'm') {
				chan->set_moderated(plus);
			}
		}
	}
	return true;
}

int	User::_next_arg_mode(vector<string> const &tab, int i) const {
	int tab_len = tab.size();

	for (; i < tab_len && !tab[i].empty(); i++) {
		if (tab[i][0] != '+' || tab[i][0] == '-')
			return i;
	}
	return -1;
}

