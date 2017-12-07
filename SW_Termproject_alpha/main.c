#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <time.h>
#include <io.h>
#include <Windows.h>

#define FILE_NUM 1000
#define FILE_NAME_LENGTH 260
#define WORD_LENGTH 20
#define _CRT_SECURE_NO_WARNINGS

struct file_list
{
	char file_name[FILE_NAME_LENGTH];
	int search_result;
	int file_size;
	float result_portion;
}fl[FILE_NUM];
//구조체에는 파일 이름, 탐색 결과, 파일 크기가 저장, 이를 배열로 묶어 관리

char* search_word;
int word_length = 0;
char* problem = "125313newsML.txt";

void list_substitute(struct file_list a, struct file_list b)
{
	strncpy(a.file_name, b.file_name, sizeof(b.file_name));
	a.search_result = b.search_result;
	a.file_size = b.file_size;
	a.result_portion = b.result_portion;
}

void current_file_address()
{
	char* cur_adrs = NULL;
	char adrs_buf[500] = { 0, };
	cur_adrs = _getcwd(adrs_buf, 500);
	printf("\n\n현재 폴더 디렉토리는 %s 입니다.", cur_adrs);
}

void input_searchword()
{
	search_word = (char*)malloc(sizeof(char)*WORD_LENGTH);
	scanf(" %s", search_word);
	word_length = strlen(search_word);	
}//키워드 입력 받는 함수

void result_sort(struct file_list* fl, int left, int right)
{
	int L = left, R = right;
	struct file_list temp;
	int pivot = fl[(left + right) / 2].search_result; //피봇 위치(중앙)의 값을 받음.
	while (L <= R)
	{
		//pivot이 중간 값이고, 비교 대상 arr[L], arr[R]은 pivot과 비교하니 중간 지점을 넘어가면 종료로 볼 수 있음.
		while (fl[L].search_result < pivot) //left부터 증가하며 pivot 이상의 값을 찾음.
			L++;
		while (fl[R].search_result > pivot) //right부터 감소하며 pivot 이하 값을 찾음.
			R--;
		//L, R 모두 최대 pivot 위치까지 이동.
		if (L <= R)
		{ //현재 L이 R이하면. (이유 : L>R 부분은 이미 정리가 된 상태임).
			if (L != R)
			{ 
				temp = fl[L];
				fl[L] = fl[R];
				fl[R] = temp;
			} //L과 R이 같다면 교환(SWAP)은 필요 없고 한 칸씩 진행만 해주면 됨.
			L++; R--; 
		}
	}
	if (left < R)
	{
		result_sort(fl, left, R);
	}
	if (L < right)
	{
		result_sort(fl, L, right);
	}
}

int string_search(char* file_name, int file_size)
{
	FILE* fp = fopen(file_name, "rt");
	
	char file_content[10000];
	char file_buf = NULL;
	int i = 0, result_cnt = 0;
	if (fp == NULL)
	{
		printf("파일을 불러오는데 실패했습니다.\n");
		return -1;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s", file_content);
		file_content[i++] = file_buf;
		if (!_strnicmp(file_content, search_word, strlen(search_word)))
		{
			result_cnt++;
		}
	}//버퍼 문자 변수로 하나씩 옮겨담기
	fclose(fp);
	return result_cnt;
}

void file_init()
{
	struct _finddata_t fd;
	long handle;
	int result = 1, i = 0;

	handle = _findfirst("*.txt", &fd);
	if (handle == -1)
	{
		printf("\n파일 탐색이 종료되었습니다.\n");
		return;
	}
	while (result != -1)
	{
		strncpy(fl[i].file_name, fd.name, sizeof(fd.name)); //구조체에 파일 이름 저장
		fl[i].search_result = string_search(fd.name, fl[i].file_size);
		if (fl[i].search_result == 6)
		{
			printf("\n찾았다!\n\n");
		}
		fl[i].file_size = fd.size; //구조체에 파일 크기 저장
		fl[i].result_portion = (float)fl[i].search_result * word_length / fl[i].file_size * 100;
		i++;
		result = _findnext(handle, &fd);
	}
	_findclose(handle);
}//구조체에 파일 목록 저장

int main()
{
	int i, file_cnt = 0;
	float time_measure;
	time_t startTime = 0, endTime = 0;
	printf("단어 검색 프로그램\n");
	//current_file_address();

	printf("\n\n영문자 소문자로 키워드를 입력해주세요 : \n");
	input_searchword();
	system("cls");
	printf("\n키워드 %s로 탐색 시작 ...\n\n", search_word);
	startTime = clock();
	file_init();

	result_sort(fl, 0, FILE_NUM);
	endTime = clock();
	time_measure = (float)(endTime - startTime) / (CLOCKS_PER_SEC);

	for (i = 0; i < FILE_NUM; i++)
	{
		if (fl[i].search_result != 0)
		{
			file_cnt++;
			printf("%s : %d개의 %s, 키워드 출현 빈도는 %.3f%%\n", fl[i].file_name, fl[i].search_result, search_word, fl[i].result_portion);
		}
	}
	if (file_cnt == 0)
	{
		printf("키워드 탐색 결과 없음.\n");
	}
	printf("\n탐색 종료. 걸린 시간 : %fs\n", time_measure);
	printf("%d개 중 키워드 포함한 파일은 %d개, 빈도는 %.3f%%.\n\n", FILE_NUM, file_cnt, (float)file_cnt/FILE_NUM*100);
	free(search_word);
	system("pause");
	return 0;
}