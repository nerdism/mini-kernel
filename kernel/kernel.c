
#define VIDEO_TEXT 0xb8000
#define ATTR 0b00001100

void main() {
    /* color graphic video memory adress */
    char *video_memory = (char*)VIDEO_TEXT;
    char *str = "A Big Hello from Mohsen noei aval";
    char *t = video_memory;

    int i = 0, j = 0;
    while (str[i] != '\0') {
	t[j] = str[i]; 
	t[j+1] = ATTR;
	j += 2;
	i++;
    }
}


