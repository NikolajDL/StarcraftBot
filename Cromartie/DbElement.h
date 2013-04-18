#pragma once

class DbElement
{
public:
	DbElement(void);
	DbElement(int id);
	~DbElement(void);
	int getId(void) const;
	void setId(int id);
protected:
	int m_id;
};