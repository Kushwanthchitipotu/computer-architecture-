#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//function to convert hexadecimal to integer
int hexCharToInt(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'F')
    {
        return 10 + (c - 'A');
    }
    else if (c >= 'a' && c <= 'f')
    {
        return 10 + (c - 'a');
    }
    return -1;
}

//function to convert hexadecimal string to binary array
int *BinaryConverter(const char *string, int *array)
{
    int i = 0;
    int j = 0;

    while (string[i] != '\0' && j < 32)
    {
        int hexValue = hexCharToInt(string[i]);
        if (hexValue == -1)
        {
            return NULL;
        }

        for (int k = 3; k >= 0; k--)
        {
            array[j++] = (hexValue >> k) & 1;
        }

        i++;
    }

    return array;
}

//function to convert binary array to decimal number
int decimal_converter(int *array, int length)
{
    int decimal = 0;
    for (int i = 0; i < length; i++)
    {
        if (array[i] == 1)
        {
            decimal += pow(2, length - i - 1);
        }
    }

    return decimal;
}

//function to convert decimal to hexadecimal 
char *DecimalToHexadecimal(int decimal)
{
    // Determine the number of digits in the hexadecimal representation
    int temp = decimal;
    int numDigits = 0;
    while (temp != 0)
    {
        temp /= 16;
        numDigits++;
    }

    // Allocate memory for the hexadecimal string
    char *hexadecimal = (char *)malloc((numDigits + 1) * sizeof(char));

    // Convert decimal to hexadecimal
    for (int i = numDigits - 1; i >= 0; i--)
    {
        int remainder = decimal % 16;
        decimal /= 16;

        // Convert remainder to hexadecimal character
        if (remainder < 10)
        {
            hexadecimal[i] = remainder + '0';
        }
        else
        {
            hexadecimal[i] = remainder - 10 + 'A';
        }
    }

    // Null-terminate the string
    hexadecimal[numDigits] = '\0';

    return hexadecimal;
}

