/* Solution to comp10002 Assignment 2, 2018 semester 2.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: [Wailam, Yip]
   Dated:     [7/10/2018]

*/
/*--------------------------------------------------------------*/
/*Libraries*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define ARGV_STARTING_VER 	argv[argc-1][0]
#define MAX_ALPHABET 			52
#define MAX_FILE_SIZE			2000
#define ALPHA_FORMAT			2
#define LOG10 					10
#define PRINT_RESTRICT 			14
#define LAST_7TH_ELE 			7
#define FIRST_7TH_ELE	 		6
#define STAGE_ONE				1
#define STAGE_TWO				2
#define FIRST_ELEMENT			0
#define ELEMENT_TWO			1
#define EMPTY					0
#define RESET					0
#define WHITESPACE				' '
#define EVEN					2
#define EOS						'\0'
#define NEWLINE					'\n'
#define PRINT_RESTRICT10		10
#define FIFTH					5

/*--------------------------------------------------------------*/
/*Define structure*/
/*	Name 	-> the label of the vertex
	info 	-> an 2D array to store the vertices. E.g {"AB10, AC2..."}
	edges 	-> Number of edges in that vertex
	degrees-> Degree of the vertex
	o_or_e	-> Odd or Even
	unprocess -> Number of unprocessed edges in that vertex
*/

typedef struct node_t
{
	char name;
	char **info;
	int edges;
	int degrees;
	char o_or_e;
	int unprocess;
}node_t;
/*--------------------------------------------------------------*/
/*Function Prototypes*/
int mygetchar();
char** dym_arr_gen(int, int);
int ver_degree(char *, char);
int count_rows(char*);
int count_colm(char*);
int str_search(char *, char);
char* convert_num(int);
int find_smallest(int*, int);
int find_largest(int*, int);
int pow_int(int, int);
int extract_int(char*);
char extract_ver(char*, char);
int proc_arr_search(char**, char*, int);
char* total_vertex_count(FILE *, int *);
int str_search_ex(char*, char, int);
void Info_vertex(FILE*, struct node_t*, int, int*, int *, char*, int*, int*, int*);
void Info_vertex_assign(char**, int, int, int, struct node_t *);
void Info_vertex_process1(FILE*, char*);
void Info_vertex_process2(char **, char*, int);
void Info_vertex_process3(char**, int, int*, int*, int*);
void Info_vertex_process4(char**, struct node_t*, int, int, char*);
void Stage_0_Main(struct node_t*, char, int, int, int, int, int, int);
void Stages_algorithm(struct node_t *, char, int,int, int, char*, int);
char Stages_Body(int, int*, int*, int*,struct node_t*, char**, int, int, int*, char*, 
					char);
char Stages_route(char*, int*, int, int);									
void Update_unproc(struct node_t *, char*, int);
void Restore_unproc(struct node_t *, int);
void Stage_1_Routing(char*, int*,char*, int*, int, int);
void Stages_append_final(char*, int*, char*, int*, int, int,int, int);
char Stages_next_vertex(char *,int,struct node_t*);
void Stage_2_Routing(char*, int*,char*, int*, int, int);
int Stage_2_Analysis(int*, char*, int*, int, int, char*, int*);
void Stages_route_copy(int*, char*, char*,int*, int);
int Total_scenic(int*, int);
void Stages_Print_route(char*, int*, int);
void Stages_Print_scene(int*, int, int); 
void Central_Process(int argc, char*argv[]);

/*--------------------------------------------------------------*/
int mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}
/*-------------------------------------------------------------*/
/*2D Dynamic (char) array Generator*/
char** dym_arr_gen(int num_edges, int max_colm)
{	
	int row;
	char **array = (char**)calloc(num_edges, sizeof(char*));
	for(row=0;row< num_edges; row++){
		array[row] = (char*)calloc(max_colm, sizeof(char));
	}
	return array;
}
/*-------------------------------------------------------------*/
/* Function to check degree of the vertex*/
int ver_degree(char *array, char needle) 
{
	int index, count=0;
	
	for(index=0;index<strlen(array);index++){
		if(array[index] == needle) 	
			count++;
	}
	return count;
}

