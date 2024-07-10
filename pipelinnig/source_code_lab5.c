#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read a file and create a character matrix with a specified column size
char **readFileToMatrix(const char *filename, int *numLines, int columnSize)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening the file");
        return NULL;
    }

    // Initialize variables
    *numLines = 0;
    int currentLine = 0;
    char buffer[25]; // Assuming a maximum line length of 25 characters plus null terminator

    // Create a character matrix
    char **matrix = NULL;

    // Read the file and store it in the matrix
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        (*numLines)++;
        matrix = (char **)realloc(matrix, *numLines * sizeof(char *));
        if (matrix == NULL)
        {
            perror("Error allocating memory for the matrix");
            fclose(file);
            return NULL;
        }

        matrix[*numLines - 1] = (char *)malloc(columnSize + 1);
        if (matrix[*numLines - 1] == NULL)
        {
            perror("Error allocating memory for a matrix row");
            fclose(file);
            free(matrix);
            return NULL;
        }

        strncpy(matrix[*numLines - 1], buffer, columnSize);
        matrix[*numLines - 1][columnSize] = '\0';
    }

    // Close the file
    fclose(file);

    return matrix;
}
void extractNumbers(const char *str, int *numbers, int maxNumbers)
{
    int length = strlen(str);
    int numCount = 0;
    int numStart = -1;

    for (int i = 0; i < length; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            if (numStart == -1)
            {
                numStart = i;
            }
        }
        else
        {
            if (numStart != -1)
            {
                if (numCount < maxNumbers)
                {
                    numbers[numCount] = atoi(str + numStart);
                    numCount++;
                }
                numStart = -1;
            }
        }
    }

    if (numStart != -1 && numCount < maxNumbers)
    {
        numbers[numCount] = atoi(str + numStart);
        numCount++;
    }
}

int classifer(const char *str)
{
    if (str[0] == 'l')
    {
        return 3;
    }
    if (str[3] == 'i' || str[2] == 'i')
    {
        return 2;
    }
    if(str[0]=='s' && str[1]!='u'){
        return 0;
    }
    else
    {
        return 1;
    }
}

void convertRegisterName(const char *inputName, char *outputName) {
    if (strcmp(inputName, "zero") == 0) {
        strcpy(outputName, "x0");
    } else if (strcmp(inputName, "ra") == 0) {
        strcpy(outputName, "x1");
    } else if (strcmp(inputName, "sp") == 0) {
        strcpy(outputName, "x2");
    } else if (strcmp(inputName, "gp") == 0) {
        strcpy(outputName, "x3");
    } else if (strcmp(inputName, "tp") == 0) {
        strcpy(outputName, "x4");
    } else if (strcmp(inputName, "t0") == 0) {
        strcpy(outputName, "x5");
    } else if (strcmp(inputName, "t1") == 0) {
        strcpy(outputName, "x6");
    } else if (strcmp(inputName, "t2") == 0) {
        strcpy(outputName, "x7");
    } else if (strcmp(inputName, "s0") == 0 || strcmp(inputName, "fp") == 0) {
        strcpy(outputName, "x8");
    } else if (strcmp(inputName, "s1") == 0) {
        strcpy(outputName, "x9");
    } else if (strcmp(inputName, "a0") == 0) {
        strcpy(outputName, "x10");
    } else if (strcmp(inputName, "a1") == 0) {
        strcpy(outputName, "x11");
    } else if (strcmp(inputName, "a2") == 0) {
        strcpy(outputName, "x12");
    } else if (strcmp(inputName, "a3") == 0) {
        strcpy(outputName, "x13");
    } else if (strcmp(inputName, "a4") == 0) {
        strcpy(outputName, "x14");
    } else if (strcmp(inputName, "a5") == 0) {
        strcpy(outputName, "x15");
    } else if (strcmp(inputName, "a6") == 0) {
        strcpy(outputName, "x16");
    } else if (strcmp(inputName, "a7") == 0) {
        strcpy(outputName, "x17");
    } else if (strcmp(inputName, "s2") == 0) {
        strcpy(outputName, "x18");
    } else if (strcmp(inputName, "s3") == 0) {
        strcpy(outputName, "x19");
    } else if (strcmp(inputName, "s4") == 0) {
        strcpy(outputName, "x20");
    } else if (strcmp(inputName, "s5") == 0) {
        strcpy(outputName, "x21");
    } else if (strcmp(inputName, "s6") == 0) {
        strcpy(outputName, "x22");
    } else if (strcmp(inputName, "s7") == 0) {
        strcpy(outputName, "x23");
    } else if (strcmp(inputName, "s8") == 0) {
        strcpy(outputName, "x24");
    } else if (strcmp(inputName, "s9") == 0) {
        strcpy(outputName, "x25");
    } else if (strcmp(inputName, "s10") == 0) {
        strcpy(outputName, "x26");
    } else if (strcmp(inputName, "s11") == 0) {
        strcpy(outputName, "x27");
    } else if (strcmp(inputName, "t3") == 0) {
        strcpy(outputName, "x28");
    } else if (strcmp(inputName, "t4") == 0) {
        strcpy(outputName, "x29");
    } else if (strcmp(inputName, "t5") == 0) {
        strcpy(outputName, "x30");
    } else if (strcmp(inputName, "t6") == 0) {
        strcpy(outputName, "x31");
    } else {
        // If the input name doesn't match any known register, copy it as is.
        strcpy(outputName, inputName);
    }
}

