/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Evaluator.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: abobas <abobas@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 00:54:14 by abobas        #+#    #+#                 */
/*   Updated: 2020/11/07 12:19:05 by abobas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Receiver.hpp"
#include "Parser.hpp"
#include "Matcher.hpp"
#include "Validator.hpp"
#include "../processor/Processor.hpp"
#include "../logger/Log.hpp"
#include "../config/Json.hpp"
#include <string>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class Evaluator
{
public:
	static Evaluator *getInstance(int socket);
	static void initializeEvaluator(Json &config);
	static void deleteInstance(int socket);
	void evaluateHeaders(std::string &&headers);
	void processRequest();
	Parser &getParsed();
	Matcher &getMatched();
	bool isEvaluated();
	bool isProcessed();
	int getError();
	int getUploadStatus();
	std::string getUploadPath();
	std::string getType();
	std::string getValidMethods();

private:
	static Log *log;
	static std::map<int, Evaluator *> evaluators;
	static Json config;
	Parser parsed;
	Matcher matched;
	Validator validated;
	Processor *processor;
	int socket;
	int error;
	std::string request_type;
	bool evaluated;
	bool processed;

	Evaluator(int socket);
	void evaluateRequest();
	bool isProxy();
	bool isCgi();
	bool isUpload();
	bool isFile(struct stat *file);
	bool isRegular(struct stat *file);
	bool isDirectory(struct stat *file);
};