/*-------------------------------------------------------------*/
/*Count no of rows in the text file, i.e. the number of edges in total */
int count_rows(char*file_array)
{
	int num_edges = 0;
	int index;
	
	for(index=0;index<strlen(file_array);index++){
		if(file_array[index] == NEWLINE)
			num_edges++;
	}
	return num_edges;
}
/*-------------------------------------------------------------*/
/*Count no of colms in input, e.g "d d 15", function returns max_colm = 4*/
int count_colm(char*file_array)
{
	int index, temp_count, max_colm;
	index = temp_count = max_colm  = 0;
	for(index=0;index<strlen(file_array);index++){
		if(file_array[index] == NEWLINE){
			if(temp_count > max_colm){
				max_colm = temp_count;
			}
			temp_count = RESET;
		}
		else{
			temp_count ++;
		}	
	}
	return max_colm;
}
/*-------------------------------------------------------------*/
/*Search if the needle is within the array, return 1 if needle was found, 
0 otherwise*/
int str_search(char *hay, char needle)
{
    int index =0;
    for(index=0;index<strlen(hay);index++){
        if(hay[index] == needle)
            return 1;
    }
    return 0;
}
/*-------------------------------------------------------------*/
/*Function to convert integer to char, e.g 15 will become "15" */
char* convert_num(int num)
{
	char *num_string = (char*)calloc((int)(log10(num)+1), sizeof(char));
	sprintf(num_string, "%d", num);
	return num_string;
}
/*-------------------------------------------------------------*/
/*Function to find the smallest element in that array*/
int find_smallest(int* array, int size_of_array)
{
	int index, min_val;
	min_val = 0;
	for(index=0;index<size_of_array;index++){
		if(index==0){
			min_val = array[index];
		}
		if(array[index] < min_val && array[index]!= EMPTY){
			min_val = array[index];
		}
	}
	return min_val;
}
/*-------------------------------------------------------------*/
/*Function to find the largest element in that array*/
int find_largest(int* array, int size_of_array)
{
	int index, max_val;
	max_val = 0;
	for(index=0;index<size_of_array;index++){
		if(index==0){
			max_val = array[index];
		}
		if(array[index] > max_val && array[index]!= EMPTY){
			max_val = array[index];
		}
	}
	return max_val;
}
/*-------------------------------------------------------------*/
/*A recursive function to count to power of a base, e.g 2^3 */
int pow_int(int base, int pow)
{
	if(pow == 0)
	{
		return 1;
	}
	return base*pow_int(base, pow-1);
}
/*-------------------------------------------------------------*/
/*Function to extract a integer from a string, e.g. string = "dd15", function will 
return integer 15 */
int extract_int(char *string)
{
	int index, count, integer;
	index = integer = count = 0;
	char*temp_s = (char*)calloc(strlen(string)-ALPHA_FORMAT, sizeof(char));
	int temp_index = 0;
	
	while(string[index]!=EOS)
	{
		if(isdigit(string[index])){
		    temp_s[temp_index] = string[index];
		    temp_index++;
			count ++;
		}
		index++;
	}
	for(index=0;index<count;index++){
		integer += (temp_s[index]-'0')*pow_int(LOG10, count-index-1);
	}
	
	return integer;
}  
/*-------------------------------------------------------------*/
/*Function to extract a vertex from a mix string, e.g. string = "dd15", function will 
return integer d*/
char extract_ver(char *string, char next_vertex)
{
	int index;
	char extracted_vertex;
	for(index=0;index<strlen(string);index++)
	{
		if(string[index] == next_vertex){
			if(index==0){
				extracted_vertex = string[index+1];
			}
			else{
				extracted_vertex = string[FIRST_ELEMENT];
			}
			break;
		}
	}
	return extracted_vertex;
}

/*-------------------------------------------------------------*/
/*Check if the needle is in the proccessed array, return 1 if element was found,
0 otherwise*/
int proc_arr_search(char**proc_arr, char*needle, int arr_size)
{
	int index;
	for(index=0;index<arr_size;index++)
	{
		if(strcmp(proc_arr[index], needle) == 0)
			return 1;
	}
	return 0;
}
/*-------------------------------------------------------------*/
/*Store the vertices into an dynamic array and calculate total number of vertices*/
char* total_vertex_count(FILE *file, int *total_vertex)
{
	char character;
	int temp_s_index = 0;
	char* vertices = (char*)calloc(MAX_ALPHABET, sizeof(char));
	
	while((character=mygetchar(file))!=EOF){
		if(isalpha(character)){
			if(str_search(vertices, character) == 0){
				vertices[temp_s_index] = character;
				temp_s_index++;}
		}
	}
	*total_vertex = strlen(vertices);
	rewind(stdin);
	return vertices;
}
/*-------------------------------------------------------------*/
/*A modified version for str_search. To search if the needle is within the haystack
and to prevent errors if they have duplicates. E.g "AB100", "BA100" will return 1*/

