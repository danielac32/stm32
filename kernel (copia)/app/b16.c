#include <stdio.h>

void b16encode(const char *input, int input_len, char *output, int *output_len)
{
	int i;
	for (i = 0; i < input_len; i++) {
		output[2 * i] = (input[i] & 0xF) + 'A';
		output[2 * i + 1] = ((input[i] >> 4) & 0xF) + 'A';
	}

	*output_len = 2 * input_len;
}

void b16decode(const char *input, int input_len, char *output, int *output_len)
{
	int i;
	for (i = 0; i < input_len / 2; i++) {
		int low = input[2 * i] - 'A';
		int high = input[2 * i + 1] - 'A';
		output[i] = low | (high << 4);
	}

	*output_len = input_len / 2;
}


#define BUF_LEN 4096
char buff[BUF_LEN];
char b16[BUF_LEN * 2 + 1];




int main(int argc, char const *argv[])
{
	int buff_len;
	int b16_len;
	FILE *fp;
	/*fp = fopen("data.txt","w+");

	fprintf(fp, "daniel quintero\n");


    fclose(fp);*/


    fp = fopen("data.txt","r");
	while (buff_len = fread(buff, 1, BUF_LEN, fp)) {
		b16encode(buff, buff_len, b16, &b16_len);
		b16[b16_len] = 0;
		printf("\"%s\", %d\n", b16,b16_len);
	}
    fclose(fp);
    



    
    b16decode(b16, b16_len, buff, &buff_len);
	b16[b16_len] = 0;
	printf("\"%s\", %d\n", buff,buff_len);


	return 0;
}