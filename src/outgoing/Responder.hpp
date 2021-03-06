/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Responder.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: abobas <abobas@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 11:51:03 by abobas        #+#    #+#                 */
/*   Updated: 2020/11/11 19:57:04 by abobas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../logger/Log.hpp"
#include "../incoming/Parser.hpp"
#include <string>
#include <map>
#include <sstream>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

class Responder
{
public:
	static Responder getResponder(int socket);
	Responder(int socket, Parser &parsed);
	void sendData(std::string &data);
	void sendData(std::string &&data);
	void sendDataRaw(std::string &data);
	void sendDataRaw(std::string &&data);
	void sendDataRaw(const char *buffer, int size);
	void sendCgiHeader();
	void sendFile(const std::string &path);
	void sendCreated(const std::string &path, std::string uri);
	void sendModified(const std::string &path, std::string uri);
	void sendNotFound();
	void sendBadRequest();
	void sendForbidden();
	void sendBadMethod(std::string allow);
	void sendPayLoadTooLarge();
	void sendUriTooLarge();
	void sendInternalError();
	void sendNotImplemented();
	void sendServiceUnavailable();
	void sendVersionNotSupported();

private:
	static Log *log;
	static std::string CRLF;
	static int CONTINUE;
	static int SWITCHING_PROTOCOL;
	static int OK;
	static int CREATED;
	static int MOVED_PERMANENTLY;
	static int BAD_REQUEST;
	static int UNAUTHORIZED;
	static int FORBIDDEN;
	static int NOT_FOUND;
	static int METHOD_NOT_ALLOWED;
	static int PAYLOAD_TOO_LARGE;
	static int URI_TOO_LARGE;
	static int INTERNAL_SERVER_ERROR;
	static int NOT_IMPLEMENTED;
	static int SERVICE_UNAVAILABLE;
	static int VERSION_NOT_SUPPORTED;
	static std::string CONNECTION_TYPE;
	static std::string ENCODING_TYPE;
	std::map<std::string, std::string> response_headers;
	int socket;
	Parser *parsed;
	int status;
	std::string status_message;

	Responder(int socket);
	void transmitData(std::string &data);
	void transmitData(std::string &&data);
	void transmitData(const char *buffer, int size);
	void transmitHeaders();
	int readFile(const std::string &path, std::string &buffer);
	void addHeader(std::string name, std::string value);
	void addStatusHeader(int http_status, std::string message);
	void addGeneralHeaders();
	void addDateHeader();
	void addServerHeader();
	void addConnectionHeader(std::string value);
	void addDataHeaders(std::string &data);
	void addDataHeaders(std::string &&data);
	void addTransferEncodingHeader(std::string value);
	void addFileHeaders(const std::string &path);
	void addFileTypeHeader(const std::string &path);
	void addFileLengthHeader(const std::string &path);
	void addLastModifiedHeader(const std::string &path);
};