int str_search_ex(char *hay, char needle, int sc_val)
{
    int max_num_length = strlen(hay)-ALPHA_FORMAT;
    int alpha_length = ALPHA_FORMAT;
    int index, temp_index1, temp_index2, confirm;
    int  sc_val_length = (int)(log10(sc_val)+1);
    index = temp_index1 = temp_index2 = confirm = 0;
    
    char* temp_str = (char*)calloc(alpha_length, sizeof(char));
    char* temp_int = (char*)calloc(max_num_length, sizeof(char));
    char* sc_val_str = (char*)calloc(sc_val_length, sizeof(char));
    
    sc_val_str = convert_num(sc_val);
    
    for(index=0;index<strlen(hay);index++)
    {
    		if(isalpha(hay[index])){   
    		    temp_str[temp_index1] = hay[index];
    		    temp_index1++;
    		}
    		if(isdigit(hay[index])){   
    		    temp_int[temp_index2] = hay[index];
    		    temp_index2++;
    		}
    }
    if((temp_str[FIRST_ELEMENT] == needle||temp_str[ELEMENT_TWO] == needle) && 
    	(strcmp(temp_int, sc_val_str) == 0)){
    		return 1;
    }
    else{
        	return 0;
    }
}
/*-------------------------------------------------------------*/
/*This function reads from FILE pointer(stdin), processed them and 
store them into different variables and structures, min_val, max_val,
vertex->name, vertex->info...etc.
*/
void Info_vertex(FILE *file, struct node_t *vertex, int total_vertex, int* num_rows, int *max_col,
							char* vertices, int *max_val, int *min_val, int *total_val)
{
	char* file_array = (char*)calloc(MAX_FILE_SIZE, sizeof(char));
	int index, num_edges, max_colm;
	num_edges = max_colm = 0;
	
	Info_vertex_process1(file, file_array);
	for(index=0;index<total_vertex;index++){
		vertex[index].name = vertices[index];
	}
	
	num_edges = count_rows(file_array);
	max_colm = count_colm(file_array);
	*num_rows =  num_edges; 
	*max_col = max_colm;
	
	char **array = dym_arr_gen(num_edges, max_colm);
	
	Info_vertex_process2(array, file_array, max_colm);
	Info_vertex_assign(array, total_vertex, num_edges, max_colm, vertex);
	Info_vertex_process3(array, num_edges, min_val, max_val, total_val);
	Info_vertex_process4(array, vertex, num_edges, total_vertex, vertices);
	
}
/*-------------------------------------------------------------*/
/*Extended function of Info_vertex.This function Assign vertex info 
into the structure from the text file*/
void Info_vertex_assign(char**array, int total_vertex, int num_edges, 
					int max_colm, struct node_t *vertex)
{
	int index, index_x, index_control,count_ver,degree,row;
	char target_ver;
	char odd_even[EVEN] = {'e', 'o'};
	int* vertex_no_info = (int*)calloc(total_vertex, sizeof(int));
	degree= count_ver= index_control = index_x = 0;
	
	for(index_x=0;index_x<total_vertex;index_x++)
	{
		target_ver = vertex[index_x].name; 
		for(index=0;index< num_edges;index++)
		{
			if(str_search(array[index], target_ver)){
				count_ver++;
			}
			degree += ver_degree(array[index], target_ver);
		}
		vertex[index_x].edges = count_ver;
		vertex[index_x].degrees = degree;
		vertex_no_info[index_control] = count_ver;
		vertex[index_x].o_or_e = odd_even[degree%EVEN]; 
		vertex[index_x].unprocess = count_ver;
		degree = RESET;
		count_ver = RESET;
		index_control++;
	}
	
	for(index=0;index<total_vertex;index++){
		vertex[index].info = (char**)calloc(vertex_no_info[index], sizeof(char*));
		for(row=0;row< vertex_no_info[index]; row++)
		{
			vertex[index].info[row] = (char*)calloc(max_colm, sizeof(char));
		} 
	}
}
/*-------------------------------------------------------------*/
/*Extended function from Info_vertex*/
void Info_vertex_process1(FILE* file, char* file_array)
{
	char character;
	int index = 0;
	while((character=mygetchar(file))!=EOF)
	{
		if(character!=WHITESPACE){ 
			file_array[index] = character;
			index++;
		}
	}
}
/*-------------------------------------------------------------*/
/*Extended function from Info_vertex*/
void Info_vertex_process2(char **array, char* file_array, int max_colm)
{
	int row, temp_str_index, file_index;
	char *temp_str = (char*)calloc(max_colm, sizeof(char));
	row = file_index = temp_str_index = 0;

	for(file_index=0;file_index<strlen(file_array);file_index++)
	{
		if(file_array[file_index]!= '\n'){
			temp_str[temp_str_index] = file_array[file_index];
			temp_str_index++;
		}
		else
		{
			strcpy(array[row], temp_str);
			memset(temp_str, 0, strlen(temp_str));
			temp_str_index = RESET;
			row++;
		}
	}	
}
/*-------------------------------------------------------------*/
/*Extended function from Info_vertex*/
void Info_vertex_process3(char**array, int num_edges, int*min_val, 
							int*max_val, int*total_val) 
{
	int index;
	int total_value, min_value, max_value;;
	int*extracted_val = (int*)calloc(num_edges, sizeof(int));
	total_value = min_value = max_value = 0;
	
	for(index=0;index<num_edges;index++){
		extracted_val[index] = extract_int(array[index]);
	}
	
	min_value = find_smallest(extracted_val, num_edges);
	max_value = find_largest(extracted_val, num_edges);
	for(index=0;index<num_edges;index++){
		total_value += extracted_val[index];	
	}
	
	*min_val = min_value; 
	*max_val = max_value; 
	*total_val = total_value;
}
/*-------------------------------------------------------------*/
/*Extended function from Info_vertex*/
void Info_vertex_process4(char**array, struct node_t *vertex, int num_edges, 
							int total_vertex, char*vertices)
{
	char vertex_now;
	int index, index2, vertex_control, row_control;
	vertex_control = row_control = 0;
	
	for(index=0;index<total_vertex;index++){
		vertex_now = vertices[index];
		for(index2=0;index2<num_edges;index2++)
		{
			if( (str_search(array[index2], vertex_now)) ){
				strcpy(vertex[vertex_control].info[row_control], array[index2]); 
				row_control++;
			}
		}
		vertex_control++;
		row_control = RESET;
	}
}
/*-------------------------------------------------------------*/
/*Main function of Stage 0, display all the required info. Not much to explain.*/
void Stage_0_Main(struct node_t *vertex, char starting_vertex, int total_vertex, 
			 int num_edges, int max_colm, int min_val, int max_val, int total_val) 
{
	int index, num_odd, num_even;
	num_odd = num_even = 0;
	
	for(index=0;index<total_vertex;index++){
		if(vertex[index].o_or_e == 'e'){
			num_even++;
		}
		else{
			num_odd++;
		}
	}
	
	printf("Stage 0 Output\n--------------\n");
	printf("S0: Map is composed of %d vertices and %d edges\n", total_vertex, num_edges);
	printf("S0: Min. edge value: %d\n",min_val);
	printf("S0: Max. edge value: %d\n",max_val);
	printf("S0: Total value of edges: %d\n",total_val);
	printf("S0: Route starts at %c%c%c\n",'"',starting_vertex, '"');
	printf("S0: Number of vertices with odd degree: %d\n", num_odd);
	printf("S0: Number of vertices with even degree: %d\n", num_even);
	if(num_even == 0){
		exit(EXIT_FAILURE);
	}
	if(num_odd == 0 && num_even >= 1){
		printf("S0: Multigraph is Eulerian\n");
	}
	else{
		printf("S0: Multigraph is traversable\n");
		exit(EXIT_FAILURE);
	}
}
/*-------------------------------------------------------------*/
/*This is the main function of Stage 1 and Stage 2. This function takes in
structure, starting vertex,to compute the route follow the rules in the assignment
final_route -> an array to store the final computed rule
final_scene -> to store the scenic val of the final_route
temp_route_f -> store the transition route
temp_scene_f -> store its scenic value
proc_arr -> an 2D array to store processed edges, e.g. {"ab10", "cd1"....etc}

E.g. A1->B2->C3->D4->A will store as "ABCDA" in final_route, and 12340 in final
scene. 
Logic:  
1) Compute the least scenic value and check if the edges has been processed;
2) If not, store it into the temp arrays...and do it until it go backs to its starting
vertex.
3). And keep doing it until all the edges are processed.
*/

