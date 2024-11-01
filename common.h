#ifndef INFO_H
#define INFO_H

//enum to hold the success and failure
typedef enum
{
	m_success,
	m_failure
}Status;

typedef enum
{
	m_help,
	m_view,
	m_edit,
	m_unsupported
}Operationtype;

typedef enum
{
	m_tit2,
	m_tpe1,
	m_talb,
	m_tyer,
	m_tcon,
	m_tcom
}TagReader;

typedef enum
{
	t,
	a,
	A,
	y,
	m,
	c
}edittags;

Operationtype check_operation_type(char *argv[]);
#endif
