
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 9

int readMachineCodeFromFile(const char *filename, char ***matrix, int *numInstructions)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }

    // Count the number of lines (instructions) in the file
    int count = 0;
    char line[9]; // Assuming each line contains a 32-bit instruction in hexadecimal (8 characters + null terminator)
    while (fgets(line, sizeof(line), file) != NULL)
    {
        count++;
    }

    // Allocate memory for the matrix
    *matrix = (char **)malloc(count * sizeof(char *));
    if (*matrix == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 0;
    }

    // Reset file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Read instructions and store them in the matrix
    for (int i = 0; i < count; i++)
    {
        (*matrix)[i] = (char *)malloc(9); // 8 characters + null terminator
        if ((*matrix)[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            fclose(file);
            return 0;
        }

        if (fgets((*matrix)[i], 9, file) == NULL)
        {
            printf("Error reading instruction from the file.\n");
            fclose(file);
            return 0;
        }
    }

    *numInstructions = count;
    fclose(file);
    return 1;
}

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
    return -1; // Invalid hexadecimal character
}

int *BinaryConverter(const char *string, int *array)
{
    int i = 0;
    int j = 0;

    while (string[i] != '\0' && j < 32)
    {
        int hexValue = hexCharToInt(string[i]);
        if (hexValue == -1)
        {
            // Handle invalid characters here, like 'G' or anything outside of 0-9 and A-F.
            return NULL;
        }

        // Convert the hex digit to 4 binary digits
        for (int k = 3; k >= 0; k--)
        {
            array[j++] = (hexValue >> k) & 1;
        }

        i++;
    }

    return array;
}

int TypeVerifier(int *array)
{
    if (array[25] == 0)
    {
        if (array[26] == 0)
        {
            if (array[27] == 0)
            {
                return 3;
            }
            if (array[27] == 1)
            {
                if (array[29] == 0)
                {
                    return 2;
                }
                if (array[29] == 1)
                {
                    return 8;
                }
            }
        }
        if (array[26] == 1)
        {
            if (array[27] == 0)
            {
                return 4;
            }
            if (array[27] == 1)
            {
                if (array[29] == 0)
                {
                    return 1;
                }
                if (array[29] == 1)
                {
                    return 7;
                }
            }
        }
    }

    if (array[25] == 1)
    {
        if (array[27] == 1)
        {
            return 9;
        }
        if (array[27] == 0)
        {
            if (array[28] == 1)
            {
                return 6;
            }
            if (array[28] == 0)
            {
                if (array[29] == 1)
                {
                    return 10;
                }
                if (array[29] == 0)
                {
                    return 5;
                }
            }
        }
    }
    
}

int decimal_converter(int p, int *array, int length)
{
    int decimal = 0;
    if (p == 2)
    {
        if (array[0] == 0)
        {
            p = 1;
        }
        else
        {
            for (int i = 0; i < length; i++)
            {
                if (array[i] == 0)
                {
                    array[i] = 1;
                }
                else
                {
                    array[i] = 0;
                }
            }
            for (int i = 0; i < length; i++)
            {
                if (array[i] == 1)
                {
                    decimal += pow(2, length - i - 1);
                }
            }
            decimal++;
            decimal = -decimal;
        }
    }
    if (p == 1)
    {
        for (int i = 0; i < length; i++)
        {
            if (array[i] == 1)
            {
                decimal += pow(2, length - i - 1);
            }
        }
    }

    return decimal;
}

