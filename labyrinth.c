#include "labyrinth.h"
int main()
{
    location loc;
    location *loca = &loc;
    //printf("���������������");
    menu(loca);
    getchar();
    system("cause");
    FILE *fp;
    if ((fp = fopen("record.txt", "a")) == NULL)
        printf("��Ϸ��¼����ʧ�ܣ�");
    else
    {
        chain *p, *q;
        p = head;
        while (p != NULL)
        {
            q = p;
            fprintf(fp, "\n%s", p->record);
            p = p->next;
            free(q);
        }
        fclose(fp);
    }
    return 0;
}
/*for (int i = 0; i < length_C;i++)
        maze[i] = (int *)malloc(length_C*sizeof(int));
    for (i = 0; i < length_C; i++)
    {
        for (j = 0; j < length_C; j++)
   { //        maze[i][j] = 0;
            printf("%d", maze[i][j]);}
        printf("\n");
    }*/
