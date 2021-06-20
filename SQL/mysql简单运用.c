/***********************************
 * 查看数据库表中的内容
 * 对表中的内容进行一些操作
 * ********************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mariadb/mysql.h>


MYSQL *conn;
 
int myselect(char* sql_stat)           //展示表中所有信息
{
	//this function execute the "select ......" sql statement
	//return true if execute successfully ,return false if an error occurred!
	
	int row, field;
	MYSQL_RES* res_ptr;
	MYSQL_ROW  res_row;
	
	//query
	int res=mysql_query(conn,sql_stat);               //使用mysql_query函数实现sql_stat中内容
	if(res)//return nonzero if an error occurred
	{
		fprintf(stderr,"Error %d: %s\n",
			mysql_errno(conn),mysql_error(conn));
		return 0;
	}
	
	//get result
	res_ptr=mysql_store_result(conn);//返回你查询到的所有数据;
	
	//get row number and fields number 
	row=mysql_num_rows(res_ptr);
	field=mysql_num_fields(res_ptr);      //返回你这张表有多少列
	printf("\t%d row(s) in test is displayed:\n",row);	
	
	//print the fields
	printf("\t-----------------------\n");
	for(int i=0;i<field;i++)
		printf("\t%s",mysql_fetch_fields(res_ptr)[i].name);
	printf("\n");
	
	//print each row
	while(res_row=mysql_fetch_row(res_ptr) )
	{
		for(int i=0;i<field;i++)
		{
			printf("\t%s",res_row[i]);
		}
		printf("\n");
	}
	printf("\t-----------------------\n");
	mysql_free_result(res_ptr);
	return 1;
}
 
int myinsert(char* sql_statement)         //向表中加入新内容
{
	//this function execute the "insert ......" sql statement
	//return true if execute successfully ,return false if an error occurred!
	
	int res=mysql_query(conn,sql_statement);
	if(res)
	{
		fprintf(stderr,"insert error %d: %s\n",
			mysql_errno(conn),mysql_error(conn));
		return 0;
	}
	
	//show details
	printf("Execute successfully!\n");
	printf("%ld row(s) was affected!\n",
		(long)mysql_affected_rows(conn));
	return 1;
}
 
int mydelete(char* sql_statement)       //删除表中内容
{
	//this function execute the "delete ......" sql statement
	//return true if execute successfully ,return false if an error occurred!
	
	int res=mysql_query(conn,sql_statement);
	if(res)
	{
		fprintf(stderr,"delete error %d: %s\n",
			mysql_errno(conn),mysql_error(conn));
		return 0;
	}
	
	//show details
	printf("Execute successfully!\n");
	printf("%ld row(s) was affected!\n",
		(long)mysql_affected_rows(conn));
	return 1;
}
 
int myupdate(char* sql_statement)       //更新（修改）表中内容
{
	//this function execute the "update ......" sql statement
	//return true if execute successfully ,return false if an error occurred!
	
	int res=mysql_query(conn,sql_statement);
	if(res)
	{
		fprintf(stderr,"Update error %d: %s\n",
			mysql_errno(conn),mysql_error(conn));
		return 0;
	}
	
	//show details
	printf("Execute successfully!\n");
	printf("%ld row(s) was affected!\n",
		(long)mysql_affected_rows(conn));
	return 1;
}
 
int main(int argc,char* argv[])
{
 
	char* sql_insert="insert into student (id,name,age,gender) values (66,'LK',12,'male')";  //想student表中添加新的信息
	char* sql_delete="delete from student where id=66";      //删除id为66的信息
	char* sql_update="update student set name='change' where id=66";  //更新数据将id = 66的name修改为change
	char* sql_select="select * from student";       //显示表student中的内容
 
	conn=mysql_init(conn);     //初始化一个句柄
 
	if(conn==NULL)
	{
		printf("mysql_init failed!\n");
		return EXIT_FAILURE;
	}
 
	conn=mysql_real_connect(conn,"127.0.0.1",
		"root","181219","db1",0,NULL,0);          //连接数据库db1
	
	if(conn == NULL)
	{
		printf("Connection failed!\n");
		if(mysql_errno(conn))
		{
			fprintf(stderr,"Connection error %d: %s\n"
				,mysql_errno(conn),mysql_error(conn));
		}
		return EXIT_FAILURE;
	}
	else printf("\nConnect to mydb Successfully!\n");
	
	printf("\nShow all the data in test\n");
	if(!myselect(sql_select))return EXIT_FAILURE;       //显示表中内容
	
	printf("\n\ninsert into test values (66,'LK',‘12’，'male') \n");
	if(!myinsert(sql_insert))return EXIT_FAILURE;       //向表中添加内容
	if(!myselect(sql_select))return EXIT_FAILURE;       //显示表中内容
 
	printf("\n\nupdate test set name='change' where id=66\n");
	if(!myupdate(sql_update))return EXIT_FAILURE;       //更新（修改）表中内容
	if(!myselect(sql_select))return EXIT_FAILURE;       //显示表中内容
	
	printf("\n\ndelete from test where id=66\n");
	if(!mydelete(sql_delete))return EXIT_FAILURE;		//删除表中内容
	if(!myselect(sql_select))return EXIT_FAILURE;       //显示表中内容
	
	mysql_close(conn);                                  //关闭数据库
	return EXIT_SUCCESS;
}