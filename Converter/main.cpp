#include <cstdio>
#include <cstdlib>
#include <cstdint>
int main(int32_t argc,char* argv[])
{

	for (int32_t i = 0; i < argc; i++)
	{
		printf(argv[i]);
		printf("\n");
	}
	system("pause");
	return 0;
}