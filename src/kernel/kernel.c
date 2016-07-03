;文件名: kernel.c
;作者: 0xC000005
;日期: 2016/06/29
;作用: 内核

void main() {
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'X';
}