char *R_type(char *string, int *array, int lenght)
{
    int a = 0;
    int b = 0;
    int d = 0;
    int temp_array[5];
    if (array[1] == 1)
    {
        if (array[17] == 1)
        {
            // sub
            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[12 + i];
            }
            a = decimal_converter(1, temp_array, 5);

            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[7 + i];
            }
            b = decimal_converter(1, temp_array, 5);
            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[20 + i];
            }
            d = decimal_converter(1, temp_array, 5);

            sprintf(string, "sra x%d, x%d, x%d", d, a, b);
        }
        if (array[17] == 0)
        {
            // sra

            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[12 + i];
            }
            a = decimal_converter(1, temp_array, 5);

            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[7 + i];
            }
            b = decimal_converter(1, temp_array, 5);
            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[20 + i];
            }
            d = decimal_converter(1, temp_array, 5);

            sprintf(string, "sub x%d, x%d, x%d", d, a, b);
        }
    }
    if (array[1] == 0)
    {
        if (array[17] == 1)
        {
            if (array[18] == 1)
            {
                if (array[19] == 1)
                {
                    // and
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "and x%d, x%d, x%d", d, a, b);
                }
                if (array[19] == 0)
                {
                    // or
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "or  x%d, x%d, x%d", d, a, b);
                }
            }
            if (array[18] == 0)
            {
                if (array[19] == 1)
                {
                    // srl
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "srl x%d, x%d, x%d", d, a, b);
                }
                if (array[19] == 0)
                {
                    // xor
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "xor x%d, x%d, x%d", d, a, b);
                }
            }
        }
        if (array[17] == 0)
        {
            if (array[18] == 1)
            {
                if (array[19] == 1)
                {
                    // sltu
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "sltu x%d, x%d, x%d", d, a, b);
                }
                if (array[19] == 0)
                {
                    // slt
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "slt x%d, x%d, x%d", d, a, b);
                }
            }
            if (array[18] == 0)
            {
                if (array[19] == 1)
                {
                    // sll
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "sll x%d, x%d, x%d", d, a, b);
                }
                if (array[19] == 0)
                {
                    // add
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[7 + i];
                    }
                    b = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);

                    sprintf(string, "add x%d, x%d, x%d", d, a, b);
                }
            }
        }
    }
    return string;
}

char *Ia_type(char *string, int *array, int lenght)
{
    int a = 0;
    int b = 0;
    int d = 0;
    int temp_array[12];
    if (array[17] == 0)
    {
        if (array[18] == 0)
        {
            if (array[19] == 0)
            {
                // addi
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "addi x%d, x%d, %d", d, a, b);
            }
            if (array[19] == 1)
            {
                // slli
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "slli x%d, x%d, %d", d, a, b);
            }
        }
        if (array[18] == 1)
        {
            if (array[19] == 0)
            {
                // slti
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "slti x%d, x%d, %d", d, a, b);
            }
            if (array[19] == 1)
            {
                // sltiu

                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "sltiu x%d, x%d, %d", d, a, b);
            }
        }
    }
    if (array[17] == 1)
    {
        if (array[18] == 0)
        {
            if (array[19] == 0)
            {
                // xori
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "xori x%d, x%d, %d", d, a, b);
            }
            if (array[19] == 1)
            {
                int alpha_array[6];

                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                if (temp_array[4] == 1)
                {
                    // srai
                    for (int i = 0; i < 5; i++)
                    {
                        alpha_array[i] = array[i];
                    }
                    b = decimal_converter(1, alpha_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    sprintf(string, "srai x%d, x%d, %d", d, a, b);
                }
                if (temp_array[4] == 0)
                {
                    // srli

                    for (int i = 0; i < 5; i++)
                    {
                        alpha_array[i] = array[i];
                    }
                    b = decimal_converter(1, alpha_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[20 + i];
                    }
                    d = decimal_converter(1, temp_array, 5);
                    for (int i = 0; i < 5; i++)
                    {
                        temp_array[i] = array[12 + i];
                    }
                    a = decimal_converter(1, temp_array, 5);

                    sprintf(string, "srli x%d, x%d, %d", d, a, b);
                }
            }
        }
        if (array[18] == 1)
        {
            if (array[19] == 0)
            {
                // ori
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "ori  x%d, x%d, %d", d, a, b);
            }
            if (array[19] == 1)
            {
                // andi
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "andi x%d, x%d, %d", d, a, b);
            }
        }
    }
}

