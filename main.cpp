#include <stdio.h>
#include <stdlib.h>
#include <error.h>


#define	HAVE_GETCH	0


#if	HAVE_GETCH
#include <conio.h>
#endif




static int isNum(char ch)
{
	if(ch>='0' && ch <='9'){
		return 0;
	}


	return -1;
}


static int ch2Num(char ch)
{
	int ret;


	ret = isNum(ch);
	if(ret){
		printf("\nUnknow error!");
		return -1;
	}


#if	HAVE_GETCH
	printf("%c", ch);
#endif


	return (int)(ch - '0');
}


int sortSimple(int len, long long *arr)
{
	int i;
	int k = 0;
	long long tmp_int;


	if( len <= 1 ){
		printf("Error because len = %d!\n", len);
		goto err;
	}


	for(i=0;k<len;i>=len-1?i=++k:i++)
	{
		if(arr[i] < arr[k] && i < len){
			tmp_int = arr[k];
			arr[k] = arr[i];
			arr[i] = tmp_int;
		}


	}


	return 0;
err:
	return -1;
}


typedef struct bucket {
	int count;
	long long *arr;
}bucket;


static int sortArray(long long *arr, int arr_len)
{
	int i;
	long long max = arr[0];
	long long min = arr[0];
	int site;
	long long *all_arr;
	int count;
	int j = 0;
	struct bucket *buc;
	float rge;
	int ret = 0;


	if(arr_len==1)
		return 0;


	all_arr = (long long *)calloc(1, arr_len * arr_len * sizeof(long long));
	if(all_arr==NULL)
		goto err;


	buc = (bucket *)calloc(1, arr_len * sizeof(bucket));
	if(buc==NULL)
		goto err;


	for(i=0;i<arr_len;i++)
	{
		buc[i].arr = all_arr + i * arr_len * sizeof(long long);
		buc[i].count = 0;
	}


	for(i=0;i<arr_len;i++)
	{
		if(max < arr[i])
			max = arr[i];


		if(min > arr[i]){
			min = arr[i];


		}
	}


	rge = (float)( max - min ) / (float)( arr_len );


	if(rge > -0.00001 && rge < 0.00001)
		goto err;


	for(i=0;i<arr_len;i++)
	{
		if(arr[i] == max){
			count = buc[arr_len - 1].count;
			buc[arr_len - 1].arr[count] = arr[i];
			buc[arr_len - 1].count++;
			continue;
		}


		site = (int)(arr[i] - min)/rge;


		if( arr[i] < (rge * site + min) )
			site--;


		count = buc[site].count;
		buc[site].arr[count] = arr[i];
		buc[site].count++;
	}


	for(i=0;i<arr_len;i++)
	{
		count = buc[j].count;
		if(count == 1){
			arr[i] = buc[j].arr[buc[j].count - count];
		}


		if(count <= 0){
			continue;
		}


		if(count > 1){
			ret = sortSimple(count, buc[j].arr);
			if( ret < 0 ){
				printf("Unknow Error in sortArr!\n");
				goto err;
			}
		}


		j++;
	}


	for(i=0,j=0;j<arr_len;j++)
	{
		count = buc[j].count;
		for(;count>0;count--)
		{
			arr[i++] = buc[j].arr[buc[j].count - count];
		}
	}


	free(all_arr);


	all_arr = NULL;


	return 0;


err:
	return -1;
}


int main(void)
{
	long long *arr;
	int arr_len = 0;
	char ch;
	int i;
	int ret = 0;
	int flag = 0;




start:
	printf("Input array length:");
	scanf("%d", &arr_len);


	if(arr_len<=0)
		goto arr_len_err;


	arr = (long long *)calloc(1, arr_len * sizeof(long long));


	if(arr==NULL)
		goto nomem;


	ret = 1;


	printf("\nInput array[0]:");
	for(i=0;i<arr_len;)
	{
#if HAVE_GETCH
		ch = (char)getch();


		if( (ch == 13 || ch == ' ') && !ret){


			i++;
			if(i >= arr_len)
				goto input_end;


			printf("\nInput array[%d]:", i);
			ret = 1;


			continue;
		}
#else
		ch = (char)getchar();


		if( (ch == 10 || ch == ' ') && !ret){


			i++;
			if(i >= arr_len)
				goto input_end;


			ret = 1;				


			if(ch == 10){
				if(flag)
					printf("\nInput array[%d]:%lld\n", i-1, arr[i-1]);


				printf("Input array[%d]:", i);
				flag = 0;
			}


			if(ch == ' '){
				printf("\nInput array[%d]:%lld", i-1, arr[i-1]);
				flag = 1;
			}


			continue;
		}
#endif
input_end:
		ret = isNum(ch);
		if(!ret){
			arr[i] = arr[i] * 10 + (long long)ch2Num(ch);
		}


	}


	printf("\nInput end.\n\nYour arrar is:\n");


	for(i=0;i<arr_len;i++)
	{
		printf("array[%d]=%lld\n", i, arr[i]);
	}


	ret = sortArray(arr, arr_len);
	if(ret<0){


		free(arr);
		printf("Sort array fail! retry?(y/n)");


#if	HAVE_GETCH
		ch = getch();
#else
		ch = getchar();
#endif


		if(ch == 'y' || ch == 'Y'){
			printf("\n\n");
			goto start;
		}


		printf("\n");
		goto err;
	}


	printf("\nSort array resault:\n");


	for(i=0;i<arr_len;i++)
	{
		printf("array[%d]=%lld\n", i, arr[i]);
	}


	free(arr);


	arr = NULL;


	system("pause");


	return 0;


arr_len_err:
	return -1;
nomem:
	return -1;
err:
	return -1;
}
