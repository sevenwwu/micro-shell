int expand(char* orig, char* new, int newsize); 
int tryExecuteBuiltin(char** argv, int argc);

extern FILE* inputStream;
extern int mainargc;
extern char** mainargv;
extern int shift;