void Stages_algorithm(struct node_t *vertex, char starting_vertex, int total_vertex,
			int num_edges, int max_colm, char*next_ver_stage2, int stage)
{
	int route_index, scene_index, index, iterate_count, line;
	int pr_index, next_ver_index, extended_size;
	
	route_index = scene_index = iterate_count = pr_index = next_ver_index = line = 0;
	extended_size = num_edges+1;
	
	char* final_route = (char*)calloc(extended_size, sizeof(char));
	int* final_scene = (int*)calloc(extended_size, sizeof(int));
	char* temp_route_f = (char*)calloc(extended_size, sizeof(char));
	int* temp_scene_f = (int*)calloc(extended_size, sizeof(int));
	char **proc_arr = dym_arr_gen(num_edges, max_colm);
	char next_vertex;
	next_vertex = starting_vertex;
	
	printf("Stage %d Output\n--------------\n", stage);
	while(starting_vertex!= EMPTY)
	{
		temp_route_f[FIRST_ELEMENT] = starting_vertex;
		for(index=0;index<total_vertex;index++){	
			if(vertex[index].name == next_vertex)
			{
				next_vertex = Stages_Body(index, &pr_index, &route_index, 
					&scene_index, vertex, proc_arr, num_edges, total_vertex, 
					temp_scene_f, temp_route_f, next_vertex);

				if(next_vertex==starting_vertex)
				{
					line = iterate_count +1;
					if(stage == STAGE_ONE){
						Stage_1_Routing(final_route, final_scene, temp_route_f, 
						temp_scene_f, iterate_count, num_edges);
						starting_vertex = Stages_next_vertex(final_route,total_vertex,vertex);
						next_ver_stage2[next_ver_index] = starting_vertex;
						next_ver_index++;
					}
					if(stage == STAGE_TWO){
						Stage_2_Routing(final_route, final_scene, temp_route_f, 
						temp_scene_f, iterate_count, num_edges);
						starting_vertex = next_ver_stage2[next_ver_index];
						next_ver_index++;
					}
					
					if(line <= PRINT_RESTRICT10){
						Stages_Print_route(final_route, final_scene, stage);
					}
					if(line > PRINT_RESTRICT&& ((line%FIFTH)==0)){
						Stages_Print_route(final_route, final_scene, stage);
					}
					
					route_index = RESET;	
					scene_index = RESET;	
					iterate_count++;
					next_vertex =  starting_vertex;
				}
			}
		}
	}
	Stages_Print_route(final_route, final_scene, stage);
	Stages_Print_scene(final_scene, num_edges, stage);
	Restore_unproc(vertex, total_vertex);
}

