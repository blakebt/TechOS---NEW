#include "commands.h"

void createDirectory(char folderName[])
{
    int isCreated = mkdir(folderName);
    if(isCreated == 0) // directory was successfully created
    {
        printf("\nDirectory successfully created.\n");
    }
    else if(errno == EEXIST)
    {
        red();
        printf("\nDirectory already exists.\n");
        reset();
    }
    else
    {
        red();
        printf("\nError when creating directory.\n");
        reset();
    }
}

void deleteDirectory(char folderName[])
{
    int isDeleted = rmdir(folderName);
    if(isDeleted == 0) // folder was successfully deleted
    {
        printf("\nDirectory successfully deleted.\n");
    }
    // if the directory is not empty
    else if(errno == ENOTEMPTY)
    {
        #ifdef _WIN32
            // build the command for the system call
            char command[MAX_COMMAND];
            strcpy(command, "rd ");
            strcat(command, folderName);
            strcat(command, " /S");
            printf("\nThis directory is not empty, this will delete all files within ");
            system(command);
        #elif __linux__
            char command[MAX_COMMAND];
            strcpy(command, "rm ");
            strcat(command, " -rf ");
            strcat(command, folderName);
            system(command);
        #endif

    }
    // if the directory does not exist
    else if(errno != EEXIST)
    {
        red();
        printf("\nDirectory does not exist.\n");
        reset();
    }
    else
    {
        red();
        printf("\nError when deleting directory.\n");
        reset();
    }
}

void viewDirectory(char directoryName[])
//currently can only read files and subdirectories within the directory files.c is saved in
{
    DIR *localFolder;
    struct dirent *files;

    localFolder = opendir(directoryName);
    if(localFolder == NULL)
    {
        red();
        printf("Directory read error\n");
        reset();
    }
    else
    {
        char currentDir[MAX_FILE_NAME_LENGTH];
        getcwd(currentDir, MAX_FILE_NAME_LENGTH);
        printf("------------------------------------------------\n%s\n------------------------------------------------\n", currentDir);
        while ((files=readdir(localFolder)))
        {
            if(strcmp(files->d_name, ".") != 0 && strcmp(files->d_name, "..") != 0)
            {
                struct stat currentFile;
                stat(files->d_name, &currentFile);
                printf("%s", files->d_name);
                for(int i = 1; i <= 3-(strlen(files->d_name) / 8); i++)
                {
                    printf("\t");
                }
                long sizeKB = round(currentFile.st_size / 1024.0);
                printf("\t%10ld KB\n", sizeKB);
            }
        }
    }

    closedir(localFolder);
}

void changeDirectory(char argument[]) //IF ANY OF THIS GETS CHANGED I VOW TERRIBLE THINGS TO HAPPEN
{
    if(strcmp(argument, "~") == 0) //This will be the up directory argument, if anyone wants to change this, just swap out the ~ with the desired argument
    {
        char currentDir[MAX_FILE_NAME_LENGTH];
        getcwd(currentDir, MAX_FILE_NAME_LENGTH);

        int lastSlashIdx = 0;
        for(int i = 0; i < strlen(currentDir); i++)
        {
            if(currentDir[i] == '\\')
            {
                lastSlashIdx = i;
            }
        }
        char thing[MAX_FILE_NAME_LENGTH];
        for(int i = 0; i < lastSlashIdx; i++)
        {
            thing[i] = currentDir[i];
        }
        thing[lastSlashIdx] = '\0';
        if(chdir(thing) == 0)
        {
            printf("Successfully changed directory\n");
        }
        else{
            printf("Failure in directory change\n");
        }
    }
    else if(strncmp(argument, "/", 1) == 0)
    {
        char output[MAX_FILE_NAME_LENGTH];
        //char prt2[MAX_FILE_NAME_LENGTH];
        //strcpy(prt2, argument);
        getcwd(output, MAX_FILE_NAME_LENGTH);
        strcat(output, argument);

        if(chdir(output) == 0)
        {
            printf("Successfully changed directory\n");
        }
        else{
            red();
            printf("Directory not found\n");
            reset();
        }
    }
    else
    {
        if(chdir(argument) == 0)
        {
            printf("Successfully changed directory\n");
        }
        else{
            red();
            printf("Directory not found\n");
            reset();
        }
    }
}

void createFile(char fileName[])
{
    char filePath[MAX_FILE_NAME_LENGTH + 1];//Holds absolute path of file
    char cwd[MAX_FILE_NAME_LENGTH];//Holds current directory
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("chdir() error()\n");
    }

    sprintf(filePath, "%s/%s", cwd, fileName);//Formats the file path

    if(access(filePath, F_OK) == 0)//Checks if a file of same name exists in current directory
    {
        red();
        printf("File name already exists in the current directory.\n");
        reset();
    }
    else
    {
        FILE* fPtr;
        if((fPtr = fopen(filePath, "w+")) != NULL)//Otherwise creates file and closes (So file can be deleted later)
        {
            printf("File %s created successfully.\n", fileName);
            fclose(fPtr);
        }
        else
        {
            red();
            printf("Error occurred during file creation.\n");
            reset();
        }

    }
}

void removeFile(char fileName[])
{

    char filePath[MAX_FILE_NAME_LENGTH + 1];//Holds absolute path of file
    char cwd[MAX_FILE_NAME_LENGTH];//Holds current directory
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("chdir() error()\n");
    }

    sprintf(filePath, "%s/%s", cwd, fileName);//Formats file path to be deleted

    if(remove(fileName) == 0)//Checks that file was deleted successfully
    {
        printf("%s successfully removed\n", fileName);
    }
    else
    {
        red();
        printf("File does not exist. ERRNO()\n");
        reset();
    }
}