char *Ib_type(char *string, int *array, int length)
{
    int a = 0;
    int b = 0;
    int d = 0;
    int temp_array[12];

    // Ensure that the string is initialized
    string[0] = '\0';

    if (array[17] == 0)
    {
        if (array[18] == 0)
        {
            if (array[19] == 0)
            {
                // lb
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "lb x%d, %d(x%d)", d, b, a);
            }
            if (array[19] == 1)
            {
                // lh
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "lh x%d, %d(x%d)", d, b, a);
            }
        }
        if (array[18] == 1)
        {
            if (array[19] == 0)
            {
                // lw
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "lw x%d, %d(x%d)", d, b, a);
            }
            if (array[19] == 1)
            {
                // ld
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "ld x%d, %d(x%d)", d, b, a);
            }
        }
    }
    if (array[17] == 1)
    {
        if (array[18] == 0)
        {
            if (array[19] == 0)
            {
                // lbu
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "lbu x%d, %d(x%d)", d, b, a);
            }
            if (array[19] == 1)
            {
                // lhu
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "lhu x%d, %d(x%d)", d, b, a);
            }
        }
        if (array[18] == 1)
        {
            if (array[19] == 0)
            {
                // lwu
                for (int i = 0; i < 12; i++)
                {
                    temp_array[i] = array[i];
                }
                b = decimal_converter(2, temp_array, 12);
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[20 + i];
                }
                d = decimal_converter(1, temp_array, 5);

                sprintf(string, "lwu x%d, %d(x%d)", d, b, a);
            }
        }
    }

    return string;
}

char *Ic_type(char *string, int *array, int length)
{
    int a = 0;
    int b = 0;
    int d = 0;
    int temp_array[12];

    // Ensure that the string is initialized
    string[0] = '\0';

    // Check if the instruction is JALR (Ic-type)
    if (array[17] == 0 && array[18] == 0 && array[19] == 0)
    {
        // Extract values from the bit array
        for (int i = 0; i < 12; i++)
        {
            temp_array[i] = array[i];
        }
        b = decimal_converter(1, temp_array, 12);
        for (int i = 0; i < 5; i++)
        {
            temp_array[i] = array[12 + i];
        }
        a = decimal_converter(1, temp_array, 5);

        for (int i = 0; i < 5; i++)
        {
            temp_array[i] = array[20 + i];
        }
        d = decimal_converter(1, temp_array, 5);

        // Create the JALR instruction string
        sprintf(string, "jalr x%d, x%d,%d", d, a, b);
    }

    return string;
}

char *S_type(char *string, int *array, int length)
{
    int a = 0;
    int b = 0;
    int d = 0;
    int temp_array[12];
    int i = 0;
    if (array[18] == 0)
    {
        if (array[19] == 0)
        {
            // sb
            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[12 + i];
            }
            a = decimal_converter(1, temp_array, 5);

            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[7 + i];
            }
            b = decimal_converter(1, temp_array, 5);
            for (int i = 0; i < 7; i++)
            {
                temp_array[i] = array[i];
            }
            for (int i = 0; i < 5; i++)
            {
                temp_array[i + 7] = array[20 + i];
            }
            d = decimal_converter(2, temp_array, 12);
            sprintf(string, "sb x%d, %d(x%d)", b, d, a);
        }
        if (array[19] == 1)
        {
            // sh
            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[12 + i];
            }
            a = decimal_converter(1, temp_array, 5);

            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[7 + i];
            }
            b = decimal_converter(1, temp_array, 5);
            for (i = 0; i < 7; i++)
            {
                temp_array[i] = array[i];
            }
            for (int i = 0; i < 5; i++)
            {
                temp_array[i + 7] = array[20 + i];
            }
            d = decimal_converter(2, temp_array, 12);
            sprintf(string, "sh x%d, %d(x%d)", b, d, a);
        }
    }
    if (array[18] == 1)
    {
        if (array[19] == 0)
        {
            // sw
            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[12 + i];
            }
            a = decimal_converter(1, temp_array, 5);

            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[7 + i];
            }
            b = decimal_converter(1, temp_array, 5);
            for (i = 0; i < 7; i++)
            {
                temp_array[i] = array[i];
            }
            for (int i = 0; i < 5; i++)
            {
                temp_array[i + 7] = array[20 + i];
            }
            d = decimal_converter(2, temp_array, 12);
            sprintf(string, "sw x%d, %d(x%d)", b, d, a);
        }
        if (array[19] == 1)
        {
            // sd
            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[12 + i];
            }
            a = decimal_converter(1, temp_array, 5);

            for (int i = 0; i < 5; i++)
            {
                temp_array[i] = array[7 + i];
            }
            b = decimal_converter(1, temp_array, 5);
            for (i = 0; i < 7; i++)
            {
                temp_array[i] = array[i];
            }
            for (int i = 0; i < 5; i++)
            {
                temp_array[i + 7] = array[20 + i];
            }
            d = decimal_converter(2, temp_array, 12);
            sprintf(string, "sd x%d, %d(x%d)", b, d, a);
        }
    }
    return string;
}