/*-------------------------------------------------------------*/
/*An extended function of Stage_algorithm. Logic same as above*/
char Stages_Body(int index, int*pr_index, int*route_index, int*scene_index,
						struct node_t* vertex, char**proc_arr, 
						int num_edges, int total_vertex, int*temp_scene_f, 
						char*temp_route_f, char next_vertex)
									
{
	int temp_index, index2, smallest;
	char* temp_vertex = (char*)calloc(vertex[index].edges, sizeof(char));
	int *temp_scene = (int*)calloc(vertex[index].edges, sizeof(int));
	temp_index = index2 = smallest = 0;
	
	for(index2=0;index2<vertex[index].edges;index2++)
	{
		if( (proc_arr_search(proc_arr, vertex[index].info[index2], num_edges)) == 0)
		{
			temp_vertex[temp_index] = extract_ver((vertex[index].info[index2]), next_vertex);
			temp_scene[temp_index] = extract_int(vertex[index].info[index2]);
			temp_index++;
			}
		}
		
	smallest = find_smallest(temp_scene, temp_index);
	next_vertex = Stages_route(temp_vertex, temp_scene, smallest, vertex[index].edges);
	
	for(index2=0;index2<vertex[index].edges;index2++){
		if(str_search_ex(vertex[index].info[index2], next_vertex, smallest))
		{
			if(proc_arr_search(proc_arr, vertex[index].info[index2],num_edges)==0)
			{
				proc_arr[*pr_index] = vertex[index].info[index2];
				Update_unproc(vertex, proc_arr[*pr_index], total_vertex);
				*pr_index = *pr_index+1;
				  break;
			}
		}
	}
	
	*route_index = *route_index+1;
	temp_scene_f[*scene_index] = smallest; 
	temp_route_f[*route_index] = next_vertex;
	*scene_index = *scene_index+1; 
	
	memset(temp_vertex, EMPTY, sizeof(char)*vertex[index].edges);
	memset(temp_scene, EMPTY, sizeof(int)*vertex[index].edges);
	
	return next_vertex;
}
/*-------------------------------------------------------------*/
/*An extended function of Stage_algorithm. Compute the next possible 
vertex with smallest ASCII code value withinone Eulerian Trail. 
Logic same as above*/

