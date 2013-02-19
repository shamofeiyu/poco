//
// main.cpp
//
// $Id$
//
// This sample shows SQL to mongo Shell to C++ examples.
//
// Copyright (c) 2013, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"

// INSERT INTO USERS
// VALUES( "Braem", 33)
void sample1(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 1 ***" << std::endl;

	Poco::MongoDB::Database db("mydb");

	Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertUserRequest = db.createInsertRequest("users");
	Poco::MongoDB::Document::Ptr user = new Poco::MongoDB::Document();
	user->add("lastname", std::string("Braem"));
	user->add("age", 33);
	insertUserRequest->documents().push_back(user);
	connection.sendRequest(*insertUserRequest);
	std::string lastError = db.getLastError(connection);
	if ( !lastError.empty() )
	{
		std::cout << "Last Error: " << db.getLastError(connection) << std::endl;
	}
}

// SELECT lastname, age FROM users
void sample2(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 2 ***" << std::endl;

	Poco::MongoDB::Cursor cursor("mydb", "users");
	// Selecting fields is done by adding them to the returnFieldSelector
	// Use 1 as value of the element.
	cursor.query().returnFieldSelector().add("lastname", 1);
	cursor.query().returnFieldSelector().add("age", 1);
	Poco::MongoDB::ResponseMessage& response = cursor.next(connection);
	while(1)
	{
		for(Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			std::cout << "LastName: " << (*it)->get<std::string>("lastname") << " Age: " << (*it)->get<int>("age") << std::endl;
		}

		// When the cursorID is 0, there are no documents left, so break out ...
		if ( response.cursorID() == 0 )
		{
			break;
		}

		// Get the next bunch of documents
		response = cursor.next(connection);
	}
}

// SELECT * FROM users
void sample3(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 3 ***" << std::endl;

	Poco::MongoDB::Cursor cursor("mydb", "users");
	Poco::MongoDB::ResponseMessage& response = cursor.next(connection);
	while(1)
	{
		for(Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			std::cout << "LastName: " << (*it)->get<std::string>("lastname") << " Age: " << (*it)->get<int>("age") << std::endl;
		}

		// When the cursorID is 0, there are no documents left, so break out ...
		if ( response.cursorID() == 0 )
		{
			break;
		}

		// Get the next bunch of documents
		response = cursor.next(connection);
	};
}

int main(int argc, char** argv)
{
	Poco::MongoDB::Connection connection("localhost", 27017);
	sample1(connection);
	sample2(connection);
	sample3(connection);

	return 0;
}
