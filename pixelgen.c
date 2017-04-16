/* !* MANDELBROT PIXELGENERATOR
 *
 * \description Generates Mandelbrot set and writes it into P3 PPM pictures
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at> <sedi343@gmail.com>
 *
 * \version Rev.: 01, 15.04.2017 - Created - Importing code from mandelbrot task
 *          Rev.: 02, 15.04.2017 - Changed code for my individual program
 *
 * \information Algorithm with information of
 *              http://stackoverflow.com/questions/16124127/improvement-to-my-mandelbrot-set-code
 *
 *
 */

#include "myhead.h"

/* ---- GLOBAL VARIABLES ---- */

PICTURE *picture_pointer = NULL;
FILE* pFout = NULL;

int main(int argc, char *argv[])
{
	struct timeval timer1, timer2, timer3, timer4;
	
	int width = 800;
	int height = 600;
	int iterations = 5000;
	int type = 0;
	
	double double_width;
	double double_height;
	
	int x, y;
	double pr, pi;
	double newRe, oldRe, newIm, oldIm;
	double zoom, moveX, moveY;
	double currentzoom;
	
	int brightnessr;
	int brightnessg;
	int brightnessb;
	
	double z;
	double double_iterations;
	int colorr;
	int colorg;
	int colorb;
	
	char widthString[STRINGLENGTH];
	char heightString[STRINGLENGTH];
	char iterationsString[STRINGLENGTH];
	char typeString[STRINGLENGTH];
	
	char filename[STRINGLENGTH];
	
	int i, k, f = 0;
	int error = 0;
	int errorcount = 0;
	int opt;
	char *pEnd;
	
	double timediff1;
	double timediff2;
	
/*------------------------------------------------------------------*/
/* I N I T                                                          */
/*------------------------------------------------------------------*/
	
	clear();
	
	while ((opt = getopt(argc, argv, "w:h:i:t:?")) != -1)
	{
		switch (opt)
		{
			case 'w':
				error = clearOptarg(widthString, optarg);
				if (error == 1)
					errorcount++;
				error = check_number(widthString);
				if (error == 1)
					errorcount++;
				width = strtod(widthString, &pEnd);
			break;
			
			case 'h':
				error = clearOptarg(heightString, optarg);
				if (error == 1)
					errorcount++;
				error = check_number(heightString);
				if (error == 1)
					errorcount++;
				height = strtod(heightString, &pEnd);
			break;
			
			case 'i':
				error = clearOptarg(iterationsString, optarg);
				if (error == 1)
					errorcount++;
				error = check_number(iterationsString);
				if (error == 1)
					errorcount++;
				iterations = strtod(iterationsString, &pEnd);
			break;
			
			case 't':
				error = clearOptarg(typeString, optarg);
				if (error == 1)
					errorcount++;
				error = check_number(typeString);
				if (error == 1)
					errorcount++;
				type = strtod(typeString, &pEnd);
			break;
			
			case '?':
				clearNoHelp();
				helpdesk_2();
				exit(EXIT_SUCCESS);
			break;
		}
	}
	
	switch (type)
	{
		case 1:
			moveX = -0.75;
			moveY = 0.1;
			zoom = 0.008;
			colorr = 60;
			colorg = -50;
			colorb = -200;
		break;
		
		case 2:
			moveX = -0.7463;
			moveY = 0.1102;
			zoom = 0.005;
			colorr = 60;
			colorg = -50;
			colorb = -120;
		break;
		
		case 3:
			moveX = -0.74529;
			moveY = 0.113075;
			zoom = 0.00015;
			colorr = 80;
			colorg = 50;
			colorb = 20;
		break;
		
		case 4: /* ---- GOOD TEMPLATE ---- */
			moveX = -0.722;
			moveY = 0.246;
			zoom = 0.019;
			colorr = 100;
			colorg = -10;
			colorb = -200;
		break;
		
		case 5: /* ---- CHANGE TEMPLATE / COLOR ALGORITHM ---- */
			moveX = -0.16070135;
			moveY = 1.0375665;
			zoom = 0.00000010;
			colorr = -100;
			colorg = -255;
			colorb = -255;
		break;
		
		case 6:
			moveX = -0.0452407411;
			moveY = 0.9868162204352258;
			zoom = 0.00000000013;
			colorr = -30;
			colorg = -180;
			colorb = -255;
		break;
		
		case 7: /* ---- CHANGE TEMPLATE / COLOR ALGORITHM ---- */
			moveX = 0.281717921930775;
			moveY = 0.5771052841488505;
			zoom = 0.0000000000000192;
			colorr = -80;
			colorg = -140;
			colorb = -140;
		break;
		
		case 8:
			moveX = 0.432539867562512;
			moveY = 0.226118675951765;
			zoom = 0.0000032;
			colorr = 30;
			colorg = -50;
			colorb = -200;
		break;
		
		case 9:
			moveX = -1.99999911758838;
			moveY = 0;
			zoom = 0.00000000000128;
			colorr = 100;
			colorg = -40;
			colorb = -200;
		break;
		
		case 10:
			moveX = -1.296354375872899;
			moveY = 0.44185155566229;
			zoom = 0.0000000000006;
			colorr = 10;
			colorg = -100;
			colorb = -200;
		break;
		
		default:
			moveX = -0.5;
			moveY = 0;
			zoom = 1;
			colorr = 30;
			colorg = -80;
			colorb = 80;
		break;
	}
	
/*------------------------------------------------------------------*/
/* E R R O R   H A N D L I N G                                      */
/*------------------------------------------------------------------*/
	
/* ---- IF ONE PARAMETER INPUT FAILED OR IS NOT CORRECT ---- */
	
	if (errorcount != 0)
	{
		perror(BOLD"\nERROR: One or more Parameters are not correct"RESET);
		exit(EXIT_FAILURE);
	}
	
/* ---- IF TYPE IS NOT BETWEEN 0 AND 10 ---- */
	
	if (type < 0 || type > 10)
	{
		perror(BOLD"\nERROR: Type value must be between 0 and 10"RESET);
		exit(EXIT_FAILURE);
	}
	
/*------------------------------------------------------------------*/
/* P R O G R A M M   S T A R T                                      */
/*------------------------------------------------------------------*/
	
/* ---- ALLOCATE MEMORY FOR PIXELS ---- */
	
	picture_pointer = (PICTURE *)malloc(width * height * sizeof(PICTURE));
	if (picture_pointer == NULL)
	{
		perror(BOLD"\nERROR: malloc: Can't allocate pixel memory"RESET);
		free(picture_pointer);
		exit(EXIT_FAILURE);
	}
	
/* ---- USER OUTPUT ---- */
	
#if DEBUG
	printf(BOLDRED"Width: %d\n"RESET, width);
	printf(BOLDRED"Height: %d\n"RESET, height);
	printf(BOLDRED"Iterations: %d\n"RESET, iterations);
	printf(BOLDRED"Type: %d\n\n"RESET, type);
	printf(BOLDRED"-moveX: %.15f -moveY: %.15f -zoom: %.15f\n"RESET, moveX, moveY, zoom);
#endif
	
	printf(BOLD ITALIC"\n*** GENERATING MANDELBROT ***\n"RESET);
	
/* ---- NEEDED FOR ALGORITHM BUT ARE STATIC ----*/
	
	double_width = width;
	double_height = height;
	currentzoom = zoom;
	
/* ---- CTRL+C HANDLER ---- */
	
	signal(SIGINT, cntrl_c_handler);
	
/* ---- ALGORITHM START ---- */
	
	while(1)
	{
		if (f > 0)
		{
			printf(ITALIC"_________________________________________\n\n"RESET);
			printf(BACKGREEN BLACK"New Image:"RESET"\n\n");
		}
		
		if (currentzoom > 5)
		{
			printf("Zoom is bigger than 1, break up...\n");
			free(picture_pointer);
			exit(EXIT_SUCCESS);
		}
		
/*------------------------------------------------------------------*/
/* P R O G R A M M   S T A R T   (GENERATE)                         */
/*------------------------------------------------------------------*/
		
/* ---- ALGORITHM CODE FOR COLOR (source in description (algorithm changed!)) ---- */
		
		gettimeofday(&timer1, NULL);
		
		printf("* Generating Mandelbrot Pixels...\n");
		
		k = 0;
		
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pr = (double_width/double_height) * (x - width / 2) / (0.5 * (1/currentzoom) * width) + moveX;
				pi = (y - height / 2) / (0.5 * (1/currentzoom) * height) + moveY;
				
				newRe = newIm = oldRe = oldIm = 0;
				
				for (i = 0; i < iterations; i++)
				{
					oldRe = newRe;
					oldIm = newIm;
					
					newRe = oldRe * oldRe - oldIm * oldIm + pr;
					newIm = 2 * oldRe * oldIm + pi;
					
					if ((newRe * newRe + newIm * newIm) > 4)
					{
						break;
					}
				}
				
/* ---- WRITE BLACK PIXELS ---- */
				
				if (i == iterations)
				{
					(picture_pointer+k)->r = 0;
					(picture_pointer+k)->g = 0;
					(picture_pointer+k)->b = 0;
				}
				
/* ---- WRITE COLORED PIXELS ---- */
				
				else
				{
					z = sqrt(newRe * newRe + newIm * newIm);
					double_iterations = iterations;
					brightnessr = 256 * log2(1.75 + i - log2(log2(z))) / log2(double_iterations) + colorr;
					brightnessg = 256 * log2(1.75 + i - log2(log2(z))) / log2(double_iterations) + colorg;
					brightnessb = 256 * log2(1.75 + i - log2(log2(z))) / log2(double_iterations) + colorb;
					
					(picture_pointer+k)->r = brightnessr;
					(picture_pointer+k)->g = brightnessg;
					(picture_pointer+k)->b = brightnessb;
					
/* ---- COLORS ARE LOWER THAN 0 ---- */
					
					if ((picture_pointer+k)->r < 0)
						(picture_pointer+k)->r = 0;
					
					if ((picture_pointer+k)->g < 0)
						(picture_pointer+k)->g = 0;
					
					if ((picture_pointer+k)->b < 0)
						(picture_pointer+k)->b = 0;
					
					
/* ---- COLORS ARE HIGHER THAN 255 ---- */
					
					if ((picture_pointer+k)->r > 255)
						(picture_pointer+k)->r = 255;
					
					if ((picture_pointer+k)->g > 255)
						(picture_pointer+k)->g = 255;
					
					if ((picture_pointer+k)->b > 255)
						(picture_pointer+k)->b = 255;
					
				}
				
				k++;
			}
		}
		
		gettimeofday(&timer2, NULL);
		
		printf("* Done generating Pixels!\n\n");
		printf("Generated Pixels for "BOLD ITALIC"\"picture-%.03d.ppm\"\n\n"RESET, f);
		
		printf(ITALIC"Values:\n"RESET);
		printf("-Zoom: %.07f -OffsetX: %f -OffsetY: %f\n\n", currentzoom, moveX, moveY);
		
		timediff1 = (timer2.tv_sec+timer2.tv_usec*0.000001)-(timer1.tv_sec+timer1.tv_usec*0.000001);
		