char Stages_route(char*array_ver, int* array_scen, int smallest, int edges)
{
	int index, temp_index=0;
	char* temp_vert = (char*)calloc(edges, sizeof(char));
	int* temp_scen = (int*)calloc(edges, sizeof(int));
	
	for(index=0;index<edges;index++){
		if(array_scen[index] == smallest){
			temp_scen[temp_index] = array_scen[index];
			temp_vert[temp_index] = array_ver[index];
			temp_index++;
		}
	}
	
	char smallest_vert;
	if(strlen(temp_vert) > 1){
		for(index=0;index<strlen(temp_vert);index++){
			if(index==0){
				smallest_vert = temp_vert[index];
			}
			if(temp_vert[index] < smallest_vert){
				smallest_vert = temp_vert[index];
			}
		}	
	}
	else{
		smallest_vert=temp_vert[FIRST_ELEMENT];
	}
	return smallest_vert;
}
/*-------------------------------------------------------------*/
/*Part of the function in Stage_algorithm. Update the vertex.unprocess
value for processing vertex*/

void Update_unproc(struct node_t *vertex, char*proc_ver, int total_vertex)
{
	int index, alpha_num = ALPHA_FORMAT;
	char* temp_str = (char*)calloc(alpha_num, sizeof(char));
	char vertex_I, vertex_II;
	
	for(index=0;index<strlen(proc_ver);index++)
	{
		if(isalpha(proc_ver[index])){ 
		temp_str[index] = proc_ver[index]; 
		}
	}
	
	vertex_I = temp_str[FIRST_ELEMENT]; vertex_II = temp_str[ELEMENT_TWO];
	
	for(index=0;index<total_vertex;index++)
	{
		if(vertex[index].name == vertex_I){
			vertex[index].unprocess --;
		}
		else if(vertex[index].name == vertex_II){
			vertex[index].unprocess --;
		}
	}
}
/*-------------------------------------------------------------*/
/*Logic of this function:
1) If it is the first loop, iterate_count = 0, then just put
temp_route into final_route.
2)Else, Acquire the first element from the transition array, e.g. JjqjJ, will get J 
from it and acquire its target index in the final_route. 
3)Shifting "length_final-target_index" units to the left and insert elements 
from transition array.
4)Reset the transition array(temp_route) using memset() */

void Stage_1_Routing(char*final_route, int*final_scene, char*temp_route_f, 
	int* temp_scene_f, int iterate_count, int num_edges)
{
	int index, target_index;
	int length_temp = strlen(temp_route_f);
	int length_final = strlen(final_route);
	int total_length = length_temp + length_final - 1;
	int last_ele = total_length -1;
	target_index = 0;
	char first_ele;
	
	if(iterate_count == 0){
		for(index=0;index<length_temp;index++){
			final_route[index] = temp_route_f[index];
			final_scene[index] = temp_scene_f[index];
		}
	}
	else{
		first_ele = temp_route_f[FIRST_ELEMENT];
		for(index=0;index<length_final;index++){
			if(final_route[index] == first_ele){
				target_index = index;
				break;
			}
		}
		Stages_append_final(final_route, final_scene, temp_route_f, temp_scene_f, 
								target_index, length_final, length_temp, last_ele);
	}
	memset(temp_route_f, EMPTY, sizeof(char)*(num_edges+1));
	memset(temp_scene_f, EMPTY, sizeof(int)*(num_edges+1));
}