char *Ua_type(char *string, int *array, int length)
{
    int d = 0;
    int a = 0;
    int temp_array[20];
    for (int i = 0; i < 5; i++)
    {
        temp_array[i] = array[20 + i];
    }
    d = decimal_converter(1, temp_array, 5);

    for (int i = 0; i < 20; i++)
    {
        temp_array[i] = array[i];
    }
    a = decimal_converter(1, temp_array, 20);

    sprintf(string, "lui x%d, 0x%05X", d, a & 0xFFFFF);

    return string;
}

char *B_type(char *string, int *array, int length,int*b_ptr)
{
    int a = 0;
    int b = 0;
    int d = 0;
    int temp_array[13];
    if (array[17] == 0)
    {
        if (array[18] == 0)
        {
            if (array[19] == 0)
            {
                // beq
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[7 + i];
                }
                b = decimal_converter(1, temp_array, 5);
                temp_array[0] = array[0];
                temp_array[1] = array[24];
                temp_array[12] = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp_array[8 + i] = array[20 + i];
                }
                for (int i = 0; i < 6; i++)
                {
                    temp_array[2 + i] = array[1 + i];
                }

                d = decimal_converter(2, temp_array, 13);
                *b_ptr=b;

                sprintf(string, "beq x%d, x%d, ", a, b);
            }
            if (array[19] == 1)
            {
                // bne
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[7 + i];
                }
                b = decimal_converter(1, temp_array, 5);
                temp_array[0] = array[0];
                temp_array[1] = array[24];
                temp_array[12] = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp_array[8 + i] = array[20 + i];
                }
                for (int i = 0; i < 6; i++)
                {
                    temp_array[2 + i] = array[1 + i];
                }

                d = decimal_converter(2, temp_array, 13);
               *b_ptr=b;

                sprintf(string, "bne x%d, x%d, ", a, b);
            }
        }
        if (array[18] == 1)
        {

            if (array[19] == 1)
            {
                // bltu
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[7 + i];
                }
                b = decimal_converter(1, temp_array, 5);
                temp_array[0] = array[0];
                temp_array[1] = array[24];
                temp_array[12] = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp_array[8 + i] = array[20 + i];
                }
                for (int i = 0; i < 6; i++)
                {
                    temp_array[2 + i] = array[1 + i];
                }

                d = decimal_converter(1, temp_array, 13);
                *b_ptr=b;

                sprintf(string, "bltu x%d, x%d, ", a, b);
            }
        }
    }
    if (array[17] == 1)
    {
        if (array[18] == 0)
        {
            if (array[19] == 0)
            {
                // blt
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[7 + i];
                }
                b = decimal_converter(1, temp_array, 5);
                temp_array[0] = array[0];
                temp_array[1] = array[24];
                temp_array[12] = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp_array[8 + i] = array[20 + i];
                }
                for (int i = 0; i < 6; i++)
                {
                    temp_array[2 + i] = array[1 + i];
                }

                d = decimal_converter(2, temp_array, 13);
                *b_ptr=b;

                sprintf(string, "blt x%d, x%d, ", a, b);
            }
            if (array[19] == 1)
            {
                // bge
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[7 + i];
                }
                b = decimal_converter(1, temp_array, 5);
                temp_array[0] = array[0];
                temp_array[1] = array[24];
                temp_array[12] = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp_array[8 + i] = array[20 + i];
                }
                for (int i = 0; i < 6; i++)
                {
                    temp_array[2 + i] = array[1 + i];
                }

                d = decimal_converter(2, temp_array, 13);
                *b_ptr=b;

                sprintf(string, "bge x%d, x%d, ", a, b);
            }
        }
        if (array[18] == 1)
        {
            if (array[19] == 0)
            {
                // bltu
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[7 + i];
                }
                b = decimal_converter(1, temp_array, 5);
                temp_array[0] = array[0];
                temp_array[1] = array[24];
                temp_array[12] = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp_array[8 + i] = array[20 + i];
                }
                for (int i = 0; i < 6; i++)
                {
                    temp_array[2 + i] = array[1 + i];
                }

                d = decimal_converter(1, temp_array, 13);
                *b_ptr=b;

                sprintf(string, "bltu x%d, x%d, ", a, b);
            }
            if (array[19] == 1)
            {
                // bgeu
                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[12 + i];
                }
                a = decimal_converter(1, temp_array, 5);

                for (int i = 0; i < 5; i++)
                {
                    temp_array[i] = array[7 + i];
                }
                b = decimal_converter(1, temp_array, 5);
                temp_array[0] = array[0];
                temp_array[1] = array[24];
                temp_array[12] = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp_array[8 + i] = array[20 + i];
                }
                for (int i = 0; i < 6; i++)
                {
                    temp_array[2 + i] = array[1 + i];
                }

                d = decimal_converter(1, temp_array, 13);
                *b_ptr=b;

                sprintf(string, "bgeu x%d, x%d, ", a, b);
            }
        }
    }
    return string;
}
char *J_type(char *string, int *array, int length, int *b_ptr)
{
    int d = 0;
    int b = 0;
    int temp_array[21];
    for (int i = 0; i < 5; i++)
    {
        temp_array[i] = array[20 + i];
    }
    d = decimal_converter(1, temp_array, 5);

    temp_array[20] = 0;
    temp_array[0] = array[0];
    temp_array[9] = array[11];
    for (int i = 0; i < 10; i++)
    {
        temp_array[10 + i] = array[1 + i];
    }
    for (int i = 0; i < 8; i++)
    {
        temp_array[8 - i] = array[12 + i];
    }

    b = decimal_converter(2, temp_array, 21);
    sprintf(string, "jalr x%d, ", d);

    *b_ptr = b;

    return string;
}

