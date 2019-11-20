#ifndef STRING_H
#define STRING_H

#include <iostream>

class String
{
public:

	//String();
	String(const char* string = "");
	String(const String& copyString);

	String& operator=(const String& copyString);
	String& operator=(const char* string);

	const char* GetString() const;

	String operator+(const String& copyString);
	String operator+(const char* string);

	String& operator+=(const String& copyString);
	String& operator+=(const char* string);

	bool operator==(const String& copyString);
	bool operator==(const char* string);

	bool operator!=(const String& copyString);
	bool operator!=(const char* string);

	size_t Length() const;

	char operator[](int index) const;

	void Clear();

	bool IsEmpty();

	int Find(char string);
	int Find(const char* string);

	int Count(char string);

	~String();

private:

	char* m_string = new char[100];
};

std::ostream& operator<<(std::ostream& os, const String& string);
std::istream& operator>>(std::istream& is, String& string);

#endif // !STRING_H
