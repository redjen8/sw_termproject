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
//����ü���� ���� �̸�, Ž�� ���, ���� ũ�Ⱑ ����, �̸� �迭�� ���� ����

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
	printf("\n\n���� ���� ���丮�� %s �Դϴ�.", cur_adrs);
}

void input_searchword()
{
	search_word = (char*)malloc(sizeof(char)*WORD_LENGTH);
	scanf(" %s", search_word);
	word_length = strlen(search_word);	
}//Ű���� �Է� �޴� �Լ�

void result_sort(struct file_list* fl, int left, int right)
{
	int L = left, R = right;
	struct file_list temp;
	int pivot = fl[(left + right) / 2].search_result; //�Ǻ� ��ġ(�߾�)�� ���� ����.
	while (L <= R)
	{
		//pivot�� �߰� ���̰�, �� ��� arr[L], arr[R]�� pivot�� ���ϴ� �߰� ������ �Ѿ�� ����� �� �� ����.
		while (fl[L].search_result < pivot) //left���� �����ϸ� pivot �̻��� ���� ã��.
			L++;
		while (fl[R].search_result > pivot) //right���� �����ϸ� pivot ���� ���� ã��.
			R--;
		//L, R ��� �ִ� pivot ��ġ���� �̵�.
		if (L <= R)
		{ //���� L�� R���ϸ�. (���� : L>R �κ��� �̹� ������ �� ������).
			if (L != R)
			{ 
				temp = fl[L];
				fl[L] = fl[R];
				fl[R] = temp;
			} //L�� R�� ���ٸ� ��ȯ(SWAP)�� �ʿ� ���� �� ĭ�� ���ุ ���ָ� ��.
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
		printf("������ �ҷ����µ� �����߽��ϴ�.\n");
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
	}//���� ���� ������ �ϳ��� �Űܴ��
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
		printf("\n���� Ž���� ����Ǿ����ϴ�.\n");
		return;
	}
	while (result != -1)
	{
		strncpy(fl[i].file_name, fd.name, sizeof(fd.name)); //����ü�� ���� �̸� ����
		fl[i].search_result = string_search(fd.name, fl[i].file_size);
		if (fl[i].search_result == 6)
		{
			printf("\nã�Ҵ�!\n\n");
		}
		fl[i].file_size = fd.size; //����ü�� ���� ũ�� ����
		fl[i].result_portion = (float)fl[i].search_result * word_length / fl[i].file_size * 100;
		i++;
		result = _findnext(handle, &fd);
	}
	_findclose(handle);
}//����ü�� ���� ��� ����

int main()
{
	int i, file_cnt = 0;
	float time_measure;
	time_t startTime = 0, endTime = 0;
	printf("�ܾ� �˻� ���α׷�\n");
	//current_file_address();

	printf("\n\n������ �ҹ��ڷ� Ű���带 �Է����ּ��� : \n");
	input_searchword();
	system("cls");
	printf("\nŰ���� %s�� Ž�� ���� ...\n\n", search_word);
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
			printf("%s : %d���� %s, Ű���� ���� �󵵴� %.3f%%\n", fl[i].file_name, fl[i].search_result, search_word, fl[i].result_portion);
		}
	}
	if (file_cnt == 0)
	{
		printf("Ű���� Ž�� ��� ����.\n");
	}
	printf("\nŽ�� ����. �ɸ� �ð� : %fs\n", time_measure);
	printf("%d�� �� Ű���� ������ ������ %d��, �󵵴� %.3f%%.\n\n", FILE_NUM, file_cnt, (float)file_cnt/FILE_NUM*100);
	free(search_word);
	system("pause");
	return 0;
}