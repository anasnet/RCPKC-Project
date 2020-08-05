#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <NTL/ZZ.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <Windows.h>

using namespace std;
using namespace NTL;



int main()
{
	double        pcFreq;        // Counter frequency (timer resolution)
	__int64       counterStart;  // Timer value
	LARGE_INTEGER li;            // Large interger for timer value
	char		  str[255];      // String for name
	double        elapsed;       // Elapsed time in seconds
	int           retcode;       // Return code
	retcode = QueryPerformanceFrequency(&li);
	if (retcode == 0)
		printf("*** ERROR - QueryPerformanceFrequency() failed \n");
	pcFreq = li.QuadPart / 1000000000.0;
	double st, ft;
	/* Fix qLen = 473, mgLen = 225 */
	long mgLen = 225, qLen = 473;
	ZZ two = conv<ZZ>("2"), f, g, m, r, e, a, q, h, Fg, ans;
	// Fix RCPKC parameters 
	q = power(two, qLen);
	g = power(two, mgLen) - 1;
	f = power(two, qLen - mgLen - 1) - 1;
	r = conv<ZZ>("7412913108366580735897405695821853516604029295037378002172858339760148934");
	h = conv<ZZ>("12194330274671844653834364178879555881830461494785043558043581873536834511135081193454831097582526575739060606284520411066902075628778099310593");
	Fg = conv<ZZ>("36526378940800627493944514404790998968012833465800336156784404945621");
	FILE *fptr = fopen("ResultsFile.csv", "w");
	ifstream in("Message_samples3.csv");//Select Message_samples3.csv, Message_samples4.csv, Message_samples5.csv for 10^3, 10^4, 10^5 plaintext messages respectively  

	while (!in.eof()) {
		in.getline(str, 255);
		m = conv<ZZ>(str);
		QueryPerformanceCounter(&li);
		counterStart = li.QuadPart;
		rem(e, h*r + m, q); //RCPKC encryption
		QueryPerformanceCounter(&li);
		elapsed = ((li.QuadPart - counterStart) / pcFreq);
		fprintf(fptr, "%f%s", elapsed, ",");
		QueryPerformanceCounter(&li);
		counterStart = li.QuadPart;
		rem(a, f*e, q);    //Step 1 of RCPKC Decryption
		rem(ans, Fg*a, g); //Step 2 of RCPKC Decryption
		QueryPerformanceCounter(&li);
		elapsed = ((li.QuadPart - counterStart) / pcFreq);
		fprintf(fptr, "%f\n", elapsed);
	}
	return 0;
}