/*-------------------------------------------------------------*/
/*Sub function of Stage_1_Routing & Stage_2_Routing
append the elements from temp_route to final_route */
void Stages_append_final(char*final_route, int* final_scene, char* temp_route_f, 
				int* temp_scene_f, int target_index, int length_final,
				int length_temp, int last_ele)
{
	int index;
	for(index=0; index< length_final-target_index;index++){
			final_route[last_ele-index] = final_route[length_final-1-index];
			final_scene[last_ele-index] = final_scene[length_final-1-index];
		}
	for(index=0;index<length_temp-1;index++){
		final_route[target_index+index] = temp_route_f[index];
		final_scene[target_index+index] = temp_scene_f[index];
		}
}
/*-------------------------------------------------------------*/
char Stages_next_vertex(char*final_route, int total_vertex, struct node_t *vertex)
{
	int length = 0; 
	int index, index2;
	char next_vertex;
	length = strlen(final_route);
	
	for(index=0;index<length;index++)
	{
		for(index2=0;index2<total_vertex;index2++)
		{	
			if(final_route[index] == vertex[index2].name)
			{
				if(vertex[index2].unprocess>0){
					next_vertex = final_route[index];
					return next_vertex;
				}
			}
		}
	}
	return 0;
}
/*-------------------------------------------------------------*/
/*Similar to Stage_1_Routing, but this time, we also compute all possible route
for each iteration, compare them and then append it into the final_route*/

void Stage_2_Routing(char*final_route, int*final_scene, char*temp_route_f, 
	int* temp_scene_f, int iterate_count, int num_edges)
{
	int index, target_index, count, length_temp, length_final, v_l_size, total_length;
	int last_ele;
	char target_vertex;
	
	length_temp = strlen(temp_route_f);  
	length_final = strlen(final_route);
	total_length = length_temp + length_final - 1;
	last_ele = total_length -1;
	v_l_size = target_index = count =0;
	target_vertex = temp_route_f[FIRST_ELEMENT];
	
	for(index=0;index<length_final;index++)
	{
		if(final_route[index] == target_vertex){
			v_l_size++;}
	}
	
	int* vertex_location = (int*)calloc(v_l_size, sizeof(int));
	
	if(iterate_count == 0){
		for(index=0;index<length_temp;index++){
			final_route[index] = temp_route_f[index];
			final_scene[index] = temp_scene_f[index];
		}
	}
	else{
		for(index=0;index<length_final;index++){
			if(final_route[index] == target_vertex){
				vertex_location[count] = index;
				count++;
			}
		}
		
		target_index = Stage_2_Analysis(final_scene, final_route, vertex_location, 
								v_l_size, num_edges, temp_route_f, temp_scene_f);
		
		Stages_append_final(final_route, final_scene, temp_route_f, 
							temp_scene_f, target_index, length_final,
							length_temp, last_ele);
	}
	memset(temp_route_f, EMPTY, sizeof(char)*(num_edges+1));
	memset(temp_scene_f, EMPTY, sizeof(int)*(num_edges+1));
}