int main()
{
    //reads the files to get data's from them
    FILE *config_file, *access_file;
    config_file = fopen("cache.config", "r");
    access_file = fopen("cache.access", "r");

    unsigned int cache_size, block_size, associativity;
    char replacement_policy[10], writeback_policy[3];

    fscanf(config_file, "%u %u %u %s %s", &cache_size, &block_size, &associativity, replacement_policy, writeback_policy);
    fclose(config_file);

    if (access_file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    int line_length = 12;
    int num_lines = 0;
    char ch;

    while (fscanf(access_file, "%c", &ch) != EOF)
    {
        if (ch == '\n')
        {
            num_lines++;
        }
    }

    fseek(access_file, 0, SEEK_SET);

    char matrix[num_lines][line_length];//contains whole cache access data in it
    char matrix2[num_lines][line_length - 4];//modified version of matrix
    int BinaryMatrix[num_lines][(line_length - 4) * 4];//binary version of matrix2
    int bin_lenght = (line_length - 4) * 4;
    for (int i = 0; i < num_lines; i++)
    {
        for (int j = 0; j < line_length; j++)
        {
            fscanf(access_file, " %c", &matrix[i][j]);
        }
    }

    for (int i = 0; i < num_lines; i++)
    {
        for (int j = 0; j < line_length - 4; j++)
        {
            matrix2[i][j] = matrix[i][j + 4];
        }
    }
    unsigned int num_sets;
    //finding number of sets i.e number of indexable set of blocks
    if (associativity >= 1)
    {
        num_sets = cache_size / (block_size * associativity);
    }
    if (associativity == 0)
    {
        num_sets = cache_size / (block_size);
    }
    int n = 0;//number of index bits
    int i = num_sets;

    while (i != 0)
    {
        i = i / 2;
        n++;
    }
    int index[num_lines];//contains data related to index of command on a particular line
    int tag[num_lines];//contains tag data of a particular data
    int hit[num_lines];//contains wherether a line is hit or miss
    i = 0;
    for (int i = 0; i < num_lines; i++)
    {
        hit[i] = -1;
    }
    while (i < num_lines)
    {
        BinaryConverter(matrix2[i], BinaryMatrix[i]);
        i++;
    }

    for (int i = 0; i < num_lines; i++)
    {
        int k = 0;
        for (int j = 0; j < n; j++)
        {
            k = k + BinaryMatrix[i][bin_lenght - 1 - j] * pow(2, j);
        }
        index[i] = k;
    }

    for (int i = 0; i < num_lines; i++)
    {
        int k = 0;
        for (int j = 0; j < bin_lenght - n; j++)
        {
            k = k + BinaryMatrix[i][bin_lenght - 1 - j-n] * pow(2, j);
        }
        tag[i] = k;
    }
    //fully associative case
    if (associativity == 0)
    {
        int cache[num_sets];
        int cache2[num_sets];
        int k = -1;
        for (int j = 0; j < num_sets; j++)
        {
            cache2[j] = -1;
        }
        for (int i = 0; i < num_lines; i++)
        {
            //read conditions
            if (matrix[i][0] == 'R')
            {
                if (strcmp(replacement_policy, "FIFO") == 0)
                {
                    for (int j = 0; j < num_sets; j++)
                    {
                        if (cache[j] == tag[i])
                        {
                            hit[i] = 1;
                            k = 0;
                            break;
                        }
                        if (cache2[j] == -1)
                        {
                            cache[j] = tag[i];
                            hit[i] = 0;
                            cache2[j] = i;
                            k = 0;
                            break;
                        }
                    }

                    if (k == -1)
                    {
                        hit[i] = 0;
                        int min = cache2[0];
                        int p = -1;
                        for (int j = 1; j < num_sets; j++)
                        {
                            if (min > cache2[j])
                            {
                                min = cache2[j];
                                p = j;
                            }
                        }
                        cache[p] = tag[i];
                    }
                }
                if (strcmp(replacement_policy, "LRU") == 0)
                {
                    for (int j = 0; j < num_sets; j++)
                    {
                        if (cache[j] == tag[i])
                        {
                            hit[i] = 1;
                            cache2[j] = i;
                            k = 0;
                            break;
                        }
                        if (cache2[j] == -1)
                        {
                            cache[j] = tag[i];
                            hit[i] = 0;
                            cache2[j] = i;
                            k = 0;
                            break;
                        }
                    }

                    if (k == -1)
                    {
                        hit[i] = 0;
                        int min = cache2[0];
                        int p = -1;
                        for (int j = 1; j < num_sets; j++)
                        {
                            if (min > cache2[j])
                            {
                                min = cache2[j];
                                p = j;
                            }
                        }
                        cache[p] = tag[i];
                    }
                }
                if (strcmp(replacement_policy, "RANDOM") == 0)
                {
                    for (int j = 0; j < num_sets; j++)
                    {
                        if (cache[j] == tag[i])
                        {
                            hit[i] = 1;
                            k = 0;
                            break;
                        }
                        if (cache2[j] == -1)
                        {
                            cache[j] = tag[i];
                            hit[i] = 0;
                            k = 0;
                            break;
                        }
                    }

                    if (k == -1)
                    {
                        hit[i] = 0;

                        srand(23);
                        int p = rand() % num_sets;
                        cache[p] = tag[i];
                    }
                }
            }
            // if it is write type
            if (matrix[i][0] == 'W')
            {
                if (strcmp(writeback_policy, "WT") == 0)
                {
                    int k = -1;
                    int p = -1;
                    for (int j = 0; j < num_sets; j++)
                    {
                        if (cache[j] == tag[i])
                        {
                            hit[i] = 1;
                            cache[j] = -1;
                            k = 1;
                            break;
                        }
                    }
                    if (k == -1)
                    {
                        hit[i] = 0;
                    }
                }
                if (strcmp(writeback_policy, "WB") == 0)
                {
                    int k = -1;
                    int p = -1;
                    for (int j = 0; j < num_sets; j++)
                    {
                        if (cache[j] == tag[i])
                        {
                            hit[i] = 1;
                            k = 1;
                            break;
                        }
                    }
                    if (k == -1)
                    {
                        hit[i] = 0;
                    }
                }
            }
        }
        printf("OUTPUT:\n");
        for (int i = 0; i < num_lines; i++)
        {
            for (int j = 0; j < line_length; j++)
            {
                printf("%c", matrix[i][j]);
            }

            char *hexadecimalString = DecimalToHexadecimal(index[i]);
            printf("  set:0x%s   ", hexadecimalString);
            if (hit[i] == 1)
            {
                printf("HIT ");
            }
            else
            {
                printf("MISS ");
            }
            hexadecimalString=DecimalToHexadecimal(tag[i]);
            printf("  tag:0x%s   ", hexadecimalString);
            printf("\n");
        }
    }
// all other cases of associativity
    else
    {
        int cache[num_sets][associativity];//cache model
        int cache_rep[num_sets][associativity];//contains data about the tag stored in cache data

        for (int i = 0; i < num_sets; i++)
        {
            for (int j = 0; j < associativity; j++)
            {
                cache_rep[i][j] = -1;
            }
        }

        for (int i = 0; i < num_lines; i++)
        {
            if (matrix[i][0] == 'R')
            {  


                //all  cases including direct mapped
                if (associativity >= 1)
                {
                    if (strcmp(replacement_policy, "FIFO") == 0)
                    {

                        int k = -1;
                        int p = -1;
                        for (int j = 0; j < associativity; j++)
                        {
                            if (cache[index[i]][j] == tag[i])
                            {
                                hit[i] = 1;
                                k = 1;
                                break;
                            }
                        }
                        if (k == -1)
                        {
                            hit[i] = 0;
                            for (int j = 0; j < associativity; j++)
                            {
                                if (cache_rep[index[i]][j] == -1)
                                {
                                    cache[index[i]][j] = tag[i];
                                    cache_rep[index[i]][j] = i;
                                    p = -2;
                                    break;
                                }
                            }
                            if (p == -1)
                            {
                                int min = cache_rep[index[i]][0];
                                p = 0;
                                for (int j = 1; j < associativity; j++)
                                {
                                    if (min > cache_rep[index[i]][j])
                                    {
                                        min = cache_rep[index[i]][j];
                                        p = j;
                                    }
                                }
                                cache[index[i]][p] = tag[i];
                                cache_rep[index[i]][p] = i;
                            }
                        }
                    }
                    if (strcmp(replacement_policy, "LRU") == 0)
                    {

                        int k = -1;
                        int p = -1;
                        for (int j = 0; j < associativity; j++)
                        {
                            if (cache[index[i]][j] == tag[i])
                            {
                                hit[i] = 1;
                                cache_rep[index[i]][j] = i;
                                k = 1;
                                break;
                            }
                        }
                        if (k == -1)
                        {
                            hit[i] = 0;
                            for (int j = 0; j < associativity; j++)
                            {
                                if (cache_rep[index[i]][j] == -1)
                                {
                                    cache[index[i]][j] = tag[i];
                                    cache_rep[index[i]][j] = i;
                                    p = -2;
                                    break;
                                }
                            }
                            if (p == -1)
                            {
                                int min = cache_rep[index[i]][0];
                                p = 0;
                                for (int j = 1; j < associativity; j++)
                                {
                                    if (min > cache_rep[index[i]][j])
                                    {
                                        min = cache_rep[index[i]][j];
                                        p = j;
                                    }
                                }
                                cache[index[i]][p] = tag[i];
                                cache_rep[index[i]][p] = i;
                            }
                        }
                    }
                    if (strcmp(replacement_policy, "RANDOM") == 0)
                    {

                        int k = -1;
                        int p = -1;
                        for (int j = 0; j < associativity; j++)
                        {
                            if (cache[index[i]][j] == tag[i])
                            {
                                hit[i] = 1;
                                k = 1;
                                break;
                            }
                        }
                        if (k == -1)
                        {
                            hit[i] = 0;
                            srand(23);
                            int k = rand() % associativity;
                            cache[index[i]][k] = tag[i];
                        }
                    }
                }
            }
            if (matrix[i][0] == 'W')
            {
                if (strcmp(writeback_policy, "WT") == 0)
                {
                    int k = -1;
                    int p = -1;
                    for (int j = 0; j < associativity; j++)
                    {
                        if (cache[index[i]][j] == tag[i])
                        {
                            hit[i] = 1;
                            cache[index[i]][j] = -1;
                            k = 1;
                            break;
                        }
                    }
                    if (k == -1)
                    {
                        hit[i] = 0;
                    }
                }
                if (strcmp(writeback_policy, "WB") == 0)
                {
                    int k = -1;
                    int p = -1;
                    for (int j = 0; j < associativity; j++)
                    {
                        if (cache[index[i]][j] == tag[i])
                        {
                            hit[i] = 1;
                            k = 1;
                            break;

                        }
                    }
                    if (k == -1)
                    {
                        hit[i] = 0;
                    }
                }
            }
        }

        

        // Print the matrix
        printf("OUTPUT:\n");
        for (int i = 0; i < num_lines; i++)
        {
            for (int j = 0; j < line_length; j++)
            {
                printf("%c", matrix[i][j]);
            }

            char *hexadecimalString = DecimalToHexadecimal(index[i]);
            printf("  set:0x%s   ", hexadecimalString);
            if (hit[i] == 1)
            {
                printf("HIT ");
            }
            else
            {
                printf("MISS ");
            }
            hexadecimalString=DecimalToHexadecimal(tag[i]);
            printf("  tag:0x%s   ", hexadecimalString);
            printf("\n");
        }
    }
    fclose(access_file);

    return 0;
}
