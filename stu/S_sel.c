#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "cgic.h"


int cgiMain()
{

	fprintf(cgiOut, "Content-type:text/html;charset=utf-8\n\n");

	fprintf(cgiOut, "<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <title>学生信息管理系统</title> <link href=\"https://cdn.bootcss.com/bootstrap/3.3.7/css/bootstrap.min.css\" rel=\"stylesheet\">\n<style type=\"text/css\"> \nbody {height: 100%%;      background-color: lightblue;text-align: center;} ul li {list-style: none;}\n.header {        margin-bottom: 50px;}.operation>div {padding: 15px;}\n.operation.row.text-center button {outline-style: none;}.forms li {display: none; }.forms li button { margin-right: 30px;}</style></head>	<body><h1 class=\"header text-center\">学生信息管理系统</h1>\n");
	
	fprintf(cgiOut, "<head><meta charset=\"utf-8\"><title>查询结果</title>\
		    <link rel=\"stylesheet\" href=\"/stu/public/css/bootstrap.min.css\">\
		</head>");

	char sno[9] = "\0";
	int status = 0;

	status = cgiFormString("sno",  sno, 9);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get sno error!\n");
		return 1;
	}

	int ret;
	MYSQL *db;
	char sql[128] = "\0";

	if (sno[0] == '*')
	{
		sprintf(sql, "select * from Score");
	}
	else
	{
		sprintf(sql, "select * from Score where sno = %d ", atoi(sno));
	}


	//初始化
	db = mysql_init(NULL);
	mysql_options(db,MYSQL_SET_CHARSET_NAME,"utf8");
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_init fail:%s\n", mysql_error(db));
		return -1;
	}

	//连接数据库
	db = mysql_real_connect(db, "127.0.0.1", "root", "123456", "stu",  3306, NULL, 0);
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_real_connect fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}


	if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	{
		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}

	MYSQL_RES *res;
	res = mysql_store_result(db);
	if (res == NULL)
	{
		fprintf(cgiOut,"mysql_store_result fail:%s\n", mysql_error(db));
		return -1;
	}

	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">查询结果</h1>");

	fprintf(cgiOut,"<table class=\"table table-striped table-bordered\"><tr>");
	int i = 0;

	unsigned int fields;
	fields = mysql_num_fields(res);

	MYSQL_FIELD *mysql_filed;
	mysql_filed = mysql_fetch_fields(res);
	for (i = 0; i < fields ; i++)
	{
		fprintf(cgiOut, "<th>%s</th>", mysql_filed[i].name);
	}
	fprintf(cgiOut,"</tr>");

	//访问每一条记录的值
	MYSQL_ROW  row;
	unsigned long  *len;

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		fprintf(cgiOut,"<tr>");
		len = mysql_fetch_lengths(res);
		for (i = 0; i < fields ; i++)
		{
			fprintf(cgiOut,"<td>%.*s</td>", (int)len[i], row[i]);
		}
		fprintf(cgiOut,"</tr>");
	}
	fprintf(cgiOut,"</table></div>");

	mysql_close(db);
	return 0;
}