/*------------------------------------------------------------------*/
/* P R O G R A M M   S T A R T   (WRITE)                            */
/*------------------------------------------------------------------*/
		
		gettimeofday(&timer3, NULL);
		
		printf("* Writing file...\n");
		
		sprintf(filename, "picture-%.03d.ppm", f);
		if (f > 999)
		{
			perror(BOLD"\nERROR: reached maximum pictures value of 1000"RESET);
			free(picture_pointer);
			exit(EXIT_FAILURE);
		}
		
		pFout = fopen(filename, "wb");
		if (pFout == NULL)
		{
			perror(BOLD"\nERROR: fopen: Couldn't open output file"RESET);
			free(picture_pointer);
			exit(EXIT_FAILURE);
		}
		
/* ---- WRITE OUTPUT FILE ---- */
		
		fprintf(pFout, "P3\n");
		fprintf(pFout, "#Mandelbrot Generator by Sebastian Dichler\n");
		fprintf(pFout, "#Type: %d\n", type);
		fprintf(pFout, "# -moveX: %.15f -moveY: %.15f -currentzoom: %.15f\n", moveX, moveY, currentzoom);
		fprintf(pFout, "%u %u\n", width, height);
		fprintf(pFout, "255\n");
		
		for (i = 0; i < height*width; i++)
		{
			fprintf(pFout, "%u %u %u\n", (picture_pointer+i)->r, (picture_pointer+i)->g, (picture_pointer+i)->b);
		}
		
		error = fclose(pFout);
		if (error == EOF)
		{
			perror(BOLD"\nERROR: fclose: Can't close output file"RESET);
			free(picture_pointer);
			exit(EXIT_FAILURE);
		}
		
		gettimeofday(&timer4, NULL);
		
		printf("* Done writing file!\n\n");
		
		timediff2 = (timer4.tv_sec+timer4.tv_usec*0.000001)-(timer3.tv_sec+timer3.tv_usec*0.000001);
		
		printf("\n"BLACK BACKYELLOW"Generated pixels within "BOLDBLACK BACKYELLOW"%f"BLACK BACKYELLOW" secs"RESET"\n", timediff1);
		printf(BLACK BACKYELLOW"Generate Speed: "BOLDBLACK BACKYELLOW"%.2fMB/s"RESET"\n\n", (((sizeof(PICTURE)*height*width)/1000000)/timediff1));
		
		printf("\n"BLACK BACKYELLOW"Wrote file within "BOLDBLACK BACKYELLOW"%f"BLACK BACKYELLOW" secs"RESET"\n", timediff2);
		printf(BLACK BACKYELLOW"Write Speed: "BOLDBLACK BACKYELLOW"%.2fMB/s"RESET"\n\n", (((sizeof(PICTURE)*height*width)/1000000)/timediff2));
		
		currentzoom = getnewzoom(currentzoom);
		f++;
	}
	
	free(picture_pointer);
	exit(EXIT_SUCCESS);	
}

/*------------------------------------------------------------------*/
/* F U N C T I O N S                                                */
/*------------------------------------------------------------------*/

void cntrl_c_handler(int dummy)
{
	printf(BOLD"You just typed CNTRL-C\nClosing everything...\n"RESET);
	if (pFout != NULL)
		fclose(pFout);
	
	free(picture_pointer);
	exit(EXIT_SUCCESS);
}
