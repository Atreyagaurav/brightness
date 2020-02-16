#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> 

int read_brightness(char fname[]);
void write_brightness(char fname[],int brightness);

int main(int argc, char **argv){
  int opt;
  char filename[55],max_filename[55];
  strcpy(filename,"/sys/class/backlight/intel_backlight/brightness");
  strcpy(max_filename,"/sys/class/backlight/intel_backlight/max_brightness");
  /* strcpy(filename,"test/brightness"); */
  /* strcpy(max_filename,"test/max_brightness"); */
  int brightness_p,brightness,max_brightness;
  brightness = read_brightness(filename);
  max_brightness = read_brightness(max_filename);
  brightness_p = brightness *100 / max_brightness;
  
  while ((opt= getopt(argc,argv,":u:d:p:h")) != -1){
    switch(opt){
    case 'p':
      brightness_p = atoi(optarg);
      break;
    case 'u':
      brightness_p += atoi(optarg);
      break;
    case 'd':
      brightness_p -= atoi(optarg);
      break;
    case 'h':
      printf("Brightness Controller -by atreyagaurav\nOptions:\n");
      printf("\t-p val\t percentage brightness, val=(0-100)\n");
      printf("\t-u val\t increse by percentage, val=(0-100)\n");
      printf("\t-d val\t decrease by percentage, val=(0-100)\n");
    }
  }

  if (brightness_p < 10) {
    brightness_p = 10;
    }
  else if (brightness_p > 100) {
    brightness_p = 100;
    }
  brightness = brightness_p * max_brightness /100;
  write_brightness(filename,brightness);
  return 0;
}


int read_brightness(char filename[]){
  int b;
  FILE * fp;
  fp = fopen(filename,"r");
  fscanf(fp,"%d",&b);
  fclose(fp);
  return b;
}

void write_brightness(char filename[],int brightness){
  FILE * fp;
  fp = fopen(filename,"w");
  fprintf(fp,"%d",brightness);
  fclose(fp);
}
