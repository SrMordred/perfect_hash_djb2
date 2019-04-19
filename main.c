#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 64
#define MAX_STRINGS   1024


char** string_array;
int    string_counter;

char*  file_string;

char** hash_values;

//ALGORITHM VARIABLES
//original values
//__hash = 5381
//__shift =5
unsigned long 	__hash 			= 0; //0 - 100000
int 			__shift        	= 1; //1 - 31
float 			__loadfactor 	= 1.0;

//BEST_RESULTS (BASED ON LOADFACTOR)

unsigned long 	__best_hash    	    = 5381;
int 			__best_shift        = 5;
float 			__best_loadfactor 	= 999;

void load_file( char* file_name )
{
	//read and allocate file string
	FILE* file = fopen( file_name , "r");
	if(!file) 
	{
        perror("File opening failed");
		exit(0);
    }

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	file_string  = malloc( file_size );
	fread(file_string, 1, file_size, file);
	fclose(file);

	string_array = malloc( MAX_STRINGS * sizeof(char*) );

	string_counter = 0;

	char* start = file_string;
	char* end   = start;

	while(1)
	{
		if(*end == 0 )
		{
			string_array[ string_counter ] = start;
			++string_counter;
			break;
		}
		if(*end == '\r')
		{
			*end = 0;
			string_array[ string_counter ] = start;
			++string_counter;
			end+=2; //jump \r\n
			start = end;
			continue;
		}
		++end;
	}
}

unsigned long dbj2(unsigned char *str)
{
    unsigned long hash = __hash;
    int c;

    while ((c = *str++))
        hash = ((hash << __shift) + hash) + c;

    return hash;
}

void perfect_hash_search()
{
	START:;

	size_t mod = string_counter * __loadfactor;

	hash_values = calloc(1, sizeof( char* ) * mod );

	for(int x = 0;x < string_counter; ++x)
	{
		unsigned long index = dbj2((unsigned char*)string_array[x]) % mod;

		if( hash_values[index] == NULL )
		{
			hash_values[index] = string_array[x];
		}
		else
		{
			if( strcmp(hash_values[index], string_array[x] ) == 0 )
			{
				printf("You cannot have two identical strings! ( %s )\n", string_array[x]);
				exit(0);
			}
			__loadfactor += 0.1;
			free( hash_values );
			goto START;
		}
	}
}


int main( int argc, char *argv[] )
{
	if( argc <= 1 ) return 0;
	char* file = argv[1];
	load_file(file);

	//progression
	long loop_counter  = 10000 * 31;
	float percent_inc = 100.f / loop_counter;
	float percent_total = 0;
	float percent_acc   = 0;

	//search space
	for(__hash = 0; __hash < 10000 ; __hash++)
	{
		for(__shift = 0; __shift < 31 ; __shift++)
		{
			perfect_hash_search();

			if( __loadfactor < __best_loadfactor )
			{
				__best_loadfactor = __loadfactor;
				__best_hash       = __hash;
				__best_shift      = __shift;
			}
			__loadfactor = 1;

			percent_acc += percent_inc;
			if(percent_acc > 1)
			{
				percent_total += percent_acc;
				percent_acc = percent_acc - 1;
				printf("%.2f %% : %f\n", percent_total, __best_loadfactor);
				fflush(stdout);
			}
		}
	}

	__loadfactor = __best_loadfactor;
	__hash       = __best_hash;
	__shift      = __best_shift;

	//calculate again with best values
	perfect_hash_search();

	printf("\n\n");

	size_t mod = string_counter * __loadfactor;
	printf("Load Factor of %f ( array of %ld length ).\n", __loadfactor, mod);
	if(__loadfactor == 1.f) printf("*** A PERFECT HASH!!! ***\n");
	printf("\n\n");

	printf("/********************/\n");
	printf("/* GENERATED C CODE */\n");
	printf("/********************/\n");

	printf("\n\n");

	printf("unsigned long dbj2(unsigned char *str)\n");
	printf("{\n");
	printf("    unsigned long hash = %ld;\n", __best_hash);
	printf("    int c;\n");
	printf("\n");
	printf("    while ((c = *str++))\n");
	printf("        hash = ((hash << %d) + hash) + c;\n", __best_shift);
	printf("\n");
	printf("    return hash %% %ld;\n", mod);
	printf("}\n");
	printf("\n\n");

	printf("const char* hash_table[] = {\n");

	for(int x = 0; x < (int)mod; ++x)
	{
		if( hash_values[x] )
			printf("\t[%d] = \"%s\",\n", x, hash_values[x] );
	}
	printf("};\n\n");

	free( file_string );
	free( string_array);

	return 0;
}
