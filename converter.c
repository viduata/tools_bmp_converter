#include <stdio.h>
#include <stdlib.h>

#define filesize 1024   /// 128*64/8

int main (int argc, char * arg[])
{
	int           counter;
	FILE        * infile;
	FILE        * outfile;
	int           data_offset;
	char          holder[20];
	unsigned char img_data[filesize];

	// Make sure that the user entered two data points
	if(argc!=3)
	{
		printf("Usage: %s: <source bmp> <target .h file>\n", arg[0]);
		exit(1);
	}

	// Open the file
	infile = fopen(arg[1], "rb");

	// Find the offset for the data portion
	// which is always located at the 10th byte
	// in a bmp file
	fseek(infile, 10, SEEK_SET);

	// Read the offset
	fread(&data_offset, 4, 1, infile );
	printf("data_offset is %02X\n", data_offset);

	// Skip to the data portion
	fseek(infile, data_offset, SEEK_SET);

	// Read the file into the buffer, which is
	// always going to be the same size in a 128x64 OLED
	fread(img_data, 1, filesize,  infile);

	// Clean up after yourself
	fclose(infile);


	// Convert the data to a flatfile

	// First, open the flatfile
	outfile = fopen(arg[2], "w");

	// Now write the header
	sprintf(holder, "img_data = { ");
	fwrite("char img_data[] = { ", 1, 20, outfile);

	// Iterate through all the data points
	for(counter=0; counter<filesize-1; counter++)
	{
		sprintf(holder, "0x%02X, ", img_data[counter]);
		fwrite(holder, 1, 6, outfile);
		if(counter%8==7){fwrite("\n", 1, 1, outfile);}
	}

	// Finish off the array you've built.
	sprintf(holder, "0x%02X};\n", img_data[counter]);
	fwrite(holder, 1, 7, outfile);

	// Close the filehandle like a decent person
	fclose(outfile);
}