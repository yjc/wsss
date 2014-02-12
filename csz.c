#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define	CONFIG_DEBUG	0

#if	CONFIG_DEBUG
#define	debug
#endif

#ifdef	debug
_CRTIMP int __cdecl d_printf (const char *format, ...)
{
   va_list arg;
   int done;

   va_start (arg, format);
   done = vfprintf (stdout, format, arg);
   va_end (arg);

   return done;
}
#else
_CRTIMP int __cdecl d_printf(const char *format, ...)
{
	return 0;
}
#endif

typedef	int bool;

#define	true	1
#define	false	0

static int initRand(void)
{
	time_t timer = time(NULL);

	if(timer == -1){
		printf("Timer init error!\n");
		return -1;
	}

	srand((unsigned int)timer);

	/* We do not use the first rand return value */
	rand();

	return 0;
}

static int	getRand(void)
{
	double tmp = 0.0;
	int ret = -1;

	tmp = (double)rand() / (double)(RAND_MAX+1);

	for(;tmp>-0.000001;tmp=tmp-0.1)
	{
		ret++;
	}

	return ret;
}

static bool flag[10];

static int initAnser(int *ans)
{
	int i;
	int num;
	int ret = 0;
	
	memset(flag, false, sizeof(bool) * 10);

	for(i=0;i<4;)
	{
		num = getRand();
		if(!flag[num]){
			flag[num] = true;
			ans[i++] = num;

			d_printf("num = %d\n", num);
		}

		ret++;
		if(ret >= 100){
			printf("Too many times to try to prepare the anser!\n");
			goto try_err;
		}
	}

	return 0;
try_err:
	return -1;
}

static int isCorrect(char *input_num, int *ans)
{
	int a = 0, b = 0;
	int tmp;
	int i;
	char num;

	for(i=0;i<4;i++)
	{
		num = input_num[i];
		tmp = (int)num - '0';
		if(flag[tmp]){
			if(tmp == ans[i])
				a++;
			else
				b++;
		}

		d_printf("tmp = %d,num = %c\n", tmp, num);

	}

	printf("\n%s:%dA%dB\n", input_num, a, b);

	if(a == 4){
		printf("Congratulations! The anser is %d%d%d%d\n", ans[0], ans[1], ans[2], ans[3]);
		return 0;
	}

	return -1;
}

static int isDiffNum(char *num)
{
	int i;
	int ret = true;

	for(i=0;i<4 && ret;i++)
	{
		if( isdigit(num[i]) ==0 ){
			ret = 0;
		}

		d_printf("num[%d] = %c is num ? (%d)\n", i, num[i], ret);
	}

	if(ret){
		if(num[0] == num[1])
			return -1;
		if(num[0] == num[2])
			return -1;
		if(num[0] == num[3])
			return -1;
		if(num[1] == num[2])
			return -1;
		if(num[1] == num[3])
			return -1;
		if(num[2] == num[3])
			return -1;
	}

	return !ret;
}

int main(int argc,char *argv[])
{
	int anser[4];
	int i;
	char *input_num;
	int ret;
	bool input_flag[10];

start:
	memset(input_flag, false, sizeof(bool) * 10);

	ret = initRand();
	if(ret){
		printf("Call initRand() faild!\n");
		return -1;
	}

	ret = initAnser(anser);
	if(ret){
		printf("Call initAnser() error");
		goto err;
	}

	d_printf("Anser is:%d%d%d%d\n", anser[0], anser[1], anser[2], anser[3]);

	input_num = calloc(1, sizeof(char) * 10);
	if(input_num == NULL){
		printf("No memery");
		goto nomem;
	}

	ret = -1;
	for(i=0;i<10 && ret;i++)
	{
		memset(input_num, 0, sizeof(char) * 10);

		printf("Please input 4 number:");

		scanf("%s", input_num);

		d_printf("You input:%s\n", input_num);

		if(input_num[4] != 0){
			printf("Please input 4 different number!\n");
			i--;
			continue;
		}

		ret = isDiffNum(input_num);
		if(ret){
			printf("Please input 4 different number!\n");
			i--;
			continue;
		}

		ret = isCorrect(input_num, anser);
	}

	printf("Play again ? (y/n)");

	while( getchar() != '\n' );
	scanf("%c", input_num);

	if( input_num[0] == 'y' || input_num[0] == 'Y' ){

		free(input_num);
		input_num = NULL;

		goto start;
	}

	return 0;
err:
nomem:
	return -1;

}