//-------| src/main.c |-------//
#include "main.h"


static int fd;
static int map_counter = 0;
static void * map_data[100];


int main(int argc, char* argv[]) {
	
	short * clcd_cmd, * clcd_data;
	
	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd == -1) {
		fprintf(stderr, "Cannot open /dev/mem file");  
		exit(EXIT_FAILURE);
	}
	clcd_cmd  = mapper(IEB_CLCD_CMD);
	clcd_data = mapper(IEB_CLCD_DATA);

	init_clcd(clcd_cmd, clcd_data);
	
	
	if( checker(argc, argv) == ERROR ) {
		error();
	}
	else {

			char c = '0';
			for(int a = 0; a<40;a++){
				clcd_set_DDRAM(a);
				if(c == '9' + 1){
					c = '0';				
				}
				clcd_write_data(c++);
			}
			usleep(500000);
			for(int j = 0; j < 40; j++){
				clcd_shift(1, 0);	
				usleep(500000);				
			}
		
		
		
	}
	
	
	unmapper();
	close(fd);
	return 0;
}

short * mapper(off_t offset) {
	map_data[map_counter] = mmap(NULL, sizeof(short), PROT_WRITE, MAP_SHARED, fd, offset);
	if ( map_data[map_counter] == MAP_FAILED ) {
		fprintf(stderr, "Cannot do mmap()");
		emergency_closer();
	}
	return (short *)map_data[map_counter++];
}

void unmapper() {
	int i;
	for( i=0; i<map_counter; i++) {
		munmap(map_data[i], sizeof(short));
	}
	map_counter = 0;
}

void emergency_closer() {
	unmapper();
	close(fd);
	exit(EXIT_FAILURE);
}

error_t checker(int argc, char* argv[]) {
	int S=0;
	if( argc <= 2 ) {
		return ERROR;
	}
	else if( argc >= 3 ) {
		if( strlen(argv[1]) > 1 ) { return ERROR; }
		if( argv[1][0] == '0') {
			clcd_entry_mode_set(0, S);
			
			clcd_set_DDRAM( strlen(argv[2]) - 1 );
			clcd_write_string(argv[2]);
			
			if( argc > 3 ) {
				clcd_set_DDRAM( 0x4F );	// 제일 오른쪽에서 시작
				clcd_write_string(argv[3]);
			}
		}
		else if( argv[1][0] == '1' ) {
			clcd_entry_mode_set(1, S);
			clcd_write_string(argv[2]);
			
			if( argc > 3 ) {
				clcd_set_DDRAM(0x4F - strlen(argv[3]) + 1);
				// 제일 오른쪽에서 글자수만큼 왼쪽으로 이동하고 1을 더한 위치부터 시작
				clcd_write_string(argv[3]);
			}
		}
		else { return ERROR; }
	}
	return SUCCESS;
}

void error() {
	clcd_clear_display();
	clcd_set_DDRAM(0x40);
	clcd_write_string("  !! ERROR !!");
}

truth_t inputter() {
	int input_int;
	printf("\n");
	printf("***************************************\n");
	printf("*     Please type a number            *\n");
	printf("*     1 : Display shift (left)        *\n");
	printf("*     2 : Display shift (right)       *\n");
	printf("*     3 : Display On                  *\n");
	printf("*     4 : Display Off                 *\n");
	printf("*     0 : EXIT                        *\n");
	printf("***************************************\n\n");
	scanf("%d", &input_int);
	
	switch(input_int) {
		case 1 :
			clcd_shift(1, 0);
			clcd_shift(1, 0);
			break;
		case 2 :
			clcd_shift(1, 1);
			clcd_shift(1, 1);
			break;
		case 3 :
			clcd_display_control(1, 0, 0);
			break;
		case 4 :
			clcd_display_control(0, 0, 0);
			break;
		case 0 :
			return FALSE;
		default :
			break;
	}
	return TRUE;
}
