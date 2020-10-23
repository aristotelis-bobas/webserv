/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   File.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: abobas <abobas@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/27 22:06:27 by abobas        #+#    #+#                 */
/*   Updated: 2020/10/19 22:42:33 by abobas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"

File::File(Data &data) : data(data)
{
    if (!setStat())
        return ;
    if (S_ISDIR(file.st_mode))
        Directory directory(data);
    else if (S_ISREG(file.st_mode))
        data.response.sendFile(data.path);
    else
        data.response.sendNotFound();
}

bool File::setStat()
{
    if (stat(data.path.c_str(), &file) < 0)
    {
        data.response.sendNotFound();
        return false;
    }
    return true;
}