int main()
{

     FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("convertedInputs.txt", "w");

    if (inputFile == NULL) {
        perror("Error opening input.txt");
        return 1;
    }

    if (outputFile == NULL) {
        perror("Error opening output.txt");
        fclose(inputFile);
        return 1;
    }

    char inputLine[256];
    char outputLine[256];

    while (fgets(inputLine, sizeof(inputLine), inputFile) != NULL) {
        // Tokenize the input instruction.
        char *token = strtok(inputLine, " ,\n");

        outputLine[0] = '\0'; // Initialize the output line.

        while (token != NULL) {
            char convertedToken[4];

            // Convert register names (e.g., "a0" to "x10").
            convertRegisterName(token, convertedToken);

            // Append the converted token to the output line.
            strcat(outputLine, convertedToken);

            // Check if there's a comma, and if so, append it to the output line.
            token = strtok(NULL, " ,\n");
            if (token != NULL && strcmp(token, ",") == 0) {
                strcat(outputLine, ", ");
                token = strtok(NULL, " ,\n");
            }
            else if (token != NULL) {
                // If not a comma, add a space.
                strcat(outputLine, " ");
            }
        }

        // Write the converted instruction to the output file.
        fprintf(outputFile, "%s\n", outputLine);
    }

    fclose(inputFile);
    fclose(outputFile);

    const char *filename = "input.txt";
    int numLines, columnSize = 24;
    char **matrix = readFileToMatrix(filename, &numLines, columnSize);
    char **temp_matrix=readFileToMatrix("convertedInputs.txt",&numLines, columnSize);
    char no_forwarding[3 * numLines][columnSize];
    char with_forwarding[2 * numLines][columnSize];
    
    int no_forwarding_index = 0;
    int with_forwarding_index = 0;
    for (int i = 0; i < 2 * numLines; i++)
    {
        for (int j = 0; j < columnSize; j++)
        {
            with_forwarding[i][j] = ' ';
        }
    }
    for (int i = 0; i < 3 * numLines; i++)
    {
        for (int j = 0; j < columnSize; j++)
        {
            no_forwarding[i][j] = ' ';
        }
    }
    int i=0;
    for (i = 0; i < numLines - 2; i++)
    {   strcpy(no_forwarding[i + no_forwarding_index], matrix[i]);
        strcpy(with_forwarding[i + with_forwarding_index], matrix[i]);
        int now[3]; // Assuming a maximum of 3 numbers
        int next[3];
        int next_1[3];
        int maxNumbers = 3;
        int skip=0;
        extractNumbers(temp_matrix[i], now, maxNumbers);
        extractNumbers(temp_matrix[i + 1], next, maxNumbers);
        extractNumbers(temp_matrix[i+2],next_1,maxNumbers) ;      
        if (now[0] == 0 || next[0]==0)
        {
            skip=1;
        }
        if (matrix[i][0] == 'a' && matrix[i][0] == 'd' && matrix[i][0] == 'd')
        {
            if (now[0] == now[2] || now[0] == now[1])
            {
                if (now[1] == 0 || now[2] == 0)
                {
                    skip=1;
                }
            }
        }
        if (matrix[i][0] == 's' && matrix[i][0] == 'u' && matrix[i][0] == 'b')
        {
            if (now[0] == now[1])
            {
                if (now[2] == 0)
                {
                    skip=1;
                }
            }
        }
    if(skip!=1){
        int class = classifer(matrix[i + 1]);
        int up_class=classifer(matrix[i]);
        if (class == 3)
        {
            if (now[0] == next[2])
            {
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                no_forwarding_index++;
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                no_forwarding_index++;
                if(up_class==3){
                strcpy(with_forwarding[i + with_forwarding_index + 1], "NOP\n");
                with_forwarding_index++;
                }
                
            }
           
        }

        if (class == 2)
        {
            if (next[1] == now[0])
            {
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                strcpy(no_forwarding[i + no_forwarding_index + 2], "NOP\n");
                no_forwarding_index = no_forwarding_index + 2;
                if(up_class==3){
                strcpy(with_forwarding[i + with_forwarding_index + 1], "NOP\n");
                with_forwarding_index++;
                }
            }
            
        }
        if (class == 1)
        {

            if (next[1] == now[0] || next[2] == now[0])
            {
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                strcpy(no_forwarding[i + no_forwarding_index + 2], "NOP\n");
                no_forwarding_index = no_forwarding_index + 2;
                if(up_class==3){
                strcpy(with_forwarding[i + with_forwarding_index + 1], "NOP\n");
                with_forwarding_index++;
                }
            }
           

        }

    }
      int class=classifer(matrix[i+2]);
      if(class==3){
        if (now[0] == next_1[2]){
            if(no_forwarding[i+no_forwarding_index+1][0]!='N'){
                 strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                no_forwarding_index++;
            }
        }
        if(class==2){
            if(next_1[1]==now[0]){
                if(no_forwarding[i+no_forwarding_index+1][0]!='N'){
               strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                no_forwarding_index++; 
                }
            }
            
        }
        if(class==1){
             if (next_1[1] == now[0] || next_1[2] == now[0])
            {
                if(no_forwarding[i+no_forwarding_index+1][0]!='N'){
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                no_forwarding_index++;
                }
            }

        }
      }
    }
   i=numLines-2;
    strcpy(no_forwarding[i + no_forwarding_index], matrix[i]);
        strcpy(with_forwarding[i + with_forwarding_index], matrix[i]);
        int now[3]; // Assuming a maximum of 3 numbers
        int next[3];
        int maxNumbers = 3;
        int skip=0;
        extractNumbers(temp_matrix[i], now, maxNumbers);
        extractNumbers(temp_matrix[i + 1], next, maxNumbers);     
        if (now[0] == 0 || next[0]==0)
        {
            skip=1;
        }
        if (matrix[i][0] == 'a' && matrix[i][0] == 'd' && matrix[i][0] == 'd')
        {
            if (now[0] == now[2] || now[0] == now[1])
            {
                if (now[1] == 0 || now[2] == 0)
                {
                    skip=1;
                }
            }
        }
        if (matrix[i][0] == 's' && matrix[i][0] == 'u' && matrix[i][0] == 'b')
        {
            if (now[0] == now[1])
            {
                if (now[2] == 0)
                {
                    skip=1;
                }
            }
        }
    if(skip!=1){
        int class = classifer(matrix[i + 1]);
        int up_class=classifer(matrix[i]);
        if (class == 3)
        {
            if (now[0] == next[2])
            {
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                no_forwarding_index++;
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                no_forwarding_index++;
               if(up_class==3){
                strcpy(with_forwarding[i + with_forwarding_index + 1], "NOP\n");
                with_forwarding_index++;
                }
            }
           
        }

        if (class == 2)
        {
            if (next[1] == now[0])
            {
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                strcpy(no_forwarding[i + no_forwarding_index + 2], "NOP\n");
                no_forwarding_index = no_forwarding_index + 2;
                if(up_class==3){
                strcpy(with_forwarding[i + with_forwarding_index + 1], "NOP\n");
                with_forwarding_index++;
                }
            }
            
        }
        if (class == 1)
        {

            if (next[1] == now[0] || next[2] == now[0])
            {
                strcpy(no_forwarding[i + no_forwarding_index + 1], "NOP\n");
                strcpy(no_forwarding[i + no_forwarding_index + 2], "NOP\n");
                no_forwarding_index = no_forwarding_index + 2;
                if(up_class==3){
                strcpy(with_forwarding[i + with_forwarding_index + 1], "NOP\n");
                with_forwarding_index++;
                }
            }
           

        }
    }


    strcpy(no_forwarding[numLines + no_forwarding_index - 1], matrix[numLines - 1]);
    strcpy(with_forwarding[numLines + with_forwarding_index - 1], matrix[numLines - 1]);



    printf("Matrix :\n");
    for (int i = 0; i < numLines; i++)
    {
        printf("%s", matrix[i]);
    }
    printf("\n");
    printf("Matrix(temp) :\n");
    for (int i = 0; i < numLines; i++)
    {
        printf("%s", temp_matrix[i]);
    }
    printf("\n");
    int t = numLines + no_forwarding_index;
    printf("Matrix (no forwarding):\n");
    for (int i = 0; i < t; i++)
    {
        printf("%s", no_forwarding[i]);
    }
    printf("Total no of cycles is %d" ,t+3);
    printf("\n");
    t = numLines + with_forwarding_index;
    printf("Matrix (with forwarding):\n");
    for (int i = 0; i < t; i++)
    {
        printf("%s", with_forwarding[i]);
    }
    printf("Total no of cycles is %d" ,t+3);

    // Free memory
    for (int i = 0; i < numLines; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}