/*-------------------------------------------------------------*/
int Stage_2_Analysis(int* final_scene, char* final_route, int* vertex_location, int v_l_size, 
	int num_edges, char*temp_route_f, int* temp_scene_f)
{
	int index;
	int temp_target_index, target_index;
	char* final_route_copy = (char*)calloc(num_edges+1, sizeof(char));
	int* final_scene_copy = (int*)calloc(num_edges+1, sizeof(int));
	int* total_scen_val = (int*)calloc(v_l_size, sizeof(int));
	int length_temp, length_final, total_scen_index, largest_scen, total_length, last_ele;
	
	total_length = total_scen_index = largest_scen = target_index = 0;
	temp_target_index = 0;
	length_temp = strlen(temp_route_f);  length_final = strlen(final_route);
	total_length = length_temp + length_final - 1; 
	last_ele = total_length -1;
	
	Stages_route_copy(final_scene, final_route, final_route_copy, final_scene_copy, 
						num_edges);
	
	for(index=0;index<v_l_size;index++)
	{
		temp_target_index = vertex_location[index];
		
		Stages_append_final(final_route_copy, final_scene_copy, temp_route_f, 
				temp_scene_f, temp_target_index, length_final,
				length_temp, last_ele);
		total_scen_val[total_scen_index] = Total_scenic(final_scene_copy, num_edges);
		
		Stages_route_copy(final_scene, final_route, final_route_copy, final_scene_copy, 
					num_edges);
		
		total_scen_index++;
	}
	largest_scen = find_largest(total_scen_val, v_l_size);
	
	for(index=0;index<v_l_size;index++)
	{
		if(total_scen_val[index] == largest_scen){ 
			target_index = vertex_location[index];
			return target_index;
		}
	}
	return target_index;
}
/*-------------------------------------------------------------*/
/*Calculate the total scenic Value for that route*/
int Total_scenic(int* scene, int size)
{
	int index, total_val= 0;
	for(index=0;index<size+1;index++)
	{
		total_val += (index+1)*scene[index];
	}
	return total_val;
}
/*-------------------------------------------------------------*/
/*Copy route to temp*/
void Stages_route_copy(int* final_scene, char* final_route, char* final_route_copy, 
				int* final_scene_copy, int num_edges)
{
	int index; 
	for(index=0;index<num_edges+1;index++){
		final_route_copy[index] = final_route[index];
		final_scene_copy[index] = final_scene[index];
	}
}
/*-------------------------------------------------------------*/
/*Stages_Print_route format*/
void Stages_Print_route(char* final_route, int* final_scene, int stage)
{
	int length = strlen(final_route);
	int index;
	printf("S%d: ", stage); 
	if(length < PRINT_RESTRICT){
		for(index=0;index<length;index++)
		{
			printf("%c", final_route[index]);
			if(final_scene[index]!=EMPTY){
				printf("-%d->", final_scene[index]);
			}
		}
	}	
	else{
		for(index=0;index<length;index++)
		{
			if(index<FIRST_7TH_ELE||index >= length-LAST_7TH_ELE){
				printf("%c", final_route[index]);
				if(final_scene[index]!=EMPTY){
					printf("-%d->", final_scene[index]);
				}
			}
			if(index==FIRST_7TH_ELE){
				printf("%c", final_route[index]);
				printf("...");
			}	
		}
	}
	printf("\n");	
}
/*-------------------------------------------------------------*/
void Stages_Print_scene(int* final_scene, int num_edges, int stage)
{
	int total_val=0;

	total_val = Total_scenic(final_scene, num_edges);
	printf("S%d: Scenic route value is %d\n", stage, total_val); 
}
/*-------------------------------------------------------------*/
void Restore_unproc(struct node_t *vertex, int total_vertex)
{
	int index;
	for(index=0;index<total_vertex;index++){
		vertex[index].unprocess = vertex[index].edges;
	}
}
/*-------------------------------------------------------------*/
/*Processing Center for all stages, calling function from here*/
void Central_Process(int argc, char*argv[])
{
	FILE *file;
	char starting_vertex;
	int total_vertex, num_edges, max_colm, max_val, min_val, total_val;
	char* vertices = (char*)calloc(MAX_ALPHABET, sizeof(char));
	
	file = stdin;
	starting_vertex = ARGV_STARTING_VER;
	vertices = total_vertex_count(file, &total_vertex);
	
	node_t* vertex;
	vertex = (node_t*)calloc(total_vertex,sizeof(struct node_t));
	
	
	Info_vertex(file, vertex, total_vertex, &num_edges, &max_colm, vertices,  
				&max_val, &min_val, &total_val);
	
	char *next_ver_stage2 = (char*)calloc(total_vertex, sizeof(char)); 
	Stage_0_Main(vertex, starting_vertex, total_vertex, num_edges, max_colm, 
			min_val, max_val, total_val);

	Stages_algorithm(vertex, starting_vertex, total_vertex, num_edges, 
					max_colm, next_ver_stage2, STAGE_ONE);
	Stages_algorithm(vertex, starting_vertex, total_vertex, num_edges, 
					max_colm, next_ver_stage2, STAGE_TWO);
	
}

/*-------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	
	Central_Process(argc, argv);
	return 0;
	/*Algorithms are fun :( */
}

/*-------------------------------------------------------------*/