#include "String.h"

// TODO: cleanup code
//		 prevent repetition

std::ostream& operator<<(std::ostream& os, const String& string)
{
	os << string.GetString();

	return os;
}

std::istream& operator>>(std::istream& is, String& string)
{
	string.Clear();

	char* temp = new char[280];

	//std::cin.ignore();
	is.getline(temp, 280);

	string = temp;

	delete[] temp;

	return is;
}

// ------------------------------------------------------------

String::String(const char* string)
{
	const size_t SIZE = strlen(string) + 1;

	m_string = new char[SIZE];

	strcpy_s(m_string, SIZE, string);
}

String::String(const String& copyString)
{
	//m_CopyString(copyString.m_string);
	const size_t SIZE = strlen(copyString.m_string) + 1;

	m_string = new char[SIZE];

	strcpy_s(m_string, SIZE, copyString.m_string);
}

String& String::operator=(const String& copyString)
{
	if (&copyString == this) return *this;

	return this->operator=(copyString.m_string);
}

String& String::operator=(const char* string)
{
	delete[] m_string;

	const size_t SIZE = strlen(string) + 1;

	m_string = new char[SIZE];

	strcpy_s(m_string, SIZE, string);

	return *this;
}

const char* String::GetString() const
{
	return m_string;
}

String String::operator+(const String& copyString)
{
	return this->operator+(copyString.m_string);
}

String String::operator+(const char* string)
{
	String newString = m_string;
	newString += string;

	return newString;
}

String& String::operator+=(const String& copyString)
{
	return this->operator+=(copyString.m_string);
}

String& String::operator+=(const char* string)
{
	size_t SIZE = strlen(m_string) + strlen(string) + 1;

	char* temp = new char[SIZE];

	strcpy_s(temp, SIZE, m_string);
	strcat_s(temp, SIZE, string);

	delete[] m_string;
	//m_string = nullptr;

	m_string = new char[SIZE];

	strcpy_s(m_string, SIZE, temp);

	return *this;
}

bool String::operator==(const String& copyString)
{
	return !strcmp(m_string, copyString.m_string);
}

bool String::operator==(const char* string)
{
	return !strcmp(m_string, string);
}

bool String::operator!=(const String& copyString)
{
	return !(this->operator==(copyString));
}

bool String::operator!=(const char* string)
{
	return !(this->operator==(string));
}

size_t String::Length() const
{
	return strlen(m_string);
}

char String::operator[](int index) const
{
	return m_string[index];
}

void String::Clear()
{
	*this = "";
}

bool String::IsEmpty()
{
	return !strcmp(m_string, "");
}

int String::Find(char string)
{
	for (size_t i = 0; i < strlen(m_string); i++)
	{
		if (m_string[i] == string) return (int)i;
	}

	return -1;
}

int String::Find(const char* string)
{
	for (size_t i = 0; i < strlen(m_string); i++)
	{
		if (m_string[i] == string[0])
		{
			bool continuing = true;

			if (i + strlen(string) <= strlen(m_string))
			{
				for (size_t j = 1; j < strlen(string); j++)
				{
					if (m_string[i + j] != string[j]) continuing = false;
				}
			}
			else
			{
				return -1;
			}

			if (continuing) return (int)i;
		}
	}

	return -1;
}

int String::Count(char string)
{
	int count = 0;

	for (size_t i = 0; i < strlen(m_string); i++)
	{
		if (m_string[i] == string) count++;
	}

	return count;
}

String::~String()
{
	delete[] m_string;
	m_string = nullptr;
}