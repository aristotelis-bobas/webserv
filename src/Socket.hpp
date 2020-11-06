/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: abobas <abobas@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/21 16:57:38 by abobas        #+#    #+#                 */
/*   Updated: 2020/11/05 23:55:29 by abobas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "logger/Log.hpp"
#include "config/Json.hpp"
#include "incoming/Receiver.hpp"
#include "incoming/Evaluator.hpp"
#include "incoming/Matcher.hpp"
#include "incoming/Parser.hpp"
#include "outgoing/Responder.hpp"
#include "outgoing/Directory.hpp"
#include <string>

/**
* @brief Handles socket operations.
*/
class Socket
{

public:
	Socket();
	Socket(std::string type, int socket);
	static void initializeSocket(Json &config);
	
	void handleIncoming();
	void handleOutgoing();
	bool isAlive();

	int getSocket() const;
	std::string getType() const;
	void setType(std::string new_type);
	
	// std::string getMessage();
	// void receiveMessage();
	// bool isReady();
	
	
private:
	static Log *log;
	static Json config;
	Receiver *receiver;
	Evaluator *evaluator;
	std::string type;
	int socket;
	bool incoming_ready = false;
	
	void responseControl(Matcher &matched, Parser &parsed);
	void errorResponse(int error, Parser &parsed);
	// void resolveProxyRequest(Matcher &matched, Parser &parsed);
	// void resolveCgiRequest(Matcher &matched, Parser &parsed);
	void resolveUploadRequest(Matcher &matched, Parser &parsed);
	void resolveDirectoryRequest(Matcher &matched, Parser &parsed);
	void resolveFileRequest(Matcher &matched, Responder &respond);
	
};

inline bool operator==(const Socket &lhs, const Socket &rhs)
{
	if (lhs.getSocket() != rhs.getSocket())
		return false;
	if (lhs.getType() != rhs.getType())
		return false;
	return true;
}

inline bool operator<(const Socket &lhs, const Socket &rhs)
{
	if (lhs.getSocket() < rhs.getSocket())
		return true;
	return false;
}