void countNonZeroElements(int *arr, int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] != 0) {
            arr[i]=count;
            count++;
        }
    }
}

int main()
{
    const char *filename = "machine_code.txt";
    char **machineCodeMatrix;
    int numInstructions;
    int i = 0;
    int type = 15;
    int num = 0;
    int count = 0;
    char string[100];
    readMachineCodeFromFile(filename, &machineCodeMatrix, &numInstructions);
    int BinaryMatrix[numInstructions][32];
    char rTypeOutputs[numInstructions][100];
    int index[numInstructions];
    int alpha[numInstructions];

    while (i < numInstructions)
    {
        BinaryConverter(machineCodeMatrix[i], BinaryMatrix[i]);

        index[i]=0;
        alpha[i]=0;
        i++;
    }

    i = 0;
    while (i < numInstructions)
    {   int b=0;
        type = TypeVerifier(BinaryMatrix[i]);

        if (type == 1)
        {
            R_type(rTypeOutputs[i], BinaryMatrix[i], 32);
        }

        if (type == 2)
        {
            Ia_type(rTypeOutputs[i], BinaryMatrix[i], 32);
        }
        if (type == 3)
        {
            Ib_type(rTypeOutputs[i], BinaryMatrix[i], 32);
        }
        if (type == 10)
        {
            Ic_type(rTypeOutputs[i], BinaryMatrix[i], 32);
        }
        if (type == 4)
        {
            S_type(rTypeOutputs[i], BinaryMatrix[i], 32);
        }
        if (type == 7)
        {
            Ua_type(rTypeOutputs[i], BinaryMatrix[i], 32);
        }
        if (type == 5)
        {
            B_type(rTypeOutputs[i], BinaryMatrix[i], 32,&b);
            b=b/4;
            index[i + b] = 1;
            alpha[i]=b;
        }
        if (type == 6)
        {
            J_type(rTypeOutputs[i], BinaryMatrix[i], 32, &b);
            b=b/4;
             index[i + b] = 1;
            alpha[i]=b;
        }

        i++;
    }
  
  countNonZeroElements(index,numInstructions);
 


    for (i = 0; i < numInstructions; i++)
    {
        if (i % 2 == 0)
        {   
            if(index[i]==0 && alpha[i]==0){
                printf("Instruction %d: %s\n", i / 2 + 1, rTypeOutputs[i]);
            }
            if(index[i]!=0){
                printf("Instruction %d: L%d: %s\n", i / 2 + 1, index[i]+1, rTypeOutputs[i]);
            }
            if(alpha[i]!=0){
                printf("Instruction %d: %s L%d \n ", i / 2 + 1, rTypeOutputs[i],index[alpha[i]]+1);
            }
            
        }
    }
    return 0;
}
