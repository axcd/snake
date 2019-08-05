#include <stdio.h>

int logger( const char* info,int code,void* pin )
{
   FILE * fp;
   fp = fopen ( "log.txt", "a+" );
   fprintf( fp, "[ %p ]   %d  %s\n", pin, code, info );
   fclose( fp ); 
   return 0;
}

void clean_log()
 {
    FILE *fp;
    fp = fopen ( "log.txt", "w" );
    fclose( fp );
    return ;
 }