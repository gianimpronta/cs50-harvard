#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
        // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile \n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[1];

    // open input file 
	FILE* inputr = fopen(infile, "r");	
    if (inputr == NULL)
    {
	    fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }


	// tracking jpegs found
	int jpgcount = 0;

	// Open outfile indicator
	int file_open = 0;
	FILE* outptr;

  // Read 512b blocks from file.
	unsigned char buffer[512];
	fread(buffer, 512, 1, inputr);	

	while(fread(buffer, 512, 1, inputr) > 0)
	{
		// Check for a jpeg signature
		if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && 
		(buffer[3] & 0xf0) == 0xe0)
		{
			if (file_open == 1)
            {
                // We found the start of a new pic so close out current picture
                fclose(outptr);
            }
            else
            {
                // jpg discovered and now we have the green light to write
                file_open = 1;
			}
			// Construct the filename
			char filename[8];
			sprintf(filename, "%03d.jpg", jpgcount);
            outptr = fopen(filename, "a");
			jpgcount++;
		}
		if(file_open == 1)
		{
				fwrite(&buffer, sizeof(buffer), 1, outptr);
		}
	}

	// close filehandles.
	if(outptr)
    {
      fclose(outptr);
    }

	fclose(inputr);
	return 0;
}